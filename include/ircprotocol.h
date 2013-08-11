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

#ifndef IRCPROTOCOL_H
#define IRCPROTOCOL_H

#include <IrcGlobal>
#include <QtCore/qhash.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qscopedpointer.h>
#include <QtNetwork/qabstractsocket.h>

class IrcSession;
class IrcMessage;
class IrcProtocolPrivate;

class COMMUNI_EXPORT IrcProtocol : public QObject
{
    Q_OBJECT

public:
    explicit IrcProtocol(IrcSession* session);
    virtual ~IrcProtocol();

    IrcSession* session() const;
    QAbstractSocket* socket() const;

    virtual void open();
    virtual void authenticate(bool secure);

    virtual void read();
    virtual bool write(const QByteArray& data);

    virtual QStringList availableCapabilities() const;
    virtual QStringList activeCapabilities() const;

protected Q_SLOTS:
    void setActive(bool active);
    void setConnected(bool connected);
    void setNick(const QString& nick);
    void setInfo(const QHash<QString, QString>& info);

    void receiveMessage(IrcMessage* message);

private:
    QScopedPointer<IrcProtocolPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcProtocol)
    Q_DISABLE_COPY(IrcProtocol)
};

#endif // IRCPROTOCOL_H
