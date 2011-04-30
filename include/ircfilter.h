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

#ifndef IRCFILTER_H
#define IRCFILTER_H

#include <ircglobal.h>
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qshareddata.h>

class IrcFilterPrivate;

class COMMUNI_EXPORT IrcFilter
{
public:
    enum FilterType
    {
        InvalidFilter,
        PropertyFilter,
        IntersectionFilter,
        UnionFilter,
        DefaultFilter
    };

    IrcFilter(FilterType type = DefaultFilter);
    virtual ~IrcFilter();
    IrcFilter(const IrcFilter& other);
    IrcFilter& operator=(const IrcFilter& other);

    FilterType type() const;
    void setType(FilterType type);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QList<IrcFilter> filters() const;
    void setFilters(const QList<IrcFilter>& filters);

    void append(const IrcFilter& filter);
    void prepend(const IrcFilter& filter);
    void remove(const IrcFilter& filter);
    void clear();

    IrcFilter& operator<<(const IrcFilter& filter);

    bool operator==(const IrcFilter& other) const;
    bool operator!=(const IrcFilter& other) const;

protected:
    QSharedDataPointer<IrcFilterPrivate> d_ptr;
};

const COMMUNI_EXPORT IrcFilter operator&(const IrcFilter& left, const IrcFilter& right);
const COMMUNI_EXPORT IrcFilter operator|(const IrcFilter& left, const IrcFilter& right);

class COMMUNI_EXPORT IrcPropertyFilter : public IrcFilter
{
public:
    IrcPropertyFilter(const QString& property = QString());
    IrcPropertyFilter(const IrcFilter& other);

    QString property() const;
    void setProperty(const QString& property);

    QString term() const;
    void setTerm(const QString& term);

    Qt::MatchFlags matchFlags() const;
    void setMatchFlags(Qt::MatchFlags flags);
};

#endif // IRCFILTER_H
