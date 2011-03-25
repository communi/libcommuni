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

#ifndef IRC_PARSER_P_H
#define IRC_PARSER_P_H

#include <QString>
#include <QByteArray>
#include <QStringList>

namespace Irc
{
    class Parser
    {
        struct ParserData
        {
            QString prefix;
            QString command;
            QStringList params;
            QByteArray encoding;
        };

    public:
        Parser();

        bool parse(const QByteArray &line);

        QString prefix() const { return d.prefix; }
        QString command() const { return d.command; }
        QStringList params() const { return d.params; }

        QByteArray encoding() const { return d.encoding; }
        void setEncoding(const QByteArray& encoding) { d.encoding = encoding; }

    private:
        QString encode(const QByteArray& data) const;

        ParserData d;
    };
}

#endif // IRC_PARSER_P_H
