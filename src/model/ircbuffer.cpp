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
#include "ircconnection.h"
#include "ircchannel.h"

IRC_BEGIN_NAMESPACE

/*!
    \file ircbuffer.h
    \brief #include &lt;IrcBuffer&gt;
 */

/*!
    \class IrcBuffer ircbuffer.h <IrcBuffer>
    \ingroup model
    \brief Keeps track of buffer status.

    \note IrcBuffer does not work on its own. Use an instance managed by IrcBufferModel.

    \sa model
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

    \sa IrcConnection::messageReceived(), IrcBufferModel::messageIgnored()
 */

IrcBufferPrivate::IrcBufferPrivate()
    : q_ptr(0), model(0), persistent(false), sticky(false)
{
}

IrcBufferPrivate::~IrcBufferPrivate()
{
}

void IrcBufferPrivate::init(const QString& title, IrcBufferModel* m)
{
    name = title;
    setModel(m);
}

void IrcBufferPrivate::setName(const QString& value)
{
    Q_Q(IrcBuffer);
    if (name != value) {
        name = value;
        emit q->nameChanged(name);
        emit q->titleChanged(q->title());
        if (model)
            IrcBufferModelPrivate::get(model)->renameBuffer(q, q->title());
    }
}

void IrcBufferPrivate::setPrefix(const QString& value)
{
    Q_Q(IrcBuffer);
    if (prefix != value) {
        prefix = value;
        emit q->prefixChanged(prefix);
        emit q->titleChanged(q->title());
        if (model)
            IrcBufferModelPrivate::get(model)->renameBuffer(q, q->title());
    }
}

void IrcBufferPrivate::setModel(IrcBufferModel* value)
{
    Q_Q(IrcBuffer);
    if (model != value) {
        model = value;
        QObject::connect(model->connection(), SIGNAL(statusChanged(IrcConnection::Status)), q, SLOT(_irc_emitActiveChanged()));
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
    case IrcMessage::Notice:
        processed = processNoticeMessage(static_cast<IrcNoticeMessage*>(message));
        break;
    case IrcMessage::Numeric:
        processed = processNumericMessage(static_cast<IrcNumericMessage*>(message));
        break;
    case IrcMessage::Part:
        processed = processPartMessage(static_cast<IrcPartMessage*>(message));
        break;
    case IrcMessage::Private:
        processed = processPrivateMessage(static_cast<IrcPrivateMessage*>(message));
        break;
    case IrcMessage::Quit:
        processed = processQuitMessage(static_cast<IrcQuitMessage*>(message));
        break;
    case IrcMessage::Topic:
        processed = processTopicMessage(static_cast<IrcTopicMessage*>(message));
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
    if (!message->nick().compare(name, Qt::CaseInsensitive)) {
        setName(message->newNick());
        return true;
    }
    return !message->newNick().compare(name, Qt::CaseInsensitive);
}

bool IrcBufferPrivate::processNoticeMessage(IrcNoticeMessage* message)
{
    Q_UNUSED(message);
    return true;
}

bool IrcBufferPrivate::processNumericMessage(IrcNumericMessage* message)
{
    Q_UNUSED(message);
    return true;
}

bool IrcBufferPrivate::processPartMessage(IrcPartMessage* message)
{
    Q_UNUSED(message);
    return false;
}

bool IrcBufferPrivate::processPrivateMessage(IrcPrivateMessage* message)
{
    Q_UNUSED(message);
    return true;
}

bool IrcBufferPrivate::processQuitMessage(IrcQuitMessage* message)
{
    return !message->nick().compare(name, Qt::CaseInsensitive);
}

bool IrcBufferPrivate::processTopicMessage(IrcTopicMessage* message)
{
    Q_UNUSED(message);
    return false;
}

/*!
    Constructs a new buffer object with \a parent.

    \note IrcBuffer does not work on its own. Use an instance managed by IrcBufferModel.

    \sa IrcBufferModel::added()
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

    \sa IrcBufferModel::removed()
 */
IrcBuffer::~IrcBuffer()
{
    emit destroyed(this);
}

/*!
    This property holds the buffer title.

    The title consists of \ref prefix and \ref name.

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
    \li void <b>setName</b>(const QString& name)

    \par Notifier signal:
    \li void <b>nameChanged</b>(const QString& name)
 */
QString IrcBuffer::name() const
{
    Q_D(const IrcBuffer);
    return d->name;
}

void IrcBuffer::setName(const QString& name)
{
    Q_D(IrcBuffer);
    d->setName(name);
}

/*!
    This property holds the buffer prefix.

    \par Access function:
    \li QString <b>prefix</b>() const
    \li void <b>setPrefix</b>(const QString& prefix)

    \par Notifier signal:
    \li void <b>prefixChanged</b>(const QString& prefix)
 */
QString IrcBuffer::prefix() const
{
    Q_D(const IrcBuffer);
    return d->prefix;
}

void IrcBuffer::setPrefix(const QString& prefix)
{
    Q_D(IrcBuffer);
    return d->setPrefix(prefix);
}

/*!
    \property bool IrcBuffer::channel
    This property holds whether the buffer is a channel.

    \par Access function:
    \li bool <b>isChannel</b>() const

    \sa toChannel()
 */
bool IrcBuffer::isChannel() const
{
    return inherits("IrcChannel");
}

/*!
    Returns the buffer cast to a IrcChannel,
    if the class is actually a channel, \c 0 otherwise.

    \sa isChannel()
*/
IrcChannel* IrcBuffer::toChannel()
{
    return qobject_cast<IrcChannel*>(this);
}

/*!
    This property holds the connection of the buffer.

    \par Access function:
    \li \ref IrcConnection* <b>connection</b>() const
 */
IrcConnection* IrcBuffer::connection() const
{
    Q_D(const IrcBuffer);
    return d->model ? d->model->connection() : 0;
}

/*!
    This property holds the network of the buffer.

    \par Access function:
    \li \ref IrcNetwork* <b>network</b>() const
 */
IrcNetwork* IrcBuffer::network() const
{
    Q_D(const IrcBuffer);
    return d->model ? d->model->network() : 0;
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
    \property bool IrcBuffer::active
    This property holds whether the buffer is active.

    A buffer is considered active when the \ref IrcBuffer::connection "connection"
    is connected. Furthermore, channel buffers are only considered active when
    the user is on the channel.

    \par Access function:
    \li bool <b>isActive</b>() const

    \par Notifier signal:
    \li void <b>activeChanged</b>(bool active)

    \sa IrcConnection::connected
 */
bool IrcBuffer::isActive() const
{
    if (IrcConnection* c = connection())
        return c->isConnected();
    return false;
}

/*!
    \property bool IrcBuffer::sticky
    This property holds whether the buffer is sticky.

    A sticky buffer stays in the beginning (Qt::AscendingOrder) or
    end (Qt::DescendingOrder) of the list buffers in IrcBufferModel.

    The default value is \c false.

    \par Access function:
    \li bool <b>isSticky</b>() const
    \li void <b>setSticky</b>(bool sticky)

    \par Notifier signal:
    \li void <b>stickyChanged</b>(bool sticky)
 */

bool IrcBuffer::isSticky() const
{
    Q_D(const IrcBuffer);
    return d->sticky;
}

void IrcBuffer::setSticky(bool sticky)
{
    Q_D(IrcBuffer);
    if (d->sticky != sticky) {
        d->sticky = sticky;
        emit stickyChanged(sticky);
    }
}

/*!
    \property bool IrcBuffer::persistent
    This property holds whether the buffer is persistent.

    A persistent buffer does not get automatically removed
    from its IrcBufferModel e.g. when leaving a channel.

    The default value is \c false.

    \par Access function:
    \li bool <b>isPersistent</b>() const
    \li void <b>setPersistent</b>(bool persistent)

    \par Notifier signal:
    \li void <b>persistentChanged</b>(bool persistent)
 */

bool IrcBuffer::isPersistent() const
{
    Q_D(const IrcBuffer);
    return d->persistent;
}

void IrcBuffer::setPersistent(bool persistent)
{
    Q_D(IrcBuffer);
    if (d->persistent != persistent) {
        d->persistent = persistent;
        emit persistentChanged(persistent);
    }
}

/*!
    Sends a \a command to the server.

    This method is provided for convenience. It is equal to:
    \code
    IrcConnection* connection = buffer->connection();
    connection->sendCommand(command);
    \endcode

    \sa IrcConnection::sendCommand()
 */
bool IrcBuffer::sendCommand(IrcCommand* command)
{
    if (IrcConnection* c = connection())
        return c->sendCommand(command);
    return false;
}

/*!
    Emits messageReceived() with \a message.

    IrcBufferModel handles only buffer specific messages and delivers them
    to the appropriate IrcBuffer instances. When applications decide to handle
    IrcBuffer::messageReceived(), IrcBufferModel::messageIgnored() makes it
    easy to implement handling for the rest, non-buffer specific messages.
    This method can be used to forward such ignored messages to the desired
    buffers (for instance the one that is currently active in the GUI).

    \sa IrcBuffer::messageReceived()
 */
void IrcBuffer::receiveMessage(IrcMessage* message)
{
    emit messageReceived(message);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcBuffer* buffer)
{
    if (!buffer)
        return debug << "IrcBuffer(0x0) ";
    debug.nospace() << buffer->metaObject()->className() << '(' << (void*) buffer;
    if (!buffer->objectName().isEmpty())
        debug.nospace() << ", name=" << buffer->objectName();
    if (!buffer->title().isEmpty())
        debug.nospace() << ", title=" << buffer->title();
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircbuffer.cpp"

IRC_END_NAMESPACE
