/*
* Copyright (C) 2008-2014 The Communi Project
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

#ifndef IRCCONNECTION_P_H
#define IRCCONNECTION_P_H

#include "ircconnection.h"

#include <QList>
#include <QHash>
#include <QStack>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include <QAbstractSocket>

IRC_BEGIN_NAMESPACE

class IrcMessageFilter;
class IrcCommandFilter;

class IrcConnectionPrivate
{
    Q_DECLARE_PUBLIC(IrcConnection)

public:
    IrcConnectionPrivate();

    void init(IrcConnection* connection);

    void _irc_connected();
    void _irc_disconnected();
    void _irc_error(QAbstractSocket::SocketError error);
    void _irc_state(QAbstractSocket::SocketState state);
    void _irc_reconnect();
    void _irc_readData();

    void _irc_filterDestroyed(QObject* filter);

    void setNick(const QString& nick);
    void setStatus(IrcConnection::Status status);
    void setInfo(const QHash<QString, QString>& info);

    void receiveMessage(IrcMessage* msg);
    IrcCommand* createCtcpReply(IrcPrivateMessage* request);

    static IrcConnectionPrivate* get(const IrcConnection* connection)
    {
        return connection->d_ptr.data();
    }

    IrcConnection* q_ptr;
    QByteArray encoding;
    IrcNetwork* network;
    IrcProtocol* protocol;
    QAbstractSocket* socket;
    QString host;
    int port;
    QString userName;
    QString nickName;
    QString realName;
    QString password;
    QString displayName;
    QVariantMap userData;
    QTimer reconnecter;
    QString saslMechanism;
    bool enabled;
    IrcConnection::Status status;
    QList<IrcCommand*> pendingCommands;
    QList<QObject*> commandFilters;
    QList<QObject*> messageFilters;
    QStack<QObject*> activeCommandFilters;
};

IRC_END_NAMESPACE

#endif // IRCCONNECTION_P_H
