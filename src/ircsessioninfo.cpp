/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
* Copyright (C) 2010-2011 SmokeX <smokexjc@gmail.com>
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

#include "ircsessioninfo.h"
#include "ircsession_p.h"
#include "ircsession.h"

/*!
    \file ircsessioninfo.h
    \brief #include &lt;IrcSessionInfo&gt;
 */

/*!
    \class IrcSessionInfo ircsessioninfo.h IrcSessionInfo
    \ingroup utility
    \brief The IrcSessionInfo class provides IRC session information.

    IrcSessionInfo provides various session information. This includes
    the network name, supported channel type prefixes, channel user mode
    letters and prefix characters, and various numerical limitations,
    such as the maximum nick, channel, topic and message lengths.

    \note A valid IrcSessionInfo can only be constructed at any time
          after the IrcSession::sessionInfoReceived() signal has been
          emitted.

    \sa isValid(), IrcSession::sessionInfoReceived()
 */

/*!
    \enum IrcSessionInfo::Limit
    This enum describes the numerical limit types.
 */

/*!
    \var IrcSessionInfo::NickLength
    \brief The maximum nick name length
 */

/*!
    \var IrcSessionInfo::ChannelLength
    \brief The maximum channel name length
 */

/*!
    \var IrcSessionInfo::TopicLength
    \brief The maximum channel topic length
 */

/*!
    \var IrcSessionInfo::MessageLength
    \brief The maximum message length
 */

/*!
    \var IrcSessionInfo::KickReasonLength
    \brief The maximum kick reason length
 */

/*!
    \var IrcSessionInfo::AwayReasonLength
    \brief The maximum away reason length
 */

/*!
    \var IrcSessionInfo::ModeCount
    \brief The maximum number of channel modes allowed per mode command
 */

class IrcSessionInfoPrivate : public QSharedData
{
public:
    IrcSessionInfoPrivate() : valid(false), modeLimit(-1), channelLimit(-1), targetLimit(-1) { }

    bool valid;
    QHash<QString, QString> info;
    QString network;
    QStringList modes, prefixes, channelTypes;
    int modeLimit, channelLimit, targetLimit;
};

/*!
    Constructs a new info object for IRC \a session.
 */
IrcSessionInfo::IrcSessionInfo(const IrcSession* session) : d(new IrcSessionInfoPrivate)
{
    Q_ASSERT(session);
    d->info = session->d_func()->info;
    d->valid = !d->info.isEmpty();
}

/*!
    Constructs a copy of \a other IRC session info object.
 */
IrcSessionInfo::IrcSessionInfo(const IrcSessionInfo& other) : d(other.d)
{
}

/*!
    Assigns an \a other IRC session info object to this.
 */
