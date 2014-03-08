/*
* Copyright (C) 2008-2014 The Communi Project
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*/

#include "irccompleter.h"
#include "irccommandparser.h"
#include "irccommandparser_p.h"
#include "ircbuffermodel.h"
#include "ircusermodel.h"
#include "ircnetwork.h"
#include "ircchannel.h"
#include "ircuser.h"

#include <QTextBoundaryFinder>
#include <QPointer>
#include <QList>
#include <QPair>

IRC_BEGIN_NAMESPACE

/*!
    \file irccompleter.h
    \brief \#include &lt;IrcCompleter&gt;
 */

/*!
    \since 3.1
    \class IrcCompleter irccompleter.h <IrcCompleter>
    \ingroup util
    \brief Provides command and name completion.

    IrcCompleter provides command and name completion for a text input field. The completer
    is made context aware by assigning a command \ref IrcCompleter::parser "parser" and a
    \ref buffer that is currently active in the GUI. The parser is used for completing
    commands, and the buffer is used for completing buffer and user names.

    In order to perform a completion, call complete() with the current text input field
    content and the cursor position. If a suitable completion is found, the completed()
    signal is emitted with a suggestion for a new content and cursor position for the
    text input field.

    \code
    TextField {
        id: textField

        Keys.onTabPressed: completer.complete(text, cursorPosition)

        IrcCompleter {
            id: completer

            buffer: ...
            parser: ...

            onCompleted: {
                textField.text = text
                textField.cursorPosition = cursor
            }
        }
    }
    \endcode

    \sa IrcCommandParser, IrcBuffer
 */

/*!
    \fn void IrcCompleter::completed(const QString& text, int cursor)

    This signal is emitted when a suitable completion with \a text and \a cursor position is found.
 */

#ifndef IRC_DOXYGEN

#if QT_VERSION < 0x050000
#define StartOfItem StartWord
#define EndOfItem EndWord
#endif // QT_VERSION

static bool isPrefixed(const QString& text, int pos, const QStringList& prefixes, int* len)
{
    foreach (const QString& prefix, prefixes) {
        const int ll = prefix.length();
        if (text.mid(pos, ll) == prefix) {
            if (len)
                *len = 0;
            return true;
        } else if (text.mid(pos - ll, ll) == prefix) {
            if (len)
                *len = ll;
            return true;
        }
    }
    return false;
}

static QPair<int, int> findWordBoundaries(const QString& text, int pos, const QStringList& prefixes)
{
    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, text);
    finder.setPosition(pos);

    int pfx = 0;
    if (finder.boundaryReasons() & QTextBoundaryFinder::StartOfItem || (finder.isAtBoundary() && isPrefixed(text, pos, prefixes, &pfx))) {
        int end = finder.toNextBoundary();
        if (end == -1)
            end = pos;
        pos -= pfx;
        return qMakePair(pos, end - pos);
    } else if (finder.boundaryReasons() & QTextBoundaryFinder::EndOfItem) {
        int begin = finder.toPreviousBoundary();
        return qMakePair(begin, pos - begin);
    } else {
        int begin = finder.toPreviousBoundary();
        if (finder.boundaryReasons() & QTextBoundaryFinder::StartOfItem || (finder.isAtBoundary() && isPrefixed(text, pos, prefixes, &pfx))) {
            int end = finder.toNextBoundary();
            if (end == -1)
                end = begin;
            begin -= pfx;
            return qMakePair(begin, end - begin);
        } else {
            int end = finder.toPreviousBoundary();
            return qMakePair(end, begin - end);
        }
    }
    return qMakePair(-1, -1);
}

static int indexOfWord(const QString& text, int pos)
{
    return text.left(pos + 1).split(QLatin1Char(' '), QString::SkipEmptyParts).count() - 1;
}

static int indexOfNonSpace(const QString& text, int pos = 0)
{
    while (pos >= 0 && pos < text.length() && text.at(pos) == QLatin1Char(' '))
        ++pos;
    return pos;
}

static QString replaceWord(const QString& text, int index, const QString& word)
{
    if (index >= 0) {
        int from = indexOfNonSpace(text);
        while (index > 0 && from < text.length()) {
            from = indexOfNonSpace(text, text.indexOf(QLatin1Char(' '), from));
            --index;
        }
        if (from < text.length()) {
            int to = text.indexOf(QLatin1Char(' '), from);
            if (to == -1)
                to = text.length();
            return QString(text).replace(from, to - from, word);
        }
    }
    return text;
}

struct IrcCompletion
{
    IrcCompletion(const QString& txt, int pos) : text(txt), cursor(pos) { }
    bool operator ==(const IrcCompletion& other) const { return text == other.text && cursor == other.cursor; }
    bool operator !=(const IrcCompletion& other) const { return text != other.text || cursor != other.cursor; }
    QString text;
    int cursor;
};

class IrcCompleterPrivate
{
    Q_DECLARE_PUBLIC(IrcCompleter)

public:
    IrcCompleterPrivate();

    void completeNext();
    QList<IrcCompletion> completeCommands(const QString& text, int pos) const;
    QList<IrcCompletion> completeWords(const QString& text, int pos) const;

    IrcCompleter* q_ptr;

    int index;
    int cursor;
    QString text;
    QList<IrcCompletion> completions;

    QString suffix;
    QPointer<IrcBuffer> buffer;
    QPointer<IrcCommandParser> parser;
};

IrcCompleterPrivate::IrcCompleterPrivate() : q_ptr(0), index(-1), cursor(-1), suffix(":"), buffer(0), parser(0)
{
}

void IrcCompleterPrivate::completeNext()
{
    Q_Q(IrcCompleter);
    Q_ASSERT(!completions.isEmpty());
    if (++index >= 0) {
        index %= completions.length();
        const IrcCompletion completion = completions.at(index);
        text = completion.text;
        cursor = completion.cursor;
        emit q->completed(text, cursor);
    }
}

static IrcCompletion completeCommand(const QString& text, const QString& command)
{
    QString completion = replaceWord(text, 0, command);
    int next = command.length();
    if (next >= completion.length() || completion.at(next) != QLatin1Char(' '))
        completion.insert(next, QLatin1Char(' '));
    return IrcCompletion(completion, ++next);
}

QList<IrcCompletion> IrcCompleterPrivate::completeCommands(const QString& text, int pos) const
{
    if (!parser)
        return QList<IrcCompletion>();

    QList<IrcCompletion> completions;

    int removed = 0;
    QString input = text;
    IrcCommandParserPrivate* pp = IrcCommandParserPrivate::get(parser);
    if (pp->processCommand(&input, &removed)) {
        const QString command = input.split(QLatin1Char(' '), QString::SkipEmptyParts).value(0).toUpper();
        if (!command.isEmpty()) {
            foreach (const IrcCommandInfo& cmd, pp->commands) {
                if (cmd.command == command)
                    return QList<IrcCompletion>() << completeCommand(text, text.left(removed) + cmd.command);
                if (cmd.command.startsWith(command))
                    completions += completeCommand(text, text.left(removed) + cmd.command);
            }
        }
        // TODO: context sensitive command parameter completion
        Q_UNUSED(pos);
    }
    return completions;
}

static IrcCompletion completeWord(const QString& text, int from, int len, const QString& word)
{
    QString completion = QString(text).replace(from, len, word);
    int next = from + word.length();
    if (next >= completion.length() || completion.at(next) != QLatin1Char(' '))
        completion.insert(next, QLatin1Char(' '));
    return IrcCompletion(completion, ++next);
}

QList<IrcCompletion> IrcCompleterPrivate::completeWords(const QString& text, int pos) const
{
    if (!buffer || !buffer->network())
        return QList<IrcCompletion>();

    QList<IrcCompletion> completions;

    const QPair<int, int> bounds = findWordBoundaries(text, pos, buffer->network()->channelTypes());
    if (bounds.first != -1 && bounds.second != -1) {
        const QString word = text.mid(bounds.first, bounds.second);

        int pfx = 0;
        QString prefix;
        bool isChannel = isPrefixed(text, bounds.first, buffer->network()->channelTypes(), &pfx);
        if (isChannel && pfx > 0)
            prefix = text.mid(bounds.first - pfx, pfx);

        QList<IrcBuffer*> buffers = buffer->model()->buffers();
        buffers.move(buffers.indexOf(buffer), 0); // promote the current buffer
        foreach (IrcBuffer* buffer, buffers) {
            const QString title = buffer->title();
            if (title.startsWith(word, Qt::CaseInsensitive))
                completions += completeWord(text, bounds.first, bounds.second, title);
            else if (isChannel && !prefix.isEmpty() && title.startsWith(prefix + word, Qt::CaseInsensitive))
                completions += completeWord(text, bounds.first - prefix.length(), bounds.second + prefix.length(), title);
        }

        if (!isChannel) {
            IrcUserModel userModel;
            userModel.setSortMethod(Irc::SortByActivity);
            userModel.setChannel(qobject_cast<IrcChannel*>(buffer));
            foreach (IrcUser* user, userModel.users()) {
                if (user->name().startsWith(word, Qt::CaseInsensitive)) {
                    QString name = user->name();
                    if (indexOfWord(text, pos) == 0)
                        name += suffix;
                    completions += completeWord(text, bounds.first, bounds.second, name);
                }
            }
        }
    }
    return completions;
}
#endif // IRC_DOXYGEN

/*!
    Constructs a completer with \a parent.
 */
IrcCompleter::IrcCompleter(QObject* parent) : QObject(parent), d_ptr(new IrcCompleterPrivate)
{
    Q_D(IrcCompleter);
    d->q_ptr = this;
}

/*!
    Destructs the completer.
 */
IrcCompleter::~IrcCompleter()
{
}

/*!
    This property holds the completion suffix.

    The suffix is appended to the end of a completed nick name, but
    only when the nick name is in the beginning of completed text.

    The default value is \c ":".

    \par Access functions:
    \li QString <b>suffix</b>() const
    \li void <b>setSuffix</b>(const QString& suffix) [slot]

    \par Notifier signal:
    \li void <b>suffixChanged</b>(const QString& suffix)
 */
QString IrcCompleter::suffix() const
{
    Q_D(const IrcCompleter);
    return d->suffix;
}

void IrcCompleter::setSuffix(const QString& suffix)
{
    Q_D(IrcCompleter);
    if (d->suffix != suffix) {
        d->suffix = suffix;
        emit suffixChanged(suffix);
    }
}

/*!
    This property holds the buffer used for name completion.

    \par Access functions:
    \li \ref IrcBuffer* <b>buffer</b>() const
    \li void <b>setBuffer</b>(\ref IrcBuffer* buffer) [slot]

    \par Notifier signal:
    \li void <b>bufferChanged</b>(\ref IrcBuffer* buffer)
 */
IrcBuffer* IrcCompleter::buffer() const
{
    Q_D(const IrcCompleter);
    return d->buffer;
}

void IrcCompleter::setBuffer(IrcBuffer* buffer)
{
    Q_D(IrcCompleter);
    if (d->buffer != buffer) {
        d->buffer = buffer;
        emit bufferChanged(buffer);
    }
}

/*!
    This property holds the parser used for command completion.

    \par Access functions:
    \li \ref IrcCommandParser* <b>parser</b>() const
    \li void <b>setParser</b>(\ref IrcCommandParser* parser) [slot]

    \par Notifier signal:
    \li void <b>parserChanged</b>(\ref IrcCommandParser* parser)
 */
IrcCommandParser* IrcCompleter::parser() const
{
    Q_D(const IrcCompleter);
    return d->parser;
}

void IrcCompleter::setParser(IrcCommandParser* parser)
{
    Q_D(IrcCompleter);
    if (d->parser != parser) {
        d->parser = parser;
        emit parserChanged(parser);
    }
}

/*!
    Completes \a text at \a cursor position and emits
    completed() if a suitable completion is found.
 */
void IrcCompleter::complete(const QString& text, int cursor)
{
    Q_D(IrcCompleter);
    if (!d->completions.isEmpty() && d->cursor == cursor && d->text == text) {
        d->completeNext();
        return;
    }

    QList<IrcCompletion> completions = d->completeCommands(text, cursor);
    if (completions.isEmpty() || indexOfWord(text, cursor) > 0)
        completions = d->completeWords(text, cursor);

    if (d->completions != completions) {
        d->index = -1;
        d->completions = completions;
    }
    if (!d->completions.isEmpty())
        d->completeNext();
}

/*!
    Resets the completer state.
 */
void IrcCompleter::reset()
{
    Q_D(IrcCompleter);
    d->index = -1;
    d->cursor = -1;
    d->text.clear();
    d->completions.clear();
}

#include "moc_irccompleter.cpp"

IRC_END_NAMESPACE
