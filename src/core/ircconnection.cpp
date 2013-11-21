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
#include "ircnetwork_p.h"
#include "ircprotocol.h"
#include "ircnetwork.h"
#include "irccommand.h"
#include "ircmessage.h"
#include "ircfilter.h"
#include "irc.h"
#include <QLocale>
#include <QDateTime>
#include <QTcpSocket>
#include <QTextCodec>
#include <QStringList>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaEnum>
#ifndef QT_NO_OPENSSL
#include <QSslSocket>
#endif // QT_NO_OPENSSL
#include <QDataStream>
#include <QVariantMap>

IRC_BEGIN_NAMESPACE

/*!
    \file ircconnection.h
    \brief \#include &lt;IrcConnection&gt;
 */

/*!
    \class IrcConnection ircconnection.h IrcConnection
    \ingroup core
    \brief Provides means to establish a connection to an IRC server.

    \section connection Connection management

    Before \ref open() "opening" a connection, it must be first initialized
    with \ref host, \ref userName, \ref nickName and \ref realName.

    The connection status may be queried at any time via status(). Also
    \ref active "isActive()" and \ref connected "isConnected()" are provided
    for convenience. In addition to the \ref status "statusChanged()" signal,
    the most important statuses are informed via the following convenience
    signals:
    \li connecting() -
        The connection is being established.
    \li \ref connected "connected()" -
        The IRC connection has been established, and the server is ready to receive commands.
    \li disconnected() -
        The connection has been lost.

    \section messages Receiving messages

    Whenever a message is received from the server, the messageReceived()
    signal is emitted. Also message type specific signals are provided
    for convenience. See messageReceived() and IrcMessage and its
    subclasses for more details.

    \section commands Sending commands

    Sending commands to a server is most conveniently done by creating
    them via the various static \ref IrcCommand "IrcCommand::createXxx()"
    methods and passing them to sendCommand(). Also sendData() is provided
    for more low-level access. See IrcCommand for more details.

    \section example Example

    \code
    IrcConnection* connection = new IrcConnection(this);
    connect(connection, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(onMessageReceived(IrcMessage*)));
    connection->setHost("irc.server.com");
    connection->setUserName("me");
    connection->setNickName("myself");
    connection->setRealName("And I");
    connection->sendCommand(IrcCommand::createJoin("#mine"));
    connection->open();
    \endcode

    \sa IrcNetwork, IrcMessage, IrcCommand
 */

/*!
    \enum IrcConnection::Status
    This enum describes the connection status.
 */

/*!
    \var IrcConnection::Inactive
    \brief The connection is inactive.
 */

/*!
    \var IrcConnection::Waiting
    \brief The connection is waiting for a reconnect.
 */

/*!
    \var IrcConnection::Connecting
    \brief The connection is being established.
 */

/*!
    \var IrcConnection::Connected
    \brief The connection has been established.
 */

/*!
    \var IrcConnection::Closing
    \brief The connection is being closed.
 */

/*!
    \var IrcConnection::Closed
    \brief The connection has been closed.
 */

/*!
    \var IrcConnection::Error
    \brief The connection has encountered an error.
 */

/*!
    \fn void IrcConnection::connecting()

    This signal is emitted when the connection is being established.

    The underlying \ref socket has connected, but the IRC handshake is
    not yet finished and the server is not yet ready to receive commands.
 */

/*!
    \fn void IrcConnection::nickNameReserved(QString* alternate)

    This signal is emitted when the requested nick name is reserved
    and an \a alternate nick name should be provided.

    An alternate nick name may be set via the provided argument, by changing
    the \ref nickName property, or by sending a nick command directly.

    \sa Irc::ERR_NICKNAMEINUSE, Irc::ERR_NICKCOLLISION
 */

/*!
    \fn void IrcConnection::disconnected()

    This signal is emitted when the connection has been lost.
 */

/*!
    \fn void IrcConnection::socketError(QAbstractSocket::SocketError error)

    This signal is emitted when a \ref socket \a error occurs.

    \sa QAbstractSocket::error()
 */

/*!
    \fn void IrcConnection::socketStateChanged(QAbstractSocket::SocketState state)

    This signal is emitted when the \a state of the underlying \ref socket changes.

    \sa QAbstractSocket::stateChanged()
 */

/*!
    \fn void IrcConnection::messageReceived(IrcMessage* message)

    This signal is emitted when a \a message is received.

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

#ifndef IRC_DOXYGEN
template<typename T>
static void irc_debug(IrcConnection* connection, const char* msg, const T& arg)
{
    static bool dbg = qgetenv("IRC_DEBUG").toInt();
    if (dbg) {
        const QString desc = QString::fromLatin1("IrcConnection(%1)").arg(connection->displayName());
        qDebug() << qPrintable(desc) << msg << arg;
    }
}

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
    enabled(true),
    status(IrcConnection::Inactive)
{
}

void IrcConnectionPrivate::init(IrcConnection* connection)
{
    q_ptr = connection;
    network = IrcNetworkPrivate::create(connection);
    connection->setSocket(new QTcpSocket(connection));
    connection->setProtocol(new IrcProtocol(connection));
    QObject::connect(&reconnecter, SIGNAL(timeout()), connection, SLOT(_irc_reconnect()));
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
    if (enabled && (status != IrcConnection::Closed || !protocol->hasQuit()) && !reconnecter.isActive() && reconnecter.interval() > 0) {
        reconnecter.start();
        setStatus(IrcConnection::Waiting);
    }
}

void IrcConnectionPrivate::_irc_error(QAbstractSocket::SocketError error)
{
    Q_Q(IrcConnection);
    if (!protocol->hasQuit() || (error != QAbstractSocket::RemoteHostClosedError && error != QAbstractSocket::UnknownSocketError)) {
        irc_debug(q, "socket error:", error);
        emit q->socketError(error);
        setStatus(IrcConnection::Error);
    }
}

void IrcConnectionPrivate::_irc_state(QAbstractSocket::SocketState state)
{
    Q_Q(IrcConnection);
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        if (protocol->hasQuit())
            setStatus(IrcConnection::Closed);
        break;
    case QAbstractSocket::ClosingState:
        if (status != IrcConnection::Error)
            setStatus(IrcConnection::Closing);
        break;
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::ConnectedState:
    default:
        setStatus(IrcConnection::Connecting);
        break;
    }
    emit q->socketStateChanged(state);
}

void IrcConnectionPrivate::_irc_reconnect()
{
    Q_Q(IrcConnection);
    if (!q->isActive()) {
        reconnecter.stop();
        q->open();
    }
}

void IrcConnectionPrivate::_irc_readData()
{
    protocol->read();
}

void IrcConnectionPrivate::_irc_filterDestroyed(QObject* filter)
{
    messageFilters.removeAll(filter);
    commandFilters.removeAll(filter);
}

void IrcConnectionPrivate::setNick(const QString& nick)
{
    Q_Q(IrcConnection);
    if (nickName != nick) {
        nickName = nick;
        emit q->nickNameChanged(nick);
    }
}

void IrcConnectionPrivate::setStatus(IrcConnection::Status value)
{
    Q_Q(IrcConnection);
    if (status != value) {
        const bool wasConnected = q->isConnected();
        status = value;
        emit q->statusChanged(value);

        if (!wasConnected && q->isConnected()) {
            emit q->connected();
            foreach (IrcCommand* cmd, pendingCommands)
                q->sendCommand(cmd);
            pendingCommands.clear();
        }
        irc_debug(q, "status:", status);
    }
}

void IrcConnectionPrivate::setInfo(const QHash<QString, QString>& info)
{
    Q_Q(IrcConnection);
    const QString oldName = q->displayName();
    IrcNetworkPrivate* priv = IrcNetworkPrivate::get(network);
    priv->setInfo(info);
    const QString newName = q->displayName();
    if (oldName != newName)
        emit q->displayNameChanged(newName);
}

void IrcConnectionPrivate::receiveMessage(IrcMessage* msg)
{
    Q_Q(IrcConnection);
    bool filtered = false;
    for (int i = messageFilters.count() - 1; !filtered && i >= 0; --i) {
        IrcMessageFilter* filter = qobject_cast<IrcMessageFilter*>(messageFilters.at(i));
        if (filter)
            filtered |= filter->messageFilter(msg);
    }

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
#endif // IRC_DOXYGEN

/*!
    Constructs a new IRC connection with \a parent.
 */
