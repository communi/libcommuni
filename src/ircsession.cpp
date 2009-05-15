/*
* Copyright (C) 2008-2009 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#include "ircsession.h"
#include "ircutil.h"
#include <QSet>
#include <QTimer>
#include <QBuffer>
#include <QPointer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QStringList>
#include <QMetaMethod>
#ifdef HAVE_ICU
#include <unicode/ucsdet.h>
#endif // HAVE_ICU

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

/*! \enum Irc::Session::Option

    This enum describes special options that may be used by Irc::Session.
 */

/*! \var Irc::Session::StripNicks

    Strips origins automatically.

    For every IRC server message, the origin is sent in standard form:
    \code
    nick!ident@host
    \endcode
    i.e. like \e jpnurmi!jaxr\@jolt.modeemi.fi. Such origins can not be used
    in IRC commands but need to be stripped (i.e. ident and host part
    should be cut off) before using. This can be done either explicitly,
    by calling Irc::Util::nickFromTarget(), or implicitly for all the
    messages - by setting this option with Irc::Session::setOption().
 */

/*!
    \fn void Irc::Session::connected()

    This signal is emitted after a connection has been successfully established.
 */

/*!
    \fn void Irc::Session::disconnected()

    This signal is emitted when the session has been disconnected.
*/

/*!
    \fn void Irc::Session::msgJoined(const QString& origin, const QString& channel)

    This signal is emitted when \a origin has joined \a channel.
 */

/*!
    \fn void Irc::Session::msgParted(const QString& origin, const QString& channel, const QString& message)

    This signal is emitted when \a origin has parted \a channel with \a message.
 */

/*!
    \fn void Irc::Session::msgQuit(const QString& origin, const QString& message)

    This signal is emitted when \a origin has quit with \a message.
 */

/*!
    \fn void Irc::Session::msgNickChanged(const QString& origin, const QString& nick)

    This signal is emitted when \a origin has changed \a nick.
 */

/*!
    \fn void Irc::Session::msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args)

    This signal is emitted when \a origin has changed \a receiver's \a mode with \a args.
 */

/*!
    \fn void Irc::Session::msgTopicChanged(const QString& origin, const QString& channel, const QString& topic)

    This signal is emitted when \a origin has changed \a channel's \a topic.
 */

/*!
    \fn void Irc::Session::msgInvited(const QString& origin, const QString& receiver, const QString& channel)

    This signal is emitted when \a origin has invited \a receiver to \a channel.
 */

/*!
    \fn void Irc::Session::msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message)

    This signal is emitted when \a origin has kicked \a nick from \a channel with \a message.
 */

/*!
    \fn void Irc::Session::msgMessageReceived(const QString& origin, const QString& receiver, const QString& message)

    This signal is emitted when \a origin has sent \a message to \a receiver.
 */

/*!
    \fn void Irc::Session::msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice)

    This signal is emitted when \a origin has sent \a notice to \a receiver.
 */

/*!
    \fn void Irc::Session::msgCtcpRequestReceived(const QString& origin, const QString& request)

    This signal is emitted when \a origin has sent a CTCP \a request.
 */

/*!
    \fn void Irc::Session::msgCtcpReplyReceived(const QString& origin, const QString& reply)

    This signal is emitted when \a origin has sent a CTCP \a reply.
 */

/*!
    \fn void Irc::Session::msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action)

    This signal is emitted when \a origin has sent a CTCP \a action to \a receiver.
 */

/*!
    \fn void Irc::Session::msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params)

    This signal is emitted when \a origin has sent a numeric message with \a code and \a params.
 */

/*!
    \fn void Irc::Session::msgUnknownMessageReceived(const QString& origin, const QStringList& params)

    This signal is emitted when \a origin has sent an unknown message with \a params.
 */

