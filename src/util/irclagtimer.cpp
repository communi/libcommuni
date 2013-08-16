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

#include "irclagtimer.h"
#include "ircconnection.h"
#include "ircmessage.h"
#include "irccommand.h"
#include "ircmessagefilter.h"
#include <QDateTime>
#include <QTimer>

IRC_BEGIN_NAMESPACE

static const int DEFAULT_INTERVAL = 60;

/*!
    \file irclagtimer.h
    \brief #include &lt;IrcLagTimer&gt;
 */

/*!
    \class IrcLagTimer irclagtimer.h <IrcLagTimer>
    \ingroup util
    \brief Provides a timer for measuring lag.

    \note IrcLagTimer relies on functionality introduced in Qt 4.7.0, and is
          therefore not functional when built against earlier versions of Qt.
 */

/*!
    \fn void IrcLagTimer::lagChanged(qint64 lag)

    This signal is emitted when the \a lag has changed.
 */

class IrcLagTimerPrivate : public IrcMessageFilter
{
    Q_DECLARE_PUBLIC(IrcLagTimer)

public:
    IrcLagTimerPrivate(IrcLagTimer* q);

    bool messageFilter(IrcMessage* msg);
    bool processPongReply(IrcPongMessage* msg);

    void _irc_connected();
    void _irc_pingServer();
    void _irc_disconnected();

    void updateTimer();
    void updateLag(qint64 value);

    IrcLagTimer* q_ptr;
    IrcConnection* connection;
    QTimer timer;
    int interval;
    qint64 lag;
};

IrcLagTimerPrivate::IrcLagTimerPrivate(IrcLagTimer* q)
    : q_ptr(q), connection(0), interval(DEFAULT_INTERVAL), lag(-1)
{
    QObject::connect(&timer, SIGNAL(timeout()), q, SLOT(_irc_pingServer()));
}

bool IrcLagTimerPrivate::messageFilter(IrcMessage* msg)
{
    if (msg->type() == IrcMessage::Pong)
        return processPongReply(static_cast<IrcPongMessage*>(msg));
    return false;
}

bool IrcLagTimerPrivate::processPongReply(IrcPongMessage* msg)
{
#if QT_VERSION >= 0x040700
    // TODO: configurable format?
    if (msg->argument().startsWith("communi/")) {
        bool ok = false;
        qint64 timestamp = msg->argument().mid(8).toLongLong(&ok);
        if (ok) {
            updateLag(QDateTime::currentMSecsSinceEpoch() - timestamp);
            return true;
        }
    }
#endif // QT_VERSION
    return false;
}

void IrcLagTimerPrivate::_irc_connected()
{
#if QT_VERSION >= 0x040700
    if (interval > 0)
        timer.start();
#endif // QT_VERSION
}

void IrcLagTimerPrivate::_irc_pingServer()
{
#if QT_VERSION >= 0x040700
    // TODO: configurable format?
    QString argument = QString("communi/%1").arg(QDateTime::currentMSecsSinceEpoch());
    IrcCommand* cmd = IrcCommand::createPing(argument);
    connection->sendCommand(cmd);
#endif // QT_VERSION
}

void IrcLagTimerPrivate::_irc_disconnected()
{
#if QT_VERSION >= 0x040700
    updateLag(-1);
    if (timer.isActive())
        timer.stop();
#endif // QT_VERSION
}

void IrcLagTimerPrivate::updateTimer()
{
#if QT_VERSION >= 0x040700
    if (connection && interval > 0) {
        timer.setInterval(interval * 1000);
        if (!timer.isActive() && connection->isConnected())
            timer.start();
    } else {
        if (timer.isActive())
            timer.stop();
        updateLag(-1);
    }
#endif // QT_VERSION
}

void IrcLagTimerPrivate::updateLag(qint64 value)
{
    Q_Q(IrcLagTimer);
    if (lag != value) {
        lag = qMax(-1ll, value);
        emit q->lagChanged(lag);
    }
}

/*!
    Constructs a new lag timer with \a parent.

    \note If \a parent is an instance of IrcConnection, it will be
    automatically assigned to \ref IrcLagTimer::connection "connection".
 */
IrcLagTimer::IrcLagTimer(QObject* parent) : QObject(parent), d_ptr(new IrcLagTimerPrivate(this))
{
    setConnection(qobject_cast<IrcConnection*>(parent));
}

/*!
    Destructs the lag timer.
 */
IrcLagTimer::~IrcLagTimer()
{
}

/*!
    This property holds the associated connection.

    \par Access functions:
    \li IrcConnection* <b>connection</b>() const
    \li void <b>setConnection</b>(IrcConnection* connection)
 */
IrcConnection* IrcLagTimer::connection() const
{
    Q_D(const IrcLagTimer);
    return d->connection;
}

void IrcLagTimer::setConnection(IrcConnection* connection)
{
    Q_D(IrcLagTimer);
    if (d->connection != connection) {
        if (d->connection) {
            d->connection->removeMessageFilter(d);
            disconnect(d->connection, SIGNAL(connected()), this, SLOT(_irc_connected()));
            disconnect(d->connection, SIGNAL(disconnected()), this, SLOT(_irc_disconnected()));
        }
        d->connection = connection;
        if (connection) {
            connection->installMessageFilter(d);
            connect(connection, SIGNAL(connected()), this, SLOT(_irc_connected()));
            connect(connection, SIGNAL(disconnected()), this, SLOT(_irc_disconnected()));
        }
        d->updateLag(-1);
        d->updateTimer();
    }
}

/*!
    This property holds the current lag in milliseconds.

    The value is \c -1 when
    \li the connection is not connected,
    \li the lag has not yet been measured,
    \li the lag timer is disabled (interval <= 0s), or
    \li the Qt version is too old (4.7.0 or later is required).

    \par Access function:
    \li qint64 <b>lag</b>() const

    \par Notifier signal:
    \li void <b>lagChanged</b>(qint64 lag)
 */
qint64 IrcLagTimer::lag() const
{
    Q_D(const IrcLagTimer);
    return d->lag;
}

/*!
    This property holds the lag measurement interval in seconds.

    The default value is \c60 seconds. A value equal to or
    less than \c 0 seconds disables the lag measurement.

    \par Access functions:
    \li int <b>interval</b>() const
    \li void <b>setInterval</b>(int seconds)
 */
int IrcLagTimer::interval() const
{
    Q_D(const IrcLagTimer);
    return d->interval;
}

void IrcLagTimer::setInterval(int seconds)
{
    Q_D(IrcLagTimer);
    if (d->interval != seconds) {
        d->interval = seconds;
        d->updateTimer();
    }
}

#include "moc_irclagtimer.cpp"

IRC_END_NAMESPACE
