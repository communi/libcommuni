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
#include <QStringList>

IrcMessage::IrcMessage(Type type) : t(type) { }
IrcMessage::~IrcMessage() { }

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

IrcPartMessage::IrcPartMessage(const QString& channel, const QString& reason) :
    IrcChannelMessage(Part, channel), rson(reason) { }

QString IrcPartMessage::toString() const
{
    if (rson.isEmpty())
        return QString("PART %1").arg(chan);
    return QString("PART %1 %2").arg(chan, rson);
}

IrcTopicMessage::IrcTopicMessage(const QString& channel, const QString& topic) :
    IrcChannelMessage(Topic, channel), tpc(topic) { }

QString IrcTopicMessage::toString() const
{
    if (tpc.isEmpty())
        return QString("TOPIC %1").arg(chan);
    return QString("TOPIC %1 :%2").arg(chan, tpc);
}

QString IrcNamesMessage::toString() const
{
    return QString("NAMES %1").arg(chan);
}

IrcListMessage::IrcListMessage(const QString& channel, const QString& server) :
    IrcChannelMessage(List, channel), srv(server) { }

QString IrcListMessage::toString() const
{
    if (srv.isEmpty())
        return QString("LIST %1").arg(chan);
    return QString("LIST %1 %2").arg(chan, srv);
}

IrcInviteMessage::IrcInviteMessage(const QString& channel, const QString& user) :
    IrcChannelMessage(Invite, channel), usr(user) { }

QString IrcInviteMessage::toString() const
{
    return QString("INVITE %1 %2").arg(usr, chan);
}

IrcKickMessage::IrcKickMessage(const QString& channel, const QString& user, const QString& reason) :
    IrcChannelMessage(Kick, channel), usr(user), rson(reason) { }

QString IrcKickMessage::toString() const
{
    if (rson.isEmpty())
        return QString("KICK %1 %2").arg(usr, chan);
    return QString("KICK %1 %2 :%3").arg(usr, chan, rson);
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

QString IrcUserModeMessage::toString() const
{
    return QString("MODE %1 %2").arg(tgt, mod);
}

IrcSendMessage::IrcSendMessage(Type type, const QString& message) :
    IrcMessage(type), msg(message) { }

QString IrcPrivateMessage::toString() const
{
    return QString("PRIVMSG %1 :%2").arg(tgt, msg);
}

QString IrcNoticeMessage::toString() const
{
    return QString("NOTICE %1 :%2").arg(tgt, msg);
}

IrcCtcpMessage::IrcCtcpMessage(Type type, const QString& message) :
    IrcMessage(type), msg(message) { }

QString IrcCtcpActionMessage::toString() const
{
    return QString("PRIVMSG %1 :\x01" "ACTION %2\x01").arg(tgt, msg);
}

QString IrcCtcpRequestMessage::toString() const
{
    return QString("PRIVMSG %1 :\x01%2\x01").arg(tgt, msg);
}

QString IrcCtcpReplyMessage::toString() const
{
    return QString("NOTICE %1 :\x01%2\x01").arg(tgt, msg);
}

IrcQueryMessage::IrcQueryMessage(Type type, const QString& user) :
    IrcMessage(type), usr(user) { }

QString IrcWhoMessage::toString() const
{
    return QString("WHO %1").arg(usr);
}

QString IrcWhoisMessage::toString() const
{
    return QString("WHOIS %1 %1").arg(usr);
}

QString IrcWhowasMessage::toString() const
{
    return QString("WHOWAS %1 %1").arg(usr);
}
