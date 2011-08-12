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
#include "ircmessagefilter.h"
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
    IrcMessageFilter filter;
};

static bool testMatch(const QVariant& value, const QVariant& match, Qt::MatchFlags flags)
{
    const uint matchType = flags & 0x0F;
    if (matchType == Qt::MatchExactly)
        return value == match;

    const Qt::CaseSensitivity cs = flags & Qt::MatchCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    const QString text = value.toString();
    const QString term = match.toString();

    switch (matchType)
    {
    case Qt::MatchRegExp:
        return QRegExp(term, cs, QRegExp::RegExp).exactMatch(text);
    case Qt::MatchWildcard:
        return QRegExp(term, cs, QRegExp::Wildcard).exactMatch(text);
    case Qt::MatchStartsWith:
        return text.startsWith(term, cs);
    case Qt::MatchEndsWith:
        return text.endsWith(term, cs);
    case Qt::MatchFixedString:
        return !text.compare(term, cs);
    case Qt::MatchContains:
    default:
        return text.contains(term, cs);
    }
}

static bool testFilter(const IrcMessageFilter &filter, IrcMessage *message)
{
    if (!filter.isEnabled())
        return false;

    switch (filter.filterType())
    {
    case IrcMessageFilter::DefaultFilter:
        return true;

    case IrcMessageFilter::IntersectionFilter:
    {
        foreach (const IrcMessageFilter& f, IrcMessageIntersectionFilter(filter).filters())
            if (!testFilter(f, message))
                return false;
        return !IrcMessageIntersectionFilter(filter).filters().isEmpty();
    }

    case IrcMessageFilter::UnionFilter:
    {
        foreach (const IrcMessageFilter& f, IrcMessageUnionFilter(filter).filters())
            if (testFilter(f, message))
                return true;
        return false;
    }

    case IrcMessageFilter::TypeFilter:
    {
        const IrcMessageTypeFilter typeFilter(filter);
        return message->type() & typeFilter.messageTypes();
    }

    case IrcMessageFilter::PropertyFilter:
    {
        const IrcMessagePropertyFilter propertyFilter(filter);
        QVariant property = message->property(propertyFilter.property().toLatin1());
        return testMatch(property, propertyFilter.term(), propertyFilter.matchFlags());
    }

    default:
        return false;
    }
}

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

IrcMessageFilter IrcReceiver::messageFilter() const
{
    Q_D(const IrcReceiver);
    return d->filter;
}

void IrcReceiver::setMessageFilter(const IrcMessageFilter& filter)
{
    Q_D(IrcReceiver);
    d->filter = filter;
}

void IrcReceiver::receiveMessage(IrcMessage* message)
{
    if (!message)
    {
        qCritical() << "IrcReceiver::receiveMessage(" << message << ")";
        return;
    }

    Q_D(IrcReceiver);
    if (!testFilter(d->filter, message))
        return;

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
