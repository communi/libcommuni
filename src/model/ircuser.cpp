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

#include "ircuser.h"
#include "ircuser_p.h"

IRC_BEGIN_NAMESPACE

/*!
    \file ircuser.h
    \brief #include &lt;IrcUser&gt;
 */

/*!
    \class IrcUser ircuser.h <IrcUser>
    \ingroup model
    \brief Keeps track of user status on a channel.

    \note IrcUser does not work on its own. Use an instance managed by IrcUserModel.

    \sa model
*/

void IrcUserPrivate::setName(const QString& n)
{
    Q_Q(IrcUser);
    if (name != n) {
        name = n;
        emit q->nameChanged(name);
    }
}

void IrcUserPrivate::setPrefix(const QString& p)
{
    Q_Q(IrcUser);
    if (prefix != p) {
        prefix = p;
        emit q->prefixChanged(prefix);
    }
}

void IrcUserPrivate::setMode(const QString& m)
{
    Q_Q(IrcUser);
    if (mode != m) {
        mode = m;
        emit q->modeChanged(mode);
    }
}

/*!
    Constructs a new user with \a parent.

    \note IrcUser does not work on its own. Use an instance managed by IrcUserModel.

    \sa IrcUserModel::added()
 */
IrcUser::IrcUser(QObject* parent)
    : QObject(parent), d_ptr(new IrcUserPrivate)
{
    Q_D(IrcUser);
    d->q_ptr = this;
}

/*!
    Destructs the user object.

    \warning Do not delete an instance managed by IrcUserModel.

    \sa IrcUserModel::removed()
 */
IrcUser::~IrcUser()
{
}

/*!
    This property holds the name.

    \par Access function:
    \li QString <b>name</b>() const

    \par Notifier signal:
    \li void <b>nameChanged</b>(const QString& name)
 */
QString IrcUser::name() const
{
    Q_D(const IrcUser);
    return d->name;
}

/*!
    This property holds the prefix character.

    Typical prefix characters are \c @ (op) and \c + (voice).

    \par Access function:
    \li QString <b>prefix</b>() const

    \par Notifier signal:
    \li void <b>prefixChanged</b>(const QString& prefix)
 */
QString IrcUser::prefix() const
{
    Q_D(const IrcUser);
    return d->prefix;
}

/*!
    This property holds the mode letter.

    Typical mode letters are \c o (op) and \c v (voice).

    \par Access function:
    \li QString <b>mode</b>() const

    \par Notifier signal:
    \li void <b>modeChanged</b>(const QString& mode)
 */
QString IrcUser::mode() const
{
    Q_D(const IrcUser);
    return d->mode;
}


/*!
    This property holds the channel of the user.

    \par Access function:
    \li \ref IrcChannel* <b>channel</b>() const
 */
IrcChannel* IrcUser::channel() const
{
    Q_D(const IrcUser);
    return d->channel;
}

IRC_END_NAMESPACE
