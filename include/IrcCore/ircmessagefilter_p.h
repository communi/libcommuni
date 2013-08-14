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

#ifndef IRCMESSAGEFILTER_P_H
#define IRCMESSAGEFILTER_P_H

#include "ircmessagefilter.h"

#include <QtCore/qlist.h>
#include <QtCore/qpointer.h>

IRC_BEGIN_NAMESPACE

class IrcConnection;

class IrcMessageFilterPrivate
{
public:
    static inline IrcMessageFilterPrivate* get(const IrcMessageFilter* filter)
    {
        return filter->d_ptr.data();
    }

    QList<QPointer<IrcConnection> > connections;
};

IRC_END_NAMESPACE

#endif // IRCMESSAGEFILTER_P_H
