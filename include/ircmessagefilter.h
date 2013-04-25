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

#ifndef IRCMESSAGEFILTER_H
#define IRCMESSAGEFILTER_H

#include <IrcGlobal>
#include <QtCore/qscopedpointer.h>

class IrcMessage;
class IrcMessageFilterPrivate;

class COMMUNI_EXPORT IrcMessageFilter
{
public:
    IrcMessageFilter();
    virtual ~IrcMessageFilter();

    virtual bool messageFilter(IrcMessage* message) = 0;

protected:
    QScopedPointer<IrcMessageFilterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcMessageFilter)
    Q_DISABLE_COPY(IrcMessageFilter)
};

#endif // IRCMESSAGEFILTER_H
