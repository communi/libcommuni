/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#include "irchandler.h"

class IrcHandlerPrivate
{
public:
    IrcHandlerPrivate(IrcHandler* handler);

    IrcHandler* _public;
};

IrcHandlerPrivate::IrcHandlerPrivate(IrcHandler* handler)
    : _public(handler)
{
}

IrcHandler::IrcHandler(QObject* parent)
    : QObject(parent), d(new IrcHandlerPrivate(this))
{
}

IrcHandler::~IrcHandler()
{
    delete d;
}
