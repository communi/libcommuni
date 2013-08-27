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

#include "ircconnection.h"
#include "ircconnection_p.h"
#include "ircmessagefilter_p.h"
#include "ircprotocol.h"
#include "ircnetwork.h"
#include "irccommand.h"
#include "ircmessage.h"
#include "irc.h"
#include <QLocale>
#include <QDateTime>
#include <QTcpSocket>
#include <QTextCodec>
#include <QStringList>
#include <QMetaObject>
#include <QMetaMethod>
#ifndef QT_NO_OPENSSL
#include <QSslSocket>
#endif // QT_NO_OPENSSL

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcCommand*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcPrivateMessage*))

IRC_BEGIN_NAMESPACE

/*!
    \file ircconnection.h
    \brief #include &lt;IrcConnection&gt;
 */

/*!
    \class IrcConnection ircconnection.h IrcConnection
    \ingroup core
    \brief Provides means to establish a connection to an IRC server.

    \section connection Connection management

    Before \ref open() "opening" a connection, it must be first initialized
    with \ref host, \ref userName, \ref nickName and \ref realName.

    The connection status may be queried at any time via \ref active
    "isActive()" and \ref connected "isConnected()". Changes in the
    connection status are informed via the following signals:
    \li connecting()   - The underlying socket has been connected, but
                         the IRC connection is not yet fully established.
    \li connected()    - The IRC connection has been established,
                         and the connection is ready to start sending commands.
    \li disconnected() - The connection has been disconnected.

    \section messages Receiving messages

    Whenever a message is received from the server, the messageReceived()
    signal is emitted. Also message type specific signals are provided
    for convenience. See messageReceived() and IrcMessage and its
    subclasses for more details.

    \section commands Sending commands

    Sending commands to a server is most conveniently done by creating
    them via the various static \ref IrcCommand "IrcCommand::createXxx()"
    methods and passing them to sendCommand(). Also sendData() is provided
    for more low-level access.

    \section example Example

    \code
    IrcConnection* connection = new IrcConnection(this);
    connect(connection, SIGNAL(connected()), this, SLOT(onConnected())); // ready to send commands
    connect(connection, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(onMessageReceived(IrcMessage*)));
    connection->setHost("irc.server.com");
    connection->setUserName("me");
    connection->setNickName("myself");
    connection->setRealName("And I");
    connection->open();
    \endcode

    \sa IrcMessage and IrcCommand
 */

/*!
    \fn void IrcConnection::connecting()

    This signal is emitted when the connection is being established.
 */

/*!
    \fn void IrcConnection::nickNameReserved(QString* alternate)

    This signal is emitted when the requested nick name is reserved
    and an \a alternate nick name should be provided.

    \sa Irc::ERR_NICKNAMEINUSE, Irc::ERR_NICKCOLLISION
 */

/*!
    \fn void IrcConnection::capabilities(const QStringList& available, QStringList* request)

    This signal is emitted when the connection capabilities may be requested.
    Fill the \a request with capabilities from the \a available capabilities.
 */

/*!
    \fn void IrcConnection::connected()

    This signal is emitted when the connection has been established ie.
    the welcome message has been received and the server is ready to receive commands.

    \sa Irc::RPL_WELCOME
 */

/*!
    \fn void IrcConnection::disconnected()

    This signal is emitted when the connection has been disconnected.
 */

/*!
    \fn void IrcConnection::socketError(QAbstractSocket::SocketError error)

    This signal is emitted whenever a socket \a error occurs.

    \sa QAbstractSocket::error()
 */

/*!
    \fn void IrcConnection::socketStateChanged(QAbstractSocket::SocketState state)

    This signal is emitted whenever the socket's \a state changes.

    \sa QAbstractSocket::stateChanged()
 */

