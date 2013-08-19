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

#ifndef IRCUSERMODEL_P_H
#define IRCUSERMODEL_P_H

#include "ircuser.h"
#include "ircchannel_p.h"
#include "ircusermodel.h"
#include <qpointer.h>

IRC_BEGIN_NAMESPACE

class IrcUserModelPrivate
{
    Q_DECLARE_PUBLIC(IrcUserModel)

public:
    IrcUserModelPrivate(IrcUserModel* q);

    void addUser(IrcUser* user);
    void removeUser(IrcUser* user);
    void setUsers(const QList<IrcUser*>& users);

    static IrcUserModelPrivate* get(IrcUserModel* model)
    {
        return model->d_func();
    }

    IrcUserModel* q_ptr;
    Irc::ItemDataRole role;
    QPointer<IrcChannel> channel;
    QList<IrcUser*> userList;
    Qt::SortOrder sortOrder;
    bool dynamicSort;
};

IRC_END_NAMESPACE

#endif // IRCUSERMODEL_P_H
