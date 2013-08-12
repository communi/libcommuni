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

#include "ircsender.h"

/*!
    \file ircsender.h
    \brief #include &lt;IrcSender&gt;
 */

/*!
    \class IrcSender ircsender.h <IrcSender>
    \ingroup utility
    \brief A helper class for handling message sender prefixes.

    An IRC message sender prefix as specified in RFC 1459:
    <pre>
    &lt;prefix&gt; ::= &lt;servername&gt; | &lt;nick&gt; [ '!' &lt;user&gt; ] [ '@' &lt;host&gt; ]
    </pre>
 */

class IrcSenderPrivate : public QSharedData
{
public:
    void init();

    bool dirty;
    QString prefix;
    QString name;
    QString user;
    QString host;
};

void IrcSenderPrivate::init()
{
    name.clear();
    user.clear();
    host.clear();

    if (prefix.contains(QLatin1Char(' ')))
        return;

    const int len = prefix.length();
    const int ex = prefix.indexOf(QLatin1Char('!'));
    const int at = prefix.indexOf(QLatin1Char('@'));

    if (ex == -1 && at == -1) {
        name = prefix;
    } else if (ex > 0 && at > 0 && ex + 1 < at && at < len - 1) {
        name = prefix.mid(0, ex);
        user = prefix.mid(ex + 1, at - ex - 1);
        host = prefix.mid(at + 1);
    } else if (ex > 0 && ex < len - 1 && at == -1) {
        name = prefix.mid(0, ex);
        user = prefix.mid(ex + 1);
    } else if (at > 0 && at < len - 1 && ex == -1) {
        name = prefix.mid(0, at);
        host = prefix.mid(at + 1);
    }
}

/*!
    Constructs an invalid IrcSender.
 */
IrcSender::IrcSender() : d(new IrcSenderPrivate)
{
    d->dirty = false;
}

/*!
    Constructs a new IrcSender initialized to \a prefix.
 */
IrcSender::IrcSender(const QString& prefix) : d(new IrcSenderPrivate)
{
    d->dirty = false;
    d->prefix = prefix.trimmed();
}

/*!
    Constructs a copy of \a other IrcSender.
 */
IrcSender::IrcSender(const IrcSender& other) : d(other.d)
{
}

/*!
    Assigns an \a other IrcSender to this.
 */
IrcSender& IrcSender::operator=(const IrcSender& other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*!
    Destructs the IrcSender.
 */
IrcSender::~IrcSender()
{
}

/*!
    Returns \c true if this sender is same than the \a other;
    otherwise returns \c false.

    \sa operator!=()
 */
bool IrcSender::operator==(const IrcSender& other) const
{
    return d == other.d || prefix() == other.prefix();
}

/*!
    Returns \c true if this sender is different from the \a other;
    otherwise returns \c false.

    \sa operator==()
 */
bool IrcSender::operator!=(const IrcSender& other) const
{
    return !(*this == other);
}

/*!
    Returns \c true if the sender is valid; otherwise \c false.

    A sender is considered valid if the name is not empty.
 */
bool IrcSender::isValid() const
{
    if (d->name.isEmpty())
        d->init();
    return !d->name.isEmpty();
}

/*!
    Returns the whole prefix.
 */
QString IrcSender::prefix() const
{
    if (d->dirty) {
        d->prefix = d->name;
        if (!d->user.isEmpty()) d->prefix += QLatin1Char('!') + d->user;
        if (!d->host.isEmpty()) d->prefix += QLatin1Char('@') + d->host;
        d->dirty = false;
    }
    return d->prefix;
}

/*!
    Sets the whole prefix.

    \warning Overrides any existing name, user or host.
 */
void IrcSender::setPrefix(const QString& prefix)
{
    d->prefix = prefix.trimmed();
    d->name.clear();
    d->user.clear();
    d->host.clear();
    d->dirty = false;
}

/*!
    \fn QString IrcSender::name() const
    Returns the name.

    <pre>
    &lt;prefix&gt; ::= <b>&lt;servername&gt;</b> | <b>&lt;nick&gt;</b> [ '!' &lt;user&gt; ] [ '@' &lt;host&gt; ]
    </pre>
 */
QString IrcSender::name() const
{
    if (d->name.isEmpty())
        d->init();
    return d->name;
}

/*!
    \fn void IrcSender::setName(const QString& name)
    Sets the name.

    <pre>
    &lt;prefix&gt; ::= <b>&lt;servername&gt;</b> | <b>&lt;nick&gt;</b> [ '!' &lt;user&gt; ] [ '@' &lt;host&gt; ]
    </pre>
 */
void IrcSender::setName(const QString& name)
{
    d->name = name;
    d->dirty = true;
}

/*!
    \fn QString IrcSender::user() const
    Returns the user.

    <pre>
    &lt;prefix&gt; ::= &lt;servername&gt; | &lt;nick&gt; [ '!' <b>&lt;user&gt;</b> ] [ '@' &lt;host&gt; ]
    </pre>
 */
QString IrcSender::user() const
{
    if (d->user.isEmpty())
        d->init();
    return d->user;
}

/*!
    \fn void IrcSender::setUser(const QString& user)
    Sets the user.

    <pre>
    &lt;prefix&gt; ::= &lt;servername&gt; | &lt;nick&gt; [ '!' <b>&lt;user&gt;</b> ] [ '@' &lt;host&gt; ]
    </pre>
 */
void IrcSender::setUser(const QString& user)
{
    d->user = user;
    d->dirty = true;
}

/*!
    \fn QString IrcSender::host() const
    Returns the host.

    <pre>
    &lt;prefix&gt; ::= &lt;servername&gt; | &lt;nick&gt; [ '!' &lt;user&gt; ] [ '@' <b>&lt;host&gt;</b> ]
    </pre>
 */
QString IrcSender::host() const
{
    if (d->host.isEmpty())
        d->init();
    return d->host;
}

/*!
    \fn void IrcSender::setHost(const QString& host)
    Sets the host.

    <pre>
    &lt;prefix&gt; ::= &lt;servername&gt; | &lt;nick&gt; [ '!' &lt;user&gt; ] [ '@' <b>&lt;host&gt;</b> ]
    </pre>
 */
void IrcSender::setHost(const QString& host)
{
    d->host = host;
    d->dirty = true;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates IrcSender
    Writes a \a sender and returns a reference to the stream.
*/
QDebug operator<<(QDebug debug, const IrcSender& sender)
{
    debug.nospace() << "IrcSender(" << qPrintable(sender.prefix()) << ")";
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM
