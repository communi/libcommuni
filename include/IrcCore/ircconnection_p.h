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

#ifndef IRCCONNECTION_P_H
#define IRCCONNECTION_P_H

#include "ircconnection.h"

#include <QList>
#include <QHash>
#include <QString>
#include <QByteArray>
#include <QAbstractSocket>

class IrcConnectionPrivate
{
    Q_DECLARE_PUBLIC(IrcConnection)

public:
    IrcConnectionPrivate(IrcConnection* connection);

    void _irc_connected();
    void _irc_disconnected();
    void _irc_error(QAbstractSocket::SocketError error);
    void _irc_state(QAbstractSocket::SocketState state);
    void _irc_readData();

    void setNick(const QString& nick);
    void setActive(bool active);
    void setConnected(bool connected);
    void receiveMessage(IrcMessage* msg);

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
    bool active;
    bool connected;
    QList<IrcMessageFilter*> filters;
};

#endif // IRCCONNECTION_P_H