IrcConnection::IrcConnection(QObject* parent) : QObject(parent), d_ptr(new IrcConnectionPrivate)
{
    Q_D(IrcConnection);
    d->init(this);
}

/*!
    Constructs a new IRC connection with \a host and \a parent.
 */
IrcConnection::IrcConnection(const QString& host, QObject* parent) : QObject(parent), d_ptr(new IrcConnectionPrivate)
{
    Q_D(IrcConnection);
    d->init(this);
    setHost(host);
}

/*!
    Destructs the IRC connection.
 */
IrcConnection::~IrcConnection()
{
    close();
    emit destroyed(this);
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
        const QString oldName = displayName();
        d->host = host;
        emit hostChanged(host);
        const QString newName = displayName();
        if (oldName != newName)
            emit displayNameChanged(newName);
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
    This property holds the display name.

    Unless explicitly set, display name resolves to IrcNetwork::name
    or IrcConnection::host while the former is not known.

    \par Access functions:
    \li QString <b>displayName</b>() const
    \li void <b>setDisplayName</b>(const QString& name)

    \par Notifier signal:
    \li void <b>displayNameChanged</b>(const QString& name)
 */
QString IrcConnection::displayName() const
{
    Q_D(const IrcConnection);
    QString name = d->displayName;
    if (name.isEmpty())
        name = d->network->name();
    if (name.isEmpty())
        name = d->host;
    return name;
}

void IrcConnection::setDisplayName(const QString& name)
{
    Q_D(IrcConnection);
    if (d->displayName != name) {
        d->displayName = name;
        emit displayNameChanged(name);
    }
}

/*!
    \property Status IrcConnection::status
    This property holds the connection status.

    \par Access function:
    \li Status <b>status</b>() const

    \par Notifier signal:
    \li void <b>statusChanged</b>(Status status)
 */
IrcConnection::Status IrcConnection::status() const
{
    Q_D(const IrcConnection);
    return d->status;
}

/*!
    \property bool IrcConnection::active
    This property holds whether the connection is active.

    The connection is considered active when its either connecting, connected or closing.

    \par Access function:
    \li bool <b>isActive</b>() const
 */
bool IrcConnection::isActive() const
{
    Q_D(const IrcConnection);
    return d->status == Connecting || d->status == Connected || d->status == Closing;
}

/*!
    \property bool IrcConnection::connected
    This property holds whether the connection has been established.

    The connection has been established when the welcome message
    has been received and the server is ready to receive commands.

    \sa Irc::RPL_WELCOME

    \par Access function:
    \li bool <b>isConnected</b>() const

    \par Notifier signal:
    \li void <b>connected</b>()
 */
bool IrcConnection::isConnected() const
{
    Q_D(const IrcConnection);
    return d->status == Connected;
}

/*!
    \property bool IrcConnection::enabled
    This property holds whether the connection is enabled.

    The default value is \c true.

    When set to \c false, a disabled connection does nothing when open() is called.

    \par Access functions:
    \li bool <b>isEnabled</b>() const
    \li void <b>setEnabled</b>(bool enabled) [slot]
    \li void <b>setDisabled</b>(bool disabled) [slot]

    \par Notifier signal:
    \li void <b>enabledChanged</b>(bool enabled)
 */
bool IrcConnection::isEnabled() const
{
    Q_D(const IrcConnection);
    return d->enabled;
}

void IrcConnection::setEnabled(bool enabled)
{
    Q_D(IrcConnection);
    if (d->enabled != enabled) {
        d->enabled = enabled;
        emit enabledChanged(enabled);
    }
}

void IrcConnection::setDisabled(bool disabled)
{
    setEnabled(!disabled);
}

/*!
    \property int IrcConnection::reconnectDelay
    This property holds the reconnect delay in seconds.

    A positive (greater than zero) value enables automatic reconnect.
    When the connection is lost due to a socket error, IrcConnection
    will automatically attempt to reconnect after the specified delay.

    The default value is \c 0 (automatic reconnect disabled).

    \par Access functions:
    \li int <b>reconnectDelay</b>() const
    \li void <b>setReconnectDelay</b>(int seconds)

    \par Notifier signal:
    \li void <b>reconnectDelayChanged</b>(int seconds)
 */
int IrcConnection::reconnectDelay() const
{
    Q_D(const IrcConnection);
    return d->reconnecter.interval() / 1000;
}

void IrcConnection::setReconnectDelay(int seconds)
{
    Q_D(IrcConnection);
    const int interval = qMax(0, seconds) * 1000;
    if (d->reconnecter.interval() != interval) {
        d->reconnecter.setInterval(interval);
        emit reconnectDelayChanged(interval);
    }
}

/*!
    This property holds the socket. The default value is an instance of QTcpSocket.

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
    if (secure) {
        qWarning("IrcConnection::setSecure(): the Qt build does not support SSL");
        return;
    }
#else
    if (secure && !QSslSocket::supportsSsl()) {
        qWarning("IrcConnection::setSecure(): the platform does not support SSL - try installing OpenSSL");
        return;
    }

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
#endif // !QT_NO_OPENSSL
}

/*!
    This property holds the used SASL (Simple Authentication and Security Layer) mechanism.

    \par Access functions:
    \li QString <b>saslMechanism</b>() const
    \li void <b>setSaslMechanism</b>(const QString& mechanism)

    \par Notifier signal:
    \li void <b>saslMechanismChanged</b>(const QString& mechanism)

    \sa supportedSaslMechanisms
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
    if (isActive())
        qWarning("IrcConnection::setSaslMechanism() has no effect until re-connect");
    if (d->saslMechanism != mechanism) {
        d->saslMechanism = mechanism.toUpper();
        emit saslMechanismChanged(mechanism);
    }
}

/*!
    This property holds the list of supported SASL (Simple Authentication and Security Layer) mechanisms.

    \par Access function:
    \li static QStringList <b>supportedSaslMechanisms</b>()

    \sa saslMechanism
 */
QStringList IrcConnection::supportedSaslMechanisms()
{
    return QStringList() << QLatin1String("PLAIN");
}

/*!
    This property holds the network information.

    \par Access function:
    \li IrcNetwork* <b>network</b>() const
 */
IrcNetwork* IrcConnection::network() const
{
    Q_D(const IrcConnection);
    return d->network;
}

/*!
    Opens a connection to the server.

    The function does nothing when the connection is already \ref active
    or explicitly \ref enabled "disabled".

    \note The function merely outputs a warnings and returns immediately if
    either \ref host, \ref userName, \ref nickName or \ref realName is empty.
 */
void IrcConnection::open()
{
    Q_D(IrcConnection);
    if (d->host.isEmpty()) {
        qWarning("IrcConnection::open(): host is empty!");
        return;
    }
    if (d->userName.isEmpty()) {
        qWarning("IrcConnection::open(): userName is empty!");
        return;
    }
    if (d->nickName.isEmpty()) {
        qWarning("IrcConnection::open(): nickName is empty!");
        return;
    }
    if (d->realName.isEmpty()) {
        qWarning("IrcConnection::open(): realName is empty!");
        return;
    }
    if (d->enabled && d->socket && !isActive())
        d->socket->connectToHost(d->host, d->port);
}

/*!
    Immediately closes the connection to the server.

    Calling close() makes the connection close immediately and thus might lead to
    "remote host closed the connection". In order to quit gracefully, call quit()
    first. This function attempts to flush the underlying socket, but this does
    not guarantee that the server ever receives the QUIT command if the connection
    is closed immediately after sending the command. In order to ensure a graceful
    quit, let the server handle closing the connection.

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
        d->socket->flush();
        d->socket->abort();
        d->socket->disconnectFromHost();
        if (d->socket->state() == QAbstractSocket::UnconnectedState)
            d->setStatus(Closed);
    }
}

/*!
    Sends a quit command with an optionally specified \a reason.

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

    If the connection is not active, the \a command is queued and sent
    later when the connection has been established.

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
    Q_D(IrcConnection);
    bool res = false;
    if (command) {
        bool filtered = false;
        for (int i = d->commandFilters.count() - 1; !filtered && i >= 0; --i) {
            QObject* filter = d->commandFilters.at(i);
            IrcCommandFilter* commandFilter = qobject_cast<IrcCommandFilter*>(filter);
            if (commandFilter && !d->activeCommandFilters.contains(filter)) {
                d->activeCommandFilters.push(filter);
                filtered |= commandFilter->commandFilter(command);
                d->activeCommandFilters.pop();
            }
        }
        if (filtered)
            return false;

        if (isActive()) {
            QTextCodec* codec = QTextCodec::codecForName(command->encoding());
            Q_ASSERT(codec);
            res = sendData(codec->fromUnicode(command->toString()));
            if (!command->parent())
                command->deleteLater();
        } else {
            Q_D(IrcConnection);
            d->pendingCommands += command;
        }
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
    Sends raw \a message to the server using UTF-8 encoding.

    \sa sendData(), sendCommand()
 */
bool IrcConnection::sendRaw(const QString& message)
{
    return sendData(message.toUtf8());
}

/*!
    Installs a message \a filter on the connection. The \a filter must implement the IrcMessageFilter interface.

    A message filter receives all messages that are sent to the connection. The filter
    receives messages via the \ref IrcMessageFilter::messageFilter() "messageFilter()"
    function. The function must return \c true if the message should be filtered,
    (i.e. stopped); otherwise it must return \c false.

    If multiple message filters are installed on the same connection, the filter
    that was installed last is activated first.

    \sa removeMessageFilter()
 */
void IrcConnection::installMessageFilter(QObject* filter)
{
    Q_D(IrcConnection);
    IrcMessageFilter* msgFilter = qobject_cast<IrcMessageFilter*>(filter);
    if (msgFilter) {
        d->messageFilters += filter;
        connect(filter, SIGNAL(destroyed(QObject*)), this, SLOT(_irc_filterDestroyed(QObject*)), Qt::UniqueConnection);
    }
}

/*!
    Removes a message \a filter from the connection.

    The request is ignored if such message filter has not been installed.
    All message filters for a connection are automatically removed
    when the connection is destroyed.

    \sa installMessageFilter()
 */
void IrcConnection::removeMessageFilter(QObject* filter)
{
    Q_D(IrcConnection);
    IrcMessageFilter* msgFilter = qobject_cast<IrcMessageFilter*>(filter);
    if (msgFilter) {
        d->messageFilters.removeAll(filter);
        disconnect(filter, SIGNAL(destroyed(QObject*)), this, SLOT(_irc_filterDestroyed(QObject*)));
    }
}

/*!
    Installs a command \a filter on the connection. The \a filter must implement the IrcCommandFilter interface.

    A command filter receives all commands that are sent from the connection. The filter
    receives commands via the \ref IrcCommandFilter::commandFilter() "commandFilter()"
    function. The function must return \c true if the command should be filtered,
    (i.e. stopped); otherwise it must return \c false.

    If multiple command filters are installed on the same connection, the filter
    that was installed last is activated first.

    \sa removeCommandFilter()
 */
void IrcConnection::installCommandFilter(QObject* filter)
{
    Q_D(IrcConnection);
    IrcCommandFilter* cmdFilter = qobject_cast<IrcCommandFilter*>(filter);
    if (cmdFilter) {
        d->commandFilters += filter;
        connect(filter, SIGNAL(destroyed(QObject*)), this, SLOT(_irc_filterDestroyed(QObject*)), Qt::UniqueConnection);
    }
}

/*!
    Removes a command \a filter from the connection.

    The request is ignored if such command filter has not been installed.
    All command filters for a connection are automatically removed when
    the connection is destroyed.

    \sa installCommandFilter()
 */
void IrcConnection::removeCommandFilter(QObject* filter)
{
    Q_D(IrcConnection);
    IrcCommandFilter* cmdFilter = qobject_cast<IrcCommandFilter*>(filter);
    if (cmdFilter) {
        d->commandFilters.removeAll(filter);
        disconnect(filter, SIGNAL(destroyed(QObject*)), this, SLOT(_irc_filterDestroyed(QObject*)));
    }
}

/*!
    Saves the state of the connection. The \a version number is stored as part of the state data.

    To restore the saved state, pass the return value and \a version number to restoreState().
 */
QByteArray IrcConnection::saveState(int version) const
{
    Q_D(const IrcConnection);
    QVariantMap args;
    args.insert("version", version);
    args.insert("host", host());
    args.insert("port", port());
    args.insert("userName", userName());
    args.insert("nickName", nickName());
    args.insert("realName", realName());
    args.insert("password", password());
    args.insert("displayName", d->displayName);
    args.insert("encoding", encoding());
    args.insert("enabled", isEnabled());
    args.insert("reconnectDelay", reconnectDelay());
    args.insert("secure", isSecure());
    args.insert("saslMechanism", saslMechanism());

    QByteArray state;
    QDataStream out(&state, QIODevice::WriteOnly);
    out << args;
    return state;
}

/*!
    Restores the \a state of the connection. The \a version number is compared with that stored in \a state.
    If they do not match, the connection state is left unchanged, and this function returns \c false; otherwise,
    the state is restored, and \c true is returned.

    \sa saveState()
 */
bool IrcConnection::restoreState(const QByteArray& state, int version)
{
    if (isActive())
        return false;

    QVariantMap args;
    QDataStream in(state);
    in >> args;
    if (in.status() != QDataStream::Ok || args.value("version", -1).toInt() != version)
        return false;

    setHost(args.value("host", host()).toString());
    setPort(args.value("port", port()).toInt());
    setUserName(args.value("userName", userName()).toString());
    setNickName(args.value("nickName", nickName()).toString());
    setRealName(args.value("realName", realName()).toString());
    setPassword(args.value("password", password()).toString());
    setDisplayName(args.value("displayName").toString());
    setEncoding(args.value("encoding", encoding()).toByteArray());
    setEnabled(args.value("enabled", isEnabled()).toBool());
    setReconnectDelay(args.value("reconnectDelay", reconnectDelay()).toInt());
    setSecure(args.value("secure", isSecure()).toBool());
    setSaslMechanism(args.value("saslMechanism", saslMechanism()).toString());
    return true;
}

/*!
    Creates a reply command for the CTCP \a request.

    The default implementation handles the following CTCP requests: CLIENTINFO, PING, SOURCE, TIME and VERSION.

    Reimplement this function in order to alter or omit the default replies.
 */
IrcCommand* IrcConnection::createCtcpReply(IrcPrivateMessage* request) const
{
    QString reply;
    QString type = request->content().split(" ", QString::SkipEmptyParts).value(0).toUpper();
    if (type == "PING")
        reply = request->content();
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
QDebug operator<<(QDebug debug, IrcConnection::Status status)
{
    const int index = IrcConnection::staticMetaObject.indexOfEnumerator("Status");
    QMetaEnum enumerator = IrcConnection::staticMetaObject.enumerator(index);
    const char* key = enumerator.valueToKey(status);
    debug << (key ? key : "Unknown");
    return debug;
}

QDebug operator<<(QDebug debug, const IrcConnection* connection)
{
    if (!connection)
        return debug << "IrcConnection(0x0) ";
    debug.nospace() << connection->metaObject()->className() << '(' << (void*) connection;
    if (!connection->displayName().isEmpty())
        debug.nospace() << ", " << qPrintable(connection->displayName());
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircconnection.cpp"

IRC_END_NAMESPACE
