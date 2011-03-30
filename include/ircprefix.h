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

#ifndef IRCPREFIX_H
#define IRCPREFIX_H

#include <ircglobal.h>
#include <QString>

class IRC_EXPORT IrcPrefix
{
public:
    IrcPrefix(const QString& prefix = QString());

    QString prefix() const;
    void setPrefix(const QString& prefix);

    QString nick() const { return n; }
    void setNick(const QString& nick) { n = nick; }

    QString user() const { return u; }
    void setUser(const QString& user) { u = user; }

    QString host() const { return h; }
    void setHost(const QString& host) { h = host; }

private:
    QString n;
    QString u;
    QString h;
};

#endif // IRCPREFIX_H
