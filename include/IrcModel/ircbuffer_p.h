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

#ifndef IRCBUFFER_P_H
#define IRCBUFFER_P_H

#include "ircbuffer.h"
#include "ircmessage.h"
#include "ircsessioninfo.h"
#include <qstringlist.h>
#include <qlist.h>
#include <qmap.h>

class IrcUser;
class IrcUserModel;

class IrcBufferPrivate
{
    Q_DECLARE_PUBLIC(IrcBuffer)

public:
    IrcBufferPrivate();
    virtual ~IrcBufferPrivate();

    virtual void init(const QString& title, IrcBufferModel* model);

    void setName(const QString& name);
    void setPrefix(const QString& prefix);

    bool processMessage(IrcMessage* message);

    virtual bool processJoinMessage(IrcJoinMessage* message);
    virtual bool processKickMessage(IrcKickMessage* message);
    virtual bool processModeMessage(IrcModeMessage* message);
    virtual bool processNamesMessage(IrcNamesMessage* message);
    virtual bool processNickMessage(IrcNickMessage* message);
    virtual bool processPartMessage(IrcPartMessage* message);
    virtual bool processQuitMessage(IrcQuitMessage* message);
    virtual bool processTopicMessage(IrcTopicMessage* message);

    void _irc_emitActiveChanged()
    {
        Q_Q(IrcBuffer);
        emit q->activeChanged(q->isActive());
    }

    static IrcBufferPrivate* get(IrcBuffer* buffer)
    {
        return buffer->d_func();
    }

    IrcBuffer* q_ptr;
    IrcBufferModel* model;
    QString name;
    QString prefix;
};

#endif // IRCBUFFER_P_H
