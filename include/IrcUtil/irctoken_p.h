/*
* Copyright (C) 2008-2014 The Communi Project
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

#ifndef IRCTOKEN_P_H
#define IRCTOKEN_P_H

#include "ircglobal.h"
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>

IRC_BEGIN_NAMESPACE

class IrcToken
{
public:
    IrcToken() : idx(-1), pos(-1) { }
    IrcToken(int index, int position, const QString& text)
        : idx(index), pos(position), str(text) { }

    bool isValid() const { return idx != -1; }
    int index() const { return idx; }
    int position() const { return pos; }
    int length() const { return str.length(); }
    QString text() const { return str; }

private:
    int idx;
    int pos;
    QString str;
    friend class IrcTokenizer;
};

class IrcTokenizer
{
public:
    IrcTokenizer(const QString& str = QString());

    int count() const;
    bool isEmpty() const;
    QList<IrcToken> tokens() const;
    IrcToken at(int index) const;
    IrcTokenizer mid(int index) const;

    void clear();
    IrcToken takeFirst();
    void replace(int index, const QString& text);
    IrcToken find(int pos) const;
    QString toString() const;

private:
    int len;
    QList<IrcToken> t;
};

IRC_END_NAMESPACE

#endif // IRCTOKEN_P_H
