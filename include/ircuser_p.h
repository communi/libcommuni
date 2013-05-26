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

class IrcUserPrivate
{
    Q_DECLARE_PUBLIC(IrcUser)

public:
    void setName(const QString& n) {
        Q_Q(IrcUser);
        if (name != n) {
            name = n;
            emit q->nameChanged(name);
        }
    }

    void setPrefix(const QString& p) {
        Q_Q(IrcUser);
        if (prefix != p) {
            prefix = p;
            emit q->prefixChanged(prefix);
        }
    }

    void setMode(const QString& m) {
        Q_Q(IrcUser);
        if (mode != m) {
            mode = m;
            emit q->modeChanged(mode);
        }
    }

    IrcUser* q_ptr;
    QString name;
    QString prefix;
    QString mode;
};

#endif // IRCUSER_P_H
