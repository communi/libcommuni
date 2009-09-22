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
*/

#ifndef IRC_SESSION_H
#define IRC_SESSION_H

#include <ircglobal.h>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QAbstractSocket)

namespace Irc
{
    class Buffer;
    class SessionPrivate;

    class IRC_EXPORT Session : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QStringList autoJoinChannels READ autoJoinChannels WRITE setAutoJoinChannels)
        Q_PROPERTY(int autoReconnectDelay READ autoReconnectDelay WRITE setAutoReconnectDelay)
        Q_PROPERTY(QByteArray encoding READ encoding WRITE setEncoding)
        Q_PROPERTY(QString host READ host WRITE setHost)
        Q_PROPERTY(QString ident READ ident WRITE setIdent)
        Q_PROPERTY(QString nick READ nick WRITE setNick)
        Q_PROPERTY(Options options READ options WRITE setOptions)
        Q_PROPERTY(QString password READ password WRITE setPassword)
        Q_PROPERTY(quint16 port READ port WRITE setPort)
        Q_PROPERTY(QString realName READ realName WRITE setRealName)
        Q_FLAGS(Options)
        Q_ENUMS(Option)

    public:
        Session(QObject* parent = 0);
        ~Session();

        QStringList autoJoinChannels() const;
        void addAutoJoinChannel(const QString& channel);
        void removeAutoJoinChannel(const QString& channel);
        void setAutoJoinChannels(const QStringList& channels);

        int autoReconnectDelay() const;
        void setAutoReconnectDelay(int seconds);

        QByteArray encoding() const;
        void setEncoding(const QByteArray& encoding);

        QString ident() const;
        void setIdent(const QString& ident);

        QString nick() const;
        void setNick(const QString& nick);

        QString password() const;
        void setPassword(const QString& password);

        QString realName() const;
        void setRealName(const QString& realName);

        QString host() const;
        void setHost(const QString& host);

        quint16 port() const;
        void setPort(quint16 port);

        enum Option
        {
            NoOptions = 0x0,
            StripNicks = 0x1,
            EchoMessages = 0x2
        };
        Q_DECLARE_FLAGS(Options, Option)

        Options options() const;
        void setOptions(Options options);

        QAbstractSocket* socket() const;
        void setSocket(QAbstractSocket* socket);

        Buffer* buffer(const QString& receiver = QString()) const;

        Buffer* defaultBuffer() const;
        void setDefaultBuffer(Buffer* buffer);

    public Q_SLOTS:
        void connectToServer(const QString& hostName = QString(), quint16 port = 6667);
        void reconnectToServer();
        void disconnectFromServer();

        bool raw(const QString& message);
        bool join(const QString& channel, const QString& key = QString());
        bool part(const QString& channel, const QString& reason = QString());
        bool quit(const QString& reason = QString());
        bool names(const QString& channel);
        bool list(const QString& channel = QString());
        bool whois(const QString& nick);
        bool whowas(const QString& nick);
        bool mode(const QString& target, const QString& mode = QString());
        bool topic(const QString& channel, const QString& topic = QString());
        bool invite(const QString& nick, const QString& channel);
        bool kick(const QString& nick, const QString& channel, const QString& reason = QString());
        bool message(const QString& receiver, const QString& message);
        bool notice(const QString& receiver, const QString& notice);
        bool ctcpAction(const QString& receiver, const QString& action);
        bool ctcpRequest(const QString& nick, const QString& request);
        bool ctcpReply(const QString& nick, const QString& reply);

    Q_SIGNALS:
        void connected();
        void reconnecting();
        void disconnected();

        void bufferAdded(Irc::Buffer* buffer);
        void bufferRemoved(Irc::Buffer* buffer);

    protected:
        virtual Buffer* createBuffer(const QString& receiver);

    private:
        SessionPrivate* const d_ptr;
        Q_DECLARE_PRIVATE(Session)
        Q_DISABLE_COPY(Session)

        Q_PRIVATE_SLOT(d_func(), void _q_connected())
        Q_PRIVATE_SLOT(d_func(), void _q_disconnected())
        Q_PRIVATE_SLOT(d_func(), void _q_reconnect())
        Q_PRIVATE_SLOT(d_func(), void _q_error())
        Q_PRIVATE_SLOT(d_func(), void _q_state(QAbstractSocket::SocketState))
        Q_PRIVATE_SLOT(d_func(), void _q_readData())
        friend class Buffer;
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Irc::Session::Options)

#endif // IRC_SESSION_H
