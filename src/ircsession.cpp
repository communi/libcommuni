/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#include "ircsession.h"
#include <QThread>

/*!
    \class IrcSession ircsession.h
    \ingroup LibIrcClient-Qt
    \brief A threaded IRC session.

    The IrcSession class provides a threaded IRC session.
 */

class IrcSessionPrivate
{
public:
    QThread thread;
};

/*!
    Constructs a new IrcSession.
 */
IrcSession::IrcSession()
    : CoreIrcSession(0), d(new IrcSessionPrivate)
{
    moveToThread(&d->thread);
    d->thread.start();
}

/*!
    Destructs the IRC session.

    This function
    \li quits the session by calling CoreIrcSession::cmdQuit()
    \li quits the thread
    \li waits for the thread to quit
 */
IrcSession::~IrcSession()
{
    cmdQuit();
    d->thread.quit();
    d->thread.wait();
    delete d;
}

/*!
    Starts a threaded IRC session.

    \sa CoreIrcSession::exec()
 */
bool IrcSession::start()
{
    return QMetaObject::invokeMethod(this, "exec");
}
