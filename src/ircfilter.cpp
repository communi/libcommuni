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

#include "ircfilter.h"

class IrcFilterPrivate : public QSharedData
{
public:
    IrcFilterPrivate();
    ~IrcFilterPrivate();

    IrcFilter::FilterType type;
    bool enabled;
    QList<IrcFilter> filters;
    QString property;
    QString term;
    Qt::MatchFlags matchFlags;
    QStringList terms;
    Qt::CaseSensitivity cs;
};

IrcFilterPrivate::IrcFilterPrivate() :
    type(IrcFilter::DefaultFilter),
    enabled(true),
    matchFlags(Qt::MatchExactly),
    cs(Qt::CaseInsensitive)
{
}

IrcFilterPrivate::~IrcFilterPrivate()
{
}

IrcFilter::IrcFilter(FilterType type)
    : d_ptr(new IrcFilterPrivate)
{
    d_ptr->type = type;
}

IrcFilter::IrcFilter(const IrcFilter& other)
    : d_ptr(other.d_ptr)
{
}

IrcFilter& IrcFilter::operator=(const IrcFilter& other)
{
    if (this != &other)
        d_ptr = other.d_ptr;
    return *this;
}

IrcFilter::~IrcFilter()
{
}

IrcFilter::FilterType IrcFilter::type() const
{
    return d_ptr->type;
}

void IrcFilter::setType(FilterType type)
{
    d_ptr->type = type;
}

bool IrcFilter::isEnabled() const
{
    return d_ptr->enabled;
}

void IrcFilter::setEnabled(bool enabled)
{
    d_ptr->enabled = enabled;
}

QList<IrcFilter> IrcFilter::filters() const
{
    return d_ptr->filters;
}

void IrcFilter::setFilters(const QList<IrcFilter>& filters)
{
    d_ptr->filters = filters;
}

void IrcFilter::append(const IrcFilter& filter)
{
    d_ptr->filters.append(filter);
}

void IrcFilter::prepend(const IrcFilter& filter)
{
    d_ptr->filters.prepend(filter);
}

void IrcFilter::remove(const IrcFilter& filter)
{
    d_ptr->filters.removeAll(filter);
}

void IrcFilter::clear()
{
    d_ptr->filters.clear();
}

IrcFilter& IrcFilter::operator<<(const IrcFilter& filter)
{
    d_ptr->filters << filter;
    return *this;
}

bool IrcFilter::operator==(const IrcFilter& other) const
{
    return d_ptr->type == other.d_ptr->type
            && d_ptr->enabled == other.d_ptr->enabled
            && d_ptr->filters == other.d_ptr->filters
            && d_ptr->property == other.d_ptr->property
            && d_ptr->term == other.d_ptr->term
            && d_ptr->matchFlags == other.d_ptr->matchFlags
            && d_ptr->terms == other.d_ptr->terms
            && d_ptr->cs == other.d_ptr->cs;
}

bool IrcFilter::operator!=(const IrcFilter& other) const
{
    return !operator==(other);
}

const IrcFilter operator&(const IrcFilter& left, const IrcFilter& right)
{
    IrcFilter filter(IrcFilter::IntersectionFilter);
    filter << left << right;
    return filter;
}

const IrcFilter operator|(const IrcFilter& left, const IrcFilter& right)
{
    IrcFilter filter(IrcFilter::UnionFilter);
    filter << left << right;
    return filter;
}

IrcPropertyFilter::IrcPropertyFilter(const QString& property)
{
    d_ptr->property = property;
}

IrcPropertyFilter::IrcPropertyFilter(const IrcFilter& other)
    : IrcFilter(other)
{
}

QString IrcPropertyFilter::property() const
{
    return d_ptr->property;
}

void IrcPropertyFilter::setProperty(const QString& property)
{
    d_ptr->property = property;
}

QString IrcPropertyFilter::term() const
{
    return d_ptr->term;
}

void IrcPropertyFilter::setTerm(const QString& term)
{
    d_ptr->term = term;
}

Qt::MatchFlags IrcPropertyFilter::matchFlags() const
{
    return d_ptr->matchFlags;
}

void IrcPropertyFilter::setMatchFlags(Qt::MatchFlags flags)
{
    d_ptr->matchFlags = flags;
}
