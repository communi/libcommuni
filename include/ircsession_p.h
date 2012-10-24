/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
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

#ifndef IRCSESSION_P_H
#define IRCSESSION_P_H

#include <QSet>
#include <QString>
#include <QByteArray>
#include <QMultiHash>
#include <QAbstractSocket>

class IrcSessionPrivate
{
    Q_DECLARE_PUBLIC(IrcSession)

public:
    IrcSessionPrivate(IrcSession* session);

    void _irc_connected();
    void _irc_disconnected();
    void _irc_reconnect();
    void _irc_error(QAbstractSocket::SocketError error);
    void _irc_state(QAbstractSocket::SocketState state);
    void _irc_readData();

    void readLines(const QByteArray& delimiter);
    void processLine(const QByteArray& line);
    void setNick(const QString& nick);
    void setActive(bool active);
    void setConnected(bool connected);

    IrcSession* q_ptr;
    QByteArray encoding;
    QByteArray buffer;
    QAbstractSocket* socket;
    QString host;
    int port;
    QString userName;
    QString nickName;
    QString realName;
    bool active;
    bool connected;
    QSet<QString> capabilities;
};

#endif // IRCSESSION_P_H
