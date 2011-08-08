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

static QHash<QString, const QMetaObject*> irc_meta_init()
{
    QHash<QString, const QMetaObject*> meta;
    meta.insert("PASS", &IrcPasswordCommand::staticMetaObject);
    meta.insert("NICK", &IrcNickCommand::staticMetaObject);
    meta.insert("OPER", &IrcOperatorCommand::staticMetaObject);
    meta.insert("QUIT", &IrcQuitCommand::staticMetaObject);
    meta.insert("JOIN", &IrcJoinCommand::staticMetaObject);
    meta.insert("PART", &IrcPartCommand::staticMetaObject);
    meta.insert("TOPIC", &IrcTopicCommand::staticMetaObject);
    meta.insert("NAMES", &IrcNamesCommand::staticMetaObject);
    meta.insert("LIST", &IrcListCommand::staticMetaObject);
    meta.insert("INVITE", &IrcInviteCommand::staticMetaObject);
    meta.insert("KICK", &IrcKickCommand::staticMetaObject);
    meta.insert("MODE", &IrcModeCommand::staticMetaObject);
    meta.insert("PRIVMSG", &IrcMessageCommand::staticMetaObject);
    meta.insert("NOTICE", &IrcNoticeCommand::staticMetaObject);
    meta.insert("QUERY", &IrcQueryCommand::staticMetaObject);
    meta.insert("WHO", &IrcWhoCommand::staticMetaObject);
    meta.insert("WHOIS", &IrcWhoisCommand::staticMetaObject);
    meta.insert("WHOWAS", &IrcWhowasCommand::staticMetaObject);
    meta.insert("PING", &IrcPingCommand::staticMetaObject);
    meta.insert("PONG", &IrcPongCommand::staticMetaObject);
    meta.insert("AWAY", &IrcAwayCommand::staticMetaObject);
    return meta;
}
static QHash<QString, const QMetaObject*> metaObjects = irc_meta_init();

IrcCommand* IrcCommand::create(const QString& command, QObject* parent)
{
    IrcCommand* cmd = 0;
    const QMetaObject* metaObject = metaObjects.value(command.toUpper());
    if (metaObject)
        cmd = qobject_cast<IrcCommand*>(metaObject->newInstance(Q_ARG(QObject*, parent)));
    return cmd;
}

QString IrcCommand::toString() const
{
    return QString("UNKNOWN %1").arg(params.join(" "));
}

QString IrcPasswordCommand::toString() const
{
    return QString("PASS %1").arg(passwd);
}

QString IrcNickCommand::toString() const
{
    return QString("NICK %1").arg(n);
}

QString IrcUserCommand::toString() const
{
    // RFC 1459 states that "hostname and servername are normally
    // ignored by the IRC server when the USER command comes from
    // a directly connected client (for security reasons)", therefore
    // we don't need them.
    return QString("USER %1 hostname servername :%2").arg(user, real);
}

QString IrcOperatorCommand::toString() const
{
    return QString("OPER %1 %2").arg(usr, passwd);
}

QString IrcQuitCommand::toString() const
{
    return QString("QUIT :%1").arg(rson);
}

QString IrcJoinCommand::toString() const
{
    if (k.isEmpty())
        return QString("JOIN %1").arg(chan);
    return QString("JOIN %1 %2").arg(chan, k);
}

QString IrcPartCommand::toString() const
{
    if (rson.isEmpty())
        return QString("PART %1").arg(chan);
    return QString("PART %1 :%2").arg(chan, rson);
}

QString IrcTopicCommand::toString() const
{
    if (tpc.isEmpty())
        return QString("TOPIC %1").arg(chan);
    return QString("TOPIC %1 :%2").arg(chan, tpc);
}

QString IrcNamesCommand::toString() const
{
    return QString("NAMES %1").arg(chan);
}

QString IrcListCommand::toString() const
{
    if (srv.isEmpty())
        return QString("LIST %1").arg(chan);
    return QString("LIST %1 %2").arg(chan, srv);
}

QString IrcInviteCommand::toString() const
{
    return QString("INVITE %1 %2").arg(usr, chan);
}

QString IrcKickCommand::toString() const
{
    if (rson.isEmpty())
        return QString("KICK %1 %2").arg(usr, chan);
    return QString("KICK %1 %2 :%3").arg(usr, chan, rson);
}

QString IrcModeCommand::toString() const
{
    QStringList lst;
    lst << QString("MODE") << tgt << mod << arg << msk;
    while (lst.last().isEmpty())
        lst.removeLast();
    return lst.join(" ");
}

QString IrcMessageCommand::toString() const
{
    QString copy(msg);
    if (act)
        copy = QString("\1ACTION %1\x01").arg(msg);
    else if (req)
        copy = QString("\1%1\1").arg(msg);
    return QString("PRIVMSG %1 :%2").arg(tgt, copy);
}

QString IrcNoticeCommand::toString() const
{
    QString copy(msg);
    if (rpl)
        copy = QString("\1%1\1").arg(msg);
    return QString("NOTICE %1 :%2").arg(tgt, copy);
}

QString IrcQueryCommand::toString() const
{
    return QString("QUERY %1").arg(usr);
}

QString IrcWhoCommand::toString() const
{
    return QString("WHO %1").arg(usr);
}

QString IrcWhoisCommand::toString() const
{
    return QString("WHOIS %1 %1").arg(usr);
}

QString IrcWhowasCommand::toString() const
{
    return QString("WHOWAS %1 %1").arg(usr);
}

QString IrcPingCommand::toString() const
{
    return QString("PING %1").arg(tgt);
}

QString IrcPongCommand::toString() const
{
    return QString("PONG %1").arg(tgt);
}

QString IrcAwayCommand::toString() const
{
    return QString("AWAY :%1").arg(msg);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcCommand* command)
{
    if (!command)
        return debug << "IrcCommand(0x0) ";
    debug.nospace() << command->metaObject()->className() << '(' << (void*) command;
    if (!command->objectName().isEmpty())
        debug << ", name = " << command->objectName();
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM
