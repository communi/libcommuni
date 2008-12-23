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

#ifndef IRC_SESSION_H
#define IRC_SESSION_H

#include <irc.h>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QHostAddress)
QT_FORWARD_DECLARE_CLASS(QNetworkProxy)
QT_FORWARD_DECLARE_CLASS(QAbstractSocket)

namespace Irc
{
    class SessionPrivate;

    class LIBIRCCLIENT_QT_EXPORT Session : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QStringList autoJoinChannels READ autoJoinChannels WRITE setAutoJoinChannels)
        Q_PROPERTY(int autoReconnectDelay READ autoReconnectDelay WRITE setAutoReconnectDelay)
        Q_PROPERTY(bool connected READ isConnected)
        Q_PROPERTY(QByteArray encoding READ encoding WRITE setEncoding)
        Q_PROPERTY(QString ident READ ident WRITE setIdent)
        Q_PROPERTY(QString nick READ nick WRITE setNick)
        Q_PROPERTY(QString password READ password WRITE setPassword)
        Q_PROPERTY(QString realName READ realName WRITE setRealName)

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

        QString password() const;
        void setPassword(const QString& password);

        QString realName() const;
        void setRealName(const QString& realName);

        QString host() const;
        quint16 port() const;

        enum Option
        {
            StripNicks = 0x1
        };
        Q_DECLARE_FLAGS(Options, Option)

        Options options() const;
        void setOptions(Options options);

        void connectSlotsByName(QObject* receiver);

        bool isConnected() const;

    public slots:
        void setNick(const QString& nick);

        void connectToServer(const QString& hostName, quint16 port = 6667);
        void connectToServer(const QHostAddress& address, quint16 port = 6667);
        void disconnectFromServer();

        bool sendRaw(const QString& message);
        bool cmdJoin(const QString& channel, const QString& key = QString());
        bool cmdPart(const QString& channel);
        bool cmdQuit(const QString& reason = QString());
        bool cmdNames(const QString& channel);
        bool cmdList(const QString& channel = QString());
        bool cmdWhois(const QString& nick);
        bool cmdMode(const QString& target, const QString& mode = QString());
        bool cmdTopic(const QString& channel, const QString& topic = QString());
        bool cmdInvite(const QString& nick, const QString& channel);
        bool cmdKick(const QString& nick, const QString& channel, const QString& reason = QString());
        bool cmdMessage(const QString& receiver, const QString& message);
        bool cmdNotice(const QString& receiver, const QString& message);
        bool cmdCtcpAction(const QString& receiver, const QString& message);
        bool cmdCtcpRequest(const QString& nick, const QString& request);
        bool cmdCtcpReply(const QString& nick, const QString& reply);

    signals:
        void connected();
        void disconnected();
        
        void msgJoined(const QString& origin, const QString& channel);
        void msgParted(const QString& origin, const QString& channel, const QString& message);
        void msgQuit(const QString& origin, const QString& message);
        void msgNickChanged(const QString& origin, const QString& nick);
        void msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args);
        void msgTopicChanged(const QString& origin, const QString& channel, const QString& topic);
        void msgInvited(const QString& origin, const QString& receiver, const QString& channel);
        void msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message);
        void msgMessageReceived(const QString& origin, const QString& receiver, const QString& message);
        void msgNoticeReceived(const QString& origin, const QString& receiver, const QString& message);
        void msgCtcpRequestReceived(const QString& origin, const QString& request);
        void msgCtcpReplyReceived(const QString& origin, const QString& reply);
        void msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action);
        void msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params);
        void msgUnknownMessageReceived(const QString& origin, const QStringList& params);

    protected:
        SessionPrivate* const d_ptr;

    private:
        Q_DECLARE_PRIVATE(Session)
        Q_DISABLE_COPY(Session)
        
        Q_PRIVATE_SLOT(d_func(), void _q_connected())
        Q_PRIVATE_SLOT(d_func(), void _q_disconnected())
        Q_PRIVATE_SLOT(d_func(), void _q_reconnect())
        Q_PRIVATE_SLOT(d_func(), void _q_error())
        Q_PRIVATE_SLOT(d_func(), void _q_state())
        Q_PRIVATE_SLOT(d_func(), void _q_readData())
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Irc::Session::Options)

#endif // IRC_SESSION_H
