/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
* Copyright (C) 2010-2011 SmokeX smokexjc@gmail.com
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

#include "ircmessage.h"
#include <QDebug>

static QHash<QString, const QMetaObject*> irc_meta_init()
{
    QHash<QString, const QMetaObject*> meta;
    meta.insert("PASS", &IrcPasswordMessage::staticMetaObject);
    meta.insert("NICK", &IrcNickNameMessage::staticMetaObject);
    meta.insert("OPER", &IrcOperatorMessage::staticMetaObject);
    meta.insert("QUIT", &IrcQuitMessage::staticMetaObject);
    meta.insert("JOIN", &IrcJoinMessage::staticMetaObject);
    meta.insert("PART", &IrcPartMessage::staticMetaObject);
    meta.insert("TOPIC", &IrcTopicMessage::staticMetaObject);
    meta.insert("NAMES", &IrcNamesMessage::staticMetaObject);
    meta.insert("LIST", &IrcListMessage::staticMetaObject);
    meta.insert("INVITE", &IrcInviteMessage::staticMetaObject);
    meta.insert("KICK", &IrcKickMessage::staticMetaObject);
    meta.insert("MODE", &IrcModeMessage::staticMetaObject);
    meta.insert("PRIVMSG", &IrcPrivateMessage::staticMetaObject);
    meta.insert("NOTICE", &IrcNoticeMessage::staticMetaObject);
    meta.insert("WHO", &IrcWhoMessage::staticMetaObject);
    meta.insert("WHOIS", &IrcWhoisMessage::staticMetaObject);
    meta.insert("WHOWAS", &IrcWhowasMessage::staticMetaObject);
    meta.insert("PING", &IrcPingMessage::staticMetaObject);
    meta.insert("PONG", &IrcPongMessage::staticMetaObject);
    meta.insert("ERROR", &IrcErrorMessage::staticMetaObject);
    meta.insert("AWAY", &IrcAwayMessage::staticMetaObject);
    // TODO: numeric?
    return meta;
}
QHash<QString, const QMetaObject*> IrcMessage::metaObjects = irc_meta_init();

void IrcMessage::unregisterCommand(const QString& command)
{
    metaObjects.remove(command);
}

IrcMessage* IrcMessage::create(const QString& command, QObject* parent)
{
    QObject* message = 0;
    const QMetaObject* metaObject = metaObjects.value(command);
    if (metaObject)
        message = metaObject->newInstance(Q_ARG(QObject*, parent));
    return qobject_cast<IrcMessage*>(message);
}

QString IrcMessage::toString() const
{
    return QString("UNKNOWN %1").arg(params.join(" "));
}

void IrcMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    params = parameters;
    pfx = prefix;
}

QString IrcPasswordMessage::toString() const
{
    return QString("PASS %1").arg(passwd);
}

void IrcPasswordMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    passwd = parameters.value(0);
}

QString IrcNickNameMessage::toString() const
{
    return QString("NICK %1").arg(nick);
}

void IrcNickNameMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    nick = parameters.value(0);
}

QString IrcUserMessage::toString() const
{
    // RFC 1459 states that "hostname and servername are normally
    // ignored by the IRC server when the USER command comes from
    // a directly connected client (for security reasons)", therefore
    // we don't need them.
    return QString("USER %1 hostname servername :%2").arg(user, real);
}

void IrcUserMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    user = parameters.value(0);
    real = parameters.value(3);
}

QString IrcOperatorMessage::toString() const
{
    return QString("OPER %1 %2").arg(usr, passwd);
}

void IrcOperatorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    passwd = parameters.value(1);
}

QString IrcQuitMessage::toString() const
{
    return QString("QUIT :%1").arg(rson);
}

void IrcQuitMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    rson = parameters.value(0);
}

void IrcChannelMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    chan = parameters.value(0);
}

QString IrcJoinMessage::toString() const
{
    if (k.isEmpty())
        return QString("JOIN %1").arg(chan);
    return QString("JOIN %1 %2").arg(chan, k);
}

void IrcJoinMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
}

QString IrcPartMessage::toString() const
{
    if (rson.isEmpty())
        return QString("PART %1").arg(chan);
    return QString("PART %1 %2").arg(chan, rson);
}

void IrcPartMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
    rson = parameters.value(1);
}

QString IrcTopicMessage::toString() const
{
    if (tpc.isEmpty())
        return QString("TOPIC %1").arg(chan);
    return QString("TOPIC %1 :%2").arg(chan, tpc);
}

void IrcTopicMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
    tpc = parameters.value(1);
}

QString IrcNamesMessage::toString() const
{
    return QString("NAMES %1").arg(chan);
}

void IrcNamesMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
}

QString IrcListMessage::toString() const
{
    if (srv.isEmpty())
        return QString("LIST %1").arg(chan);
    return QString("LIST %1 %2").arg(chan, srv);
}

void IrcListMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
    srv = parameters.value(1);
}

QString IrcInviteMessage::toString() const
{
    return QString("INVITE %1 %2").arg(usr, chan);
}

void IrcInviteMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
}

QString IrcKickMessage::toString() const
{
    if (rson.isEmpty())
        return QString("KICK %1 %2").arg(usr, chan);
    return QString("KICK %1 %2 :%3").arg(usr, chan, rson);
}

void IrcKickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
    rson = parameters.value(2);
}

void IrcModeMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    mod = parameters.value(1);
}

QString IrcChannelModeMessage::toString() const
{
    QStringList lst;
    lst << QString("MODE") << tgt << mod << arg << msk;
    while (lst.last().isEmpty())
        lst.removeLast();
    return lst.join(" ");
}

void IrcChannelModeMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcModeMessage::initFrom(prefix, parameters);
    arg = parameters.value(2);
    msk = parameters.value(3);
}

QString IrcUserModeMessage::toString() const
{
    return QString("MODE %1 %2").arg(tgt, mod);
}

void IrcUserModeMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcModeMessage::initFrom(prefix, parameters);
}

void IrcSendMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    msg = parameters.value(1);
}

QString IrcPrivateMessage::toString() const
{
    return QString("PRIVMSG %1 :%2").arg(tgt, msg);
}

void IrcPrivateMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcSendMessage::initFrom(prefix, parameters);
}

QString IrcNoticeMessage::toString() const
{
    return QString("NOTICE %1 :%2").arg(tgt, msg);
}

void IrcNoticeMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcSendMessage::initFrom(prefix, parameters);
}

void IrcCtcpMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    msg = parameters.value(1);
}

QString IrcCtcpActionMessage::toString() const
{
    return QString("PRIVMSG %1 :\1ACTION %2\x01").arg(tgt, msg);
}

void IrcCtcpActionMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcCtcpMessage::initFrom(prefix, parameters);
    if (msg.startsWith("\1ACTION "))
        msg.remove(0, 8);
    if (msg.endsWith('\1'))
        msg.chop(1);
}

QString IrcCtcpRequestMessage::toString() const
{
    return QString("PRIVMSG %1 :\1%2\1").arg(tgt, msg);
}

void IrcCtcpRequestMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcCtcpMessage::initFrom(prefix, parameters);
    if (msg.startsWith('\1'))
        msg.remove(0, 1);
    if (msg.endsWith('\1'))
        msg.chop(1);
}

QString IrcCtcpReplyMessage::toString() const
{
    return QString("NOTICE %1 :\1%2\1").arg(tgt, msg);
}

void IrcCtcpReplyMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcCtcpMessage::initFrom(prefix, parameters);
    if (msg.startsWith('\1'))
        msg.remove(0, 1);
    if (msg.endsWith('\1'))
        msg.chop(1);
}

void IrcQueryMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
}

QString IrcWhoMessage::toString() const
{
    return QString("WHO %1").arg(usr);
}

void IrcWhoMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcWhoisMessage::toString() const
{
    return QString("WHOIS %1 %1").arg(usr);
}

void IrcWhoisMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcWhowasMessage::toString() const
{
    return QString("WHOWAS %1 %1").arg(usr);
}

void IrcWhowasMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcPingMessage::toString() const
{
    return QString("PING %1").arg(tgt);
}

void IrcPingMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
}

QString IrcPongMessage::toString() const
{
    return QString("PONG %1").arg(tgt);
}

void IrcPongMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
}

QString IrcErrorMessage::toString() const
{
    return QString("ERROR :%1").arg(err);
}

void IrcErrorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    err = parameters.value(0);
}

QString IrcNumericMessage::toString() const
{
    return QString("%1 :%2").arg(c).arg(params.join(" "));
}

void IrcNumericMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    c = parameters.value(0).toInt();
    params = parameters.mid(1);
}

QString IrcAwayMessage::toString() const
{
    return QString("AWAY :%1").arg(msg);
}

void IrcAwayMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    msg = parameters.value(0);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcMessage* message)
{
    if (!message)
        return debug << "IrcMessage(0x0) ";
    debug.nospace() << message->metaObject()->className() << '(' << (void*) message;
    if (!message->objectName().isEmpty())
        debug << ", name = " << message->objectName();
    if (!message->prefix().isEmpty())
        debug << ", prefix = " << message->prefix();
    if (!message->parameters().isEmpty())
        debug << ", params = " << message->parameters();
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM
