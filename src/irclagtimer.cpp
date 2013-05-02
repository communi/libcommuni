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
#include "ircsession.h"
#include "ircmessage.h"
#include "irccommand.h"
#include "ircmessagefilter.h"
#include <QDateTime>
#include <QTimer>

static const int DEFAULT_INTERVAL = 60;

/*!
    \file irclagtimer.h
    \brief #include &lt;IrcLagTimer&gt;
 */

/*!
    \class IrcLagTimer irclagtimer.h <IrcLagTimer>
    \ingroup utility
    \brief The IrcLagTimer class provides a timer for measuring IRC session lag.

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
    IrcLagTimerPrivate(IrcSession* session, IrcLagTimer* q);

    bool messageFilter(IrcMessage* msg);
    bool processPongReply(IrcPongMessage* msg);

    void _irc_connected();
    void _irc_pingServer();
    void _irc_disconnected();

    void updateLag(qint64 value);

    IrcLagTimer* q_ptr;
    IrcSession* session;
    QTimer timer;
    int interval;
    qint64 lag;
};

IrcLagTimerPrivate::IrcLagTimerPrivate(IrcSession* session, IrcLagTimer* q)
    : q_ptr(q), session(session), interval(-1), lag(-1)
{
    session->installMessageFilter(this);
    QObject::connect(&timer, SIGNAL(timeout()), q, SLOT(_irc_pingServer()));
    QObject::connect(session, SIGNAL(connected()), q, SLOT(_irc_connected()));
    QObject::connect(session, SIGNAL(disconnected()), q, SLOT(_irc_disconnected()));
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
    // TODO: add IrcCommand::createPing(const QString& argument)
    //IrcCommand* cmd = IrcCommand::createPing(argument);
    //session->sendCommand(cmd);
    session->sendData("PING " + argument.toUtf8());
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

void IrcLagTimerPrivate::updateLag(qint64 value)
{
    Q_Q(IrcLagTimer);
    if (lag != value) {
        lag = qMax(-1ll, value);
        emit q->lagChanged(lag);
    }
}

/*!
    Constructs a new lag timer for \a session.
 */
IrcLagTimer::IrcLagTimer(IrcSession* session) : QObject(session), d_ptr(new IrcLagTimerPrivate(session, this))
{
    setInterval(DEFAULT_INTERVAL);
}

/*!
    Destructs the lag timer.
 */
IrcLagTimer::~IrcLagTimer()
{
}

/*!
    This property holds the associated session.

    \par Access functions:
    \li IrcSession* <b>session</b>() const
 */
IrcSession* IrcLagTimer::session() const
{
    Q_D(const IrcLagTimer);
    return d->session;
}

/*!
    This property holds the current lag in milliseconds.

    The value is \c -1 when
    \li the session is not connected,
    \li the lag has not yet been measured,
    \li the lag timer is disabled (interval <= 0s), or
    \li the Qt version is too old (4.7.0 or later is required).

    \par Access functions:
    \li qint64 <b>lag</b>() const
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
#if QT_VERSION >= 0x040700
        if (seconds > 0) {
            d->timer.setInterval(seconds * 1000);
            if (!d->timer.isActive() && d->session->isConnected())
                d->timer.start();
        } else {
            if (d->timer.isActive())
                d->timer.stop();
            d->updateLag(-1);
        }
#endif // QT_VERSION
    }
}

#include "moc_irclagtimer.cpp"
