/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#include <QString>
#include <QStringList>

class IrcBuffer;

class IrcBufferPrivate
{
    Q_DECLARE_PUBLIC(IrcBuffer)

public:
    IrcBufferPrivate();

    IrcBuffer* q_ptr;
    QString pattern;
    QStringList names;
};

#endif // IRCBUFFER_P_H
