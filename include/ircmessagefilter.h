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

#ifndef IRCMESSAGEFILTER_H
#define IRCMESSAGEFILTER_H

#include <ircglobal.h>
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qshareddata.h>

class IrcMessageFilterPrivate;

class COMMUNI_EXPORT IrcMessageFilter
{
public:
    enum FilterType
    {
        DefaultFilter,
        IntersectionFilter,
        UnionFilter,
        TypeFilter,
        PropertyFilter
    };

    IrcMessageFilter();
    IrcMessageFilter(const IrcMessageFilter& other);
    IrcMessageFilter& operator=(const IrcMessageFilter& other);
    virtual ~IrcMessageFilter();

    FilterType filterType() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool operator==(const IrcMessageFilter& other) const;
    bool operator!=(const IrcMessageFilter& other) const;

protected:
    QSharedDataPointer<IrcMessageFilterPrivate> d_ptr;
};

const COMMUNI_EXPORT IrcMessageFilter operator&(const IrcMessageFilter& left, const IrcMessageFilter& right);
const COMMUNI_EXPORT IrcMessageFilter operator|(const IrcMessageFilter& left, const IrcMessageFilter& right);

class COMMUNI_EXPORT IrcMessageIntersectionFilter : public IrcMessageFilter
{
public:
    IrcMessageIntersectionFilter();
    IrcMessageIntersectionFilter(const IrcMessageFilter& other);

    QList<IrcMessageFilter> filters() const;
    void setFilters(const QList<IrcMessageFilter>& filters);

    void append(const IrcMessageFilter& filter);
    void prepend(const IrcMessageFilter& filter);
    void remove(const IrcMessageFilter& filter);
    void clear();

    IrcMessageIntersectionFilter& operator<<(const IrcMessageFilter& filter);
};

class COMMUNI_EXPORT IrcMessageUnionFilter : public IrcMessageFilter
{
public:
    IrcMessageUnionFilter();
    IrcMessageUnionFilter(const IrcMessageFilter& other);

    QList<IrcMessageFilter> filters() const;
    void setFilters(const QList<IrcMessageFilter>& filters);

    void append(const IrcMessageFilter& filter);
    void prepend(const IrcMessageFilter& filter);
    void remove(const IrcMessageFilter& filter);
    void clear();

    IrcMessageUnionFilter& operator<<(const IrcMessageFilter& filter);
};

class COMMUNI_EXPORT IrcMessageTypeFilter : public IrcMessageFilter
{
public:
    IrcMessageTypeFilter(uint messageTypes = 0xffffffff);
    IrcMessageTypeFilter(const IrcMessageFilter& other);

    uint messageTypes() const;
    void setMessageTypes(uint messageTypes);
};

class COMMUNI_EXPORT IrcMessagePropertyFilter : public IrcMessageFilter
{
public:
    IrcMessagePropertyFilter(const QString& property = QString());
    IrcMessagePropertyFilter(const IrcMessageFilter& other);

    QString property() const;
    void setProperty(const QString& property);

    QString term() const;
    void setTerm(const QString& term);

    Qt::MatchFlags matchFlags() const;
    void setMatchFlags(Qt::MatchFlags flags);
};

#endif // IRCMESSAGEFILTER_H
