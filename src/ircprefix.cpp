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

#include "ircprefix.h"
#include <QRegExp>

/*!
    \class IrcPrefix ircprefix.h <IrcPrefix>
    \brief The IrcPrefix class is a helper class for handling IRC message prefixes.

    An IRC message prefix as specified in RFC 1459:
    <pre>
    <prefix> ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
    </pre>
 */

/*!
    \fn QString IrcPrefix::name() const
    Returns the name.

    <pre>
    <prefix> ::= <b><servername></b> | <b><nick></b> [ '!' <user> ] [ '@' <host> ]
    </pre>
 */

/*!
    \fn void IrcPrefix::setName(const QString& name)
    Sets the name.

    <pre>
    <prefix> ::= <b><servername></b> | <b><nick></b> [ '!' <user> ] [ '@' <host> ]
    </pre>
 */

/*!
    \fn QString IrcPrefix::user() const
    Returns the user.

    <pre>
    <prefix> ::= <servername> | <nick> [ '!' <b><user></b> ] [ '@' <host> ]
    </pre>
 */

/*!
    \fn void IrcPrefix::setUser(const QString& user)
    Sets the user.

    <pre>
    <prefix> ::= <servername> | <nick> [ '!' <b><user></b> ] [ '@' <host> ]
    </pre>
 */

/*!
    \fn QString IrcPrefix::host() const
    Returns the host.

    <pre>
    <prefix> ::= <servername> | <nick> [ '!' <user> ] [ '@' <b><host></b> ]
    </pre>
 */

/*!
    \fn void IrcPrefix::setHost(const QString& host)
    Sets the host.

    <pre>
    <prefix> ::= <servername> | <nick> [ '!' <user> ] [ '@' <b><host></b> ]
    </pre>
 */

/*!
    Constructs a new IrcPrefix, optionally initializing to \a prefix.
 */
IrcPrefix::IrcPrefix(const QString& prefix)
{
    setPrefix(prefix);
}

/*!
    Returns \c true if the prefix is valid; otherwise \c false.

    A prefix is considered valid if the name is not empty.
 */
bool IrcPrefix::isValid() const
{
    return !n.isEmpty();
}

/*!
    Returns \c true if the prefix is a user prefix; otherwise \c false.

    A prefix is considered as user prefix if either user or host is not empty.
 */
bool IrcPrefix::isUser() const
{
    return isValid() && (!u.isEmpty() || !h.isEmpty());
}

/*!
    Returns the whole prefix.
 */
QString IrcPrefix::prefix() const
{
    if (!isValid())
        return QString();

    QString pfx = n;
    if (!u.isEmpty()) pfx += "!" + u;
    if (!h.isEmpty()) pfx += "@" + h;
    return pfx;
}

/*!
    Sets the whole prefix.

    \warning Overrides any existing name, user or host.
 */
void IrcPrefix::setPrefix(const QString& prefix)
{
    QRegExp rx("([^!\\s]+)(![^@\\s]+)(@\\S+)");
    bool match = rx.exactMatch(prefix.trimmed());
    n = match ? rx.cap(1) : QString();
    u = match ? rx.cap(2).mid(1) : QString();
    h = match ? rx.cap(3).mid(1) : QString();
}
