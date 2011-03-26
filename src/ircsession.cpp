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

#include "ircsession.h"
#include "ircsession_p.h"
#include "ircbuffer.h"
#include "ircbuffer_p.h"
#include "ircutil.h"
#include <QSet>
#include <QTimer>
#include <QBuffer>
#include <QPointer>
#include <QTcpSocket>
#include <QStringList>
#include <QMetaMethod>
#include <QAuthenticator>

/*!
    \class Irc::Session ircsession.h
    \brief The Irc::Session class provides an IRC session.

    IRC (Internet Relay Chat protocol) is a simple communication protocol.
    Irc::Session provides means to establish a connection to an IRC server.
    Irc::Session works asynchronously. None of the functions block the
    calling thread but they return immediately and the actual work is done
    behind the scenes in the event loop.

    Example usage:
    \code
    Irc::Session* session = new Irc::Session(this);
    session->setNick("jpnurmi");
    session->setIdent("jpn");
    session->setRealName("J-P Nurmi");
    session->connectToServer("irc.freenode.net", 6667);
    \endcode

    \note Irc::Session supports SSL (Secure Sockets Layer) connections since version 0.3.0 

    Example SSL usage:
    \code
    Irc::Session* session = new Irc::Session(this);
    // ...
    QSslSocket* socket = new QSslSocket(session);
    socket->ignoreSslErrors();
    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    session->setSocket(socket);
    session->connectToServer("irc.secure.ssl", 6669);
    \endcode

    \sa setSocket()
 */

/*!
    \fn void Irc::Session::connected()

    This signal is emitted after a connection has been successfully established.
 */

/*!
    \fn void Irc::Session::welcomed()

    This signal is emitted when the welcome message has been received.

    \sa Irc::Rfc::RPL_WELCOME
 */

/*!
    \fn void Irc::Session::reconnecting()

    This signal is emitted when the session is about to reconnect.
 */

/*!
    \fn void Irc::Session::disconnected()

    This signal is emitted when the session has been disconnected.
 */

/*!
    \fn void Irc::Session::bufferAdded(Irc::Buffer* buffer)

    This signal is emitted whenever a \a buffer was added.
 */

/*!
    \fn void Irc::Session::bufferRemoved(Irc::Buffer* buffer)

    This signal is emitted whenever a \a buffer was removed.
 */

namespace Irc
{
    SessionPrivate::SessionPrivate() :
        q_ptr(0),
        parser(),
        buffer(),
        socket(0),
        host(),
        port(6667),
        motd(),
        welcomed(false)
    {
    }

    void SessionPrivate::init(Session* session)
    {
        q_ptr = session;

        Q_Q(Session);
        q->setSocket(new QTcpSocket(q));
    }

    void SessionPrivate::_q_connected()
    {
        Q_Q(Session);
        welcomed = false;
        emit q->connecting();

        QAuthenticator auth;
        emit q->authenticate(&auth);
        Q_ASSERT(!auth.user().isEmpty());

        if (!auth.password().isEmpty())
            socket->write(QString(QLatin1String("PASS %1\r\n")).arg(auth.password()).toLocal8Bit());

        socket->write(QString(QLatin1String("NICK %1\r\n")).arg(auth.user()).toLocal8Bit());

        // RFC 1459 states that "hostname and servername are normally
        // ignored by the IRC server when the USER command comes from
        // a directly connected client (for security reasons)", therefore
        // we don't need them.
        socket->write(QString(QLatin1String("USER %1 unknown unknown :%2\r\n")).arg(auth.user(), auth.user()).toLocal8Bit());

        //defaultBuffer = createBuffer(host);
    }

    void SessionPrivate::_q_disconnected()
    {
        Q_Q(Session);
        emit q->disconnected();
    }

    void SessionPrivate::_q_reconnect()
    {
        if (socket)
        {
            socket->connectToHost(host, port);
            if (socket->inherits("QSslSocket"))
                QMetaObject::invokeMethod(socket, "startClientEncryption");
        }
    }

    void SessionPrivate::_q_error()
    {
        Q_ASSERT(false);
    }

    void SessionPrivate::_q_state(QAbstractSocket::SocketState state)
    {
        Q_UNUSED(state);
    }

    void SessionPrivate::_q_readData()
    {
        buffer += socket->readAll();
        // try reading RFC compliant message lines first
        readLines("\r\n");
        // fall back to RFC incompliant lines...
        readLines("\n");
    }

    void SessionPrivate::readLines(const QByteArray& delimiter)
    {
        int i = -1;
        while ((i = buffer.indexOf(delimiter)) != -1)
        {
            QByteArray line = buffer.left(i).trimmed();
            buffer = buffer.mid(i + delimiter.length());
            if (!line.isEmpty())
                processLine(line);
        }
    }

    void SessionPrivate::processLine(const QByteArray& line)
    {
        Q_Q(Session);
        parser.parse(line);

        QString prefix = parser.prefix();
        QString command = parser.command();
        QStringList params = parser.params();

        bool isNumeric = false;
        uint code = command.toInt(&isNumeric);

        // handle PING/PONG
        if (command == QLatin1String("PING"))
        {
            QString arg = params.value(0);
            raw(QString(QLatin1String("PONG %1")).arg(arg));
            return;
        }

        // and dump
        if (isNumeric)
        {
            switch (code)
            {
                case Irc::Rfc::RPL_WELCOME:
                {
                    welcomed = true;
                    emit q->connected();
                    break;
                }

                /* TODO:
                case Irc::Rfc::RPL_TOPIC:
                {
                    QString channel = params.value(1);
                    QString target = resolveTarget(QString(), channel);
                    createBuffer(target);
                    break;
                }

                case Irc::Rfc::RPL_NAMREPLY:
                {
                    QStringList list = params;
                    list.removeAll(QLatin1String("="));
                    list.removeAll(QLatin1String("@"));
                    list.removeAll(QLatin1String("*"));

                    QString target = resolveTarget(QString(), list.value(1));
                    Buffer* buffer = createBuffer(target);
                    buffer->d_func()->names += list.value(2).split(QLatin1String(" "), QString::SkipEmptyParts);
                    break;
                }

                case Irc::Rfc::RPL_ENDOFNAMES:
                {
                    QString target = resolveTarget(QString(), params.value(1));
                    Buffer* buffer = createBuffer(target);
                    emit buffer->namesReceived(buffer->d_func()->names);
                    break;
                }

                case Irc::Rfc::RPL_MOTDSTART:
                    motd.clear();
                    break;

                case Irc::Rfc::RPL_MOTD:
                    motd.append(params.value(1) + QLatin1Char('\n'));
                    break;

                case Irc::Rfc::RPL_ENDOFMOTD:
                    if (defaultBuffer)
                        emit defaultBuffer->motdReceived(motd);
                    motd.clear();
                    break;
                */

                default:
                    break;
            }

            //if (defaultBuffer)
            //    emit defaultBuffer->numericMessageReceived(prefix, code, params);
        }
        /*else
        {
            if (command == QLatin1String("NICK"))
            {
                QString oldNick = Util::nickFromTarget(prefix);
                QString newNick = params.value(0);

                if (nick == oldNick)
                    nick = newNick;

                foreach (Buffer* buffer, buffers)
                {
                    if (buffer->receiver() == oldNick)
                        buffer->d_func()->setReceiver(newNick);

                    if (buffer->names().contains(oldNick))
                    {
                        buffer->d_func()->removeName(oldNick);
                        buffer->d_func()->addName(newNick);
                        emit buffer->nickChanged(prefix, newNick);
                    }
                }
            }
            else if (command == QLatin1String("QUIT"))
            {
                QString reason = params.value(0);
                QString nick = Util::nickFromTarget(prefix);

                foreach (Buffer* buffer, buffers)
                {
                    if (buffer->names().contains(nick))
                    {
                        buffer->d_func()->removeName(nick);
                        emit buffer->quit(prefix, reason);
                    }
                }
            }
            else if (command == QLatin1String("JOIN"))
            {
                QString channel = params.value(0);
                QString target = resolveTarget(prefix, channel);
                Buffer* buffer = createBuffer(target);
                //buffer->d_func()->addName(Util::nickFromTarget(prefix));
                emit buffer->joined(prefix);
            }
            else if (command == QLatin1String("PART"))
            {
                QString channel = params.value(0);
                QString target = resolveTarget(prefix, channel);
                if (nick != Util::nickFromTarget(prefix))
                {
                    QString message = params.value(1);
                    Buffer* buffer = createBuffer(target);
                    //buffer->d_func()->removeName(Util::nickFromTarget(prefix));
                    emit buffer->parted(prefix, message);
                }
                else if (buffers.contains(target.toLower()))
                {
                    Buffer* buffer = buffers.value(target.toLower());
                    removeBuffer(buffer);
                    buffer->deleteLater();
                }
            }
            else if (command == QLatin1String("MODE"))
            {
                QString receiver = params.value(0);
                QString mode = params.value(1);
                QString args = params.value(2);
                QString target = resolveTarget(prefix, receiver);
                Buffer* buffer = createBuffer(target);
                //buffer->d_func()->updateMode(args, mode);
                emit buffer->modeChanged(prefix, mode, args);
            }
            else if (command == QLatin1String("TOPIC"))
            {
                QString channel = params.value(0);
                QString topic = params.value(1);
                QString target = resolveTarget(prefix, channel);
                Buffer* buffer = createBuffer(target);
                //buffer->d_func()->topic = topic;
                emit buffer->topicChanged(prefix, topic);
            }
            else if (command == QLatin1String("INVITE"))
            {
                QString receiver = params.value(0);
                QString channel = params.value(1);
                if (defaultBuffer)
                    emit defaultBuffer->invited(prefix, receiver, channel);
            }
            else if (command == QLatin1String("KICK"))
            {
                QString channel = params.value(0);
                QString nick = params.value(1);
                QString message = params.value(2);
                QString target = resolveTarget(prefix, channel);
                Buffer* buffer = createBuffer(target);
                //buffer->d_func()->removeName(nick);
                emit buffer->kicked(prefix, nick, message);
            }
            else if (command == QLatin1String("PRIVMSG"))
            {
                QString message = params.value(1);

                if (message.startsWith(QLatin1Char('\1')) && message.endsWith(QLatin1Char('\1')))
                {
                    message.remove(0, 1);
                    message.remove(message.length() - 1, 1);

                    if (message.startsWith(QLatin1String("ACTION ")))
                    {
                        QString receiver = params.value(0);
                        QString target = resolveTarget(prefix, receiver);
                        Buffer* buffer = createBuffer(target);
                        emit buffer->ctcpActionReceived(prefix, message.mid(7));
                    }
                    else
                    {
                        if (defaultBuffer)
                            emit defaultBuffer->ctcpRequestReceived(prefix, message);
                    }
                }
                else
                {
                    QString receiver = params.value(0);
                    QString target = resolveTarget(prefix, receiver);
                    Buffer* buffer = createBuffer(target);
                    emit buffer->messageReceived(prefix, message);
                }
            }
            else if (command == QLatin1String("NOTICE"))
            {
                if (!welcomed)
                {
                    Q_ASSERT(defaultBuffer);
                    defaultBuffer->d_func()->setReceiver(prefix, false);
                }

                QString receiver = params.value(0);
                QString message = params.value(1);

                if (message.startsWith(QLatin1Char('\1')) && message.endsWith(QLatin1Char('\1')))
                {
                    message.remove(0, 1);
                    message.remove(message.length() - 1, 1);

                    if (defaultBuffer)
                        emit defaultBuffer->ctcpReplyReceived(prefix, message);
                }
                else
                {
                    QString target = resolveTarget(prefix, receiver);
                    Buffer* buffer = createBuffer(target);
                    emit buffer->noticeReceived(prefix, message);
                }
            }
            else if (command == QLatin1String("KILL"))
            {
                ; // ignore this event - not all servers generate this
            }
            else
            {
                // The "unknown" event is triggered upon receipt of any number of
                // unclassifiable miscellaneous messages, which aren't handled by
                // the library.
                if (defaultBuffer)
                    emit defaultBuffer->unknownMessageReceived(prefix, params);
            }
        }*/
    }

