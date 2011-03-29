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

class IRC_EXPORT IrcReceiver
{
public:
    virtual ~IrcReceiver() { }
    virtual void receiveMessage(const IrcMessage* message);

    // TODO: filters

protected:
    virtual void inviteMessage(const IrcInviteMessage*) { }
    virtual void joinMessage(const IrcJoinMessage*) { }
    virtual void kickMessage(const IrcKickMessage*) { }
    virtual void modeMessage(const IrcModeMessage*) { }
    virtual void nickNameMessage(const IrcNickMessage*) { }
    virtual void noticeMessage(const IrcNoticeMessage*) { }
    virtual void numericMessage(const IrcNumericMessage*) { }
    virtual void partMessage(const IrcPartMessage*) { }
    virtual void privateMessage(const IrcPrivateMessage*) { }
    virtual void quitMessage(const IrcQuitMessage*) { }
    virtual void topicMessage(const IrcTopicMessage*) { }
    virtual void unknownMessage(const IrcMessage*) { }

private:
    // TODO: filters
};

#endif // IRCRECEIVER_H
