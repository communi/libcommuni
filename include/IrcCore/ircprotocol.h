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

#ifndef IRCPROTOCOL_H
#define IRCPROTOCOL_H

#include <IrcGlobal>
#include <IrcConnection>
#include <QtCore/qset.h>
#include <QtCore/qhash.h>
#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>
#include <QtNetwork/qabstractsocket.h>

IRC_BEGIN_NAMESPACE

class IrcMessage;
class IrcConnection;
class IrcProtocolPrivate;

class IRC_CORE_EXPORT IrcProtocol : public QObject
{
    Q_OBJECT

public:
    explicit IrcProtocol(IrcConnection* connection);
    virtual ~IrcProtocol();

    IrcConnection* connection() const;
    QAbstractSocket* socket() const;

    virtual void open();
    virtual void authenticate(bool secure);

    virtual void read();
    virtual bool write(const QByteArray& data);

    virtual bool hasQuit() const;

protected Q_SLOTS:
    void setNick(const QString& nick);
    void setStatus(IrcConnection::Status status);
    void setInfo(const QHash<QString, QString>& info);
    void setAvailableCapabilities(const QSet<QString>& capabilities);
    void setActiveCapabilities(const QSet<QString>& capabilities);

    void receiveMessage(IrcMessage* message);

private:
    QScopedPointer<IrcProtocolPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcProtocol)
    Q_DISABLE_COPY(IrcProtocol)

    Q_PRIVATE_SLOT(d_func(), void _irc_pauseHandshake())
    Q_PRIVATE_SLOT(d_func(), void _irc_resumeHandshake())
};

IRC_END_NAMESPACE

#endif // IRCPROTOCOL_H
