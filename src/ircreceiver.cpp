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

#include "ircreceiver.h"
#include "ircsession.h"
#include "ircsession_p.h"
#include <QVariant>
#include <QDebug>

class IrcReceiverPrivate
{
    Q_DECLARE_PUBLIC(IrcReceiver)

public:
    IrcReceiverPrivate(IrcReceiver* session) : q_ptr(session), session(0)
    {
    }

    IrcReceiver* q_ptr;
    IrcSession* session;
};

IrcReceiver::IrcReceiver(IrcSession* session) : d_ptr(new IrcReceiverPrivate(this))
{
    setSession(session);
}

IrcReceiver::~IrcReceiver()
{
    Q_D(IrcReceiver);
    if (d->session)
        d->session->d_func()->receivers.removeAll(this);
}

IrcSession* IrcReceiver::session() const
{
    Q_D(const IrcReceiver);
    return d->session;
}

void IrcReceiver::setSession(IrcSession* session)
{
    Q_D(IrcReceiver);
    if (d->session != session)
    {
        if (d->session)
            d->session->d_func()->receivers.removeAll(this);
        d->session = session;
        if (d->session)
            d->session->d_func()->receivers.append(this);
    }
}

void IrcReceiver::receiveMessage(IrcMessage* message)
{
    if (!message)
    {
        qCritical() << "IrcReceiver::receiveMessage(" << message << ")";
        return;
    }

    switch (message->type())
    {
    case IrcMessage::Invite:
        inviteMessage(static_cast<IrcInviteMessage*>(message));
        break;
    case IrcMessage::Join:
        joinMessage(static_cast<IrcJoinMessage*>(message));
        break;
    case IrcMessage::Kick:
        kickMessage(static_cast<IrcKickMessage*>(message));
        break;
    case IrcMessage::Mode:
        modeMessage(static_cast<IrcModeMessage*>(message));
        break;
    case IrcMessage::Nick:
        nickMessage(static_cast<IrcNickMessage*>(message));
        break;
    case IrcMessage::Notice:
        noticeMessage(static_cast<IrcNoticeMessage*>(message));
        break;
    case IrcMessage::Numeric:
        numericMessage(static_cast<IrcNumericMessage*>(message));
        break;
    case IrcMessage::Part:
        partMessage(static_cast<IrcPartMessage*>(message));
        break;
    case IrcMessage::Private:
        privateMessage(static_cast<IrcPrivateMessage*>(message));
        break;
    case IrcMessage::Quit:
        quitMessage(static_cast<IrcQuitMessage*>(message));
        break;
    case IrcMessage::Topic:
        topicMessage(static_cast<IrcTopicMessage*>(message));
        break;
    case IrcMessage::Unknown:
        unknownMessage(static_cast<IrcMessage*>(message));
        break;
    }
}
