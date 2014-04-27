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

IrcTokenizer::IrcTokenizer(const QString& str) : len(str.length()), t(tokenize(str))
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
    IrcTokenizer tt;
    tt.t = t.mid(index);
    if (!tt.isEmpty()) {
        int d = tt.t.first().position();
        tt.len = len - d;
        for (int i = 0; i < tt.t.length(); ++i) {
            tt.t[i].idx = i;
            tt.t[i].pos -= d;
        }
    }
    return tt;
}

void IrcTokenizer::clear()
{
    t.clear();
}

void IrcTokenizer::replace(int index, const QString& text)
{
    IrcToken token = t.value(index);
    if (token.isValid()) {
        int d = text.length() - token.length();
        token = IrcToken(index, token.position(), text);
        t.replace(index, token);
        len += d;
        for (int i = index + 1; i < t.length(); ++i)
            t[i].pos += d;
    }
}

IrcToken IrcTokenizer::find(int pos) const
{
    IrcToken token;
    foreach (const IrcToken& tk, t) {
        if (tk.position() > pos)
            break;
        token = tk;
    }
    return token;
}

QString IrcTokenizer::toString() const
{
    QString str(len, QLatin1Char(' '));
    foreach (const IrcToken& token, t)
        str.replace(token.position(), token.length(), token.text());
    return str;
}

IRC_END_NAMESPACE
