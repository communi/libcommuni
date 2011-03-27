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
#include <QMetaEnum>
#include <QDebug>

IrcMessage::IrcMessage(Type type) : t(type) { }
IrcMessage::~IrcMessage() { }

IrcPasswordMessage::IrcPasswordMessage(const QString& password) :
    IrcMessage(Password), passwd(password) { }

QString IrcPasswordMessage::toString() const
{
    return QString("PASS %1").arg(passwd);
}

IrcPasswordMessage IrcPasswordMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString password = params.value(0);
    IrcPasswordMessage msg(password);
    msg.pfx = prefix;
    return msg;
}

IrcNickNameMessage::IrcNickNameMessage(const QString& nickName) :
    IrcMessage(NickName), nick(nickName) { }

QString IrcNickNameMessage::toString() const
{
    return QString("NICK %1").arg(nick);
}

IrcNickNameMessage IrcNickNameMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString nickName = params.value(0);
    IrcNickNameMessage msg(nickName);
    msg.pfx = prefix;
    return msg;
}

IrcUserMessage::IrcUserMessage(const QString& userName, const QString& realName) :
    IrcMessage(User), user(userName), real(realName) { }

QString IrcUserMessage::toString() const
{
    // RFC 1459 states that "hostname and servername are normally
    // ignored by the IRC server when the USER command comes from
    // a directly connected client (for security reasons)", therefore
    // we don't need them.
    return QString("USER %1 hostname servername :%2").arg(user, real);
}

IrcUserMessage IrcUserMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString userName = params.value(0);
    const QString realName = params.value(3);
    IrcUserMessage msg(userName, realName);
    msg.pfx = prefix;
    return msg;
}

IrcOperatorMessage::IrcOperatorMessage(const QString& user, const QString& password) :
    IrcMessage(Operator), usr(user), passwd(password) { }

QString IrcOperatorMessage::toString() const
{
    return QString("OPER %1 %2").arg(usr, passwd);
}

IrcOperatorMessage IrcOperatorMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    const QString password = params.value(1);
    IrcOperatorMessage msg(user, password);
    msg.pfx = prefix;
    return msg;
}

IrcQuitMessage::IrcQuitMessage(const QString& reason) :
    IrcMessage(Quit), rson(reason) { }

QString IrcQuitMessage::toString() const
{
    return QString("QUIT :%1").arg(rson);
}

IrcQuitMessage IrcQuitMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString reason = params.value(0);
    IrcQuitMessage msg(reason);
    msg.pfx = prefix;
    return msg;
}

IrcChannelMessage::IrcChannelMessage(Type type, const QString& channel) :
    IrcMessage(type), chan(channel) { }

IrcJoinMessage::IrcJoinMessage(const QString& channel, const QString& key) :
    IrcChannelMessage(Join, channel), k(key) { }

QString IrcJoinMessage::toString() const
{
    if (k.isEmpty())
        return QString("JOIN %1").arg(chan);
    return QString("JOIN %1 %2").arg(chan, k);
}

IrcJoinMessage IrcJoinMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString channel = params.value(0);
    IrcJoinMessage msg(channel);
    msg.pfx = prefix;
    return msg;
}

IrcPartMessage::IrcPartMessage(const QString& channel, const QString& reason) :
    IrcChannelMessage(Part, channel), rson(reason) { }

QString IrcPartMessage::toString() const
{
    if (rson.isEmpty())
        return QString("PART %1").arg(chan);
    return QString("PART %1 %2").arg(chan, rson);
}

IrcPartMessage IrcPartMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString channel = params.value(0);
    const QString reason = params.value(1);
    IrcPartMessage msg(channel, reason);
    msg.pfx = prefix;
    return msg;
}

IrcTopicMessage::IrcTopicMessage(const QString& channel, const QString& topic) :
    IrcChannelMessage(Topic, channel), tpc(topic) { }

QString IrcTopicMessage::toString() const
{
    if (tpc.isEmpty())
        return QString("TOPIC %1").arg(chan);
    return QString("TOPIC %1 :%2").arg(chan, tpc);
}

IrcTopicMessage IrcTopicMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString channel = params.value(0);
    const QString topic = params.value(1);
    IrcTopicMessage msg(channel, topic);
    msg.pfx = prefix;
    return msg;
}