static QByteArray detectEncoding(const QByteArray& text)
{
    Q_UNUSED(text);
    QByteArray encoding;
#ifdef HAVE_ICU
    UErrorCode status = U_ZERO_ERROR;
    UCharsetDetector* detector = ucsdet_open(&status);
    if (detector && !U_FAILURE(status))
    {
        ucsdet_setText(detector, text.constData(), text.length(), &status);
        if (!U_FAILURE(status))
        {
            const UCharsetMatch* match = ucsdet_detect(detector, &status);
            if (match && !U_FAILURE(status))
                encoding = ucsdet_getName(match, &status);
        }
    }

    if (U_FAILURE(status)) {
        qWarning("detectEncoding() failed: %s", u_errorName(status));
    }

    ucsdet_close(detector);
#endif // HAVE_ICU
    return encoding;
}

namespace Irc
{
    class SessionPrivate
    {
        Q_DECLARE_PUBLIC(Session)

    public:
        SessionPrivate();
        void init(Session* session);

        void _q_connected();
        void _q_disconnected();
        void _q_reconnect();
        void _q_error();
        void _q_state();
        void _q_readData();

        QString readString(const QByteArray& data) const;
        void processLine(const QByteArray& line);

        Session* q_ptr;

        QBuffer buffer;
        Session::Options options;
        QPointer<QAbstractSocket> socket;

        QString ident;
        QString password;
        QString nick;
        QString realName;
        QString host;
        quint16 port;
        bool motdReceived;
        QStringList channels;
        QByteArray encoding;
        int delay;
        QTimer timer;
    };

    SessionPrivate::SessionPrivate() :
        q_ptr(0),
        buffer(),
        options(Session::StripNicks),
        socket(0),
        ident(QLatin1String("libircclient-qt")),
        password(),
        nick(),
        realName(QLatin1String("libircclient-qt")),
        host(),
        port(6667),
        motdReceived(false),
        channels(),
        encoding(),
        delay(-1),
        timer()
    {
    }

    void SessionPrivate::init(Session* session)
    {
        q_ptr = session;

        Q_Q(Session);
        q->setSocket(new QTcpSocket(q));
        buffer.open(QIODevice::ReadWrite);
        q->connect(&timer, SIGNAL(timeout()), q, SLOT(_q_reconnect()));
    }

    void SessionPrivate::_q_connected()
    {
        Q_Q(Session);

        // stop autoreconnecting...
        if (timer.isActive())
            timer.stop();

        if (!password.isEmpty())
            socket->write(QString(QLatin1String("PASS %1\r\n")).arg(password).toLocal8Bit());

        socket->write(QString(QLatin1String("NICK %1\r\n")).arg(nick).toLocal8Bit());

		// RFC 1459 states that "hostname and servername are normally
        // ignored by the IRC server when the USER command comes from
        // a directly connected client (for security reasons)", therefore
        // we don't need them.
        socket->write(QString(QLatin1String("USER %1 unknown unknown :%2\r\n")).arg(ident).arg(realName).toLocal8Bit());

        emit q->connected();
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
        // start reconnecting...
        if (delay >= 0)
            timer.start(delay * 1000);
    }

    void SessionPrivate::_q_state()
    {
    }

    void SessionPrivate::_q_readData()
    {
        qint64 bytes = buffer.write(socket->readAll());
        buffer.seek(buffer.pos() - bytes);
        while (buffer.canReadLine())
        {
            QByteArray line = buffer.readLine();
            processLine(line.trimmed());
        }
    }

    QString SessionPrivate::readString(const QByteArray& data) const
    {
        QByteArray enc = encoding;
        if (enc.isNull())
            enc = detectEncoding(data);
        QTextCodec *codec = QTextCodec::codecForName(enc);
        if (!codec)
            codec = QTextCodec::codecForLocale();
        return codec->toUnicode(data);
    }

    void SessionPrivate::processLine(const QByteArray& line)
    {
        Q_Q(Session);
        QString process = readString(line);
        QString prefix, command;
        QStringList params;

        // From RFC 1459:
        //  <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
        //  <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
        //  <command>  ::= <letter> { <letter> } | <number> <number> <number>
        //  <SPACE>    ::= ' ' { ' ' }
        //  <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
        //  <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
        //                 or NUL or CR or LF, the first of which may not be ':'>
        //  <trailing> ::= <Any, possibly *empty*, sequence of octets not including
        //                   NUL or CR or LF>

        // parse <prefix>
        if (process.startsWith(QLatin1Char(':')))
        {
            prefix = process.mid(1, process.indexOf(QLatin1Char(' ')) - 1);
            process.remove(0, prefix.length() + 2);

            if (options & Session::StripNicks)
            {
                int index = prefix.indexOf(QRegExp(QLatin1String("[@!]")));
                if (index != -1)
                    prefix.truncate(index);
            }
        }

        // parse <command>
        command = process.mid(0, process.indexOf(QLatin1Char(' ')));
        process.remove(0, command.length() + 1);
        bool isNumeric = false;
        uint code = command.toInt(&isNumeric);

        // parse middle/params
        while (!process.isEmpty())
        {
            if (process.startsWith(QLatin1Char(':')))
            {
                process.remove(0, 1);
                params << process;
                process.clear();
            }
            else
            {
                QString param = process.mid(0, process.indexOf(QLatin1Char(' ')));
                process.remove(0, param.length() + 1);
                params << param;
            }
        }

        // handle PING/PONG
        if (command == QLatin1String("PING"))
        {
            QString arg = params.value(0);
            q->sendRaw(QString(QLatin1String("PONG %1")).arg(arg));
            return;
        }

        // and dump
        if (isNumeric)
        {
            static const QList<int> MOTD_LIST =
                QList<int>() << Rfc::RPL_MOTDSTART << Rfc::RPL_MOTD << Rfc::RPL_ENDOFMOTD << Rfc::ERR_NOMOTD;

            if (code == Rfc::RPL_TOPICSET && options & Session::StripNicks)
            {
                QString user = params.value(2);
                int index = user.indexOf(QRegExp(QLatin1String("[@!]")));
                if (index != -1)
                {
                    user.truncate(index);
                    params.replace(2, user);
                }
            }

            if (!motdReceived || (code >= 300 && !MOTD_LIST.contains(code)))
                emit q->msgNumericMessageReceived(prefix, code, params);

            // check whether it is the first RPL_ENDOFMOTD or ERR_NOMOTD after the connection
            if (!motdReceived && (code == Rfc::RPL_ENDOFMOTD || code == Rfc::ERR_NOMOTD))
                motdReceived = true;

            // join auto-join channels after MOTD
            if (code == Rfc::RPL_ENDOFMOTD || code == Rfc::ERR_NOMOTD)
            {
                foreach (const QString& channel, channels)
                    q->cmdJoin(channel);
            }
        }
        else
        {
            if (command == QLatin1String("NICK"))
            {
                QString newNick = params.value(0);
                if (nick == Util::nickFromTarget(prefix))
                    nick = newNick;
                emit q->msgNickChanged(prefix, newNick);
            }
            else if (command == QLatin1String("QUIT"))
            {
                QString reason = params.value(0);
                emit q->msgQuit(prefix, reason);
            }
            else if (command == QLatin1String("JOIN"))
            {
                QString channel = params.value(0);
                emit q->msgJoined(prefix, channel);
            }
            else if (command == QLatin1String("PART"))
            {
                QString channel = params.value(0);
                QString message = params.value(1);
                emit q->msgParted(prefix, channel, message);
            }
            else if (command == QLatin1String("MODE"))
            {
                QString receiver = params.value(0);
                QString mode = params.value(1);
                QString args = params.value(2);
                emit q->msgModeChanged(prefix, receiver, mode, args);
            }
            else if (command == QLatin1String("TOPIC"))
            {
                QString channel = params.value(0);
                QString topic = params.value(1);
                emit q->msgTopicChanged(prefix, channel, topic);
            }
            else if (command == QLatin1String("INVITE"))
            {
                QString receiver = params.value(0);
                QString channel = params.value(1);
                emit q->msgInvited(prefix, receiver, channel);
            }
            else if (command == QLatin1String("KICK"))
            {
                QString channel = params.value(0);
                QString nick = params.value(1);
                QString message = params.value(2);
                emit q->msgKicked(prefix, channel, nick, message);
            }
            else if (command == QLatin1String("PRIVMSG"))
            {
                QString receiver = params.value(0);
                QString message = params.value(1);

                if (message.startsWith(QLatin1Char('\1')) && message.endsWith(QLatin1Char('\1')))
                {
                    message.remove(0, 1);
                    message.remove(message.length() - 1, 1);

                    if (message.startsWith(QLatin1String("ACTION ")))
                        emit q->msgCtcpActionReceived(prefix, receiver, message.mid(7));
                    else
                        // TODO: check params
                        emit q->msgCtcpRequestReceived(prefix, /*receiver,*/ message);
                }
                else
                {
                    emit q->msgMessageReceived(prefix, receiver, message);
                }
            }
            else if (command == QLatin1String("NOTICE"))
            {
                QString receiver = params.value(0);
                QString message = params.value(1);

                if (message.startsWith(QLatin1Char('\1')) && message.endsWith(QLatin1Char('\1')))
                {
                    message.remove(0, 1);
                    message.remove(message.length() - 1, 1);

                    // TODO: check params
                    emit q->msgCtcpReplyReceived(prefix, /*receiver,*/ message);
                }
                else
                {
                    emit q->msgNoticeReceived(prefix, receiver, message);
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
                emit q->msgUnknownMessageReceived(prefix, params);
            }
        }
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
        Returns the list of auto-join channels.
     */
    QStringList Session::autoJoinChannels() const
    {
        Q_D(const Session);
        return d->channels;
    }

    /*!
        Adds \a channel to the list of auto-join channels.
     */
    void Session::addAutoJoinChannel(const QString& channel)
    {
        Q_D(Session);
        if (!d->channels.contains(channel, Qt::CaseInsensitive))
            d->channels.append(channel);
    }

    /*!
        Removes \a channels from the list of auto-join channels.
     */
    void Session::removeAutoJoinChannel(const QString& channel)
    {
        Q_D(Session);
        int index = d->channels.indexOf(QRegExp(channel, Qt::CaseInsensitive));
        if (index != -1)
            d->channels.removeAt(index);
    }

    /*!
        Sets the list of auto-join \a channels.
     */
    void Session::setAutoJoinChannels(const QStringList& channels)
    {
        Q_D(Session);
        d->channels = channels;
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
        return d->encoding;
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
        d->encoding = encoding;
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
        if (isConnected())
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
            	sendRaw(QString(QLatin1String("NICK %1")).arg(nick));
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
        if (isConnected())
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
        if (isConnected())
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
        if (isConnected())
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
        if (isConnected())
            qWarning("Session::setPort() has no effect until re-connect");
        d->port = port;
    }

    /*!
        Returns the options.

        The default value is \c StripNicks.
     */
    Session::Options Session::options() const
    {
        Q_D(const Session);
        return d->options;
    }

    /*!
        Sets the \a options.
     */
    void Session::setOptions(Options options)
    {
        Q_D(Session);
        d->options = options;
    }

    /*!
        Returns \c true if connected or \c false otherwise.
     */
    bool Session::isConnected() const
    {
        Q_D(const Session);
        return d->socket &&
            (d->socket->state() == QAbstractSocket::ConnectingState
            || d->socket->state() == QAbstractSocket::ConnectedState);
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
                connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(_q_state()));
            }
        }
    }

    /*!
        Resolves target for \a sender and \a receiver.
     */
    QString Session::resolveTarget(const QString& sender, const QString& receiver) const
    {
        Q_D(const Session);

        QString target = receiver;

        if (target.contains(QLatin1Char('*')) || target.contains(QLatin1Char('?')))
            target = d->nick;

        if (target == d->nick)
        {
            if (target == sender)
                target = d->host;
            else
                target = sender;
        }

        if (target.isEmpty() || target == QLatin1String("AUTH"))
            target = d->host;

        return target;
    }

    /*!
        Connects Irc::Session signals to matching slots of the \a receiver.

        Receiver slots must follow the following syntax:
        \code
        void on_<signal name>(<signal parameters>);
        \endcode
    */
    void Session::connectSlotsByName(const QObject* receiver)
    {
        if (!receiver)
            return;
        const QMetaObject *thisMo = metaObject();
        const QMetaObject *thatMo = receiver->metaObject();
        Q_ASSERT(thisMo && thatMo);
        QSet<QByteArray> connectedSlots;
        for (int j = 0; j < thatMo->methodCount(); ++j) {
            QMetaMethod slot = thatMo->method(j);
            const char* slotSignature = slot.signature();
            Q_ASSERT(slotSignature);
            if (qstrncmp(slotSignature, "on_", 3))
                continue;
            for (int i = 0; i < thisMo->methodCount(); ++i) {
                QMetaMethod signal = thisMo->method(i);
                if (signal.parameterTypes() != slot.parameterTypes())
                    continue;
                if (signal.methodType() == QMetaMethod::Signal) {
                    const char* signalSignature = signal.signature();
                    Q_ASSERT(signalSignature);
                    if (qstrcmp(slotSignature + 3, signalSignature))
                        continue;
                    QByteArray slotName = QByteArray::fromRawData(slotSignature, qstrlen(slotSignature));
                    if (!connectedSlots.contains(slotName)) {
                        // prevent double connection to overridden slots
                        connectedSlots.insert(slotName);
                        QMetaObject::connect(this, i, receiver, j);
                    }
                }
            }
        }
    }

    /*!
        Connects to server with \a hostName and \a port.
     */
    void Session::connectToServer(const QString& hostName, quint16 port)
    {
        Q_D(Session);
        d->motdReceived = false;
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
        d->motdReceived = true;
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
    bool Session::sendRaw(const QString& message)
    {
        Q_D(Session);
        qint64 bytes = -1;
        if (d->socket)
            d->socket->write(message.toUtf8() + QByteArray("\r\n"));
        return bytes != -1;
    }

    /*!
        Joins \a channel with optional \a key.
     */
    bool Session::cmdJoin(const QString& channel, const QString& key)
    {
        if (key.isEmpty())
            return sendRaw(QString(QLatin1String("JOIN %1")).arg(channel));
        else
            return sendRaw(QString(QLatin1String("JOIN %1 %2")).arg(channel).arg(key));
    }

    /*!
        Parts \a channel with \a reason.
     */
    bool Session::cmdPart(const QString& channel, const QString& reason)
    {
        if (reason.isEmpty())
            return sendRaw(QString(QLatin1String("PART %1")).arg(channel));
        else
            return sendRaw(QString(QLatin1String("PART %1 :%2")).arg(channel).arg(reason));
    }

    /*!
        Quits with optional \a reason.
     */
    bool Session::cmdQuit(const QString& reason)
    {
        return sendRaw(QString(QLatin1String("QUIT :%1")).arg(reason.isEmpty() ? QLatin1String("Quit") : reason));
    }

    /*!
        Requests the list of names on \a channel.
     */
    bool Session::cmdNames(const QString& channel)
    {
        return sendRaw(QString(QLatin1String("NAMES %1")).arg(channel));
    }

    /*!
        Requests the list of channels.
     */
    bool Session::cmdList(const QString& channel)
    {
        if (channel.isEmpty())
            return sendRaw(QString(QLatin1String("LIST")));
        else
            return sendRaw(QString(QLatin1String("LIST %1")).arg(channel));
    }

    /*!
        Sends a whois command on \a nick.
     */
    bool Session::cmdWhois(const QString& nick)
    {
        return sendRaw(QString(QLatin1String("WHOIS %1 %2")).arg(nick).arg(nick));
    }

    /*!
        Sends a whowas command on \a nick.
     */
    bool Session::cmdWhowas(const QString& nick)
    {
        return sendRaw(QString(QLatin1String("WHOWAS %1 %2")).arg(nick).arg(nick));
    }

    /*!
        Sends a mode command on \a target.
     */
    bool Session::cmdMode(const QString& target, const QString& mode)
    {
        if (mode.isEmpty())
            return sendRaw(QString(QLatin1String("MODE %1")).arg(target));
        else
            return sendRaw(QString(QLatin1String("MODE %1 %2")).arg(target).arg(mode));
    }

    /*!
        Sends a topic command on \a channel.
     */
    bool Session::cmdTopic(const QString& channel, const QString& topic)
    {
        if (topic.isEmpty())
            return sendRaw(QString(QLatin1String("TOPIC %1")).arg(channel));
        else
            return sendRaw(QString(QLatin1String("TOPIC %1 :%2")).arg(channel).arg(topic));
    }

    /*!
        Sends an invitation to \a nick.
     */
    bool Session::cmdInvite(const QString& nick, const QString& channel)
    {
        return sendRaw(QString(QLatin1String("INVITE %1 %2")).arg(nick).arg(channel));
    }

    /*!
        Kicks \a nick from \a channel with \a reason.
     */
    bool Session::cmdKick(const QString& nick, const QString& channel, const QString& reason)
    {
        if (reason.isEmpty())
            return sendRaw(QString(QLatin1String("KICK %1 %2")).arg(channel).arg(nick));
        else
            return sendRaw(QString(QLatin1String("KICK %1 %2 :%3")).arg(channel).arg(nick).arg(reason));
    }

    /*!
        Sends a \a message to \a receiver.
     */
    bool Session::cmdMessage(const QString& receiver, const QString& message)
    {
        Q_D(Session);
        emit msgMessageReceived(d->nick, receiver, message);
        return sendRaw(QString(QLatin1String("PRIVMSG %1 :%2")).arg(receiver).arg(message));
    }

    /*!
        Sends a \a notice to \a receiver.
     */
    bool Session::cmdNotice(const QString& receiver, const QString& notice)
    {
        Q_D(Session);
        emit msgNoticeReceived(d->nick, receiver, notice);
        return sendRaw(QString(QLatin1String("NOTICE %1 :%2")).arg(receiver).arg(notice));
    }

    /*!
        Sends a CTCP \a action to \a receiver.
     */
    bool Session::cmdCtcpAction(const QString& receiver, const QString& action)
    {
        Q_D(Session);
        emit msgCtcpActionReceived(d->nick, receiver, action);
        return sendRaw(QString(QLatin1String("PRIVMSG %1 :\x01" "ACTION %2\x01")).arg(receiver).arg(action));
    }

    /*!
        Sends a CTCP \a request to \a receiver.
     */
    bool Session::cmdCtcpRequest(const QString& nick, const QString& request)
    {
        return sendRaw(QString(QLatin1String("PRIVMSG %1 :\x01%2\x01")).arg(nick).arg(request));
    }

    /*!
        Sends a CTCP \a reply to \a receiver.
     */
    bool Session::cmdCtcpReply(const QString& nick, const QString& reply)
    {
        return sendRaw(QString(QLatin1String("NOTICE %1 :\x01%2\x01")).arg(nick).arg(reply));
    }
}

#include "moc_ircsession.cpp"
