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

#include "ircmessagefilter.h"
#include "ircmessagefilter_p.h"
#include "ircsession.h"

/*!
    \file ircmessagefilter.h
    \brief #include &lt;IrcMessageFilter&gt;
 */

/*!
    \class IrcMessageFilter ircmessagefilter.h.h <IrcMessageFilter>
    \ingroup utility
    \brief The IrcMessageFilter class provides an interface for filtering messages

    IrcMessageFilter may be used to intercept messages before
    IrcSession::messageReceived() is emitted and the messages get delivered
    further. In order to use IrcMessageFilter, it must be installed via
    IrcSession::installMessageFilter().

    Message filtering is mostly useful for handling specific replies before
    the rest of the application receives it. This way there is no need to
    for example ignore and hide such replies later in the application code.

    \code
    class MyPongMessageFilter : public IrcMessageFilter
    {
    public:
        virtual bool messageFilter(IrcMessage* msg)
        {
            if (msg->type() == IrcMessage::Pong)
                return processPongReply(static_cast<IrcPongMessage*>(msg));
            return false;
        }

    protected:
        bool processPongReply(IrcPongMessage* msg)
        {
            // Process IrcMessage::argument() to recognize whether the PONG
            // message is a reply to a PING command we have sent earlier:
            //
            //     QString argument = QString("communi/%1").arg(QDateTime::currentMSecsSinceEpoch());
            //     IrcCommand* cmd = IrcCommand::createPing(argument);
            //     session->sendCommand(cmd);

            if (msg->argument().startsWith("communi/")) {
                bool ok = false;
                qint64 timestamp = msg->argument().mid(8).toLongLong(&ok);
                if (ok) {
                    qint64 lag = QDateTime::currentMSecsSinceEpoch() - timestamp;
                    // ...
                    return true;
                }
            }
            return false;
        }
    };
    \endcode

    \sa IrcSession::installMessageFilter(), IrcSession::removeMessageFilter()
 */

/*!
    Constructs a new message filter.
 */
IrcMessageFilter::IrcMessageFilter() : d_ptr(new IrcMessageFilterPrivate)
{
}

/*!
    Destructs the message filter.

    The message filter is automatically removed from any session(s)
    it is installed on.

    \sa IrcSession::removeMessageFilter()
 */
IrcMessageFilter::~IrcMessageFilter()
{
    Q_D(IrcMessageFilter);
    foreach (QPointer<IrcSession> session, d->sessions) {
        if (session)
            session->removeMessageFilter(this);
    }
}

/*!
    \fn virtual bool IrcMessageFilter::messageFilter(IrcMessage* message) = 0

    Reimplement this function to filter messages from installed sessions.

    Return \c true to filter the message out, i.e. stop it being handled further;
    otherwise return \c false.

    \sa IrcSession::installMessageFilter()
 */
