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

#ifndef IRC_SESSION_P_H
#define IRC_SESSION_P_H

#include <QHash>
#include <QTimer>
#include <QStringList>
#include <QAbstractSocket>

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
        void _q_state(QAbstractSocket::SocketState state);
        void _q_readData();

        QString readString(const QByteArray& data) const;
        void readLines(const QByteArray& delimiter);
        void processLine(const QByteArray& line);
        bool isConnected() const;
        QString resolveTarget(const QString& sender, const QString& receiver) const;

        Buffer* createBuffer(const QString& receiver);
        void removeBuffer(Buffer* buffer);

        Session* q_ptr;
        QByteArray buffer;
        Session::Options options;
        QAbstractSocket* socket;
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
        Buffer* defaultBuffer;
        QHash<QString, Buffer*> buffers;
    };
}

#endif // IRC_BUFFER_P_H