    bool SessionPrivate::isConnected() const
    {
        return socket &&
            (socket->state() == QAbstractSocket::ConnectingState
             || socket->state() == QAbstractSocket::ConnectedState);
    }

    /*
    QString SessionPrivate::resolveTarget(const QString& sender, const QString& receiver) const
    {
        QString target = receiver;

        if (target.contains(QLatin1Char('*')) || target.contains(QLatin1Char('?')))
            target = nick;

        if (target == nick)
        {
            if (target == sender)
                target = host;
            else
                target = sender;
        }

        if (target.isEmpty() || target == QLatin1String("AUTH"))
            target = host;

        return target;
    }
    */

    /*
    Buffer* SessionPrivate::createBuffer(const QString& receiver)
    {
        Q_Q(Session);
        QString lower = receiver.toLower();
        QString lowerNick = Util::nickFromTarget(receiver).toLower();
        if (lower != lowerNick && buffers.contains(lowerNick))
        {
            Buffer* buffer = buffers.value(lowerNick);
            //buffer->d_func()->setReceiver(lower);
            buffers.insert(lower, buffer);
            buffers.remove(lowerNick);
        }
        else if (!buffers.contains(lower) && !buffers.contains(lowerNick))
        {
            Buffer* buffer = q->createBuffer(receiver);
            buffers.insert(lower, buffer);
            emit q->bufferAdded(buffer);
        }
        return buffers.value(lower);
    }
    */