/*!
    \fn void IrcConnection::messageReceived(IrcMessage* message)

    This signal is emitted whenever any type of \a message is received.

    In addition, message type specific signals are provided for convenience:
    \li void <b>capabilityMessageReceived</b>(\ref IrcCapabilityMessage* message)
    \li void <b>errorMessageReceived</b>(\ref IrcErrorMessage* message)
    \li void <b>inviteMessageReceived</b>(\ref IrcInviteMessage* message)
    \li void <b>joinMessageReceived</b>(\ref IrcJoinMessage* message)
    \li void <b>kickMessageReceived</b>(\ref IrcKickMessage* message)
    \li void <b>modeMessageReceived</b>(\ref IrcModeMessage* message)
    \li void <b>namesMessageReceived</b>(\ref IrcNamesMessage* message)
    \li void <b>nickMessageReceived</b>(\ref IrcNickMessage* message)
    \li void <b>noticeMessageReceived</b>(\ref IrcNoticeMessage* message)
    \li void <b>numericMessageReceived</b>(\ref IrcNumericMessage* message)
    \li void <b>motdMessageReceived</b>(\ref IrcMotdMessage* message)
    \li void <b>partMessageReceived</b>(\ref IrcPartMessage* message)
    \li void <b>pingMessageReceived</b>(\ref IrcPingMessage* message)
    \li void <b>pongMessageReceived</b>(\ref IrcPongMessage* message)
    \li void <b>privateMessageReceived</b>(\ref IrcPrivateMessage* message)
    \li void <b>quitMessageReceived</b>(\ref IrcQuitMessage* message)
    \li void <b>topicMessageReceived</b>(\ref IrcTopicMessage* message)
 */

/*!
    \fn void IrcConnection::connectionInfoReceived(const IrcNetwork& info)

    This signal is emitted when the connection \a info has been received.
 */

IrcConnectionPrivate::IrcConnectionPrivate() :
    q_ptr(0),
    encoding("ISO-8859-15"),
    network(0),
    protocol(0),
    socket(0),
    host(),
    port(6667),
    userName(),
    nickName(),
    realName(),
    active(false),
    connected(false)
{
}

void IrcConnectionPrivate::_irc_connected()
{
    Q_Q(IrcConnection);
    emit q->connecting();
    protocol->open();
}

void IrcConnectionPrivate::_irc_disconnected()
{
    Q_Q(IrcConnection);
    emit q->disconnected();
}

void IrcConnectionPrivate::_irc_error(QAbstractSocket::SocketError error)
{
    Q_Q(IrcConnection);
    static bool dbg = qgetenv("IRC_DEBUG").toInt();
    if (dbg) qWarning() << "IrcConnection: socket error:" << error;
    setConnected(false);
    setActive(false);
    emit q->socketError(error);
}

void IrcConnectionPrivate::_irc_state(QAbstractSocket::SocketState state)
{
    Q_Q(IrcConnection);
    setActive(state != QAbstractSocket::UnconnectedState);
    if (state != QAbstractSocket::ConnectedState)
        setConnected(false);

    static bool dbg = qgetenv("IRC_DEBUG").toInt();
    if (dbg) qDebug() << "IrcConnection: socket state:" << state << host;
    emit q->socketStateChanged(state);
}

void IrcConnectionPrivate::_irc_readData()
{
    protocol->read();
}

void IrcConnectionPrivate::setNick(const QString& nick)
{
    Q_Q(IrcConnection);
    if (nickName != nick) {
        nickName = nick;
        emit q->nickNameChanged(nick);
    }
}

void IrcConnectionPrivate::setActive(bool value)
{
    Q_Q(IrcConnection);
    if (active != value) {
        active = value;
        emit q->activeChanged(active);
    }
}

void IrcConnectionPrivate::setConnected(bool value)
{
    Q_Q(IrcConnection);
    if (connected != value) {
        connected = value;
        emit q->connectedChanged(connected);
        if (connected)
            emit q->connected();
    }
}

