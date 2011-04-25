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

bool IrcPrefix::isValid() const
{
    return !n.isEmpty() && !u.isEmpty() && !h.isEmpty();
}

QString IrcPrefix::prefix() const
{
    return isValid() ? QString("%1!%2@%3").arg(n, u, h) : QString();
}

void IrcPrefix::setPrefix(const QString& prefix)
{
    QRegExp rx("([^!\\s]+)!([^@\\s]+)@(\\S+)");
    bool match = rx.exactMatch(prefix.trimmed());
    n = match ? rx.cap(1) : QString();
    u = match ? rx.cap(2) : QString();
    h = match ? rx.cap(3) : QString();
}
