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

#ifndef IRCFILTER_H
#define IRCFILTER_H

#include <IrcGlobal>
#include <QtCore/qobject.h>

IRC_BEGIN_NAMESPACE

class IrcMessage;
class IrcCommand;

class IRC_CORE_EXPORT IrcMessageFilter
{
public:
    virtual ~IrcMessageFilter() { }
    virtual bool messageFilter(IrcMessage* message) = 0;
};

class IRC_CORE_EXPORT IrcCommandFilter
{
public:
    virtual ~IrcCommandFilter() { }
    virtual bool commandFilter(IrcCommand* command) = 0;
};

IRC_END_NAMESPACE

// TODO: fixme
#ifdef IRC_NAMESPACE
using IRC_NAMESPACE::IrcMessageFilter;
using IRC_NAMESPACE::IrcCommandFilter;
#endif

Q_DECLARE_INTERFACE(IrcMessageFilter, "Communi.IrcMessageFilter")
Q_DECLARE_INTERFACE(IrcCommandFilter, "Communi.IrcCommandFilter")

#endif // IRCFILTER_H
