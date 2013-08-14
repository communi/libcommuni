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

#ifndef IRCUSER_P_H
#define IRCUSER_P_H

#include "ircuser.h"

IRC_BEGIN_NAMESPACE

class IrcUserPrivate
{
    Q_DECLARE_PUBLIC(IrcUser)

public:
    void setName(const QString& n);
    void setPrefix(const QString& p);
    void setMode(const QString& m);

    static IrcUserPrivate* get(IrcUser* user)
    {
        return user->d_func();
    }

    IrcUser* q_ptr;
    IrcChannel* channel;
    QString name;
    QString prefix;
    QString mode;
};

IRC_END_NAMESPACE

#endif // IRCUSER_P_H
