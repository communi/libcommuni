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
#include "ircfilter.h"
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
    QHash<uint, IrcFilter*> filters;
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

IrcFilter* IrcReceiver::filter(uint message) const
{
    Q_D(const IrcReceiver);
    return d->filters.value(message);
}

void IrcReceiver::setFilter(uint message, IrcFilter* filter)
{
    Q_D(IrcReceiver);
    d->filters.insert(message, filter);
}

void IrcReceiver::receiveMessage(IrcMessage* message)
{
    if (!message)
    {
        qCritical() << "IrcReceiver::receiveMessage(" << message << ")";
        return;
    }

    // TODO: filters

    switch (message->type())
    {
    case Irc::Invite:
        inviteMessage(static_cast<IrcInviteMessage*>(message));
        break;
    case Irc::Join:
        joinMessage(static_cast<IrcJoinMessage*>(message));
        break;
    case Irc::Kick:
        kickMessage(static_cast<IrcKickMessage*>(message));
        break;
    case Irc::Mode:
        modeMessage(static_cast<IrcModeMessage*>(message));
        break;
    case Irc::Nick:
        nickNameMessage(static_cast<IrcNickMessage*>(message));
        break;
    case Irc::Notice:
        noticeMessage(static_cast<IrcNoticeMessage*>(message));
        break;
    case Irc::Numeric:
        numericMessage(static_cast<IrcNumericMessage*>(message));
        break;
    case Irc::Part:
        partMessage(static_cast<IrcPartMessage*>(message));
        break;
    case Irc::Private:
        privateMessage(static_cast<IrcPrivateMessage*>(message));
        break;
    case Irc::Quit:
        quitMessage(static_cast<IrcQuitMessage*>(message));
        break;
    case Irc::Topic:
        topicMessage(static_cast<IrcTopicMessage*>(message));
        break;
    case Irc::Unknown:
        unknownMessage(static_cast<IrcMessage*>(message));
        break;
    }
}
