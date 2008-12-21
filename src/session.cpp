/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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

#include "session.h"
#include <QBuffer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QStringList>
#ifdef HAVE_ICU
#include <unicode/ucsdet.h>
#endif // HAVE_ICU

static void irc_target_get_nick (const char * target, char *nick, size_t size)
{
	const char *p = strstr (target, "!");
	unsigned int len;

	if ( p )
		len = p - target;
	else
		len = strlen (target);

	if ( len > size-1 )
		len = size - 1;

	memcpy (nick, target, len);
	nick[len] = '\0';
}

static void irc_target_get_host (const char * target, char *host, size_t size)
{
	unsigned int len;
	const char *p = strstr (target, "!");

	if ( !p )
		p = target;

	len = strlen (p);

	if ( len > size-1 )
		len = size - 1;

	memcpy (host, p, len);
	host[len] = '\0';
}

static QByteArray detectEncoding(const QByteArray& text)
{
    Q_UNUSED(text);
    QByteArray encoding;
#ifdef HAVE_ICU
    UErrorCode status = U_ZERO_ERROR;
    UCharsetDetector* detector = ucsdet_open(&status);
    if (!U_FAILURE(status))
    {
        ucsdet_setText(detector, text.constData(), text.length(), &status);
        if (!U_FAILURE(status))
        {
            const UCharsetMatch* match = ucsdet_detect(detector, &status);
            if (!U_FAILURE(status))
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
        void _q_error();
        void _q_state();
        void _q_readData();

        void processLine(QByteArray line);
        QString readString(const QByteArray& data) const;
        QStringList readStrings(const QList<QByteArray>& list) const;

        Session* q_ptr;

        QBuffer buffer;
        QTcpSocket socket;
        Session::Options options;

        QString ident;
        QString password;
        QString nick;
        QString realName;
        bool motdReceived;
        QStringList channels;
        QByteArray encoding;
        bool reconnect;
    };

    SessionPrivate::SessionPrivate() :
        q_ptr(0),
        buffer(),
        socket(),
        options(0),
        ident(),
        password(),
        nick(),
        realName(),
        motdReceived(false),
        channels(),
        encoding(),
        reconnect(false)
    {
    }

    void SessionPrivate::init(Session* session)
    {
        q_ptr = session;

        buffer.open(QIODevice::ReadWrite);

        Q_Q(Session);
        q->connect(&socket, SIGNAL(connected()), q, SIGNAL(connected()));
        q->connect(&socket, SIGNAL(disconnected()), q, SIGNAL(disconnected()));
        q->connect(&socket, SIGNAL(connected()), q, SLOT(_q_connected()));
        q->connect(&socket, SIGNAL(readyRead()), q, SLOT(_q_readData()));
        q->connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), q, SLOT(_q_error()));
        q->connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), q, SLOT(_q_state()));
    }

    void SessionPrivate::_q_connected()
    {
        if (!password.isEmpty())
            socket.write(QString("PASS %1\r\n").arg(password).toLocal8Bit());

        socket.write(QString("NICK %1\r\n").arg(nick).toLocal8Bit());

		// RFC 1459 states that "hostname and servername are normally 
        // ignored by the IRC server when the USER command comes from 
        // a directly connected client (for security reasons)", therefore 
        // we don't need them.
        socket.write(QString("USER %1 unknown unknown :%2\r\n").arg(ident).arg(realName).toLocal8Bit());
    }

    void SessionPrivate::_q_error()
    {
        qDebug("ERROR %i", socket.error());
    }

    void SessionPrivate::_q_state()
    {
        qDebug("STATE %i", socket.state());
    }

    void SessionPrivate::_q_readData()
    {
        qint64 bytes = buffer.write(socket.readAll());
        buffer.seek(buffer.pos() - bytes);
        while (buffer.canReadLine())
        {
            QByteArray line = buffer.readLine();
            processLine(line);
        }
    }

    QString SessionPrivate::readString(const QByteArray& data) const
    {
        QByteArray enc = encoding;
        if (enc.isNull())
            enc = detectEncoding(data);
        QTextCodec *codec = QTextCodec::codecForName(encoding);
        if (!codec)
            codec = QTextCodec::codecForLocale();
        return codec->toUnicode(data);
    }

    QStringList SessionPrivate::readStrings(const QList<QByteArray>& list) const
    {
        QStringList strings;
        foreach (const QByteArray& data, list)
            strings << readString(data);
        return strings;
    }

    void SessionPrivate::processLine(QByteArray line)
    {
        Q_Q(Session);
        QByteArray prefix, command;
        QList<QByteArray> params;

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
        if (line.startsWith(':'))
        {
            prefix = line.mid(1, line.indexOf(' ') - 1);
            line.remove(0, prefix.length() + 2);
        }

        // parse <command>
        command = line.mid(0, line.indexOf(' '));
        line.remove(0, command.length() + 1);
        bool isNumeric = false;
        uint code = command.toInt(&isNumeric);

        // parse middle/params
        while (!line.isEmpty())
        {
            if (line.startsWith(':'))
            {
                line.remove(0, 1);
                params << line;
                line.clear();
            }
            else
            {
                QByteArray param = line.mid(0, line.indexOf(' '));
                line.remove(0, param.length() + 1);
                params << param;
            }
        }

        // handle PING/PONG
        if (!qstrcmp(command, "PING"))
        {
            QString arg = readString(params.value(0));
            q->sendRaw(QString(QLatin1String("PONG %s")).arg(arg));
            return;
        }

        // and dump
        if (isNumeric)
        {
            // check whether it is the first RPL_ENDOFMOTD or ERR_NOMOTD after the connection
            if ((code == 376 || code == 422) && !motdReceived)
                motdReceived = true;
            QStringList strings = readStrings(params);
            emit q->msgNumericMessageReceived(prefix, code, strings);
        }
        else
        {
            if (!qstrcmp(command, "NICK"))
            {
                QString nick = readString(params.value(0));
                // TODO: irc_target_get_nick(prefix)
                if (this->nick == prefix)
                    this->nick = nick;
                emit q->msgNickChanged(prefix, nick);
            }
            else if (!qstrcmp(command, "QUIT"))
            {
                QString reason = readString(params.value(0));
                emit q->msgQuit(prefix, reason);
            }
            else if (!qstrcmp(command, "JOIN"))
            {
                QString channel = readString(params.value(0));
                emit q->msgJoined(prefix, channel);
            }
            else if (!qstrcmp(command, "PART"))
            {
                QString channel = readString(params.value(0));
                QString message = readString(params.value(1));
                emit q->msgParted(prefix, channel, message);
            }
            else if (!qstrcmp(command, "MODE"))
            {
                QString receiver = readString(params.value(0));
                QString mode = readString(params.value(1));
                QString args = readString(params.value(2));
                emit q->msgModeChanged(prefix, receiver, mode, args);
            }
            else if (!qstrcmp(command, "TOPIC"))
            {
                QString channel = readString(params.value(0));
                QString topic = readString(params.value(1));
                emit q->msgTopicChanged(prefix, channel, topic);
            }
            else if (!qstrcmp(command, "INVITE"))
            {
                QString receiver = readString(params.value(0));
                QString channel = readString(params.value(1));
                emit q->msgInvited(prefix, receiver, channel);
            }
            else if (!qstrcmp(command, "KICK"))
            {
                QString channel = readString(params.value(0));
                QString nick = readString(params.value(1));
                QString message = readString(params.value(2));
                emit q->msgKicked(prefix, channel, nick, message);
            }
            else if (!qstrcmp(command, "PRIVMSG"))
            {
                QString receiver = readString(params.value(0));
                QString message = readString(params.value(1));

                if (message.startsWith(QLatin1Char('\1')) && message.endsWith(QLatin1Char('\1')))
                {
                    message.remove(0, 1);
                    message.remove(message.length() - 2, 1);

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
            else if (!qstrcmp(command, "NOTICE"))
            {
                QString receiver = readString(params.value(0));
                QString message = readString(params.value(1));

                if (message.startsWith(QLatin1Char('\1')) && message.endsWith(QLatin1Char('\1')))
                {
                    message.remove(0, 1);
                    message.remove(message.length() - 2, 1);

                    // TODO: check params
                    emit q->msgCtcpReplyReceived(prefix, /*receiver,*/ message);
                }
                else
                {
                    emit q->msgNoticeReceived(prefix, receiver, message);
                }
            }
            else if (!qstrcmp(command, "KILL"))
            {
                ; // ignore this event - not all servers generate this
            }
            else
            {
                // The "unknown" event is triggered upon receipt of any number of 
                // unclassifiable miscellaneous messages, which aren't handled by 
                // the library.
                QStringList strings = readStrings(params);
                emit q->msgUnknownMessageReceived(prefix, strings);
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
        d->socket.close();
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
        {
            d->channels.append(channel);
        }
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
        Returns \c true if auto-reconnecting is enabled.    

        The default value is \c false.
     */
    bool Session::isAutoReconnectEnabled() const
    {
        Q_D(const Session);
        return d->reconnect;
    }

    /*!
        Sets auto-reconnecting \a enabled.
     */
    void Session::setAutoReconnectEnabled(bool enabled)
    {
        Q_D(Session);
        d->reconnect = enabled;
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

    QString Session::ident() const
    {
        Q_D(const Session);
        return d->ident;
    }

    void Session::setIdent(const QString& ident)
    {
        Q_D(Session);
        if (isConnected())
            qWarning("Session::setIdent() has no effect until re-connect");
        d->ident = ident;
    }

    QString Session::nick() const
    {
        Q_D(const Session);
        return d->nick;
    }

    void Session::setNick(const QString& nick)
    {
        Q_D(Session);
        if (d->nick != nick)
        {
            d->nick = nick;
        	sendRaw(QString(QLatin1String("NICK %1")).arg(nick));
        }
    }

    QString Session::password() const
    {
        Q_D(const Session);
        return d->password;
    }

    void Session::setPassword(const QString& password)
    {
        Q_D(Session);
        if (isConnected())
            qWarning("Session::setPassword() has no effect until re-connect");
        d->password = password;
    }

    QString Session::realName() const
    {
        Q_D(const Session);
        return d->realName;
    }

    void Session::setRealName(const QString& realName)
    {
        Q_D(Session);
        if (isConnected())
            qWarning("Session::setRealName() has no effect until re-connect");
        d->realName = realName;
    }

    Session::Options Session::options() const
    {
        Q_D(const Session);
        return d->options;
    }

    void Session::setOptions(Session::Options options)
    {
        Q_D(Session);
        d->options = options;
    }

    bool Session::isConnected() const
    {
        Q_D(const Session);
        return d->socket.state() == QAbstractSocket::ConnectingState
            || d->socket.state() == QAbstractSocket::ConnectedState;
    }

    void Session::connectToServer(const QString& hostName, quint16 port)
    {
        Q_D(Session);
        d->motdReceived = false;
        d->socket.connectToHost(hostName, port);
    }

    void Session::connectToServer(const QHostAddress& address, quint16 port)
    {
        Q_D(Session);
        d->motdReceived = false;
        d->socket.connectToHost(address, port);
    }

    void Session::disconnectFromServer()
    {
        Q_D(Session);
        d->socket.disconnectFromHost();
    }

    bool Session::sendRaw(const QString& message)
    {
        Q_D(Session);
        qint64 bytes = d->socket.write(message.toUtf8() + QByteArray("\r\n"));
        return bytes != -1;
    }

    bool Session::cmdJoin(const QString& channel, const QString& key)
    {
        if (key.isNull())
            return sendRaw(QString(QLatin1String("JOIN %1")).arg(channel));
        else
            return sendRaw(QString(QLatin1String("JOIN %1 %2")).arg(channel).arg(key));
    }

    bool Session::cmdPart(const QString& channel)
    {
        return sendRaw(QString(QLatin1String("PART %1")).arg(channel));
    }

    bool Session::cmdQuit(const QString& reason)
    {
        return sendRaw(QString(QLatin1String("QUIT :%1")).arg(reason.isNull() ? "Quit" : reason));
    }

    bool Session::cmdNames(const QString& channel)
    {
        return sendRaw(QString(QLatin1String("NAMES %1")).arg(channel));
    }

    bool Session::cmdList(const QString& channel)
    {
        if (channel.isNull())
            return sendRaw(QString(QLatin1String("LIST")));
        else
            return sendRaw(QString(QLatin1String("LIST %1")).arg(channel));
    }

    bool Session::cmdWhois(const QString& nick)
    {
        return sendRaw(QString(QLatin1String("WHOIS %1 %2")).arg(nick).arg(nick));
    }

    bool Session::cmdMode(const QString& target, const QString& mode)
    {
        if (mode.isNull())
            return sendRaw(QString(QLatin1String("MODE %1")).arg(target));
        else
            return sendRaw(QString(QLatin1String("MODE %1 %2")).arg(target).arg(mode));
    }

    bool Session::cmdTopic(const QString& channel, const QString& topic)
    {
        if (topic.isNull())
            return sendRaw(QString(QLatin1String("TOPIC %1")).arg(channel));
        else
            return sendRaw(QString(QLatin1String("TOPIC %1 %2")).arg(channel).arg(topic));
    }

    bool Session::cmdInvite(const QString& nick, const QString& channel)
    {
        return sendRaw(QString(QLatin1String("INVITE %1 %2")).arg(nick).arg(channel));
    }

    bool Session::cmdKick(const QString& nick, const QString& channel, const QString& reason)
    {
        if (reason.isNull())
            return sendRaw(QString(QLatin1String("KICK %1 %2")).arg(channel).arg(nick));
        else
            return sendRaw(QString(QLatin1String("KICK %1 %2 :%3")).arg(channel).arg(nick).arg(reason));
    }

    bool Session::cmdMessage(const QString& receiver, const QString& message)
    {
        return sendRaw(QString(QLatin1String("PRIVMSG %1 :%2")).arg(receiver).arg(message));
    }

    bool Session::cmdNotice(const QString& receiver, const QString& message)
    {
        return sendRaw(QString(QLatin1String("NOTICE %1 :%2")).arg(receiver).arg(message));
    }

    bool Session::cmdCtcpAction(const QString& receiver, const QString& message)
    {
        return sendRaw(QString(QLatin1String("PRIVMSG %1 :\x01" "ACTION %2\x01")).arg(receiver).arg(message));
    }

    bool Session::cmdCtcpRequest(const QString& nick, const QString& request)
    {
        return sendRaw(QString(QLatin1String("PRIVMSG %1 :\x01%2\x01")).arg(nick).arg(request));
    }

    bool Session::cmdCtcpReply(const QString& nick, const QString& reply)
    {
        return sendRaw(QString(QLatin1String("NOTICE %1 :\x01%2\x01")).arg(nick).arg(reply));
    }
}

#include "moc_session.cpp"
