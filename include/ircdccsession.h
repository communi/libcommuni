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

#ifndef IRC_DCC_SESSION_H
#define IRC_DCC_SESSION_H

#include <ircglobal.h>
#include <QObject>

class Session;

QT_FORWARD_DECLARE_CLASS(QHostAddress)
QT_FORWARD_DECLARE_CLASS(QNetworkProxy)
QT_FORWARD_DECLARE_CLASS(QAbstractSocket)

namespace Irc
{
    class DccSessionPrivate;

    class IRC_EXPORT DccSession : public QObject
    {
        Q_OBJECT

    public:
        DccSession(QObject* parent = 0);
        ~DccSession();

        Session* session() const;

    public Q_SLOTS:
        bool accept();
        bool decline();

        bool sendMessage(const QString& message);
        bool sendFile(const QString& fileName);

    Q_SIGNALS:

    private:
        DccSessionPrivate* const d_ptr;
        Q_DECLARE_PRIVATE(DccSession)
        Q_DISABLE_COPY(DccSession)
    };
}

#endif // IRC_DCC_SESSION_H
