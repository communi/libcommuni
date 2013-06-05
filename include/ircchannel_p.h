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
#include "ircmessage.h"
#include <qstringlist.h>
#include <qlist.h>
#include <qmap.h>

class IrcUser;
class IrcUserModel;

class IrcChannelPrivate
{
    Q_DECLARE_PUBLIC(IrcChannel)

public:
    void init(const QString& title, IrcSession* session);

    void changeMode(const QString& value);
    void setMode(const QString& value);
    void setTopic(const QString& value);

    void addUsers(const QStringList& users, IrcMessage* message = 0);
    bool removeUser(const QString& user, IrcMessage* message);
    bool renameUser(const QString& from, const QString& to, IrcMessage* message);
    void setUserMode(const QString& user, const QString& mode, IrcMessage* message);
    void clearUsers();

    bool processMessage(IrcMessage* message);

    bool processJoinMessage(IrcJoinMessage* message);
    bool processKickMessage(IrcKickMessage* message);
    bool processModeMessage(IrcModeMessage* message);
    bool processNamesMessage(IrcNamesMessage* message);
    bool processNickMessage(IrcNickMessage* message);
    bool processNoticeMessage(IrcNoticeMessage* message);
    bool processNumericMessage(IrcNumericMessage* message);
    bool processPartMessage(IrcPartMessage* message);
    bool processPrivateMessage(IrcPrivateMessage* message);
    bool processQuitMessage(IrcQuitMessage* message);
    bool processTopicMessage(IrcTopicMessage* message);

    static IrcChannelPrivate* get(IrcChannel* channel)
    {
        return channel->d_func();
    }

    IrcChannel* q_ptr;
    IrcSession* session;
    QString name;
    QString prefix;
    QString mode;
    QString topic;
    QList<IrcUser*> userList;
    QMap<QString, IrcUser*> userMap;
    QList<IrcUserModel*> models;
};

#endif // IRCCHANNEL_P_H
