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
        ident(QLatin1String("libircclient-qt")),
        password(),
        nick(QLatin1String("libircclient")),
        realName(QLatin1String("libircclient-qt")),
        host(),
        port(6667),
        motd(),
        delay(-1),
        timer(),
        defaultBuffer(),
        buffers(),
        welcomed(false)
    {
    }

    void SessionPrivate::init(Session* session)
    {
        q_ptr = session;

        Q_Q(Session);
        q->setSocket(new QTcpSocket(q));
        q->connect(&timer, SIGNAL(timeout()), q, SLOT(_q_reconnect()));
    }

    void SessionPrivate::_q_connected()
    {
        Q_Q(Session);

        Q_ASSERT(!nick.isEmpty());
        Q_ASSERT(!ident.isEmpty());

        // stop autoreconnecting...
        if (timer.isActive())
            timer.stop();

        // Send CAP LS first; if the server understands it this will
        // temporarily pause the handshake until CAP END is sent, so we
        // know whether the server supports the CAP extension.
        socket->write("CAP LS\r\n");

        if (!password.isEmpty())
            socket->write(QString(QLatin1String("PASS %1\r\n")).arg(password).toLocal8Bit());

        socket->write(QString(QLatin1String("NICK %1\r\n")).arg(nick).toLocal8Bit());

        // RFC 1459 states that "hostname and servername are normally
        // ignored by the IRC server when the USER command comes from
        // a directly connected client (for security reasons)", therefore
        // we don't need them.
        socket->write(QString(QLatin1String("USER %1 unknown unknown :%2\r\n")).arg(ident, realName).toLocal8Bit());

        defaultBuffer = createBuffer(host);
        emit q->connected();
        welcomed = false;
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

        // stop autoreconnecting...
        if (timer.isActive())
            timer.stop();
    }

    void SessionPrivate::_q_error()
    {
        Q_Q(Session);
        // start reconnecting...
        if (delay >= 0)
        {
            timer.start(delay * 1000);
            emit q->reconnecting();
        }
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
            q->raw(QString(QLatin1String("PONG %1")).arg(arg));
            return;
        }

        // and dump
        if (isNumeric)
        {
            switch (code)
            {
                case Irc::Rfc::RPL_WELCOME:
                {
                    Q_ASSERT(defaultBuffer);
                    defaultBuffer->d_func()->setReceiver(prefix, false);

                    emit q->welcomed();
                    welcomed = true;
                    break;
                }

                case Irc::Rfc::RPL_TOPIC:
                {
                    QString channel = params.value(1);
                    QString target = resolveTarget(QString(), channel);
                    Buffer* buffer = createBuffer(target);
                    buffer->d_func()->topic = params.value(2);
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
                    QStringList names = list.value(2).split(QLatin1String(" "), QString::SkipEmptyParts);
                    foreach (const QString& name, names)
                        buffer->d_func()->addName(name);
                    break;
                }

                case Irc::Rfc::RPL_ENDOFNAMES:
                {
                    QString target = resolveTarget(QString(), params.value(1));
                    Buffer* buffer = createBuffer(target);
                    emit buffer->namesReceived(buffer->names());
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

                default:
                    break;
            }

            if (defaultBuffer)
                emit defaultBuffer->numericMessageReceived(prefix, code, params);
        }
        else
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
                buffer->d_func()->addName(Util::nickFromTarget(prefix));
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
                    buffer->d_func()->removeName(Util::nickFromTarget(prefix));
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
                buffer->d_func()->updateMode(args, mode);
                emit buffer->modeChanged(prefix, mode, args);
            }
            else if (command == QLatin1String("TOPIC"))
            {
                QString channel = params.value(0);
                QString topic = params.value(1);
                QString target = resolveTarget(prefix, channel);
                Buffer* buffer = createBuffer(target);
                buffer->d_func()->topic = topic;
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
                buffer->d_func()->removeName(nick);
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
                        // TODO: check params
                        if (defaultBuffer)
                            emit defaultBuffer->ctcpRequestReceived(prefix, /*receiver,*/ message);
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

                    // TODO: check params
                    if (defaultBuffer)
                        emit defaultBuffer->ctcpReplyReceived(prefix, /*receiver,*/ message);
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
        }
    }

    bool SessionPrivate::isConnected() const
    {
        return socket &&
            (socket->state() == QAbstractSocket::ConnectingState
             || socket->state() == QAbstractSocket::ConnectedState);
    }

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

    Buffer* SessionPrivate::createBuffer(const QString& receiver)
    {
        Q_Q(Session);
        QString lower = receiver.toLower();
        QString lowerNick = Util::nickFromTarget(receiver).toLower();
        if (lower != lowerNick && buffers.contains(lowerNick))
        {
            Buffer* buffer = buffers.value(lowerNick);
            buffer->d_func()->setReceiver(lower);
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

    void SessionPrivate::removeBuffer(Buffer* buffer)
    {
        Q_Q(Session);
        QString lower = buffer->receiver().toLower();
        if (buffers.take(lower) == buffer)
            emit q->bufferRemoved(buffer);
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
        delete d;
    }

    /*!
        Returns the auto-reconnecting delay.

        A negative value means no auto-reconnecting.

        The default value is \c -1.
     */
    int Session::autoReconnectDelay() const
    {
        Q_D(const Session);
        return d->delay;
    }

    /*!
        Sets auto-reconnecting delay in \a seconds.
     */
    void Session::setAutoReconnectDelay(int seconds)
    {
        Q_D(Session);
        d->delay = seconds;
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
        Returns the ident.
     */
    QString Session::ident() const
    {
        Q_D(const Session);
        return d->ident;
    }

    /*!
        Sets the \a ident.

        \note setIdent() has no effect on already established connection.
     */
    void Session::setIdent(const QString& ident)
    {
        Q_D(Session);
        if (d->isConnected())
            qWarning("Session::setIdent() has no effect until re-connect");
        d->ident = ident;
    }

    /*!
        Returns the nick.
     */
    QString Session::nick() const
    {
        Q_D(const Session);
        return d->nick;
    }

    /*!
        Sets the \a nick.
     */
    void Session::setNick(const QString& nick)
    {
        Q_D(Session);
        if (d->nick != nick)
        {
            d->nick = nick;
            if (d->socket)
            	raw(QString(QLatin1String("NICK %1")).arg(nick));
        }
    }

    /*!
        Returns the password.
     */
    QString Session::password() const
    {
        Q_D(const Session);
        return d->password;
    }

    /*!
        Sets the \a password.

        \note setPassword() has no effect on already established connection.
     */
    void Session::setPassword(const QString& password)
    {
        Q_D(Session);
        if (d->isConnected())
            qWarning("Session::setPassword() has no effect until re-connect");
        d->password = password;
    }

    /*!
        Returns the real name.
     */
    QString Session::realName() const
    {
        Q_D(const Session);
        return d->realName;
    }

    /*!
        Sets the \a realName.

        \note setRealName() has no effect on already established connection.
     */
    void Session::setRealName(const QString& realName)
    {
        Q_D(Session);
        if (d->isConnected())
            qWarning("Session::setRealName() has no effect until re-connect");
        d->realName = realName;
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
    QList<Buffer*> Session::buffers() const
    {
        Q_D(const Session);
        return d->buffers.values();
    }

    /*!
        Returns the buffer for \a receiver. Returns \c 0
        if the buffer does not exist.

        \sa buffers()
        \sa addBuffer()
     */
    Buffer* Session::buffer(const QString& receiver) const
    {
        Q_D(const Session);
        if (receiver.isNull())
            return d->buffers.value(d->host.toLower());
        return d->buffers.value(receiver.toLower());
    }

    /*!
        Adds a buffer for \a receiver and returns it.

        \sa buffer()
     */
    Buffer* Session::addBuffer(const QString& receiver)
    {
        Q_D(Session);
        return d->createBuffer(receiver);
    }

    /*!
        Returns the default buffer.
     */
    Buffer* Session::defaultBuffer() const
    {
        Q_D(const Session);
        return d->defaultBuffer;
    }

    /*!
        Sets the default \a buffer.
     */
    void Session::setDefaultBuffer(Buffer* buffer)
    {
        Q_D(Session);
        d->defaultBuffer = buffer;
    }

    /*!
        Connects to server with \a hostName and \a port.
     */
    void Session::connectToServer(const QString& hostName, quint16 port)
    {
        Q_D(Session);
        if (!hostName.isNull())
            d->host = hostName;
        if (port != 6667)
            d->port = port;
        d->_q_reconnect();
    }

    /*!
        Reconnects to server.
     */
    void Session::reconnectToServer()
    {
        Q_D(Session);
        d->_q_reconnect();
    }

    /*!
        Disconnects from the server.
     */
    void Session::disconnectFromServer()
    {
        Q_D(Session);
        if (d->socket)
            d->socket->disconnectFromHost();
    }

    /*!
        Sends a raw message to the server.
     */
    bool Session::raw(const QString& message)
    {
        Q_D(Session);
        qint64 bytes = -1;
        if (d->socket)
            bytes = d->socket->write(message.toUtf8() + QByteArray("\r\n"));
        return bytes != -1;
    }

    /*!
        Requests message of the day.
     */
    bool Session::motd()
    {
        return raw(QLatin1String("MOTD"));
    }

    /*!
        Joins \a channel with optional \a key.
     */
    bool Session::join(const QString& channel, const QString& key)
    {
        if (key.isEmpty())
            return raw(QString(QLatin1String("JOIN %1")).arg(channel));
        else
            return raw(QString(QLatin1String("JOIN %1 %2")).arg(channel, key));
    }

    /*!
        Parts \a channel with \a reason.
     */
    bool Session::part(const QString& channel, const QString& reason)
    {
        if (reason.isEmpty())
            return raw(QString(QLatin1String("PART %1")).arg(channel));
        else
            return raw(QString(QLatin1String("PART %1 :%2")).arg(channel, reason));
    }

    /*!
        Quits with optional \a reason.
     */
    bool Session::quit(const QString& reason)
    {
        return raw(QString(QLatin1String("QUIT :%1")).arg(reason.isEmpty() ? QLatin1String("Quit") : reason));
    }

    /*!
        Requests the list of names on \a channel.
     */
    bool Session::names(const QString& channel)
    {
        return raw(QString(QLatin1String("NAMES %1")).arg(channel));
    }

    /*!
        Requests the list of channels.
     */
    bool Session::list(const QString& channel)
    {
        if (channel.isEmpty())
            return raw(QLatin1String("LIST"));
        else
            return raw(QString(QLatin1String("LIST %1")).arg(channel));
    }

    /*!
        Sends a whois command on \a nick.
     */
    bool Session::whois(const QString& nick)
    {
        return raw(QString(QLatin1String("WHOIS %1 %2")).arg(nick, nick));
    }

    /*!
        Sends a whowas command on \a nick.
     */
    bool Session::whowas(const QString& nick)
    {
        return raw(QString(QLatin1String("WHOWAS %1 %2")).arg(nick, nick));
    }

    /*!
        Sends a mode command on \a target.
     */
    bool Session::mode(const QString& target, const QString& mode)
    {
        if (mode.isEmpty())
            return raw(QString(QLatin1String("MODE %1")).arg(target));
        else
            return raw(QString(QLatin1String("MODE %1 %2")).arg(target, mode));
    }

    /*!
        Sends a topic command on \a channel.
     */
    bool Session::topic(const QString& channel, const QString& topic)
    {
        if (topic.isEmpty())
            return raw(QString(QLatin1String("TOPIC %1")).arg(channel));
        else
            return raw(QString(QLatin1String("TOPIC %1 :%2")).arg(channel, topic));
    }

    /*!
        Sends an invitation to \a nick.
     */
    bool Session::invite(const QString& nick, const QString& channel)
    {
        return raw(QString(QLatin1String("INVITE %1 %2")).arg(nick, channel));
    }

    /*!
        Kicks \a nick from \a channel with \a reason.
     */
    bool Session::kick(const QString& nick, const QString& channel, const QString& reason)
    {
        if (reason.isEmpty())
            return raw(QString(QLatin1String("KICK %1 %2")).arg(channel, nick));
        else
            return raw(QString(QLatin1String("KICK %1 %2 :%3")).arg(channel, nick, reason));
    }

    /*!
        Sends a \a message to \a receiver.
     */
    bool Session::message(const QString& receiver, const QString& message)
    {
        return raw(QString(QLatin1String("PRIVMSG %1 :%2")).arg(Util::nickFromTarget(receiver), message));
    }

    /*!
        Sends a \a notice to \a receiver.
     */
    bool Session::notice(const QString& receiver, const QString& notice)
    {
        return raw(QString(QLatin1String("NOTICE %1 :%2")).arg(Util::nickFromTarget(receiver), notice));
    }

    /*!
        Sends a CTCP \a action to \a receiver.
     */
    bool Session::ctcpAction(const QString& receiver, const QString& action)
    {
        return raw(QString(QLatin1String("PRIVMSG %1 :\x01" "ACTION %2\x01")).arg(Util::nickFromTarget(receiver), action));
    }

    /*!
        Sends a CTCP \a request to \a receiver.
     */
    bool Session::ctcpRequest(const QString& nick, const QString& request)
    {
        return raw(QString(QLatin1String("PRIVMSG %1 :\x01%2\x01")).arg(Util::nickFromTarget(nick), request));
    }

    /*!
        Sends a CTCP \a reply to \a receiver.
     */
    bool Session::ctcpReply(const QString& nick, const QString& reply)
    {
        return raw(QString(QLatin1String("NOTICE %1 :\x01%2\x01")).arg(Util::nickFromTarget(nick), reply));
    }

    /*!
        Returns a new Irc::Buffer object to act as an IRC buffer for \a receiver.

        This virtual factory method can be overridden for example in order to make
        Irc::Session use a subclass of Irc::Buffer.
     */
    Buffer* Session::createBuffer(const QString& receiver)
    {
        return new Buffer(receiver, this);
    }
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
