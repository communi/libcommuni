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

#ifndef IRCLAGTIMER_P_H
#define IRCLAGTIMER_P_H

#include "irclagtimer.h"
#include "ircfilter.h"
#include <QTimer>

IRC_BEGIN_NAMESPACE

class IrcPongMessage;

class IrcLagTimerPrivate : public QObject,  public IrcMessageFilter
{
    Q_OBJECT
    Q_INTERFACES(IrcMessageFilter)
    Q_DECLARE_PUBLIC(IrcLagTimer)

public:
    IrcLagTimerPrivate();

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

IRC_END_NAMESPACE

#endif // IRCLAGTIMER_P_H
