/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#ifndef IRCRECEIVER_H
#define IRCRECEIVER_H

#include <ircglobal.h>
#include <ircmessage.h>

class IrcMessageFilter;
class IrcSession;
class IrcReceiverPrivate;

class COMMUNI_EXPORT IrcReceiver
{
public:
    IrcReceiver(IrcSession* session = 0);
    virtual ~IrcReceiver();

    IrcSession* session() const;
    void setSession(IrcSession* session);

    virtual void receiveMessage(IrcMessage* message);

    IrcMessageFilter messageFilter() const;
    void setMessageFilter(const IrcMessageFilter& filter);

protected:
    virtual void inviteMessage(IrcInviteMessage*) { }
    virtual void joinMessage(IrcJoinMessage*) { }
    virtual void kickMessage(IrcKickMessage*) { }
    virtual void modeMessage(IrcModeMessage*) { }
    virtual void nickNameMessage(IrcNickMessage*) { }
    virtual void noticeMessage(IrcNoticeMessage*) { }
    virtual void numericMessage(IrcNumericMessage*) { }
    virtual void partMessage(IrcPartMessage*) { }
    virtual void privateMessage(IrcPrivateMessage*) { }
    virtual void quitMessage(IrcQuitMessage*) { }
    virtual void topicMessage(IrcTopicMessage*) { }
    virtual void unknownMessage(IrcMessage*) { }

private:
    QScopedPointer<IrcReceiverPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcReceiver)
    Q_DISABLE_COPY(IrcReceiver)
};

#endif // IRCRECEIVER_H
