/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
* Copyright (C) 2010-2011 SmokeX <smokexjc@gmail.com>
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
#include "irccommand.h"
#include "ircmessage.h"
#include "ircsender.h"
#include "ircutil.h"
#include "irc.h"
#include <QLocale>
#include <QDateTime>
#include <QTcpSocket>
#include <QStringList>

/*!
    \file ircsession.h
    \brief #include &lt;IrcSession&gt;
 */

/*!
    \class IrcSession ircsession.h IrcSession
    \ingroup core
    \brief The IrcSession class provides an IRC session.

    IrcSession provides means to establish a connection to an IRC server.

    IrcSession works asynchronously ie. it is non-blocking, emitting signals
    to notify when the state of connection changes or data has arrived. The
    asynchronous approach depends on an event loop. See QCoreApplication::exec()
    for more details.

    Example usage:
    \code
    IrcSession* session = new IrcSession(this);
    connect(session, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(onMessageReceived(IrcMessage*)));
    session->setHost("irc.server.com");
    session->setUserName("me");
    session->setNickName("myself");
    session->setRealName("And I");
    session->open();
    \endcode

    \sa IrcMessage and IrcCommand
 */

/*!
    \fn void IrcSession::connecting()

    This signal is emitted when the connection is being established.
 */

/*!
    \fn void IrcSession::password(QString* password)

    This signal is emitted when the connection \a password may be set.

    \note IrcSession does not store the password.
 */

/*!
    \fn void IrcSession::connected()

    This signal is emitted when the connection has been established ie.
    the welcome message has been received and the server is ready to receive commands.

    \sa Irc::RPL_WELCOME
 */

/*!
    \fn void IrcSession::disconnected()

    This signal is emitted when the session has been disconnected.
 */

/*!
    \fn void IrcSession::messageReceived(IrcMessage* message)

    This signal is emitted whenever a \a message is received.
 */

/*!
    \fn void IrcSession::nickNameChanged(const QString& name)

    This signal is emitted when the nick \a name has changed.
 */

IrcSessionPrivate::IrcSessionPrivate(IrcSession* session) :
    q_ptr(session),
    encoder(),
    buffer(),
    socket(0),
    host(),
    port(6667),
    userName(),
    nickName(),
    realName(),
    active(false)
{
}

void IrcSessionPrivate::_q_connected()
{
    Q_Q(IrcSession);
    emit q->connecting();

    QString password;
    emit q->password(&password);
    if (!password.isEmpty())
        q->sendRaw(QString("PASS %1").arg(password));

    q->sendCommand(IrcCommand::createNick(nickName));
    q->sendRaw(QString("USER %1 hostname servername :%2").arg(userName, realName));
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
    qCritical() << "IrcSessionPrivate::_q_error():" << error;
}

void IrcSessionPrivate::_q_state(QAbstractSocket::SocketState state)
{
    Q_Q(IrcSession);
    bool wasActive = active;
    active = state != QAbstractSocket::UnconnectedState;
    if (wasActive != active)
        emit q->activeChanged(active);

    static bool dbg = qgetenv("COMMUNI_DEBUG").toInt();
    if (dbg) qDebug() << "IrcSessionPrivate::_q_state():" << state;
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

    QString encoded = encoder.encode(line);
    static bool dbg = qgetenv("COMMUNI_DEBUG").toInt();
    if (dbg) qDebug() << encoded;

    IrcMessage* msg = IrcMessage::fromString(encoded);
    if (msg)
    {
        switch (msg->type())
        {
        case IrcMessage::Numeric:
            if (static_cast<IrcNumericMessage*>(msg)->code() == Irc::RPL_WELCOME)
                emit q->connected();
            break;
        case IrcMessage::Ping:
            q->sendRaw("PONG " + static_cast<IrcPingMessage*>(msg)->argument());
            break;
        case IrcMessage::Private: {
            IrcPrivateMessage* privMsg = static_cast<IrcPrivateMessage*>(msg);
            if (privMsg->isRequest())
            {
                QString reply;
                QString request = privMsg->message().split(" ", QString::SkipEmptyParts).value(0).toUpper();
                if (request == "PING")
                    reply = privMsg->message();
                else if (request == "TIME")
                    reply = "TIME " + QLocale().toString(QDateTime::currentDateTime(), QLocale::ShortFormat);
                else if (request == "VERSION")
                    reply = QString("VERSION Communi ") + Irc::version();
                if (!reply.isNull())
                    q->sendCommand(IrcCommand::createCtcpReply(msg->sender().name(), reply));
            }
            break;
            }
        case IrcMessage::Nick:
            if (msg->sender().name() == nickName)
            {
                QString newNick = static_cast<IrcNickMessage*>(msg)->nick();
                if (nickName != newNick)
                {
                    nickName = newNick;
                    emit q->nickNameChanged(nickName);
                }
            }
            break;
        default:
            break;
        }

        emit q->messageReceived(msg);
        msg->deleteLater();
    }
}

bool IrcSessionPrivate::isConnected() const
{
    return socket &&
        (socket->state() == QAbstractSocket::ConnectingState
         || socket->state() == QAbstractSocket::ConnectedState);
}

/*!
    Constructs a new IRC session with \a parent.
 */
