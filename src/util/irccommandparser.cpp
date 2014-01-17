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

#include "irccommandparser.h"
#include <climits>
#include <qmap.h>

IRC_BEGIN_NAMESPACE

/*!
    \file irccommandparser.h
    \brief \#include &lt;IrcCommandParser&gt;
 */

/*!
    \class IrcCommandParser irccommandparser.h <IrcCommandParser>
    \ingroup util
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
    (&lt;\#param&gt;)  | (&lt;\#channel&gt;)  | An optional channel parameter. (2)
    [param]            | [target]             | Inject the current target.

    -# Multi-word parameters are only supported in the last parameter position.
    -# An optional channel parameter is filled up with the current channel when absent.

    The following example presents introducing some typical commands.
    \code
    IrcCommandParser* parser = new IrcCommandParser(this);
    parser->addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser->addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
    parser->addCommand(IrcCommand::Kick, "KICK (<#channel>) <nick> (<reason...>)");
    parser->addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser->addCommand(IrcCommand::CtcpAction, "ACTION <target> <message...>");
    \endcode

    \note The parameter names are insignificant, but descriptive
    parameter names are recommended for the sake of readability.

    \section context Context

    Notice that commands are often context sensitive. While some command
    may accept an optional parameter that is filled up with the current
    target (channel/query) name when absent, another command may always
    inject the current target name as a certain parameter. Therefore
    IrcCommandParser must be kept up-to-date with the \ref target
    "current target" and the \ref channels "list of channels".

    \code
    // currently in a query, and also present on some channels
    parser->setTarget("jpnurmi");
    parser->setChannels(QStringList() << "#communi" << "#freenode");
    \endcode

    \section triggers Command triggers

    IrcCommandParser serves as a generic parser for typical IRC commands.
    It can be utilized for parsing commands from user input in GUI clients,
    and from messages from other clients when implementing IRC bots.

    The command parsing behavior is controlled by setting up command
    \ref triggers. Whilst a typical GUI client might use \c "/" as a command
    trigger, an IRC bot might use \c "!" and the nick name of the bot. The
    following snippet illustrates a typical GUI client usage.

    \code
    parser->setTarget("#communi");
    parser->setTriggers(QStringList() << "/");
    parser->parse(input);
    \endcode

    \p
    Input             | Result              | Description
    ------------------|---------------------|------------
    "hello"           | IrcCommand::Message | No matching command trigger => a message "hello" to \#communi
    "/join #channel"  | IrcCommand::Join    | Matching command trigger => a command to join "#channel"

    See the \ref bot "bot example" to see how the parser can be effectively utilized for IRC bots.

    \section custom Custom commands

    The parser also supports such custom client specific commands that
    are not sent to the server. Since IrcCommand does not know how to
    handle custom commands, the parser treats them as a special case
    injecting the command as a first parameter.

    \code
    IrcParser parser;
    parser.addCommand(IrcCommand::Custom, "QUERY <user>");
    IrcCommand* command = parser.parse("/query jpnurmi");
    Q_ASSERT(command->type() == IrcCommand::Custom);
    qDebug() << command->parameters(); // ("QUERY", "jpnurmi")
    \endcode
 */

/*!
    \enum IrcCommandParser::Detail
    This enum describes the available syntax details.
 */

/*!
    \var IrcCommandParser::Full
    \brief The syntax in full details
 */

/*!
    \var IrcCommandParser::NoTarget
    \brief The syntax has injected [target] removed
 */

/*!
    \var IrcCommandParser::NoPrefix
    \brief The syntax has \#channel prefixes removed
 */

/*!
    \var IrcCommandParser::NoEllipsis
    \brief The syntax has ellipsis... removed
 */

/*!
    \var IrcCommandParser::NoParentheses
    \brief The syntax has parentheses () removed
 */

/*!
    \var IrcCommandParser::NoBrackets
    \brief The syntax has brackets [] removed
 */

/*!
    \var IrcCommandParser::NoAngles
    \brief The syntax has angle brackets <> removed
 */

