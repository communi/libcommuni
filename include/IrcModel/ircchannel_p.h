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

#ifndef IRCCHANNEL_P_H
#define IRCCHANNEL_P_H

#include "ircchannel.h"
#include "ircnetwork.h"
#include "ircbuffer_p.h"
#include <qstringlist.h>
#include <qlist.h>
#include <qmap.h>

class IrcChannelPrivate : public IrcBufferPrivate
{
    Q_DECLARE_PUBLIC(IrcChannel)

public:
    IrcChannelPrivate();
    virtual ~IrcChannelPrivate();

    virtual void init(const QString& title, IrcBufferModel* model);

    void changeMode(const QString& value);
    void setMode(const QString& value);
    void setTopic(const QString& value);

    void addUsers(const QStringList& users);
    bool removeUser(const QString& user);
    bool renameUser(const QString& from, const QString& to);
    void setUserMode(const QString& user, const QString& mode);
    void clearUsers();

    virtual bool processJoinMessage(IrcJoinMessage* message);
    virtual bool processKickMessage(IrcKickMessage* message);
    virtual bool processModeMessage(IrcModeMessage* message);
    virtual bool processNamesMessage(IrcNamesMessage* message);
    virtual bool processNickMessage(IrcNickMessage* message);
    virtual bool processPartMessage(IrcPartMessage* message);
    virtual bool processQuitMessage(IrcQuitMessage* message);
    virtual bool processTopicMessage(IrcTopicMessage* message);

    static IrcChannelPrivate* get(IrcChannel* channel)
    {
        return channel->d_func();
    }

    QString mode;
    QString topic;
    int joined, left;
    QList<IrcUser*> userList;
    QMap<QString, IrcUser*> userMap;
    QList<IrcUserModel*> userModels;
};

#endif // IRCCHANNEL_P_H
