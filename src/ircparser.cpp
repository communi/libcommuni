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

#include "ircparser_p.h"

IrcParser::IrcParser()
{
}

bool IrcParser::parse(const QString& line)
{
    d.prefix.clear();
    d.command.clear();
    d.params.clear();

    // From RFC 1459:
    //  <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
    //  <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
    //  <command>  ::= <letter> { <letter> } | <number> <number> <number>
    //  <SPACE>    ::= ' ' { ' ' }
    //  <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
    //  <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
    //                 or NUL or CR or LF, the first of which may not be ':'>
    //  <trailing> ::= <Any, possibly *empty*, sequence of octets not including
    //                   NUL or CR or LF>
    QString process = line;

    // parse <prefix>
    if (process.startsWith(QLatin1Char(':')))
    {
        d.prefix = process.mid(1, process.indexOf(QLatin1Char(' ')) - 1);
        process.remove(0, d.prefix.length() + 2);
    }

    // parse <command>
    d.command = process.mid(0, process.indexOf(QLatin1Char(' ')));
    process.remove(0, d.command.length() + 1);

    // parse <params>
    while (!process.isEmpty())
    {
        if (process.startsWith(QLatin1Char(':')))
        {
            process.remove(0, 1);
            d.params += process;
            process.clear();
        }
        else
        {
            QString param = process.mid(0, process.indexOf(QLatin1Char(' ')));
            process.remove(0, param.length() + 1);
            d.params += param;
        }
    }

    return !d.command.isEmpty() && process.trimmed().isEmpty();
}
