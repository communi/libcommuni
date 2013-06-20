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

#include "ircbuffer.h"
#include "ircbuffer_p.h"
#include "ircbuffermodel.h"
#include "ircbuffermodel_p.h"
#include "ircsession.h"
#include "ircsender.h"

/*!
    \file ircbuffer.h
    \brief #include &lt;IrcBuffer&gt;
 */

/*!
    \class IrcBuffer ircbuffer.h <IrcBuffer>
    \ingroup models
    \brief Keeps track of buffer status.

    \note IrcBuffer does not work on its own. Use an instance managed by IrcBufferModel.

    \sa models
*/

/*!
    \fn void IrcBuffer::messageReceived(IrcMessage* message)

    This signal is emitted when a buffer specific message is received.

    The message may one of the following types:
    - IrcMessage::Join
    - IrcMessage::Kick
    - IrcMessage::Mode
    - IrcMessage::Names
    - IrcMessage::Nick
    - IrcMessage::Notice
    - IrcMessage::Numeric
    - IrcMessage::Part
    - IrcMessage::Private
    - IrcMessage::Quit
    - IrcMessage::Topic

    \sa IrcSession::messageReceived(), IrcBufferModel::messageIgnored()
 */

IrcBufferPrivate::IrcBufferPrivate()
    : q_ptr(0), model(0)
{
}

IrcBufferPrivate::~IrcBufferPrivate()
{
}

void IrcBufferPrivate::init(const QString& title, IrcBufferModel* m)
{
    model = m;
    name = title;
}

void IrcBufferPrivate::setName(const QString& value)
{
    Q_Q(IrcBuffer);
    if (name != value) {
        name = value;
        emit q->nameChanged(name);
        emit q->titleChanged(q->title());
    }
}

bool IrcBufferPrivate::processMessage(IrcMessage* message)
{
    Q_Q(IrcBuffer);
    bool processed = false;
    switch (message->type()) {
    case IrcMessage::Join:
        processed = processJoinMessage(static_cast<IrcJoinMessage*>(message));
        break;
    case IrcMessage::Kick:
        processed = processKickMessage(static_cast<IrcKickMessage*>(message));
        break;
    case IrcMessage::Mode:
        processed = processModeMessage(static_cast<IrcModeMessage*>(message));
        break;
    case IrcMessage::Names:
        processed = processNamesMessage(static_cast<IrcNamesMessage*>(message));
        break;
    case IrcMessage::Nick:
        processed = processNickMessage(static_cast<IrcNickMessage*>(message));
        break;
    case IrcMessage::Part:
        processed = processPartMessage(static_cast<IrcPartMessage*>(message));
        break;
    case IrcMessage::Quit:
        processed = processQuitMessage(static_cast<IrcQuitMessage*>(message));
        break;
    case IrcMessage::Topic:
        processed = processTopicMessage(static_cast<IrcTopicMessage*>(message));
        break;
    case IrcMessage::Notice:
    case IrcMessage::Numeric:
    case IrcMessage::Private:
        processed = true;
        break;
    default:
        break;
    }
    if (processed)
        emit q->messageReceived(message);
    return processed;
}

bool IrcBufferPrivate::processJoinMessage(IrcJoinMessage* message)
{
    Q_UNUSED(message);
    return false;
}

bool IrcBufferPrivate::processKickMessage(IrcKickMessage* message)
{
    Q_UNUSED(message);
    return false;
}

bool IrcBufferPrivate::processModeMessage(IrcModeMessage* message)
{
    Q_UNUSED(message);
    return false;
}

bool IrcBufferPrivate::processNamesMessage(IrcNamesMessage* message)
{
    Q_UNUSED(message);
    return false;
}

bool IrcBufferPrivate::processNickMessage(IrcNickMessage* message)
{
    Q_Q(IrcBuffer);
    if (!message->sender().name().compare(name, Qt::CaseInsensitive)) {
        if (IrcBufferModelPrivate::get(model)->renameBuffer(q, message->nick()))
            setName(message->nick());
        return true;
    }
    return !message->nick().compare(name, Qt::CaseInsensitive);
}

bool IrcBufferPrivate::processPartMessage(IrcPartMessage* message)
{
    Q_UNUSED(message);
    return false;
}

bool IrcBufferPrivate::processQuitMessage(IrcQuitMessage* message)
{
    return !message->sender().name().compare(name, Qt::CaseInsensitive);
}

bool IrcBufferPrivate::processTopicMessage(IrcTopicMessage* message)
{
    Q_UNUSED(message);
    return false;
}

/*!
    Constructs a new buffer object with \a parent.

    \note IrcBuffer does not work on its own. Use an instance managed by IrcBufferModel.

    \sa IrcBufferModel::bufferAdded()
 */
IrcBuffer::IrcBuffer(QObject* parent)
    : QObject(parent), d_ptr(new IrcBufferPrivate)
{
    Q_D(IrcBuffer);
    d->q_ptr = this;
}

/*!
    \internal
 */
IrcBuffer::IrcBuffer(IrcBufferPrivate& dd, QObject* parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(IrcBuffer);
    d->q_ptr = this;
}

/*!
    Destructs the buffer object.

    \sa IrcBufferModel::bufferRemoved()
 */
IrcBuffer::~IrcBuffer()
{
    emit destroyed(this);
}

/*!
    This property holds the buffer title.

    The title consists of prefix and name.

    \par Access function:
    \li QString <b>title</b>() const

    \par Notifier signal:
    \li void <b>titleChanged</b>(const QString& title)
 */
QString IrcBuffer::title() const
{
    Q_D(const IrcBuffer);
    return d->prefix + d->name;
}

/*!
    This property holds the buffer name.

    \par Access function:
    \li QString <b>name</b>() const

    \par Notifier signal:
    \li void <b>nameChanged</b>(const QString& name)
 */
QString IrcBuffer::name() const
{
    Q_D(const IrcBuffer);
    return d->name;
}

/*!
    This property holds the buffer prefix.

    \par Access function:
    \li QString <b>prefix</b>() const
 */
QString IrcBuffer::prefix() const
{
    Q_D(const IrcBuffer);
    return d->prefix;
}

/*!
    This property holds the session of the buffer.

    \par Access function:
    \li \ref IrcSession* <b>session</b>() const
 */
IrcSession* IrcBuffer::session() const
{
    Q_D(const IrcBuffer);
    return d->model->session();
}

/*!
    This property holds the model of the buffer.

    \par Access function:
    \li \ref IrcBufferModel* <b>model</b>() const
 */
IrcBufferModel* IrcBuffer::model() const
{
    Q_D(const IrcBuffer);
    return d->model;
}

/*!
    Sends a \a command to the server.

    This method is provided for convenience. It is equal to:
    \code
    IrcSession* session = buffer->model()->session();
    session->sendCommand(command);
    \endcode

    \sa IrcSession::sendCommand()
 */
bool IrcBuffer::sendCommand(IrcCommand* command)
{
    Q_D(IrcBuffer);
    if (d->model && d->model->session())
        return d->model->session()->sendCommand(command);
    return false;
}