void IrcConnectionPrivate::receiveMessage(IrcMessage* msg)
{
    Q_Q(IrcConnection);
    bool filtered = false;
    for (int i = filters.count() - 1; !filtered && i >= 0; --i)
        filtered |= filters.at(i)->messageFilter(msg);

    if (!filtered) {
        emit q->messageReceived(msg);

        switch (msg->type()) {
        case IrcMessage::Nick:
            emit q->nickMessageReceived(static_cast<IrcNickMessage*>(msg));
            break;
        case IrcMessage::Quit:
            emit q->quitMessageReceived(static_cast<IrcQuitMessage*>(msg));
            break;
        case IrcMessage::Join:
            emit q->joinMessageReceived(static_cast<IrcJoinMessage*>(msg));
            break;
        case IrcMessage::Part:
            emit q->partMessageReceived(static_cast<IrcPartMessage*>(msg));
            break;
        case IrcMessage::Topic:
            emit q->topicMessageReceived(static_cast<IrcTopicMessage*>(msg));
            break;
        case IrcMessage::Invite:
            emit q->inviteMessageReceived(static_cast<IrcInviteMessage*>(msg));
            break;
        case IrcMessage::Kick:
            emit q->kickMessageReceived(static_cast<IrcKickMessage*>(msg));
            break;
        case IrcMessage::Mode:
            emit q->modeMessageReceived(static_cast<IrcModeMessage*>(msg));
            break;
        case IrcMessage::Private:
            emit q->privateMessageReceived(static_cast<IrcPrivateMessage*>(msg));
            break;
        case IrcMessage::Notice:
            emit q->noticeMessageReceived(static_cast<IrcNoticeMessage*>(msg));
            break;
        case IrcMessage::Ping:
            emit q->pingMessageReceived(static_cast<IrcPingMessage*>(msg));
            break;
        case IrcMessage::Pong:
            emit q->pongMessageReceived(static_cast<IrcPongMessage*>(msg));
            break;
        case IrcMessage::Error:
            emit q->errorMessageReceived(static_cast<IrcErrorMessage*>(msg));
            break;
        case IrcMessage::Numeric:
            emit q->numericMessageReceived(static_cast<IrcNumericMessage*>(msg));
            break;
        case IrcMessage::Capability:
            emit q->capabilityMessageReceived(static_cast<IrcCapabilityMessage*>(msg));
            break;
        case IrcMessage::Motd:
            emit q->motdMessageReceived(static_cast<IrcMotdMessage*>(msg));
            break;
        case IrcMessage::Names:
            emit q->namesMessageReceived(static_cast<IrcNamesMessage*>(msg));
            break;
        case IrcMessage::Unknown:
        default:
            break;
        }
    }
    msg->deleteLater();
}

IrcCommand* IrcConnectionPrivate::createCtcpReply(IrcPrivateMessage* request)
{
    Q_Q(IrcConnection);
    IrcCommand* reply = 0;
    const QMetaObject* metaObject = q->metaObject();
    int idx = metaObject->indexOfMethod("createCtcpReply(QVariant)");
    if (idx != -1) {
        // QML: QVariant createCtcpReply(QVariant)
        QVariant ret;
        QMetaMethod method = metaObject->method(idx);
        method.invoke(q, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(request)));
        reply = ret.value<IrcCommand*>();
    } else {
        // C++: IrcCommand* createCtcpReply(IrcPrivateMessage*)
        idx = metaObject->indexOfMethod("createCtcpReply(IrcPrivateMessage*)");
        QMetaMethod method = metaObject->method(idx);
        method.invoke(q, Q_RETURN_ARG(IrcCommand*, reply), Q_ARG(IrcPrivateMessage*, request));
    }
    return reply;
}

/*!
    Constructs a new IRC connection with \a parent.
 */
IrcConnection::IrcConnection(QObject* parent) : QObject(parent), d_ptr(new IrcConnectionPrivate)
{
    Q_D(IrcConnection);
    d->q_ptr = this;
    d->network = new IrcNetwork(this);
    setSocket(new QTcpSocket(this));
    setProtocol(new IrcProtocol(this));
    qRegisterMetaType<IrcNetwork*>();
}

/*!
    Destructs the IRC connection.
 */
IrcConnection::~IrcConnection()
{
    Q_D(IrcConnection);
    if (d->socket)
        d->socket->close();
}

/*!
    This property holds the FALLBACK encoding for received messages.

    The fallback encoding is used when the message is detected not
    to be valid \c UTF-8 and the consequent auto-detection of message
    encoding fails. See QTextCodec::availableCodecs() for the list of
    supported encodings.

    The default value is \c ISO-8859-15.

    \par Access functions:
    \li QByteArray <b>encoding</b>() const
    \li void <b>setEncoding</b>(const QByteArray& encoding)

    \sa QTextCodec::availableCodecs(), QTextCodec::codecForLocale()
 */
QByteArray IrcConnection::encoding() const
{
    Q_D(const IrcConnection);
    return d->encoding;
}

void IrcConnection::setEncoding(const QByteArray& encoding)
{
    Q_D(IrcConnection);
    extern bool irc_is_supported_encoding(const QByteArray& encoding); // ircmessagedecoder.cpp
    if (!irc_is_supported_encoding(encoding)) {
        qWarning() << "IrcConnection::setEncoding(): unsupported encoding" << encoding;
        return;
    }
    d->encoding = encoding;
}

/*!
    This property holds the server host.

    \par Access functions:
    \li QString <b>host</b>() const
    \li void <b>setHost</b>(const QString& host)

    \par Notifier signal:
    \li void <b>hostChanged</b>(const QString& host)
 */
QString IrcConnection::host() const
{
    Q_D(const IrcConnection);
    return d->host;
}

void IrcConnection::setHost(const QString& host)
{
    Q_D(IrcConnection);
    if (isActive())
        qWarning("IrcConnection::setHost() has no effect until re-connect");
    if (d->host != host) {
        d->host = host;
        emit hostChanged(host);
    }
}

/*!
    This property holds the server port.

    The default value is \c 6667.

    \par Access functions:
    \li int <b>port</b>() const
    \li void <b>setPort</b>(int port)

    \par Notifier signal:
    \li void <b>portChanged</b>(int port)
 */
int IrcConnection::port() const
{
    Q_D(const IrcConnection);
    return d->port;
}

void IrcConnection::setPort(int port)
{
    Q_D(IrcConnection);
    if (isActive())
        qWarning("IrcConnection::setPort() has no effect until re-connect");
    if (d->port != port) {
        d->port = port;
        emit portChanged(port);
    }
}

/*!
    This property holds the user name.

    \note Changing the user name has no effect until the connection is re-established.

    \par Access functions:
    \li QString <b>userName</b>() const
    \li void <b>setUserName</b>(const QString& name)

    \par Notifier signal:
    \li void <b>userNameChanged</b>(const QString& name)
 */
QString IrcConnection::userName() const
{
    Q_D(const IrcConnection);
    return d->userName;
}

void IrcConnection::setUserName(const QString& name)
{
    Q_D(IrcConnection);
    if (isActive())
        qWarning("IrcConnection::setUserName() has no effect until re-connect");
    QString user = name.split(" ", QString::SkipEmptyParts).value(0).trimmed();
    if (d->userName != user) {
        d->userName = user;
        emit userNameChanged(user);
    }
}

/*!
    This property holds the nick name.

    \par Access functions:
    \li QString <b>nickName</b>() const
    \li void <b>setNickName</b>(const QString& name)

    \par Notifier signal:
    \li void <b>nickNameChanged</b>(const QString& name)
 */
QString IrcConnection::nickName() const
{
    Q_D(const IrcConnection);
    return d->nickName;
}

void IrcConnection::setNickName(const QString& name)
{
    Q_D(IrcConnection);
    QString nick = name.split(" ", QString::SkipEmptyParts).value(0).trimmed();
    if (d->nickName != nick) {
        if (isActive())
            sendCommand(IrcCommand::createNick(nick));
        else
            d->setNick(nick);
    }
}

/*!
    This property holds the real name.

    \note Changing the real name has no effect until the connection is re-established.

    \par Access functions:
    \li QString <b>realName</b>() const
    \li void <b>setRealName</b>(const QString& name)

    \par Notifier signal:
    \li void <b>realNameChanged</b>(const QString& name)
 */
QString IrcConnection::realName() const
{
    Q_D(const IrcConnection);
    return d->realName;
}

void IrcConnection::setRealName(const QString& name)
{
    Q_D(IrcConnection);
    if (isActive())
        qWarning("IrcConnection::setRealName() has no effect until re-connect");
    if (d->realName != name) {
        d->realName = name;
        emit realNameChanged(name);
    }
}

