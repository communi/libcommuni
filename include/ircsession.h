/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qscopedpointer.h>

class IrcCommand;

QT_FORWARD_DECLARE_CLASS(QAuthenticator)
QT_FORWARD_DECLARE_CLASS(QAbstractSocket)

namespace Irc
{
    class Buffer;
    class SessionPrivate;

    class IRC_EXPORT Session : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString host READ host WRITE setHost)
        Q_PROPERTY(quint16 port READ port WRITE setPort)
        Q_PROPERTY(QByteArray encoding READ encoding WRITE setEncoding)

        Q_ENUMS(MessageType ChannelAction UserAction CtcpType)

    public:
        Session(QObject* parent = 0);
        ~Session();

        QString host() const;
        void setHost(const QString& host);

        quint16 port() const;
        void setPort(quint16 port);

        QByteArray encoding() const;
        void setEncoding(const QByteArray& encoding);

        QAbstractSocket* socket() const;
        void setSocket(QAbstractSocket* socket);

        bool sendCommand(IrcCommand* command);

        /* TODO:
        QList<Buffer*> buffers() const;
        Buffer* buffer(const QString& receiver = QString()) const;
        Buffer* addBuffer(const QString& receiver);

        Buffer* defaultBuffer() const;
        void setDefaultBuffer(Buffer* buffer);
        */

    public Q_SLOTS:
        void open();
        void close();

    Q_SIGNALS:
        void connecting();
        void authenticate(QAuthenticator* authenticator);
        void connected();
        void disconnected();

        /*TODO:
    protected:
        virtual Buffer* createBuffer(const QString& receiver);
        */

    private:
        QScopedPointer<SessionPrivate> d_ptr;
        Q_DECLARE_PRIVATE(Session)
        Q_DISABLE_COPY(Session)

        Q_PRIVATE_SLOT(d_func(), void _q_connected())
        Q_PRIVATE_SLOT(d_func(), void _q_disconnected())
        Q_PRIVATE_SLOT(d_func(), void _q_reconnect())
        Q_PRIVATE_SLOT(d_func(), void _q_error())
        Q_PRIVATE_SLOT(d_func(), void _q_state(QAbstractSocket::SocketState))
        Q_PRIVATE_SLOT(d_func(), void _q_readData())
    };
}

#ifndef QT_NO_DEBUG_STREAM
IRC_EXPORT QDebug operator<<(QDebug debug, const Irc::Session* session);
#endif // QT_NO_DEBUG_STREAM

#endif // IRC_SESSION_H
