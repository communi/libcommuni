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

#include "ircprefix.h"
#include <QRegExp>

IrcPrefix::IrcPrefix(const QString& prefix)
{
    setPrefix(prefix);
}

QString IrcPrefix::prefix() const
{
    return QString("%1!%2@%3").arg(n, u, h);
}

void IrcPrefix::setPrefix(const QString& prefix)
{
    QRegExp rx("([^!]+)!([^@]+)@(.+)");
    if (rx.exactMatch(prefix))
    {
        n = rx.cap(1);
        u = rx.cap(2);
        h = rx.cap(3);
    }
    else
    {
        n = prefix;
        u.clear();
        h.clear();
    }
}
