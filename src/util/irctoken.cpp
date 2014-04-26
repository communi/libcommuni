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

#include "irctoken_p.h"
#include <QStringList>

IRC_BEGIN_NAMESPACE

static QList<IrcToken> tokenize(const QString& str)
{
    int idx = -1;
    int pos = 0;
    QList<IrcToken> tokens;
    foreach (const QString& txt, str.split(QLatin1String(" "))) {
        if (!txt.isEmpty())
            tokens += IrcToken(++idx, pos, txt);
        pos += txt.length() + 1;
    }
    return tokens;
}

IrcTokenizer::IrcTokenizer(const QString& str) : t(tokenize(str))
{
}

IrcTokenizer::IrcTokenizer(const QList<IrcToken>& tokens) : t(tokens)
{
}

int IrcTokenizer::count() const
{
    return t.count();
}

bool IrcTokenizer::isEmpty() const
{
    return t.isEmpty();
}

QList<IrcToken> IrcTokenizer::tokens() const
{
    return t;
}

IrcToken IrcTokenizer::at(int index) const
{
    return t.value(index);
}

IrcTokenizer IrcTokenizer::mid(int index) const
{
    IrcTokenizer tt = t.mid(index);
    if (!tt.isEmpty()) {
        int d = tt.t.first().position();
        for (int i = 0; i < tt.t.length(); ++i)
            tt.t[i].pos -= d;
    }
    return tt;
}

void IrcTokenizer::clear()
{
    t.clear();
}

IrcToken IrcTokenizer::takeFirst()
{
    IrcToken token = t.value(0);
    t = mid(1).tokens();
    return token;
}

void IrcTokenizer::replace(int index, const QString& text)
{
    IrcToken token = t.value(index);
    if (token.isValid()) {
        int d = text.length() - token.length();
        token = IrcToken(index, token.position(), text);
        t.replace(index, token);
        for (int i = index; i < t.length(); ++i)
            t[i].pos += d;
    }
}

IrcToken IrcTokenizer::find(int pos) const
{
    foreach (const IrcToken& token, t) {
        if (token.position() <= pos && pos < token.position() + token.length())
            return token;
    }
    return IrcToken();
}

QString IrcTokenizer::toString() const
{
    QString str;
    if (!t.isEmpty()) {
        str = QString(t.last().position() + t.last().length(), QLatin1Char(' '));
        foreach (const IrcToken& token, t)
            str.replace(token.position(), token.length(), token.text());
    }
    return str;
}

IRC_END_NAMESPACE