IrcSessionInfo& IrcSessionInfo::operator=(const IrcSessionInfo& other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*!
    Destructs the IRC session info.
 */
IrcSessionInfo::~IrcSessionInfo()
{
}

/*!
    Returns \c true if the info object is valid, \c false otherwise.

    \note A valid IrcSessionInfo can only be constructed at any time
          after the IrcSession::sessionInfoReceived() signal has been
          emitted. Constructing an IrcSessionInfo before the session
          information has been received results to an invalid
          IrcSessionInfo.

    \sa isValid(), IrcSession::sessionInfoReceived()
 */
bool IrcSessionInfo::isValid() const
{
    return d->valid;
}

/*!
    Returns the IRC network name.
 */
QString IrcSessionInfo::network() const
{
    if (d->network.isEmpty())
        d->network = d->info.take("NETWORK");
    return d->network;
}

/*!
    Returns the supported channel user mode letters.

    \sa prefixes(), modeToPrefix()
 */
QStringList IrcSessionInfo::modes() const
{
    if (d->modes.isEmpty()) {
        QString pfx = d->info.take("PREFIX");
        d->modes = pfx.mid(1, pfx.indexOf(')') - 1).split("", QString::SkipEmptyParts);
        d->prefixes = pfx.mid(pfx.indexOf(')') + 1).split("", QString::SkipEmptyParts);
    }
    return d->modes;
}

/*!
    Returns the supported channel user mode prefix characters.

    \sa modes(), prefixToMode()
 */
QStringList IrcSessionInfo::prefixes() const
{
    if (d->prefixes.isEmpty()) {
        QString pfx = d->info.take("PREFIX");
        d->modes = pfx.mid(1, pfx.indexOf(')') - 1).split("", QString::SkipEmptyParts);
        d->prefixes = pfx.mid(pfx.indexOf(')') + 1).split("", QString::SkipEmptyParts);
    }
    return d->prefixes;
}

/*!
    Returns the supported channel type prefix characters.
 */
QStringList IrcSessionInfo::channelTypes() const
{
    if (d->channelTypes.isEmpty())
        d->channelTypes = d->info.take("CHANTYPES").split("", QString::SkipEmptyParts);
    return d->channelTypes;
}

/*!
    Converts a channel user mode letter to a prefix character.

    \sa modes(), prefixToMode()
 */
QString IrcSessionInfo::modeToPrefix(const QString& mode) const
{
    return prefixes().value(modes().indexOf(mode));
}

/*!
    Converts a channel mode prefix character to a letter.

    \sa prefixes(), modeToPrefix()
 */
QString IrcSessionInfo::prefixToMode(const QString& prefix) const
{
    return modes().value(prefixes().indexOf(prefix));
}

/*!
    Returns a numerical \a type of limit, or -1 if the limitation is not known.

    \sa modeLimit(), channelLimit(), targetLimit()
 */
int IrcSessionInfo::numericLimit(Limit limit) const
{
    QString key;
    switch (limit) {
        case NickLength:        key = QLatin1String("NICKLEN"); break;
        case ChannelLength:     key = QLatin1String("CHANNELLEN"); break;
        case TopicLength:       key = QLatin1String("TOPICLEN"); break;
        case MessageLength:     return 512; // RFC 1459
        case KickReasonLength:  key = QLatin1String("KICKLEN"); break;
        case AwayReasonLength:  key = QLatin1String("AWAYLEN"); break;
        case ModeCount:         key = QLatin1String("MODES"); break;
        default:                break;
    }
    return d->info.value(key, "-1").toInt();
}

/*!
    \internal
 */
static int numericValue(const QString& key, const QString& parameter)
{
    QStringList keyValues = parameter.split(",", QString::SkipEmptyParts);
    foreach (const QString& keyValue, keyValues) {
        if (!keyValue.section(":", 0, 0).compare(key, Qt::CaseInsensitive))
            return keyValue.section(":", 1, 1).toInt();
    }
    return -1;
}

/*!
    Returns the limit of entries in the list per mode, or -1 if the limitation is not known.

    \sa limit(), channelLimit(), targetLimit()
 */
int IrcSessionInfo::modeLimit(const QString& mode) const
{
    if (d->modeLimit == -1)
        d->modeLimit = numericValue(mode, d->info.take("MAXLIST"));
    return d->modeLimit;
}

/*!
    Returns the limit for a \a type of channels, or -1 if the limitation is not known.

    \sa limit(), modeLimit(), targetLimit()
 */
int IrcSessionInfo::channelLimit(const QString& type) const
{
    if (d->channelLimit == -1)
        d->channelLimit = numericValue(type, d->info.take("CHANLIMIT"));
    return d->channelLimit;
}

/*!
    Returns the target limit for a \a command, or -1 if the limitation is not known.

    \sa limit(), modeLimit(), channelLimit()
 */
int IrcSessionInfo::targetLimit(const QString& command) const
{
    if (d->targetLimit == -1)
        d->targetLimit = numericValue(command, d->info.take("TARGMAX"));
    return d->targetLimit;
}
