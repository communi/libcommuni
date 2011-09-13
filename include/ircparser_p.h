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

#ifndef IRCPARSER_P_H
#define IRCPARSER_P_H

#include <QtCore/qstring.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstringlist.h>

class IrcParser
{
    struct Data
    {
        QString prefix;
        QString command;
        QStringList params;
    };

public:
    IrcParser();

    bool parse(const QString &line);

    QString prefix() const { return d.prefix; }
    QString command() const { return d.command; }
    QStringList params() const { return d.params; }

private:
    Data d;
};

#endif // IRCPARSER_P_H