/*!
    \var IrcCommandParser::Visual
    \brief The syntax suitable for visual representation
 */

struct IrcCommandInfo
{
    QString fullSyntax()
    {
        return command + QLatin1Char(' ') + syntax;
    }

    IrcCommand::Type type;
    QString command;
    QString syntax;
};

class IrcCommandParserPrivate
{
public:
    IrcCommandParserPrivate() : tolerant(false) { }

    QList<IrcCommandInfo> find(const QString& command) const;
    IrcCommand* parse(const IrcCommandInfo& command, QStringList params) const;

    bool tolerant;
    QString target;
    QStringList triggers;
    QStringList channels;
    QMultiMap<QString, IrcCommandInfo> commands;
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
    const bool onChannel = channels.contains(target, Qt::CaseInsensitive);

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
                    params.insert(i, target);
            } else  if (!channels.contains(params.value(i))) {
                return 0;
            }
            ++min;
        } else if (isCurrent(token)) {
            params.insert(i, target);
        }
    }

    if (params.count() >= min && params.count() <= max) {
        IrcCommand* cmd = new IrcCommand;
        cmd->setType(command.type);
        if (command.type == IrcCommand::Custom)
            params.prepend(command.command);
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
    if (!d->commands.isEmpty()) {
        d->commands.clear();
        emit commandsChanged(QStringList());
    }
}

/*!
    Resets the channels and the current target.

    \sa clear()
 */
void IrcCommandParser::reset()
{
    setChannels(QStringList());
    setTarget(QString());
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

    The commands are uppercased and in alphabetical order.

    \par Access function:
    \li QStringList <b>commands</b>() const

    \par Notifier signal:
    \li void <b>commandsChanged</b>(const QStringList& commands)

    \sa addCommand(), removeCommand()
 */
QStringList IrcCommandParser::commands() const
{
    Q_D(const IrcCommandParser);
    return d->commands.uniqueKeys();
}

/*!
    Returns syntax for the given \a command in given \a details level.
 */
QString IrcCommandParser::syntax(const QString& command, Details details) const
{
    Q_D(const IrcCommandParser);
    IrcCommandInfo info = d->find(command.toUpper()).value(0);
    if (!info.command.isEmpty()) {
        QString str = info.fullSyntax();
        if (details != Full) {
            if (details & NoTarget)
                str.remove(QRegExp("\\[[^\\]]+\\]"));
            if (details & NoPrefix)
                str.remove("#");
            if (details & NoEllipsis)
                str.remove("...");
            if (details & NoParentheses)
                str.remove("(").remove(")");
            if (details & NoBrackets)
                str.remove("[").remove("]");
            if (details & NoAngles)
                str.remove("<").remove(">");
        }
        return str.simplified();
    }
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
        const bool contains = d->commands.contains(cmd.command);
        d->commands.insert(cmd.command, cmd);
        if (!contains)
            emit commandsChanged(commands());
    }
}

/*!
    Removes the command with \a type and \a syntax.
 */
void IrcCommandParser::removeCommand(IrcCommand::Type type, const QString& syntax)
{
    Q_D(IrcCommandParser);
    bool changed = false;
    QMutableMapIterator<QString, IrcCommandInfo> it(d->commands);
    while (it.hasNext()) {
        IrcCommandInfo cmd = it.next().value();
        if (cmd.type == type && (syntax.isEmpty() || !syntax.compare(cmd.fullSyntax(), Qt::CaseInsensitive))) {
            it.remove();
            if (!d->commands.contains(cmd.command))
                changed = true;
        }
    }
    if (changed)
        emit commandsChanged(commands());
}

/*!
    This property holds the available channels.

    \par Access functions:
    \li QStringList <b>channels</b>() const
    \li void <b>setChannels</b>(const QStringList& channels) [slot]

    \par Notifier signal:
    \li void <b>channelsChanged</b>(const QStringList& channels)

    \sa IrcBufferModel::channels()
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

    \par Access functions:
    \li QString <b>target</b>() const
    \li void <b>setTarget</b>(const QString& target) [slot]

    \par Notifier signal:
    \li void <b>targetChanged</b>(const QString& target)
 */
