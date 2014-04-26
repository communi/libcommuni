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

#ifndef IRCCOMMANDPARSER_P_H
#define IRCCOMMANDPARSER_P_H

#include "irccommandparser.h"
#include "irccommand.h"

#include <QList>
#include <QString>
#include <QMultiMap>
#include <QStringList>

IRC_BEGIN_NAMESPACE

struct IrcParameterInfo
{
    IrcParameterInfo() : optional(false), channel(false), current(false), multi(false) { }
    bool optional;
    bool channel;
    bool current;
    bool multi;
    QString value;
    QString syntax;
};

struct IrcCommandInfo
{
    IrcCommandInfo() : type(IrcCommand::Custom), min(0), max(0) { }

    QString fullSyntax()
    {
        return command + QLatin1Char(' ') + syntax;
    }

    IrcCommand::Type type;
    QString command;
    QString syntax;
    int min, max;
    QList<IrcParameterInfo> params;
};

class IrcCommandParserPrivate
{
public:
    IrcCommandParserPrivate();

    QList<IrcCommandInfo> find(const QString& command) const;
    static IrcCommandInfo parseSyntax(IrcCommand::Type type, const QString& syntax);
    IrcCommand* parseCommand(const IrcCommandInfo& command, const QString& input) const;
    bool processParameters(const IrcCommandInfo& command, const QString& input, QStringList* params) const;
    bool processCommand(QString* input, int* removed = 0) const;
    bool processMessage(QString* input, int* removed = 0) const;
    bool onChannel() const;

    static IrcCommandParserPrivate* get(IrcCommandParser* parser)
    {
        return parser->d_func();
    }

    bool tolerant;
    QString target;
    QStringList triggers;
    QStringList channels;
    QMultiMap<QString, IrcCommandInfo> commands;
};

IRC_END_NAMESPACE

#endif // IRCCOMMANDPARSER_P_H
