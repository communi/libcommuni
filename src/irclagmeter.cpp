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

#include "irclagmeter.h"
#include "ircsession.h"
#include "ircmessage.h"
#include "irccommand.h"
#include "ircmessagefilter.h"
#include <QDateTime>
#include <QTimer>

static const int DEFAULT_INTERVAL = 60;

/*!
    \file irclagmeter.h
    \brief #include &lt;IrcLagMeter&gt;
 */

/*!
    \class IrcLagMeter irclagmeter.h IrcSession
    \ingroup utility
    \brief The IrcLagMeter class provides IRC session lag meter.
 */

/*!
    \fn void IrcLagMeter::lagChanged(qint64 lag)

    This signal is emitted when the \a lag has changed.
 */

class IrcLagMeterPrivate : public IrcMessageFilter
{
    Q_DECLARE_PUBLIC(IrcLagMeter)

public:
    IrcLagMeterPrivate(IrcSession* session, IrcLagMeter* meter);

    bool messageFilter(IrcMessage* msg);
    bool processPongReply(IrcPongMessage* msg);

    void _irc_connected();
    void _irc_pingServer();
    void _irc_disconnected();

    void updateLag(qint64 value);

    IrcLagMeter* q_ptr;
    IrcSession* session;
    QTimer timer;
    int interval;
    qint64 lag;
};

IrcLagMeterPrivate::IrcLagMeterPrivate(IrcSession* session, IrcLagMeter* meter)
    : q_ptr(meter), session(session), interval(-1), lag(-1)
{
    session->installMessageFilter(this);
    QObject::connect(&timer, SIGNAL(timeout()), meter, SLOT(_irc_pingServer()));
    QObject::connect(session, SIGNAL(connected()), meter, SLOT(_irc_connected()));
    QObject::connect(session, SIGNAL(disconnected()), meter, SLOT(_irc_disconnected()));
}

bool IrcLagMeterPrivate::messageFilter(IrcMessage* msg)
{
    if (msg->type() == IrcMessage::Pong)
        return processPongReply(static_cast<IrcPongMessage*>(msg));
    return false;
}

bool IrcLagMeterPrivate::processPongReply(IrcPongMessage* msg)
{
    // TODO: configurable format?
    if (msg->argument().startsWith("communi/")) {
        bool ok = false;
        qint64 timestamp = msg->argument().mid(8).toLongLong(&ok);
        if (ok) {
            updateLag(QDateTime::currentMSecsSinceEpoch() - timestamp);
            return true;
        }
    }
    return false;
}

void IrcLagMeterPrivate::_irc_connected()
{
    if (interval > 0)
        timer.start();
}

void IrcLagMeterPrivate::_irc_pingServer()
{
    // TODO: configurable format?
    QString argument = QString("communi/%1").arg(QDateTime::currentMSecsSinceEpoch());
    // TODO: add IrcCommand::createPing(const QString& argument)
    //IrcCommand* cmd = IrcCommand::createPing(argument);
    //session->sendCommand(cmd);
    session->sendData("PING " + argument.toUtf8());
}

void IrcLagMeterPrivate::_irc_disconnected()
{
    updateLag(-1);
    if (timer.isActive())
        timer.stop();
}

void IrcLagMeterPrivate::updateLag(qint64 value)
{
    Q_Q(IrcLagMeter);
    if (lag != value) {
        lag = qMax(-1ll, value);
        emit q->lagChanged(lag);
    }
}

/*!
    Constructs a new lag meter for \a session.
 */
IrcLagMeter::IrcLagMeter(IrcSession* session) : QObject(session), d_ptr(new IrcLagMeterPrivate(session, this))
{
    setInterval(DEFAULT_INTERVAL);
}

/*!
    Destructs the lag meter session.
 */
IrcLagMeter::~IrcLagMeter()
{
}

/*!
    This property holds the associated session.

    \par Access functions:
    \li IrcSession* <b>session</b>() const
 */
IrcSession* IrcLagMeter::session() const
{
    Q_D(const IrcLagMeter);
    return d->session;
}

/*!
    This property holds the current lag in milliseconds.

    The value is \c -1 when
    \li the session is not connected,
    \li the lag has not yet been measured, or
    \li the lag meter is disabled (interval <= 0s).

    \par Access functions:
    \li qint64 <b>lag</b>() const
 */
qint64 IrcLagMeter::lag() const
{
    Q_D(const IrcLagMeter);
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
int IrcLagMeter::interval() const
{
    Q_D(const IrcLagMeter);
    return d->interval;
}

void IrcLagMeter::setInterval(int seconds)
{
    Q_D(IrcLagMeter);
    if (d->interval != seconds) {
        d->interval = seconds;
        if (seconds > 0) {
            d->timer.setInterval(seconds * 1000);
            if (!d->timer.isActive() && d->session->isConnected())
                d->timer.start();
        } else {
            if (d->timer.isActive())
                d->timer.stop();
            d->updateLag(-1);
        }
    }
}

#include "moc_irclagmeter.cpp"