QString IrcNamesMessage::toString() const
{
    return QString("NAMES %1").arg(chan);
}

IrcNamesMessage IrcNamesMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString channel = params.value(0);
    IrcNamesMessage msg(channel);
    msg.pfx = prefix;
    return msg;
}

IrcListMessage::IrcListMessage(const QString& channel, const QString& server) :
    IrcChannelMessage(List, channel), srv(server) { }

QString IrcListMessage::toString() const
{
    if (srv.isEmpty())
        return QString("LIST %1").arg(chan);
    return QString("LIST %1 %2").arg(chan, srv);
}

IrcListMessage IrcListMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString channel = params.value(0);
    const QString server = params.value(1);
    IrcListMessage msg(channel, server);
    msg.pfx = prefix;
    return msg;
}

IrcInviteMessage::IrcInviteMessage(const QString& channel, const QString& user) :
    IrcChannelMessage(Invite, channel), usr(user) { }

QString IrcInviteMessage::toString() const
{
    return QString("INVITE %1 %2").arg(usr, chan);
}

IrcInviteMessage IrcInviteMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    const QString channel = params.value(1);
    IrcInviteMessage msg(channel, user);
    msg.pfx = prefix;
    return msg;
}

IrcKickMessage::IrcKickMessage(const QString& channel, const QString& user, const QString& reason) :
    IrcChannelMessage(Kick, channel), usr(user), rson(reason) { }

QString IrcKickMessage::toString() const
{
    if (rson.isEmpty())
        return QString("KICK %1 %2").arg(usr, chan);
    return QString("KICK %1 %2 :%3").arg(usr, chan, rson);
}

IrcKickMessage IrcKickMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    const QString channel = params.value(1);
    const QString reason = params.value(2);
    IrcKickMessage msg(channel, user, reason);
    msg.pfx = prefix;
    return msg;
}

IrcModeMessage::IrcModeMessage(Type type, const QString& target, const QString& mode) :
    IrcMessage(type), tgt(target), mod(mode) { }

IrcChannelModeMessage::IrcChannelModeMessage(const QString& channel, const QString& mode,
                                             const QString& argument, const QString& mask) :
    IrcModeMessage(ChannelMode, channel, mode), arg(argument), msk(mask) { }

QString IrcChannelModeMessage::toString() const
{
    QStringList lst;
    lst << QString("MODE") << tgt << mod << arg << msk;
    while (lst.last().isEmpty())
        lst.removeLast();
    return lst.join(" ");
}

IrcChannelModeMessage IrcChannelModeMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString channel = params.value(0);
    const QString mode = params.value(1);
    const QString argument = params.value(2);
    const QString mask = params.value(3);
    IrcChannelModeMessage msg(channel, mode, argument, mask);
    msg.pfx = prefix;
    return msg;
}

QString IrcUserModeMessage::toString() const
{
    return QString("MODE %1 %2").arg(tgt, mod);
}

IrcUserModeMessage IrcUserModeMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    const QString mode = params.value(1);
    IrcUserModeMessage msg(user, mode);
    msg.pfx = prefix;
    return msg;
}

IrcSendMessage::IrcSendMessage(Type type, const QString& target, const QString& message) :
    IrcMessage(type), tgt(target), msg(message) { }

QString IrcPrivateMessage::toString() const
{
    return QString("PRIVMSG %1 :%2").arg(tgt, msg);
}

IrcPrivateMessage IrcPrivateMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    const QString message = params.value(1);
    IrcPrivateMessage msg(target, message);
    msg.pfx = prefix;
    return msg;
}

QString IrcNoticeMessage::toString() const
{
    return QString("NOTICE %1 :%2").arg(tgt, msg);
}

IrcNoticeMessage IrcNoticeMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    const QString message = params.value(1);
    IrcNoticeMessage msg(target, message);
    msg.pfx = prefix;
    return msg;
}

IrcCtcpMessage::IrcCtcpMessage(Type type, const QString& target, const QString& message) :
    IrcMessage(type), tgt(target), msg(message) { }

QString IrcCtcpActionMessage::toString() const
{
    return QString("PRIVMSG %1 :\1ACTION %2\x01").arg(tgt, msg);
}

