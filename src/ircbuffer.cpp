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

#include "ircbuffer.h"
#include "ircbuffer_p.h"
#include "ircsession.h"
#include "ircsession_p.h"

/*!
    \class Irc::Buffer ircbuffer.h
    \brief The Irc::Buffer class provides an IRC buffer.

    The Irc::Buffer class acts as a buffer for a single receiver. Receivers
    can be:
    \li the server
    \li channels
    \li queries.

    Server/channel/query specific messages are delivered to the corresponding
    buffer.

    \note Buffers are not intended to be instantiated directly, but via the
    virtual factory method Irc::Session::createBuffer().
 */

/*!
    \fn void Irc::Buffer::motdReceived(const QString& motd)

    This signal is emitted when message of the day \a motd has been received.
 */

/*!
    \fn void Irc::Buffer::joined(const QString& origin)

    This signal is emitted when \a origin has joined.
 */

/*!
    \fn void Irc::Buffer::parted(const QString& origin, const QString& message)

    This signal is emitted when \a origin has parted with \a message.
 */

/*!
    \fn void Irc::Buffer::quit(const QString& origin, const QString& message)

    This signal is emitted when \a origin has quit with \a message.
 */

/*!
    \fn void Irc::Buffer::namesReceived(const QStringList& names)

    This signal is emitted when the list of \a names has been received.
 */

/*!
    \fn void Irc::Buffer::nickChanged(const QString& origin, const QString& nick)

    This signal is emitted when \a origin has changed \a nick.
 */

/*!
    \fn void Irc::Buffer::modeChanged(const QString& origin, const QString& mode, const QString& args)

    This signal is emitted when \a origin has changed \a mode with \a args.
 */

/*!
    \fn void Irc::Buffer::topicChanged(const QString& origin, const QString& topic)

    This signal is emitted when \a origin has changed \a topic.
 */

/*!
    \fn void Irc::Buffer::invited(const QString& origin, const QString& receiver, const QString& channel)

    This signal is emitted when \a origin has invited \a receiver to \a channel.
 */

/*!
    \fn void Irc::Buffer::kicked(const QString& origin, const QString& nick, const QString& message)

    This signal is emitted when \a origin has kicked \a nick with \a message.
 */

/*!
    \fn void Irc::Buffer::messageReceived(const QString& origin, const QString& message)

    This signal is emitted when \a origin has sent \a message.
 */

/*!
    \fn void Irc::Buffer::noticeReceived(const QString& origin, const QString& notice)

    This signal is emitted when \a origin has sent \a notice.
 */

/*!
    \fn void Irc::Buffer::ctcpRequestReceived(const QString& origin, const QString& request)

    This signal is emitted when \a origin has sent a CTCP \a request.
 */

/*!
    \fn void Irc::Buffer::ctcpReplyReceived(const QString& origin, const QString& reply)

    This signal is emitted when \a origin has sent a CTCP \a reply.
 */

/*!
    \fn void Irc::Buffer::ctcpActionReceived(const QString& origin, const QString& action)

    This signal is emitted when \a origin has sent a CTCP \a action.
 */

/*!
    \fn void Irc::Buffer::numericMessageReceived(const QString& origin, uint code, const QStringList& params)

    This signal is emitted when \a origin has sent a numeric message with \a code and \a params.
 */

/*!
    \fn void Irc::Buffer::unknownMessageReceived(const QString& origin, const QStringList& params)

    This signal is emitted when \a origin has sent an unknown message with \a params.
 */

namespace Irc
{
    BufferPrivate::BufferPrivate() :
        q_ptr(0)
    {
    }

    /*!
        Constructs a new IRC buffer with \a receiver and \a parent.

        \sa Session::createBuffer()
     */
    Buffer::Buffer(const QString& receiver, Session* parent) : QObject(parent), d_ptr(new BufferPrivate)
    {
        Q_D(Buffer);
        d->q_ptr = this;
        d->receiver = receiver;
    }

    Buffer::Buffer(BufferPrivate& dd, const QString& receiver, Session* parent) : QObject(parent), d_ptr(&dd)
    {
        Q_D(Buffer);
        d->q_ptr = this;
        d->receiver = receiver;
    }

    /*!
        Destructs the IRC buffer.
     */
    Buffer::~Buffer()
    {
        Session* s = session();
        /* TODO:
        if (s)
            s->d_func()->removeBuffer(this);
            */

        Q_D(Buffer);
        delete d;
    }

    /*!
        Returns the session.
     */
    Session* Buffer::session() const
    {
        return qobject_cast<Session*>(parent());
    }

    /*!
        Returns the receiver.
     */
    QString Buffer::receiver() const
    {
        Q_D(const Buffer);
        return d->receiver;
    }
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const Irc::Buffer* buffer)
{
    if (!buffer)
        return debug << "Irc::Buffer(0x0) ";
    debug.nospace() << buffer->metaObject()->className() << '(' << (void*) buffer;
    if (!buffer->objectName().isEmpty())
        debug << ", name = " << buffer->objectName();
    if (!buffer->receiver().isEmpty())
        debug << ", receiver = " << buffer->receiver();
    debug << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircbuffer.cpp"
