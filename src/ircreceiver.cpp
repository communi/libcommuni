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

#include "ircreceiver.h"
#include <QDebug>

void IrcReceiver::receiveMessage(const IrcMessage* message)
{
    if (!message)
    {
        qCritical() << "IrcReceiver::receiveMessage(" << message << ")";
        return;
    }

    // TODO: filters

    switch (message->type())
    {
    case IrcMessage::CtcpAction:
        ctcpActionMessage(static_cast<const IrcCtcpActionMessage*>(message));
        break;
    case IrcMessage::CtcpReply:
        ctcpReplyMessage(static_cast<const IrcCtcpReplyMessage*>(message));
        break;
    case IrcMessage::CtcpRequest:
        ctcpRequestMessage(static_cast<const IrcCtcpRequestMessage*>(message));
        break;
    case IrcMessage::Invite:
        inviteMessage(static_cast<const IrcInviteMessage*>(message));
        break;
    case IrcMessage::Join:
        joinMessage(static_cast<const IrcJoinMessage*>(message));
        break;
    case IrcMessage::Kick:
        kickMessage(static_cast<const IrcKickMessage*>(message));
        break;
    case IrcMessage::ChannelMode: // TODO: fix
        modeMessage(static_cast<const IrcModeMessage*>(message));
        break;
    case IrcMessage::NickName:
        nickNameMessage(static_cast<const IrcNickNameMessage*>(message));
        break;
    case IrcMessage::Notice:
        noticeMessage(static_cast<const IrcNoticeMessage*>(message));
        break;
    case IrcMessage::Numeric:
        numericMessage(static_cast<const IrcNumericMessage*>(message));
        break;
    case IrcMessage::Part:
        partMessage(static_cast<const IrcPartMessage*>(message));
        break;
    case IrcMessage::Private:
        privateMessage(static_cast<const IrcPrivateMessage*>(message));
        break;
    case IrcMessage::Quit:
        quitMessage(static_cast<const IrcQuitMessage*>(message));
        break;
    case IrcMessage::Topic:
        topicMessage(static_cast<const IrcTopicMessage*>(message));
        break;
    case IrcMessage::Unknown:
        unknownMessage(static_cast<const IrcMessage*>(message));
        break;
    }
}
