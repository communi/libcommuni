/*
* Copyright (C) 2008-2013 The Communi Project
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

#include "ircchannel.h"
#include "ircchannel_p.h"
#include "ircusermodel.h"
#include "ircmessage.h"

/*!
    \file ircchannel.h
    \brief #include &lt;IrcChannel&gt;
 */

/*!
    \class IrcChannel ircchannel.h <IrcChannel>
    \ingroup models
    \brief Keeps track of channel status.

    \note IrcChannel does not work on its own. Use an instance managed by IrcChannelModel.

    \sa models
*/

/*!
    \fn void IrcChannel::messageReceived(IrcMessage* message)

    This signal is emitted when a channel specific message is received.

    The message may one of the following types:
    - IrcMessage::Join
    - IrcMessage::Kick
    - IrcMessage::Mode
    - IrcMessage::Names
    - IrcMessage::Nick
    - IrcMessage::Notice
    - IrcMessage::Part
    - IrcMessage::Private
    - IrcMessage::Quit
    - IrcMessage::Topic

    \sa IrcSession::messageReceived(), IrcChannelModel::messageIgnored()
 */

void IrcChannelPrivate::init(const QString& p, const QString& n)
{
    prefix = p;
    name = n;
}

void IrcChannelPrivate::changeMode(const QString& value)
{
    QString copy = mode;
    bool add = true;
    for (int i = 0; i < value.size(); ++i) {
        QChar c = value.at(i);
        if (c == QLatin1Char('+')) {
            add = true;
        } else if (c == QLatin1Char('-')) {
            add = false;
        } else {
            if (add) {
                if (!copy.contains(c))
                    copy += c;
            } else {
                copy.remove(c);
            }
        }
    }
    setMode(copy);
}

void IrcChannelPrivate::setMode(const QString& value)
{
    Q_Q(IrcChannel);
    if (mode != value) {
        mode = value;
        emit q->modeChanged(mode);
    }
}

void IrcChannelPrivate::setTopic(const QString& value)
{
    Q_Q(IrcChannel);
    if (topic != value) {
        topic = value;
        emit q->topicChanged(topic);
    }
}

bool IrcChannelPrivate::processMessage(IrcMessage* message)
{
    Q_Q(IrcChannel);
    bool handled = true;
    switch (message->type()) {
    case IrcMessage::Mode: {
        IrcModeMessage* modeMsg = static_cast<IrcModeMessage*>(message);
        if (modeMsg->kind() != IrcModeMessage::Channel)
            // TODO: q->model()->d_func()->_irc_processMessage(message);
            QMetaObject::invokeMethod(q->model(), "_irc_processMessage", Q_ARG(IrcMessage*, message));
        else if (modeMsg->isReply())
            setMode(modeMsg->mode());
        else
            changeMode(modeMsg->mode());
        break;
    }
    case IrcMessage::Topic:
        setTopic(static_cast<IrcTopicMessage*>(message)->topic());
        break;
    default:
        // TODO: q->model()->d_func()->_irc_processMessage(message);
        QMetaObject::invokeMethod(q->model(), "_irc_processMessage", Q_RETURN_ARG(bool, handled), Q_ARG(IrcMessage*, message));
        break;
    }
    if (handled)
        emit q->messageReceived(message);
    return handled;
}

/*!
    Constructs a new channel object with \a parent.

    \note IrcChannel does not work on its own. Use an instance managed by IrcChannelModel.

    \sa IrcChannelModel::channelAdded()
 */
IrcChannel::IrcChannel(QObject* parent)
    : QObject(parent), d_ptr(new IrcChannelPrivate)
{
    Q_D(IrcChannel);
    d->q_ptr = this;
    d->model = 0;
}

/*!
    Destructs the channel object.

    \sa IrcChannelModel::channelRemoved()
 */
IrcChannel::~IrcChannel()
{
    emit destroyed(this);
}

/*!
    This property holds the channel title.

    The title consists of prefix and name.

    \par Access function:
    \li QString <b>title</b>() const
 */
QString IrcChannel::title() const
{
    Q_D(const IrcChannel);
    return d->prefix + d->name;
}

/*!
    This property holds the channel name.

    \par Access function:
    \li QString <b>name</b>() const
 */
QString IrcChannel::name() const
{
    Q_D(const IrcChannel);
    return d->name;
}

/*!
    This property holds the channel prefix.

    \par Access function:
    \li QString <b>prefix</b>() const
 */
QString IrcChannel::prefix() const
{
    Q_D(const IrcChannel);
    return d->prefix;
}

/*!
    This property holds the channel mode.

    \par Access function:
    \li QString <b>mode</b>() const

    \par Notifier signal:
    \li void <b>modeChanged</b>(const QString& mode)
 */
QString IrcChannel::mode() const
{
    Q_D(const IrcChannel);
    return d->mode;
}

/*!
    This property holds the channel topic.

    \par Access function:
    \li QString <b>topic</b>() const

    \par Notifier signal:
    \li void <b>topicChanged</b>(const QString& topic)
 */
QString IrcChannel::topic() const
{
    Q_D(const IrcChannel);
    return d->topic;
}

/*!
    This property holds the list model of users on the channel.

    \par Access function:
    \li \ref IrcUserModel* <b>model</b>() const
 */
IrcUserModel* IrcChannel::model() const
{
    Q_D(const IrcChannel);
    if (!d->model)
        d->model = new IrcUserModel(const_cast<IrcChannel*>(this));
    return d->model;
}