IrcCtcpActionMessage IrcCtcpActionMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    QString message = params.value(1);
    if (message.startsWith("\1ACTION "))
        message.remove(0, 8);
    if (message.endsWith('\1'))
        message.chop(1);
    IrcCtcpActionMessage msg(target, message);
    msg.pfx = prefix;
    return msg;
}

QString IrcCtcpRequestMessage::toString() const
{
    return QString("PRIVMSG %1 :\1%2\1").arg(tgt, msg);
}

IrcCtcpRequestMessage IrcCtcpRequestMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    QString message = params.value(1);
    if (message.startsWith('\1'))
        message.remove(0, 1);
    if (message.endsWith('\1'))
        message.chop(1);
    IrcCtcpRequestMessage msg(target, message);
    msg.pfx = prefix;
    return msg;
}

QString IrcCtcpReplyMessage::toString() const
{
    return QString("NOTICE %1 :\1%2\1").arg(tgt, msg);
}

IrcCtcpReplyMessage IrcCtcpReplyMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    QString message = params.value(1);
    if (message.startsWith('\1'))
        message.remove(0, 1);
    if (message.endsWith('\1'))
        message.chop(1);
    IrcCtcpReplyMessage msg(target, message);
    msg.pfx = prefix;
    return msg;
}

IrcQueryMessage::IrcQueryMessage(Type type, const QString& user) :
    IrcMessage(type), usr(user) { }

QString IrcWhoMessage::toString() const
{
    return QString("WHO %1").arg(usr);
}

IrcWhoMessage IrcWhoMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    IrcWhoMessage msg(user);
    msg.pfx = prefix;
    return msg;
}

QString IrcWhoisMessage::toString() const
{
    return QString("WHOIS %1 %1").arg(usr);
}

IrcWhoisMessage IrcWhoisMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    IrcWhoisMessage msg(user);
    msg.pfx = prefix;
    return msg;
}

QString IrcWhowasMessage::toString() const
{
    return QString("WHOWAS %1 %1").arg(usr);
}

IrcWhowasMessage IrcWhowasMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString user = params.value(0);
    IrcWhowasMessage msg(user);
    msg.pfx = prefix;
    return msg;
}

IrcPingMessage::IrcPingMessage(const QString& target) :
    IrcMessage(Ping), tgt(target) { }

QString IrcPingMessage::toString() const
{
    return QString("PING %1").arg(tgt);
}

IrcPingMessage IrcPingMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    IrcPingMessage msg(target);
    msg.pfx = prefix;
    return msg;
}

IrcPongMessage::IrcPongMessage(const QString& target) :
    IrcMessage(Pong), tgt(target) { }

QString IrcPongMessage::toString() const
{
    return QString("PONG %1").arg(tgt);
}

IrcPongMessage IrcPongMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString target = params.value(0);
    IrcPongMessage msg(target);
    msg.pfx = prefix;
    return msg;
}

IrcErrorMessage::IrcErrorMessage(const QString& error) :
    IrcMessage(Error), err(error) { }

QString IrcErrorMessage::toString() const
{
    return QString("ERROR :%1").arg(err);
}

IrcErrorMessage IrcErrorMessage::fromString(const QString& prefix, const QStringList& params)
{
    const QString error = params.value(0);
    IrcErrorMessage msg(error);
    msg.pfx = prefix;
    return msg;
}

IrcNumericMessage::IrcNumericMessage(uint code, const QStringList& params) :
    IrcMessage(Numeric), c(code), p(params) { }

QString IrcNumericMessage::toString() const
{
    return QString("%1 :%2").arg(c).arg(p.join(" "));
}

IrcNumericMessage IrcNumericMessage::fromString(const QString& prefix, const QStringList& params)
{
    const uint code = params.value(0).toInt();
    IrcNumericMessage msg(code, params.mid(1));
    msg.pfx = prefix;
    return msg;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcMessage& message)
{
    int index = message.staticMetaObject.indexOfEnumerator("Type");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = message.staticMetaObject.enumerator(index);

    debug.nospace() << message.staticMetaObject.className();
    debug << '(' << enumerator.valueToKey(message.type()) << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM
