/*
* Copyright (C) 2008-2013 The Communi Project
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

#include "irccommandparser.h"
#include <climits>

/*!
    \file irccommandparser.h
    \brief #include &lt;IrcCommandParser&gt;
 */

/*!
    \class IrcCommandParser irccommandparser.h <IrcCommandParser>
    \ingroup utility
    \brief Parses commands from user input.

    \section syntax Syntax

    Since the list of supported commands and the exact syntax for each
    command is application specific, IrcCommandParser does not provide
    any built-in command syntaxes. It is left up to the applications
    to introduce the supported commands and syntaxes.
    IrcCommandParser supports the following command syntax markup:

    Syntax             | Example              | Description
    -------------------|----------------------|------------
    &lt;param&gt;      | &lt;target&gt;       | A required parameter.
    (&lt;param&gt;)    | (&lt;key&gt;)        | An optional parameter.
    &lt;param...&gt;   | &lt;message...&gt;   | A required parameter, multiple words accepted. (1)
    (&lt;param...&gt;) | (&lt;message...&gt;) | An optional parameter, multiple words accepted. (1)
    (&lt;#param&gt;)   | (&lt;#channel&gt;)   | An optional channel parameter. (2)
    [param]            | [target]             | Inject the current target.

    -# Multi-word parameters are only supported in the last parameter position.
    -# An optional channel parameter is filled up with the current channel when absent.

    \note The parameter names are insignificant, but descriptive
    parameter names are recommended for the sake of readability.

    \section context Context

    Notice that commands are often context sensitive. While some command
    may accept an optional parameter that is filled up with the current
    target (channel/query) name when absent, another command may always
    inject the current target name as a certain parameter. Therefore
    IrcCommandParser must be kept up-to-date with the \ref currentTarget
    "current target" and the \ref channels "list of channels".

    \section example Example

    \code
    IrcCommandParser* parser = new IrcCommandParser(this);
    parser->addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser->addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
    parser->addCommand(IrcCommand::Kick, "KICK (<#channel>) <nick> (<reason...>)");
    parser->addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser->addCommand(IrcCommand::CtcpAction, "ACTION <target> <message...>");

    // currently in a query, and also present on some channels
    parser->setCurrentTarget("jpnurmi");
    parser->setChannels(QStringList() << "#communi" << "#freenode");
    \endcode
 */

struct IrcCommandInfo
{
    IrcCommand::Type type;
    QString command;
    QString syntax;
};

class IrcCommandParserPrivate
{
public:
    IrcCommandParserPrivate() : prefix(QLatin1Char('/')) { }

    QList<IrcCommandInfo> find(const QString& command) const;
    IrcCommand* parse(const IrcCommandInfo& command, QStringList params) const;

    QString prefix;
    QString current;
    QStringList channels;
    QList<IrcCommandInfo> commands;
};

QList<IrcCommandInfo> IrcCommandParserPrivate::find(const QString& command) const
{
    QList<IrcCommandInfo> result;
    foreach (const IrcCommandInfo& cmd, commands) {
        if (cmd.command == command)
            result += cmd;
    }
    return result;
}

static inline bool isOptional(const QString& token)
{
    return token.startsWith(QLatin1Char('(')) && token.endsWith(QLatin1Char(')'));
}

static inline bool isMultiWord(const QString& token)
{
    return token.contains(QLatin1String("..."));
}

static inline bool isChannel(const QString& token)
{
    return token.contains(QLatin1Char('#'));
}

static inline bool isCurrent(const QString& token)
{
    return token.startsWith(QLatin1Char('[')) && token.endsWith(QLatin1Char(']'));
}

IrcCommand* IrcCommandParserPrivate::parse(const IrcCommandInfo& command, QStringList params) const
{
    const QStringList tokens = command.syntax.split(QLatin1Char(' '), QString::SkipEmptyParts);
    const bool onChannel = channels.contains(current, Qt::CaseInsensitive);

    int min = 0;
    int max = tokens.count();

    for (int i = 0; i < tokens.count(); ++i) {
        const QString token = tokens.at(i);
        if (!isOptional(token))
            ++min;
        const bool last = (i == tokens.count() - 1);
        if (last && isMultiWord(token))
            max = INT_MAX;
        if (isOptional(token) && isChannel(token)) {
            if (onChannel) {
                const QString param = params.value(i);
                if (param.isNull() || !channels.contains(param, Qt::CaseInsensitive))
                    params.insert(i, current);
            } else  if (!channels.contains(params.value(i))) {
                return 0;
            }
            ++min;
        } else if (isCurrent(token)) {
            params.insert(i, current);
        }
    }

    if (params.count() >= min && params.count() <= max) {
        IrcCommand* cmd = new IrcCommand;
        cmd->setType(command.type);
        cmd->setParameters(params);
        return cmd;
    }

    return 0;
}

/*!
    Clears the list of commands.

    \sa reset()
 */
void IrcCommandParser::clear()
{
    Q_D(IrcCommandParser);
    d->commands.clear();
    emit commandsChanged(QStringList());
}

/*!
    Resets the channels and the current target.

    \sa clear()
 */
void IrcCommandParser::reset()
{
    setChannels(QStringList());
    setCurrentTarget(QString());
}

/*!
    Constructs a command parser with \a parent.
 */
IrcCommandParser::IrcCommandParser(QObject* parent) : QObject(parent), d_ptr(new IrcCommandParserPrivate)
{
}

/*!
    Destructs the command parser.
 */
IrcCommandParser::~IrcCommandParser()
{
}

/*!
    This property holds the known commands.

    \par Access function:
    \li QStringList <b>commands</b>() const

    \par Notifier signal:
    \li void <b>commandsChanged</b>(const QStringList& commands)

    \sa addCommand(), removeCommand()
 */
QStringList IrcCommandParser::commands() const
{
    Q_D(const IrcCommandParser);
    QStringList result;
    foreach (const IrcCommandInfo& cmd, d->commands)
        result += cmd.command;
    return result;
}

/*!
    Returns the syntax for the given \a command.
 */
QString IrcCommandParser::syntax(const QString& command) const
{
    Q_D(const IrcCommandParser);
    IrcCommandInfo info = d->find(command.toUpper()).value(0);
    if (!info.command.isEmpty())
        return info.command + QLatin1Char(' ') + info.syntax;
    return QString();
}

/*!
    Adds a command with \a type and \a syntax.
 */
void IrcCommandParser::addCommand(IrcCommand::Type type, const QString& syntax)
{
    Q_D(IrcCommandParser);
    QStringList words = syntax.split(QLatin1Char(' '), QString::SkipEmptyParts);
    if (!words.isEmpty()) {
        IrcCommandInfo cmd;
        cmd.type = type;
        cmd.command = words.takeFirst().toUpper();
        cmd.syntax = words.join(QLatin1String(" "));
        d->commands += cmd;
        emit commandsChanged(commands());
    }
}

/*!
    Removes the command with \a type and \a syntax.
 */
void IrcCommandParser::removeCommand(IrcCommand::Type type, const QString& syntax)
{
    Q_D(IrcCommandParser);
    int count = d->commands.count();
    QMutableListIterator<IrcCommandInfo> it(d->commands);
    while (it.hasNext()) {
        IrcCommandInfo cmd = it.next();
        if (cmd.type == type && (syntax.isEmpty() || !syntax.compare(cmd.syntax, Qt::CaseInsensitive)))
            it.remove();
    }
    if (d->commands.count() != count)
        emit commandsChanged(commands());
}

/*!
    This property holds the available channels.

    \par Access function:
    \li QStringList <b>channels</b>() const
    \li void <b>setChannels</b>(const QStringList& channels)

    \par Notifier signal:
    \li void <b>channelsChanged</b>(const QStringList& channels)
 */
QStringList IrcCommandParser::channels() const
{
    Q_D(const IrcCommandParser);
    return d->channels;
}

void IrcCommandParser::setChannels(const QStringList& channels)
{
    Q_D(IrcCommandParser);
    if (d->channels != channels) {
        d->channels = channels;
        emit channelsChanged(channels);
    }
}

/*!
    This property holds the current target.

    \par Access function:
    \li QString <b>currentTarget</b>() const
    \li void <b>setCurrentTarget</b>(const QString& target)

    \par Notifier signal:
    \li void <b>currentTargetChanged</b>(const QString& target)
 */
QString IrcCommandParser::currentTarget() const
{
    Q_D(const IrcCommandParser);
    return d->current;
}

void IrcCommandParser::setCurrentTarget(const QString& target)
{
    Q_D(IrcCommandParser);
    if (d->current != target) {
        d->current = target;
        emit currentTargetChanged(target);
    }
}

/*!
    This property holds the command prefix.

    The default value is "/".

    \par Access function:
    \li QString <b>prefix</b>() const
    \li void <b>setPrefix</b>(const QString& prefix)

    \par Notifier signal:
    \li void <b>prefixChanged</b>(const QString& prefix)
 */
QString IrcCommandParser::prefix() const
{
    Q_D(const IrcCommandParser);
    return d->prefix;
}

void IrcCommandParser::setPrefix(const QString& prefix)
{
    Q_D(IrcCommandParser);
    if (d->prefix != prefix) {
        d->prefix = prefix;
        emit prefixChanged(prefix);
    }
}

static bool isMessage(const QString& input, const QString& prefix)
{
    if (prefix.isEmpty())
        return false;

    return !input.startsWith(prefix) || input.startsWith(prefix.repeated(2)) || input.startsWith(prefix + QLatin1Char(' '));
}

/*!
    Parses and returns the command for \a input, or \c 0 if the input is not valid.
 */
IrcCommand* IrcCommandParser::parse(const QString& input) const
{
    Q_D(const IrcCommandParser);
    if (isMessage(input, d->prefix)) {
        QString message = input;
        if (message.startsWith(d->prefix))
            message.remove(0, 1);
        return IrcCommand::createMessage(d->current, message.trimmed());
    } else {
        QStringList params = input.mid(d->prefix.length()).split(QLatin1Char(' '), QString::SkipEmptyParts);
        if (!params.isEmpty()) {
            const QString& command = params.takeFirst().toUpper();
            foreach (const IrcCommandInfo& c, d->find(command)) {
                IrcCommand* cmd = d->parse(c, params);
                if (cmd)
                    return cmd;
            }
        }
    }
    return 0;
}