    /*
    void SessionPrivate::removeBuffer(Buffer* buffer)
    {
        Q_Q(Session);
        QString lower = buffer->receiver().toLower();
        if (buffers.take(lower) == buffer)
            emit q->bufferRemoved(buffer);
    }
    */

    bool SessionPrivate::raw(const QString& msg)
    {
        qint64 bytes = -1;
        if (socket)
            bytes = socket->write(msg.toUtf8() + QByteArray("\r\n"));
        return bytes != -1;
    }

    /*!
        Constructs a new IRC session with \a parent.
     */
    Session::Session(QObject* parent) : QObject(parent), d_ptr(new SessionPrivate)
    {
        Q_D(Session);
        d->init(this);
    }

    /*!
        Destructs the IRC session.
     */
    Session::~Session()
    {
        Q_D(Session);
        if (d->socket)
            d->socket->close();
    }

    /*!
        Returns the encoding.

        The default value is a null QByteArray.
     */
    QByteArray Session::encoding() const
    {
        Q_D(const Session);
        return d->parser.encoding();
    }

    /*!
        Sets the \a encoding.

        See QTextCodec documentation for supported encodings.

        Encoding auto-detection can be turned on by passing a null QByteArray.

        The fallback locale is QTextCodec::codecForLocale().
     */
    void Session::setEncoding(const QByteArray& encoding)
    {
        Q_D(Session);
        d->parser.setEncoding(encoding);
    }

    /*!
        Returns the host.
     */
    QString Session::host() const
    {
        Q_D(const Session);
        return d->host;
    }

    /*!
        Sets the \a host.
     */
    void Session::setHost(const QString& host)
    {
        Q_D(Session);
        if (d->isConnected())
            qWarning("Session::setHost() has no effect until re-connect");
        d->host = host;
    }

    /*!
        Returns the port.
     */
    quint16 Session::port() const
    {
        Q_D(const Session);
        return d->port;
    }