/*!
    This property holds the password.

    \par Access functions:
    \li QString <b>password</b>() const
    \li void <b>setPassword</b>(const QString& password)

    \par Notifier signal:
    \li void <b>passwordChanged</b>(const QString& password)
 */
QString IrcConnection::password() const
{
    Q_D(const IrcConnection);
    return d->password;
}

void IrcConnection::setPassword(const QString& password)
{
    Q_D(IrcConnection);
    if (isActive())
        qWarning("IrcConnection::setPassword() has no effect until re-connect");
    if (d->password != password) {
        d->password = password;
        emit passwordChanged(password);
    }
}

/*!
    \property bool IrcConnection::active
    This property holds whether the connection is active.

    The connection is considered active when the socket
    state is not QAbstractSocket::UnconnectedState.

    \par Access functions:
    \li bool <b>isActive</b>() const

    \par Notifier signal:
    \li void <b>activeChanged</b>(bool active)
 */
bool IrcConnection::isActive() const
{
    Q_D(const IrcConnection);
    return d->active;
}

/*!
    \property bool IrcConnection::connected
    This property holds whether the connection is connected.

    The connection is considered connected when the welcome message
    has been received and the server is ready to receive commands.

    \sa Irc::RPL_WELCOME

    \par Access functions:
    \li bool <b>isConnected</b>() const

    \par Notifier signals:
    \li void <b>connectedChanged</b>(bool connected)
 */
bool IrcConnection::isConnected() const
{
    Q_D(const IrcConnection);
    return d->connected;
}

/*!
    This property holds the socket. IrcConnection creates an instance of QTcpSocket by default.

    The previously set socket is deleted if its parent is \c this.

    \note IrcConnection supports QSslSocket in the way that it automatically
    calls QSslSocket::startClientEncryption() while connecting.

    \par Access functions:
    \li \ref QAbstractSocket* <b>socket</b>() const
    \li void <b>setSocket</b>(\ref QAbstractSocket* socket)

    \sa IrcConnection::secure
 */
QAbstractSocket* IrcConnection::socket() const
{
    Q_D(const IrcConnection);
    return d->socket;
}

void IrcConnection::setSocket(QAbstractSocket* socket)
{
    Q_D(IrcConnection);
    if (d->socket != socket) {
        if (d->socket) {
            d->socket->disconnect(this);
            if (d->socket->parent() == this)
                d->socket->deleteLater();
        }

        d->socket = socket;
        if (socket) {
            connect(socket, SIGNAL(connected()), this, SLOT(_irc_connected()));
            connect(socket, SIGNAL(disconnected()), this, SLOT(_irc_disconnected()));
            connect(socket, SIGNAL(readyRead()), this, SLOT(_irc_readData()));
            connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_irc_error(QAbstractSocket::SocketError)));
            connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(_irc_state(QAbstractSocket::SocketState)));
        }
    }
}

/*!
    \property bool IrcConnection::secure
    This property holds whether the socket is an SSL socket.

    This property is provided for convenience. Calling
    \code
    connection->setSecure(true);
    \endcode

    is equivalent to:

    \code
    QSslSocket* socket = new QSslSocket(socket);
    socket->setPeerVerifyMode(QSslSocket::QueryPeer);
    connection->setSocket(socket);
    \endcode

    \note IrcConnection does not handle SSL errors, see
    QSslSocket::sslErrors() for more details on the subject.

    \par Access functions:
    \li bool <b>isSecure</b>() const
    \li void <b>setSecure</b>(bool secure)

    \par Notifier signal:
    \li void <b>secureChanged</b>(bool secure)

    \sa IrcConnection::socket
 */
bool IrcConnection::isSecure() const
{
#ifdef QT_NO_OPENSSL
    return false;
#else
    return qobject_cast<QSslSocket*>(socket());
#endif // QT_NO_OPENSSL
}

