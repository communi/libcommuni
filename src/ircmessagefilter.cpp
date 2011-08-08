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

#include "ircmessagefilter.h"

class IrcMessageFilterPrivate : public QSharedData
{
public:
    IrcMessageFilterPrivate();
    ~IrcMessageFilterPrivate();

    IrcMessageFilter::FilterType filterType;
    bool enabled;
    QList<IrcMessageFilter> filters;
    uint messageTypes;
    QString property;
    QString term;
    Qt::MatchFlags matchFlags;
    QStringList terms;
};

IrcMessageFilterPrivate::IrcMessageFilterPrivate() :
    filterType(IrcMessageFilter::DefaultFilter),
    enabled(true),
    messageTypes(0xffffffff),
    matchFlags(Qt::MatchExactly)
{
}

IrcMessageFilterPrivate::~IrcMessageFilterPrivate()
{
}

IrcMessageFilter::IrcMessageFilter()
    : d_ptr(new IrcMessageFilterPrivate)
{
}

IrcMessageFilter::IrcMessageFilter(const IrcMessageFilter& other)
    : d_ptr(other.d_ptr)
{
}

IrcMessageFilter& IrcMessageFilter::operator=(const IrcMessageFilter& other)
{
    if (this != &other)
        d_ptr = other.d_ptr;
    return *this;
}

IrcMessageFilter::~IrcMessageFilter()
{
}

IrcMessageFilter::FilterType IrcMessageFilter::filterType() const
{
    return d_ptr->filterType;
}

bool IrcMessageFilter::isEnabled() const
{
    return d_ptr->enabled;
}

void IrcMessageFilter::setEnabled(bool enabled)
{
    d_ptr->enabled = enabled;
}

bool IrcMessageFilter::operator==(const IrcMessageFilter& other) const
{
    return d_ptr->filterType == other.d_ptr->filterType
            && d_ptr->enabled == other.d_ptr->enabled
            && d_ptr->filters == other.d_ptr->filters
            && d_ptr->messageTypes == other.d_ptr->messageTypes
            && d_ptr->property == other.d_ptr->property
            && d_ptr->term == other.d_ptr->term
            && d_ptr->matchFlags == other.d_ptr->matchFlags
            && d_ptr->terms == other.d_ptr->terms;
}

bool IrcMessageFilter::operator!=(const IrcMessageFilter& other) const
{
    return !operator==(other);
}

const IrcMessageFilter operator&(const IrcMessageFilter& left, const IrcMessageFilter& right)
{
    IrcMessageIntersectionFilter filter;
    filter << left << right;
    return filter;
}

const IrcMessageFilter operator|(const IrcMessageFilter& left, const IrcMessageFilter& right)
{
    IrcMessageUnionFilter filter;
    filter << left << right;
    return filter;
}

IrcMessageIntersectionFilter::IrcMessageIntersectionFilter()
{
    d_ptr->filterType = IntersectionFilter;
}

IrcMessageIntersectionFilter::IrcMessageIntersectionFilter(const IrcMessageFilter& other)
    : IrcMessageFilter(other)
{
}

QList<IrcMessageFilter> IrcMessageIntersectionFilter::filters() const
{
    return d_ptr->filters;
}

void IrcMessageIntersectionFilter::setFilters(const QList<IrcMessageFilter>& filters)
{
    d_ptr->filters = filters;
}

void IrcMessageIntersectionFilter::append(const IrcMessageFilter& filter)
{
    d_ptr->filters.append(filter);
}

void IrcMessageIntersectionFilter::prepend(const IrcMessageFilter& filter)
{
    d_ptr->filters.prepend(filter);
}

void IrcMessageIntersectionFilter::remove(const IrcMessageFilter& filter)
{
    d_ptr->filters.removeAll(filter);
}

void IrcMessageIntersectionFilter::clear()
{
    d_ptr->filters.clear();
}

IrcMessageIntersectionFilter& IrcMessageIntersectionFilter::operator<<(const IrcMessageFilter& filter)
{
    d_ptr->filters << filter;
    return *this;
}

IrcMessageUnionFilter::IrcMessageUnionFilter()
{
    d_ptr->filterType = UnionFilter;
}

IrcMessageUnionFilter::IrcMessageUnionFilter(const IrcMessageFilter& other)
    : IrcMessageFilter(other)
{
}

QList<IrcMessageFilter> IrcMessageUnionFilter::filters() const
{
    return d_ptr->filters;
}

void IrcMessageUnionFilter::setFilters(const QList<IrcMessageFilter>& filters)
{
    d_ptr->filters = filters;
}

void IrcMessageUnionFilter::append(const IrcMessageFilter& filter)
{
    d_ptr->filters.append(filter);
}

void IrcMessageUnionFilter::prepend(const IrcMessageFilter& filter)
{
    d_ptr->filters.prepend(filter);
}

void IrcMessageUnionFilter::remove(const IrcMessageFilter& filter)
{
    d_ptr->filters.removeAll(filter);
}

void IrcMessageUnionFilter::clear()
{
    d_ptr->filters.clear();
}

IrcMessageUnionFilter& IrcMessageUnionFilter::operator<<(const IrcMessageFilter& filter)
{
    d_ptr->filters << filter;
    return *this;
}

IrcMessageTypeFilter::IrcMessageTypeFilter(uint messageTypes)
{
    d_ptr->filterType = TypeFilter;
    d_ptr->messageTypes = messageTypes;
}

IrcMessageTypeFilter::IrcMessageTypeFilter(const IrcMessageFilter& other)
    : IrcMessageFilter(other)
{
}

uint IrcMessageTypeFilter::messageTypes() const
{
    return d_ptr->messageTypes;
}

void IrcMessageTypeFilter::setMessageTypes(uint messageTypes)
{
    d_ptr->messageTypes = messageTypes;
}

IrcMessagePropertyFilter::IrcMessagePropertyFilter(const QString& property)
{
    d_ptr->filterType = PropertyFilter;
    d_ptr->property = property;
}

IrcMessagePropertyFilter::IrcMessagePropertyFilter(const IrcMessageFilter& other)
    : IrcMessageFilter(other)
{
}

QString IrcMessagePropertyFilter::property() const
{
    return d_ptr->property;
}

void IrcMessagePropertyFilter::setProperty(const QString& property)
{
    d_ptr->property = property;
}

QString IrcMessagePropertyFilter::term() const
{
    return d_ptr->term;
}

void IrcMessagePropertyFilter::setTerm(const QString& term)
{
    d_ptr->term = term;
}

Qt::MatchFlags IrcMessagePropertyFilter::matchFlags() const
{
    return d_ptr->matchFlags;
}

void IrcMessagePropertyFilter::setMatchFlags(Qt::MatchFlags flags)
{
    d_ptr->matchFlags = flags;
}
