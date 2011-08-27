/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#include "irccommand.h"
#include <QDebug>

class IrcCommandPrivate
{
public:
    IrcCommand::Type type;
    QStringList parameters;

    static IrcCommand* createCommand(IrcCommand::Type type, const QStringList& parameters);
};

IrcCommand* IrcCommandPrivate::createCommand(IrcCommand::Type type, const QStringList& parameters)
{
    IrcCommand* command = new IrcCommand;
    command->setType(type);
    command->setParameters(parameters);
    return command;
}

IrcCommand::IrcCommand(QObject* parent) : QObject(parent), d_ptr(new IrcCommandPrivate)
{
    Q_D(IrcCommand);
    d->type = Unknown;
}

IrcCommand::~IrcCommand()
{
}

IrcCommand::Type IrcCommand::type() const
{
    Q_D(const IrcCommand);
    return d->type;
}

void IrcCommand::setType(Type type)
{
    Q_D(IrcCommand);
    d->type = type;
}

QStringList IrcCommand::parameters() const
{
    Q_D(const IrcCommand);
    return d->parameters;
}

void IrcCommand::setParameters(QStringList parameters)
{
    Q_D(IrcCommand);
    d->parameters = parameters;
}

QString IrcCommand::toString() const
{
    Q_D(const IrcCommand);
    const QString p0 = d->parameters.value(0);
    const QString p1 = d->parameters.value(1);
    const QString p2 = d->parameters.value(2);

    switch (d->type)
    {
    case Password:      return QString("PASS %1").arg(p0); // pass
    case Nick:          return QString("NICK %1").arg(p0); // nick
    case User:          return QString("USER %1 hostname servername :%2").arg(p0, p1); // user, real
    case Operator:      return QString("OPER %1 %2").arg(p0, p1); // user, passwd
    case Quit:          return QString("QUIT :%1").arg(p0); // reason
    case Join:          return p1.isNull() ? QString("JOIN %1").arg(p0) : QString("JOIN %1 %2").arg(p0, p1); // chan, key
    case Part:          return p1.isNull() ? QString("PART %1").arg(p0) : QString("PART %1 :%2").arg(p0, p1); // chan, reason
    case Topic:         return p1.isNull() ? QString("TOPIC %1").arg(p0) : QString("TOPIC %1 :%2").arg(p0, p1); // chan, topic
    case Names:         return QString("NAMES %1").arg(p0); // chan
    case List:          return p1.isNull() ? QString("LIST %1").arg(p0) : QString("LIST %1 %2").arg(p0, p1); // chan, server
    case Invite:        return QString("INVITE %1 %2").arg(p0, p1); // user, chan
    case Kick:          return p2.isNull() ? QString("KICK %1 %2").arg(p0, p1) : QString("KICK %1 %2 :%3").arg(p0, p1, p2); // user, chan, reason
    case Mode:          return QString("MODE ") + d->parameters.join(" "); // target, mode, arg, mask
    case Message:       return QString("PRIVMSG %1 :%2").arg(p0, p1); // target, msg
    case Notice:        return QString("NOTICE %1 :%2").arg(p0, p1); // target, msg
    case CtcpAction:    return QString("PRIVMSG %1 :\1ACTION %2\1").arg(p0, p1); // target, msg
    case CtcpRequest:   return QString("PRIVMSG %1 :\1%2\1").arg(p0, p1); // target, msg
    case CtcpReply:     return QString("NOTICE %1 :\1%2\1").arg(p0, p1); // target, msg
    case Who:           return QString("WHO %1").arg(p0); // user
    case Whois:         return QString("WHOIS %1 %1").arg(p0); // user
    case Whowas:        return QString("WHOWAS %1 %1").arg(p0); // user
    case Ping:          return QString("PING %1").arg(p0); // target
    case Pong:          return QString("PONG %1").arg(p0); // target
    case Away:          return QString("AWAY :%1").arg(p0); // reason
    case Unknown:
    default:            return QString();
    }
}

IrcCommand* IrcCommand::createPassword(const QString& password)
{
    return IrcCommandPrivate::createCommand(Password, QStringList() << password);
}

IrcCommand* IrcCommand::createNick(const QString& nick)
{
    return IrcCommandPrivate::createCommand(Nick, QStringList() << nick);
}