IrcSession::IrcSession(QObject* parent) : QObject(parent), d_ptr(new IrcSessionPrivate(this))
{
    setSocket(new QTcpSocket(this));
    qRegisterMetaType<IrcSender>("IrcSender");
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
    This property holds the message encoding. See QTextCodec documentation for supported encodings.

    Encoding auto-detection can be turned on by passing a null QByteArray. The fallback codec is QTextCodec::codecForLocale().

    The default value is a null QByteArray.

    \par Access functions:
    \li QByteArray <b>encoding</b>() const
    \li void <b>setEncoding</b>(const QByteArray& encoding)
 */
QByteArray IrcSession::encoding() const
{
    Q_D(const IrcSession);
    return d->encoder.encoding();
}

void IrcSession::setEncoding(const QByteArray& encoding)
{
    Q_D(IrcSession);
    d->encoder.setEncoding(encoding);
}

/*!
    This property holds the server host.

    \par Access functions:
    \li QString <b>host</b>() const
    \li void <b>setHost</b>(const QString& host)
 */
QString IrcSession::host() const
{
    Q_D(const IrcSession);
    return d->host;
}

void IrcSession::setHost(const QString& host)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setHost() has no effect until re-connect");
    d->host = host;
}

/*!
    This property holds the server port.

    The default value is \c 6667.

    \par Access functions:
    \li int <b>port</b>() const
    \li void <b>setPort</b>(int port)
 */
int IrcSession::port() const
{
    Q_D(const IrcSession);
    return d->port;
}

void IrcSession::setPort(int port)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setPort() has no effect until re-connect");
    d->port = port;
}

/*!
    This property holds the user name.

    \note Changing the user name has no effect until the connection is re-established.

    \par Access functions:
    \li QString <b>userName</b>() const
    \li void <b>setUserName</b>(const QString& name)
 */
QString IrcSession::userName() const
{
    Q_D(const IrcSession);
    return d->userName;
}

void IrcSession::setUserName(const QString& name)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setUserName() has no effect until re-connect");
    d->userName = name.split(" ", QString::SkipEmptyParts).value(0).trimmed();
}

/*!
    This property holds the nick name.

    \par Access functions:
    \li QString <b>nickName</b>() const
    \li void <b>setNickName</b>(const QString& name)
 */
QString IrcSession::nickName() const
{
    Q_D(const IrcSession);
    return d->nickName;
}

void IrcSession::setNickName(const QString& name)
{
    Q_D(IrcSession);
    QString nick = name.split(" ", QString::SkipEmptyParts).value(0).trimmed();
    if (d->nickName != nick)
    {
        d->nickName = nick;
        if (d->isConnected())
            sendCommand(IrcCommand::createNick(nick));
        emit nickNameChanged(nick);
    }
}

/*!
    This property holds the real name.

    \note Changing the real name has no effect until the connection is re-established.

    \par Access functions:
    \li QString <b>realName</b>() const
    \li void <b>setRealName</b>(const QString& name)
 */
QString IrcSession::realName() const
{
    Q_D(const IrcSession);
    return d->realName;
}

void IrcSession::setRealName(const QString& name)
{
    Q_D(IrcSession);
    if (d->isConnected())
        qWarning("IrcSession::setRealName() has no effect until re-connect");
    d->realName = name;
}

/*!
    \property bool IrcSession::active
    This property holds whether the session is active.

    The session is considered active when the socket
    state is not QAbstractSocket::UnconnectedState.

    \par Access functions:
    \li bool <b>isActive</b>() const
 */
bool IrcSession::isActive() const
{
    Q_D(const IrcSession);
    return d->active;
}

/*!
    This property holds the socket. IrcSession creates an instance of QTcpSocket by default.

    The previously set socket is deleted if its parent is \c this.

    \note IrcSession supports QSslSocket in the way that it automatically
    calls QSslSocket::startClientEncryption() while connecting.

    \par Access functions:
    \li QAbstractSocket* <b>socket</b>() const
    \li void <b>setSocket</b>(QAbstractSocket* socket)
 */
QAbstractSocket* IrcSession::socket() const
{
    Q_D(const IrcSession);
    return d->socket;
}

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
    Opens a connection to the server.

    \note The function merely outputs a warnings and returns immediately if
    either \ref host, \ref userName, \ref nickName or \ref realName is empty.
 */
void IrcSession::open()
{
    Q_D(IrcSession);
    if (d->host.isEmpty())
    {
        qCritical("IrcSession::open(): host is empty!");
        return;
    }
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
    Closes the connection to the server.
 */
void IrcSession::close()
{
    Q_D(IrcSession);
    if (d->socket)
        d->socket->disconnectFromHost();
}

/*!
    Sends a \a command to the server.

    \warning The command must be allocated on the heap since the session
    will take ownership of the command and delete it once it has been sent.
    It is not safe to access the command after it has been sent.

    \sa sendRaw()
 */
bool IrcSession::sendCommand(IrcCommand* command)
{
    bool res = false;
    if (command)
    {
        res = sendRaw(command->toString());
        command->deleteLater();
    }
    return res;
}

/*!
    Sends a raw \a message to the server.

    \sa sendCommand()
 */
bool IrcSession::sendRaw(const QString& message)
{
    Q_D(IrcSession);
    qint64 bytes = -1;
    if (d->socket)
        bytes = d->socket->write(message.toUtf8() + QByteArray("\r\n"));
    return bytes != -1;
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