QString IrcCommandParser::target() const
{
    Q_D(const IrcCommandParser);
    return d->target;
}

void IrcCommandParser::setTarget(const QString& target)
{
    Q_D(IrcCommandParser);
    if (d->target != target) {
        d->target = target;
        emit targetChanged(target);
    }
}

/*!
    This property holds the command triggers.

    \par Access functions:
    \li QStringList <b>triggers</b>() const
    \li void <b>setTriggers</b>(const QStringList& triggers) [slot]

    \par Notifier signal:
    \li void <b>triggersChanged</b>(const QStringList& triggers)
 */
QStringList IrcCommandParser::triggers() const
{
    Q_D(const IrcCommandParser);
    return d->triggers;
}

void IrcCommandParser::setTriggers(const QStringList& triggers)
{
    Q_D(IrcCommandParser);
    if (d->triggers != triggers) {
        d->triggers = triggers;
        emit triggersChanged(triggers);
    }
}

/*!
    \property bool IrcCommandParser::tolerant

    This property holds whether the parser is tolerant.

    A tolerant parser creates message commands out of input that does not
    start with a command trigger, and raw server commands when the input
    starts with a command trigger but the command is unrecognized. Known
    commands with invalid arguments are still considered invalid.

    The default value is \c false.

    \par Access functions:
    \li bool <b>isTolerant</b>() const
    \li void <b>setTolerant</b>(bool tolerant)

    \par Notifier signal:
    \li void <b>tolerancyChanged</b>(bool tolerant)

    \sa IrcCommand::Quote
 */
bool IrcCommandParser::isTolerant() const
{
    Q_D(const IrcCommandParser);
    return d->tolerant;
}

void IrcCommandParser::setTolerant(bool tolerant)
{
    Q_D(IrcCommandParser);
    if (d->tolerant != tolerant) {
        d->tolerant = tolerant;
        emit tolerancyChanged(tolerant);
    }
}

static bool processMessage(QString* input, const QStringList& triggers, bool tolerant)
{
    if (input->isEmpty())
        return false;
    if (triggers.isEmpty())
        return tolerant;

    foreach (const QString& trigger, triggers) {
        if (tolerant && trigger.length() == 1 && (input->startsWith(trigger.repeated(2)) || input->startsWith(trigger + QLatin1Char(' ')))) {
            // treat "//cmd" and "/ /cmd" as message (-> "/cmd")
            input->remove(0, 1);
            return true;
        } else if (input->startsWith(trigger)) {
            input->remove(0, trigger.length());
            return false;
        }
    }
    return tolerant;
}

/*!
    Parses and returns the command for \a input, or \c 0 if the input is not valid.
 */
IrcCommand* IrcCommandParser::parse(const QString& input) const
{
    Q_D(const IrcCommandParser);
    QString message = input;
    if (processMessage(&message, d->triggers, d->tolerant)) {
        return IrcCommand::createMessage(d->target, message.trimmed());
    } else {
        QStringList params = message.split(QLatin1Char(' '), QString::SkipEmptyParts);
        if (!params.isEmpty()) {
            const QString command = params.takeFirst().toUpper();
            const QList<IrcCommandInfo> commands = d->find(command);
            if (!commands.isEmpty()) {
                foreach (const IrcCommandInfo& c, commands) {
                    IrcCommand* cmd = d->parse(c, params);
                    if (cmd)
                        return cmd;
                }
            } else if (d->tolerant) {
                IrcCommandInfo info;
                info.type = IrcCommand::Quote;
                info.syntax = QLatin1String("(<parameters...>)");
                params.prepend(command);
                return d->parse(info, params);
            }
        }
    }
    return 0;
}

#include "moc_irccommandparser.cpp"

IRC_END_NAMESPACE