void IrcConnection::setSecure(bool secure)
{
#ifdef QT_NO_OPENSSL
    Q_UNUSED(secure)
#else
    QSslSocket* sslSocket = qobject_cast<QSslSocket*>(socket());
    if (secure && !sslSocket) {
        sslSocket = new QSslSocket(this);
        sslSocket->setPeerVerifyMode(QSslSocket::QueryPeer);
        setSocket(sslSocket);
        emit secureChanged(true);
    } else if (!secure && sslSocket) {
        setSocket(new QTcpSocket(this));
        emit secureChanged(false);
    }
#endif // QT_NO_OPENSSL
}

/*!
    This property holds the used SASL mechanism.

    \par Access functions:
    \li QString <b>saslMechanism</b>() const
    \li void <b>setSaslMechanism</b>(const QString& mechanism)

    \par Notifier signal:
    \li void <b>saslMechanismChanged</b>(const QString& mechanism)

    \sa supportedSaslMechanisms()
 */
QString IrcConnection::saslMechanism() const
{
    Q_D(const IrcConnection);
    return d->saslMechanism;
}

void IrcConnection::setSaslMechanism(const QString& mechanism)
{
    Q_D(IrcConnection);
    if (!mechanism.isEmpty() && !supportedSaslMechanisms().contains(mechanism.toUpper())) {
        qWarning("IrcConnection::setSaslMechanism(): unsupported mechanism: '%s'", qPrintable(mechanism));
        return;
    }
    if (d->saslMechanism != mechanism) {
        d->saslMechanism = mechanism.toUpper();
        emit saslMechanismChanged(mechanism);
    }
}

/*!
    This property holds the list of supported SASL mechanism.

    \par Access functions:
    \li static QStringList <b>supportedSaslMechanisms</b>()

    \sa saslMechanism
 */
QStringList IrcConnection::supportedSaslMechanisms()
{
    return QStringList() << QLatin1String("PLAIN");
}

/*!
    This property holds the network information.

    \par Access functions:
    \li IrcNetwork* <b>network</b>() const
 */
IrcNetwork* IrcConnection::network() const
{
    Q_D(const IrcConnection);
    return d->network;
}

/*!
    Opens a connection to the server.

    \note The function merely outputs a warnings and returns immediately if
    either \ref host, \ref userName, \ref nickName or \ref realName is empty.
 */
void IrcConnection::open()
{
    Q_D(IrcConnection);
    if (d->host.isEmpty()) {
        qCritical("IrcConnection::open(): host is empty!");
        return;
    }
    if (d->userName.isEmpty()) {
        qCritical("IrcConnection::open(): userName is empty!");
        return;
    }
    if (d->nickName.isEmpty()) {
        qCritical("IrcConnection::open(): nickName is empty!");
        return;
    }
    if (d->realName.isEmpty()) {
        qCritical("IrcConnection::open(): realName is empty!");
        return;
    }
    if (d->socket)
        d->socket->connectToHost(d->host, d->port);
}

/*!
    Immediately closes the connection to the server.

    \note Calling close() when the connection is connected makes
    the connection close immediately and thus leads to
    "remote host closed the connection". In order to quit
    gracefully, send a QUIT command and let the server handle
    closing the connection.

    C++ example:
    \code
    connection->quit(reason);
    QTimer::singleShot(timeout, connection, SLOT(deleteLater()));
    \endcode

    QML example:
    \code
    connection.quit(reason);
    connection.destroy(timeout);
    \endcode

    \sa quit()
 */
void IrcConnection::close()
{
    Q_D(IrcConnection);
    if (d->socket) {
        d->socket->abort();
        d->socket->disconnectFromHost();
    }
}

/*!
    Quits with \a reason.

    This method is provided for convenience. It is equal to:
    \code
    IrcCommand* command = IrcCommand::createQuit(reason);
    connection->sendCommand(command);
    \endcode

    \sa IrcCommand::createQuit()
 */
void IrcConnection::quit(const QString& reason)
{
    sendCommand(IrcCommand::createQuit(reason));
}

/*!
    Sends a \a command to the server.

    \note If the command has a valid parent, it is an indication that
    the caller of this method is be responsible for freeing the command.
    If the command does not have a valid parent (like the commands
    created via various IrcCommand::createXxx() methods) the connection
    will take ownership of the command and delete it once it has been
    sent. Thus, the command must have been allocated on the heap and
    it is not safe to access the command after it has been sent.

    \sa sendData()
 */
bool IrcConnection::sendCommand(IrcCommand* command)
{
    bool res = false;
    if (command) {
        QTextCodec* codec = QTextCodec::codecForName(command->encoding());
        Q_ASSERT(codec);
        res = sendData(codec->fromUnicode(command->toString()));
        if (!command->parent())
            command->deleteLater();
    }
    return res;
}

/*!
    Sends raw \a data to the server.

    \sa sendCommand()
 */
bool IrcConnection::sendData(const QByteArray& data)
{
    Q_D(IrcConnection);
    if (d->socket) {
        static bool dbg = qgetenv("IRC_DEBUG").toInt();
        if (dbg) qDebug() << "->" << data;
        return d->protocol->write(data);
    }
    return false;
}

/*!
    Sends raw \a message to the server.

    \note The \a message is sent using UTF-8 encoding.

    \sa sendData(), sendCommand()
 */
bool IrcConnection::sendRaw(const QString& message)
{
    return sendData(message.toUtf8());
}

/*!
    Installs a message \a filter on this connection.

    A message filter receives all messages that are sent to this connection.
    The message filter receives events via its messageFilter() function.
    The messageFilter() function must return \c true if the message should
    be filtered, (i.e. stopped); otherwise it must return \c false.

    If multiple message filters are installed on the same connection, the filter
    that was installed last is activated first.

    \sa removeMessageFilter()
 */
void IrcConnection::installMessageFilter(IrcMessageFilter* filter)
{
    Q_D(IrcConnection);
    d->filters += filter;
    IrcMessageFilterPrivate::get(filter)->connections.append(this);
}

/*!
    Removes a message \a filter from this connection.

    The request is ignored if such an event filter has not been installed.
    All message filters for this connection are automatically removed when this connection is destroyed.

    \sa installMessageFilter()
 */
void IrcConnection::removeMessageFilter(IrcMessageFilter* filter)
{
    Q_D(IrcConnection);
    d->filters.removeAll(filter);
    IrcMessageFilterPrivate::get(filter)->connections.removeAll(this);
}

/*!
    Creates a reply command for the CTCP \a request.

    The default implementation handles the following CTCP requests: CLIENTINFO, PING, SOURCE, TIME and VERSION.

    Reimplement this function in order to alter or omit the default replies.
 */
IrcCommand* IrcConnection::createCtcpReply(IrcPrivateMessage* request) const
{
    QString reply;
    QString type = request->message().split(" ", QString::SkipEmptyParts).value(0).toUpper();
    if (type == "PING")
        reply = request->message();
    else if (type == "TIME")
        reply = QLatin1String("TIME ") + QLocale().toString(QDateTime::currentDateTime(), QLocale::ShortFormat);
    else if (type == "VERSION")
        reply = QLatin1String("VERSION Using libcommuni ") + Irc::version() + QLatin1String(" - http://communi.github.com");
    else if (type == "SOURCE")
        reply = QLatin1String("SOURCE http://communi.github.com");
    else if (type == "CLIENTINFO")
        reply = QLatin1String("CLIENTINFO PING SOURCE TIME VERSION");
    if (!reply.isEmpty())
        return IrcCommand::createCtcpReply(request->nick(), reply);
    return 0;
}

/*!
    \internal
 */
IrcProtocol* IrcConnection::protocol() const
{
    Q_D(const IrcConnection);
    return d->protocol;
}

/*!
    \internal
 */
void IrcConnection::setProtocol(IrcProtocol* proto)
{
    Q_D(IrcConnection);
    if (d->protocol != proto) {
        if (d->protocol && d->protocol->parent() == this)
            delete d->protocol;
        d->protocol = proto;
    }
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcConnection* connection)
{
    if (!connection)
        return debug << "IrcConnection(0x0) ";
    debug.nospace() << connection->metaObject()->className() << '(' << (void*) connection;
    if (!connection->objectName().isEmpty())
        debug << ", name = " << connection->objectName();
    if (!connection->host().isEmpty())
        debug << ", host = " << connection->host()
              << ", port = " << connection->port();
    debug << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircconnection.cpp"

IRC_END_NAMESPACE
