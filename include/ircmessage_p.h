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

#ifndef IRCMESSAGE_P_H
#define IRCMESSAGE_P_H

#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstringlist.h>

#include "ircsender.h"
#include "ircmessage.h"

class IrcSession;

class IrcMessageData
{
public:
    IrcMessageData() : valid(false) { }

    static IrcMessageData fromData(const QByteArray& data);

    bool valid;
    QByteArray data;
    QByteArray prefix;
    QByteArray command;
    QList<QByteArray> params;
};

class IrcMessageContent
{
public:
    IrcMessageContent() : dirty(true) { }

    static IrcMessageContent fromData(const IrcMessageData& data, const QByteArray& encoding);

    bool dirty;
    IrcSender sender;
    QString command;
    QStringList params;
};

class IrcMessagePrivate
{
public:
    IrcMessagePrivate();

    IrcSender sender() const;
    QString command() const;
    QStringList params() const;
    QString param(int index) const;

    IrcSession* session;
    IrcMessage::Type type;
    QDateTime timeStamp;
    QByteArray encoding;
    IrcMessageData message;
    mutable IrcMessageContent content;
    mutable int flags;
};

#endif // IRCMESSAGE_P_H
