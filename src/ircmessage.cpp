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

IrcMessage::IrcMessage(Type type) : t(type)
{
}

IrcMessage::~IrcMessage()
{
}

IrcChannelMessage::IrcChannelMessage(Type type, const QString& channel) :
    IrcMessage(type), chan(channel)
{
}

IrcTopicMessage::IrcTopicMessage(const QString& channel, const QString& topic) :
    IrcChannelMessage(Topic, channel), tpc(topic)
{
}

IrcListMessage::IrcListMessage(const QString& channel, const QString& server) :
    IrcChannelMessage(List, channel), srv(server)
{
}

IrcInviteMessage::IrcInviteMessage(const QString& channel, const QString& user) :
    IrcChannelMessage(Invite, channel), usr(user)
{
}

IrcKickMessage::IrcKickMessage(const QString& channel, const QString& user, const QString& reason) :
    IrcChannelMessage(Kick, channel), usr(user), rson(reason)
{
}

IrcModeMessage::IrcModeMessage(Type type, const QString& target, const QString& mode) :
    IrcMessage(type), tgt(target), mod(mode)
{
}

IrcChannelModeMessage::IrcChannelModeMessage(const QString& channel, const QString& mode,
                                             const QString& argument, const QString& mask) :
    IrcModeMessage(ChannelMode, channel, mode), arg(argument), msk(mask)
{
}

IrcSendMessage::IrcSendMessage(Type type, const QString& message) :
    IrcMessage(type), msg(message)
{
}

IrcCtcpMessage::IrcCtcpMessage(Type type, const QString& message) :
    IrcMessage(type), msg(message)
{
}

IrcQueryMessage::IrcQueryMessage(Type type, const QString& user) :
    IrcMessage(type), usr(user)
{
}