    /*!
        Sets the \a port.
     */
    void Session::setPort(quint16 port)
    {
        Q_D(Session);
        if (d->isConnected())
            qWarning("Session::setPort() has no effect until re-connect");
        d->port = port;
    }

    /*!
        Returns the socket.

        Irc::Session creates an instance of QTcpSocket by default.

        This function was introduced in version 0.3.0.
     */
    QAbstractSocket* Session::socket() const
    {
        Q_D(const Session);
        return d->socket;
    }

    /*!
        Sets the \a socket. The previously set socket is deleted if its parent is \c this.

        Irc::Session supports QSslSocket in the way that it automatically calls
        QSslSocket::startClientEncryption() while connecting.

        This function was introduced in version 0.3.0.
     */
    void Session::setSocket(QAbstractSocket* socket)
    {
        Q_D(Session);
        if (d->socket != socket)
        {
            if (d->socket)
            {
                d->socket->disconnect(this);
                if (d->socket->parent() == this)
                    d->socket->deleteLater();
            }

            d->socket = socket;
            if (socket)
            {
                connect(socket, SIGNAL(connected()), this, SLOT(_q_connected()));
                connect(socket, SIGNAL(disconnected()), this, SLOT(_q_disconnected()));
                connect(socket, SIGNAL(readyRead()), this, SLOT(_q_readData()));
                connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_q_error()));
                connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(_q_state(QAbstractSocket::SocketState)));
            }
        }
    }

    /*!
        Returns all allocated buffers.

        \sa buffer()
     */
    /*
    QList<Buffer*> Session::buffers() const
    {
        Q_D(const Session);
        return d->buffers.values();
    }*/

    /*!
        Returns the buffer for \a receiver. Returns \c 0
        if the buffer does not exist.

        \sa buffers()
        \sa addBuffer()
     */
    /*
    Buffer* Session::buffer(const QString& receiver) const
    {
        Q_D(const Session);
        if (receiver.isNull())
            return d->buffers.value(d->host.toLower());
        return d->buffers.value(receiver.toLower());
    }
    */

    /*!
        Adds a buffer for \a receiver and returns it.

        \sa buffer()
     */
    /*
    Buffer* Session::addBuffer(const QString& receiver)
    {
        Q_D(Session);
        return d->createBuffer(receiver);
    }
    */

    /*!
        Returns the default buffer.
     */
    /*
    Buffer* Session::defaultBuffer() const
    {
        Q_D(const Session);
        return d->defaultBuffer;
    }
    */

    /*!
        Sets the default \a buffer.
     */
    /*
    void Session::setDefaultBuffer(Buffer* buffer)
    {
        Q_D(Session);
        d->defaultBuffer = buffer;
    }
    */

    /*!
        Connects to the server.
     */
    void Session::open()
    {
        Q_D(Session);
        d->_q_reconnect();
    }

    /*!
        Disconnects from the server.
     */
    void Session::close()
    {
        Q_D(Session);
        if (d->socket)
            d->socket->disconnectFromHost();
    }

    /*!
        Sends a \a command to the server.
     */
    bool Session::sendCommand(IrcCommand *command)
    {
        //Q_D(Session);
        //return d->raw(cmd);
        return false;
    }

    /*!
        Returns a new Irc::Buffer object to act as an IRC buffer for \a receiver.

        This virtual factory method can be overridden for example in order to make
        Irc::Session use a subclass of Irc::Buffer.
     */
    /*
    Buffer* Session::createBuffer(const QString& receiver)
    {
        return new Buffer(receiver, this);
    }
    */
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const Irc::Session* session)
{
    if (!session)
        return debug << "Irc::Session(0x0) ";
    debug.nospace() << session->metaObject()->className() << '(' << (void*) session;
    if (!session->objectName().isEmpty())
        debug << ", name = " << session->objectName();
    if (!session->host().isEmpty())
        debug << ", host = " << session->host()
              << ", port = " << session->port();
    debug << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircsession.cpp"
