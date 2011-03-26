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
    \class IrcBuffer ircbuffer.h
    \brief The IrcBuffer class provides an IRC buffer.

    The IrcBuffer class acts as a buffer for a pattern. A pattern can
    match any target such as channels and queries.

    \note IrcBuffers are not intended to be instantiated directly, but via the
    virtual factory method IrcSession::createIrcBuffer().
 */

/*!
    \fn void IrcBuffer::motdReceived(const QString& motd)

    This signal is emitted when message of the day \a motd has been received.
 */

/*!
    \fn void IrcBuffer::joined(const QString& origin)

    This signal is emitted when \a origin has joined.
 */

/*!
    \fn void IrcBuffer::parted(const QString& origin, const QString& message)

    This signal is emitted when \a origin has parted with \a message.
 */

/*!
    \fn void IrcBuffer::quit(const QString& origin, const QString& message)

    This signal is emitted when \a origin has quit with \a message.
 */

/*!
    \fn void IrcBuffer::namesReceived(const QStringList& names)

    This signal is emitted when the list of \a names has been received.
 */

/*!
    \fn void IrcBuffer::nickChanged(const QString& origin, const QString& nick)

    This signal is emitted when \a origin has changed \a nick.
 */

/*!
    \fn void IrcBuffer::modeChanged(const QString& origin, const QString& mode, const QString& args)

    This signal is emitted when \a origin has changed \a mode with \a args.
 */

/*!
    \fn void IrcBuffer::topicChanged(const QString& origin, const QString& topic)

    This signal is emitted when \a origin has changed \a topic.
 */

/*!
    \fn void IrcBuffer::invited(const QString& origin, const QString& receiver, const QString& channel)

    This signal is emitted when \a origin has invited \a receiver to \a channel.
 */

/*!
    \fn void IrcBuffer::kicked(const QString& origin, const QString& nick, const QString& message)

    This signal is emitted when \a origin has kicked \a nick with \a message.
 */

/*!
    \fn void IrcBuffer::messageReceived(const QString& origin, const QString& message)

    This signal is emitted when \a origin has sent \a message.
 */

/*!
    \fn void IrcBuffer::noticeReceived(const QString& origin, const QString& notice)

    This signal is emitted when \a origin has sent \a notice.
 */

/*!
    \fn void IrcBuffer::ctcpRequestReceived(const QString& origin, const QString& request)

    This signal is emitted when \a origin has sent a CTCP \a request.
 */

/*!
    \fn void IrcBuffer::ctcpReplyReceived(const QString& origin, const QString& reply)

    This signal is emitted when \a origin has sent a CTCP \a reply.
 */

/*!
    \fn void IrcBuffer::ctcpActionReceived(const QString& origin, const QString& action)

    This signal is emitted when \a origin has sent a CTCP \a action.
 */

/*!
    \fn void IrcBuffer::numericMessageReceived(const QString& origin, uint code, const QStringList& params)

    This signal is emitted when \a origin has sent a numeric message with \a code and \a params.
 */

/*!
    \fn void IrcBuffer::unknownMessageReceived(const QString& origin, const QStringList& params)

    This signal is emitted when \a origin has sent an unknown message with \a params.
 */

IrcBufferPrivate::IrcBufferPrivate() :
    q_ptr(0)
{
}

/*!
    Constructs a new IRC buffer for \a pattern with \a parent.

    \sa Session::createIrcBuffer()
 */
IrcBuffer::IrcBuffer(const QString& pattern, IrcSession* parent) :
    QObject(parent), d_ptr(new IrcBufferPrivate)
{
    Q_D(IrcBuffer);
    d->q_ptr = this;
    d->pattern = pattern;
}

IrcBuffer::IrcBuffer(IrcBufferPrivate& dd, const QString& pattern, IrcSession* parent) :
    QObject(parent), d_ptr(&dd)
{
    Q_D(IrcBuffer);
    d->q_ptr = this;
    d->pattern = pattern;
}

/*!
    Destructs the IRC buffer.
 */
IrcBuffer::~IrcBuffer()
{
    /* TODO:
    Session* s = session();
    if (s)
        s->d_func()->removeIrcBuffer(this);
        */
}

/*!
    Returns the session.
 */
IrcSession* IrcBuffer::session() const
{
    return qobject_cast<IrcSession*>(parent());
}

/*!
    Returns the pattern.
 */
QString IrcBuffer::pattern() const
{
    Q_D(const IrcBuffer);
    return d->pattern;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcBuffer* buffer)
{
    if (!buffer)
        return debug << "IrcBuffer(0x0) ";
    debug.nospace() << buffer->metaObject()->className() << '(' << (void*) buffer;
    if (!buffer->objectName().isEmpty())
        debug << ", name = " << buffer->objectName();
    if (!buffer->pattern().isEmpty())
        debug << ", pattern = " << buffer->pattern();
    debug << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircbuffer.cpp"
