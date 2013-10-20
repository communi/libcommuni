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

#ifndef IRCLAGTIMER_H
#define IRCLAGTIMER_H

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qscopedpointer.h>

IRC_BEGIN_NAMESPACE

class IrcConnection;
class IrcLagTimerPrivate;

class IRC_UTIL_EXPORT IrcLagTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 lag READ lag NOTIFY lagChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval)
    Q_PROPERTY(IrcConnection* connection READ connection WRITE setConnection)

public:
    explicit IrcLagTimer(QObject* parent = 0);
    virtual ~IrcLagTimer();

    IrcConnection* connection() const;
    void setConnection(IrcConnection* connection);

    qint64 lag() const;

    int interval() const;
    void setInterval(int seconds);

Q_SIGNALS:
    void lagChanged(qint64 lag);

private:
    QScopedPointer<IrcLagTimerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcLagTimer)
    Q_DISABLE_COPY(IrcLagTimer)

    Q_PRIVATE_SLOT(d_func(), void _irc_connected())
    Q_PRIVATE_SLOT(d_func(), void _irc_pingServer())
    Q_PRIVATE_SLOT(d_func(), void _irc_disconnected())
};

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcLagTimer*))

#endif // IRCLAGTIMER_H
