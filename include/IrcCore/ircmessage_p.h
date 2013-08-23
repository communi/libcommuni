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

#include "ircmessage.h"

IRC_BEGIN_NAMESPACE

class IrcConnection;

template <class T>
class IrcExplicitValue
{
public:
    IrcExplicitValue() : exp(false), null(true) { }
    IrcExplicitValue(const T& value) : v(value), exp(false), null(false) { }

    bool isNull() const { return null; }
    bool isExplicit() const { return exp; }

    const T& value() const { return v; }
    void setValue(const T& value) { v = value; exp = true; null = false; }

    void clear() { v = T(); exp = false; null = true; }

private:
    T v;
    bool exp;
    bool null;
};

class IrcMessageData
{
public:
    static IrcMessageData fromData(const QByteArray& data);

    QByteArray content;
    QByteArray prefix;
    QByteArray command;
    QList<QByteArray> params;
};

class IrcMessagePrivate
{
public:
    IrcMessagePrivate();

    QString prefix() const;
    void setPrefix(const QString& prefix);

    QString nick() const;
    QString ident() const;
    QString host() const;

    QString command() const;
    void setCommand(const QString& command);

    QStringList params() const;
    QString param(int index) const;
    void setParams(const QStringList& params);

    void invalidate();

    static QString decode(const QByteArray& data, const QByteArray& encoding);
    static bool parsePrefix(const QString& prefix, QString* nick, QString* ident, QString* host);

    IrcConnection* connection;
    IrcMessage::Type type;
    QDateTime timeStamp;
    QByteArray encoding;
    mutable int flags;
    IrcMessageData data;

private:
    mutable QString m_nick, m_ident, m_host;
    mutable IrcExplicitValue<QString> m_prefix;
    mutable IrcExplicitValue<QString> m_command;
    mutable IrcExplicitValue<QStringList> m_params;
};

IRC_END_NAMESPACE

#endif // IRCMESSAGE_P_H
