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
        StringListFilter,
        IntersectionFilter,
        UnionFilter,
        DefaultFilter
    };

    IrcFilter(FilterType type = DefaultFilter);
    ~IrcFilter();
    IrcFilter(const IrcFilter& other);
    IrcFilter& operator=(const IrcFilter& other);

    FilterType type() const;

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

    QString property() const;
    void setProperty(const QString& property);

    QString term() const;
    void setTerm(const QString& term);

    Qt::MatchFlags matchFlags() const;
    void setMatchFlags(Qt::MatchFlags flags);
};

class COMMUNI_EXPORT IrcStringListFilter : public IrcFilter
{
public:
    IrcStringListFilter(const QStringList& terms = QStringList());

    QStringList terms() const;
    void setTerms(const QStringList& terms);

    Qt::CaseSensitivity caseSensitivity() const;
    void	 setCaseSensitivity(Qt::CaseSensitivity cs);
};

#endif // IRCFILTER_H