IrcCommand* IrcCommand::createUser(const QString& userName, const QString& realName)
{
    return IrcCommandPrivate::createCommand(User, QStringList() << userName << realName);
}

IrcCommand* IrcCommand::createOperator(const QString& user, const QString& password)
{
    return IrcCommandPrivate::createCommand(Operator, QStringList() << user << password);
}

IrcCommand* IrcCommand::createQuit(const QString& reason)
{
    return IrcCommandPrivate::createCommand(Quit, QStringList() << reason);
}

IrcCommand* IrcCommand::createJoin(const QString& channel, const QString& key)
{
    return IrcCommandPrivate::createCommand(Join, QStringList() << channel << key);
}

IrcCommand* IrcCommand::createPart(const QString& channel, const QString& reason)
{
    return IrcCommandPrivate::createCommand(Part, QStringList() << channel << reason);
}

IrcCommand* IrcCommand::createTopic(const QString& channel, const QString& topic)
{
    return IrcCommandPrivate::createCommand(Topic, QStringList() << channel << topic);
}

IrcCommand* IrcCommand::createNames(const QString& channel)
{
    return IrcCommandPrivate::createCommand(Names, QStringList() << channel);
}

IrcCommand* IrcCommand::createList(const QString& channel, const QString& server)
{
    return IrcCommandPrivate::createCommand(List, QStringList() << channel << server);
}

IrcCommand* IrcCommand::createInvite(const QString& user, const QString& channel)
{
    return IrcCommandPrivate::createCommand(Invite, QStringList() << user << channel);
}

IrcCommand* IrcCommand::createKick(const QString& user, const QString& channel, const QString& reason)
{
    return IrcCommandPrivate::createCommand(Kick, QStringList() << user << channel << reason);
}

IrcCommand* IrcCommand::createMode(const QString& target, const QString& mode, const QString& arg, const QString& mask)
{
    return IrcCommandPrivate::createCommand(Mode, QStringList() << target << mode << arg << mask);
}

IrcCommand* IrcCommand::createMessage(const QString& target, const QString& message)
{
    return IrcCommandPrivate::createCommand(Message, QStringList() << target << message);
}

IrcCommand* IrcCommand::createNotice(const QString& target, const QString& message)
{
    return IrcCommandPrivate::createCommand(Notice, QStringList() << target << message);
}

IrcCommand* IrcCommand::createCtcpAction(const QString& target, const QString& action)
{
    return IrcCommandPrivate::createCommand(CtcpAction, QStringList() << target << action);
}

IrcCommand* IrcCommand::createCtcpRequest(const QString& target, const QString& request)
{
    return IrcCommandPrivate::createCommand(CtcpRequest, QStringList() << target << request);
}

IrcCommand* IrcCommand::createCtcpReply(const QString& target, const QString& reply)
{
    return IrcCommandPrivate::createCommand(CtcpReply, QStringList() << target << reply);
}

IrcCommand* IrcCommand::createWho(const QString& user)
{
    return IrcCommandPrivate::createCommand(Who, QStringList() << user);
}

IrcCommand* IrcCommand::createWhois(const QString& user)
{
    return IrcCommandPrivate::createCommand(Whois, QStringList() << user);
}

IrcCommand* IrcCommand::createWhowas(const QString& user)
{
    return IrcCommandPrivate::createCommand(Whowas, QStringList() << user);
}

IrcCommand* IrcCommand::createPing(const QString& target)
{
    return IrcCommandPrivate::createCommand(Ping, QStringList() << target);
}

IrcCommand* IrcCommand::createPong(const QString& target)
{
    return IrcCommandPrivate::createCommand(Pong, QStringList() << target);
}

IrcCommand* IrcCommand::createAway(const QString& reason)
{
    return IrcCommandPrivate::createCommand(Away, QStringList() << reason);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcCommand* command)
{
    if (!command)
        return debug << "IrcCommand(0x0) ";
    debug.nospace() << command->metaObject()->className() << '(' << (void*) command;
    if (!command->objectName().isEmpty())
        debug << ", name = " << command->objectName();
    QString str = command->toString();
    if (!str.isEmpty())
        debug << "'" << str.left(20) << "'";
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM
