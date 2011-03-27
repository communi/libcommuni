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
#include "ircmessage.h"
#include "ircutil.h"
#include <QTcpSocket>
#include <QStringList>

/*!
    \class IrcSession ircsession.h
    \brief The IrcSession class provides an IRC session.

    IRC (Internet Relay Chat protocol) is a simple communication protocol.
    IrcSession provides means to establish a connection to an IRC server.
    IrcSession works asynchronously. None of the functions block the
    calling thread but they return immediately and the actual work is done
    behind the scenes in the event loop.

    Example usage:
    \code
    IrcSession* session = new IrcSession(this);
    session->setNick("jpnurmi");
    session->setIdent("jpn");
    session->setRealName("J-P Nurmi");
    session->connectToServer("irc.freenode.net", 6667);
    \endcode

    \note IrcSession supports SSL (Secure Sockets Layer) connections since version 0.3.0

    Example SSL usage:
    \code
    IrcSession* session = new IrcSession(this);
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
    \fn void IrcSession::connecting()

    This signal is emitted when the connection is being established.
 */

/*!
    \fn void IrcSession::connected()

    This signal is emitted when the welcome message has been received.

    \sa Irc::RPL_WELCOME
 */

/*!
    \fn void IrcSession::disconnected()

    This signal is emitted when the session has been disconnected.
 */

IrcSessionPrivate::IrcSessionPrivate(IrcSession* session) :
    q_ptr(session),
    parser(),
    buffer(),
    socket(0),
    host(),
    port(6667),
    userName(),
    nickName(),
    realName()
{
}

void IrcSessionPrivate::_q_connected()
{
    Q_Q(IrcSession);
    emit q->connecting();

    QString password;
    emit q->password(&password);
    if (!password.isEmpty()) {
        IrcPasswordMessage passwdMsg(password);
        q->sendMessage(passwdMsg);
    }

    IrcNickNameMessage nickMsg(nickName);
    q->sendMessage(nickMsg);

    IrcUserMessage userMsg(userName, realName);
    q->sendMessage(userMsg);
}

void IrcSessionPrivate::_q_disconnected()
{
    Q_Q(IrcSession);
    emit q->disconnected();
}

void IrcSessionPrivate::_q_reconnect()
{
    if (socket)
    {
        socket->connectToHost(host, port);
        if (socket->inherits("QSslSocket"))
            QMetaObject::invokeMethod(socket, "startClientEncryption");
    }
}

void IrcSessionPrivate::_q_error(QAbstractSocket::SocketError error)
{
    qDebug() << "IrcSessionPrivate::_q_error():" << error;
}

void IrcSessionPrivate::_q_state(QAbstractSocket::SocketState state)
{
    qDebug() << "IrcSessionPrivate::_q_state():" << state;
}

void IrcSessionPrivate::_q_readData()
{
    buffer += socket->readAll();
    // try reading RFC compliant message lines first
    readLines("\r\n");
    // fall back to RFC incompliant lines...
    readLines("\n");
}

void IrcSessionPrivate::readLines(const QByteArray& delimiter)
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

void IrcSessionPrivate::processLine(const QByteArray& line)
{
    Q_Q(IrcSession);
    parser.parse(line);

    qDebug() << line;

    QString prefix = parser.prefix();
    QString command = parser.command();
    QStringList params = parser.params();

    // numeric
    bool isNumeric = false;
    uint code = command.toInt(&isNumeric);
    if (isNumeric)
    {
        // connected!
        if (code == Irc::RPL_WELCOME)
            emit q->connected();

        IrcNumericMessage msg = IrcNumericMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }

    // error
    else if (command == QLatin1String("ERROR"))
    {
        IrcErrorMessage msg = IrcErrorMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }

    // handle PING/PONG
    else if (command == QLatin1String("PING"))
    {
        IrcPingMessage ping = IrcPingMessage::fromString(prefix, params);
        emit q->messageReceived(ping);
        // TODO: ifAutomatic?
        IrcPongMessage pong(ping.target());
        q->sendMessage(pong);
    }

    // connection registration
    else if (command == QLatin1String("NICK"))
    {
        IrcNickNameMessage msg = IrcNickNameMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("QUIT"))
    {
        IrcQuitMessage msg = IrcQuitMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }

    // channel operations
    else if (command == QLatin1String("JOIN"))
    {
        IrcJoinMessage msg = IrcJoinMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("PART"))
    {
        IrcPartMessage msg = IrcPartMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("TOPIC"))
    {
        IrcTopicMessage msg = IrcTopicMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("NAMES"))
    {
        IrcNamesMessage msg = IrcNamesMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("LIST"))
    {
        IrcListMessage msg = IrcListMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("INVITE"))
    {
        IrcInviteMessage msg = IrcInviteMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("KICK"))
    {
        IrcKickMessage msg = IrcKickMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }

    // mode operations
    else if (command == QLatin1String("MODE"))
    {
        if (params.value(0).startsWith(QLatin1Char('#')))
        {
            IrcChannelModeMessage msg = IrcChannelModeMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
        else
        {
            IrcUserModeMessage msg = IrcUserModeMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
    }

    // sending messages & ctcp messages
    else if (command == QLatin1String("PRIVMSG"))
    {
        if (params.value(1).startsWith(QLatin1String("\1ACTION ")))
        {
            IrcCtcpActionMessage msg = IrcCtcpActionMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
        else if (params.value(1).startsWith(QLatin1Char('\1')))
        {
            IrcCtcpRequestMessage msg = IrcCtcpRequestMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
        else
        {
            IrcPrivateMessage msg = IrcPrivateMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
    }
    else if (command == QLatin1String("NOTICE"))
    {
        if (params.value(1).startsWith(QLatin1Char('\1')))
        {
            IrcCtcpReplyMessage msg = IrcCtcpReplyMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
        else
        {
            IrcNoticeMessage msg = IrcNoticeMessage::fromString(prefix, params);
            emit q->messageReceived(msg);
        }
    }

    // user-based queries
    else if (command == QLatin1String("WHO"))
    {
        IrcWhoMessage msg = IrcWhoMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("WHOIS"))
    {
        IrcWhoisMessage msg = IrcWhoisMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
    else if (command == QLatin1String("WHOWAS"))
    {
        IrcWhowasMessage msg = IrcWhowasMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }

    // unknown
    else
    {
        IrcMessage msg = IrcMessage::fromString(prefix, params);
        emit q->messageReceived(msg);
    }
}

bool IrcSessionPrivate::isConnected() const
{
    return socket &&
        (socket->state() == QAbstractSocket::ConnectingState
         || socket->state() == QAbstractSocket::ConnectedState);
}

bool IrcSessionPrivate::raw(const QString& msg)
{
    qint64 bytes = -1;
    if (socket)
        bytes = socket->write(msg.toUtf8() + QByteArray("\r\n"));
    return bytes != -1;
}

/*!
    Constructs a new IRC session with \a parent.
 */
IrcSession::IrcSession(QObject* parent) : QObject(parent), d_ptr(new IrcSessionPrivate(this))
{
    setSocket(new QTcpSocket(this));
}

/*!
    Destructs the IRC session.
 */
IrcSession::~IrcSession()
{
    Q_D(IrcSession);
    if (d->socket)
        d->socket->close();
}

/*!
    Returns the encoding.

    The default value is a null QByteArray.
 */
QByteArray IrcSession::encoding() const
{
    Q_D(const IrcSession);
    return d->parser.encoding();
}

/*!
    Sets the \a encoding.

    See QTextCodec documentation for supported encodings.

    Encoding auto-detection can be turned on by passing a null QByteArray.

    The fallback locale is QTextCodec::codecForLocale().
 */
void IrcSession::setEncoding(const QByteArray& encoding)
{
    Q_D(IrcSession);
    d->parser.setEncoding(encoding);
}

/*!
    Returns the host.
 */
QString IrcSession::host() const
{
    Q_D(const IrcSession);
    return d->host;
}

/*!
    Sets the \a host.
 */
void IrcSession::setHost(const QString& host)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setHost() has no effect until re-connect");
    d->host = host;
}

/*!
    Returns the port.
 */
quint16 IrcSession::port() const
{
    Q_D(const IrcSession);
    return d->port;
}

/*!
    Sets the \a port.
 */
void IrcSession::setPort(quint16 port)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setPort() has no effect until re-connect");
    d->port = port;
}

/*!
    Returns the user name.
 */
QString IrcSession::userName() const
{
    Q_D(const IrcSession);
    return d->userName;
}

/*!
    Sets the user \a name.

    \note setUserName() has no effect on already established connection.
 */
void IrcSession::setUserName(const QString& name)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setUserName() has no effect until re-connect");
    d->userName = name;
}

/*!
    Returns the nick name.
 */
QString IrcSession::nickName() const
{
    Q_D(const IrcSession);
    return d->nickName;
}

/*!
    Sets the nick \a name.
 */
void IrcSession::setNickName(const QString& name)
{
    Q_D(IrcSession);
    if (d->nickName != name)
    {
        d->nickName = name;
        // TODO:
        //if (d->socket)
        //    raw(QString(QLatin1String("NICK %1")).arg(nick));
    }
}

/*!
    Returns the real name.
 */
QString IrcSession::realName() const
{
    Q_D(const IrcSession);
    return d->realName;
}

/*!
    Sets the real \a name.

    \note setRealName() has no effect on already established connection.
 */
void IrcSession::setRealName(const QString& name)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setRealName() has no effect until re-connect");
    d->realName = name;
}

/*!
    Returns the socket.

    IrcSession creates an instance of QTcpSocket by default.

    This function was introduced in version 0.3.0.
 */
QAbstractSocket* IrcSession::socket() const
{
    Q_D(const IrcSession);
    return d->socket;
}

/*!
    Sets the \a socket. The previously set socket is deleted if its parent is \c this.

    IrcSession supports QSslSocket in the way that it automatically calls
    QSslSocket::startClientEncryption() while connecting.

    This function was introduced in version 0.3.0.
 */
void IrcSession::setSocket(QAbstractSocket* socket)
{
    Q_D(IrcSession);
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
            connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_q_error(QAbstractSocket::SocketError)));
            connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(_q_state(QAbstractSocket::SocketState)));
        }
    }
}

/*!
    Connects to the server.
 */
void IrcSession::open()
{
    Q_D(IrcSession);
    if (d->userName.isEmpty())
    {
        qCritical("IrcSession::open(): userName is empty!");
        return;
    }
    if (d->nickName.isEmpty())
    {
        qCritical("IrcSession::open(): nickName is empty!");
        return;
    }
    if (d->realName.isEmpty())
    {
        qCritical("IrcSession::open(): realName is empty!");
        return;
    }
    d->_q_reconnect();
}

/*!
    Disconnects from the server.
 */
void IrcSession::close()
{
    Q_D(IrcSession);
    if (d->socket)
        d->socket->disconnectFromHost();
}

/*!
    Sends a \a message to the server.
 */
bool IrcSession::sendMessage(const IrcMessage& message)
{
    Q_D(IrcSession);
    return d->raw(message.toString());
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcSession* session)
{
    if (!session)
        return debug << "IrcSession(0x0) ";
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
