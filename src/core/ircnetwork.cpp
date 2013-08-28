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

#include "ircnetwork.h"
#include "ircnetwork_p.h"
#include "ircconnection_p.h"
#include "ircprotocol.h"
#include "ircconnection.h"
#include "irccommand.h"
#include <QPointer>

IRC_BEGIN_NAMESPACE

/*!
    \file ircnetwork.h
    \brief #include &lt;IrcNetwork&gt;
 */

/*!
    \class IrcNetwork ircnetwork.h IrcNetwork
    \ingroup core
    \brief Provides network information.

    IrcNetwork provides various IRC network information. This includes
    the network name, supported channel type prefixes, channel user mode
    letters and prefix characters, and various numerical limitations,
    such as the maximum nick, channel, topic and message lengths, and
    available and active capabilities.

    \note IrcNetwork can be accessed via IrcConnection::network().

    \sa isValid(), IrcConnection::network()
 */

/*!
    \enum IrcNetwork::ModeType
    This enum describes the channel mode types.
 */

/*!
    \var IrcNetwork::TypeA
    \brief Type A modes

    Modes that add or remove an address to or from a list.
    These modes always take a parameter when sent by the server to a
    client; when sent by a client, they may be specified without a
    parameter, which requests the server to display the current
    contents of the corresponding list on the channel to the client.
 */

/*!
    \var IrcNetwork::TypeB
    \brief Type B modes

    Modes that change a setting on the channel. These modes
    always take a parameter.
 */

/*!
    \var IrcNetwork::TypeC
    \brief Type C modes

    Modes that change a setting on the channel. These modes
    take a parameter only when set; the parameter is absent when the
    mode is removed both in the client's and server's MODE command.
 */

/*!
    \var IrcNetwork::TypeD
    \brief Type D modes

    Modes that change a setting on the channel. These modes
    never take a parameter.
 */

/*!
    \var IrcNetwork::AllTypes
    \brief All type modes
 */

/*!
    \enum IrcNetwork::Limit
    This enum describes the numerical limit types.
 */

/*!
    \var IrcNetwork::NickLength
    \brief The maximum nick name length
 */

/*!
    \var IrcNetwork::ChannelLength
    \brief The maximum channel name length
 */

/*!
    \var IrcNetwork::TopicLength
    \brief The maximum channel topic length
 */

/*!
    \var IrcNetwork::MessageLength
    \brief The maximum message length
 */

/*!
    \var IrcNetwork::KickReasonLength
    \brief The maximum kick reason length
 */

/*!
    \var IrcNetwork::AwayReasonLength
    \brief The maximum away reason length
 */

/*!
    \var IrcNetwork::ModeCount
    \brief The maximum number of channel modes allowed per mode command
 */

IrcNetworkPrivate::IrcNetworkPrivate() : q_ptr(0), valid(false)
{
}

static QHash<QString, int> numericValues(const QString& parameter)
{
    QHash<QString, int> values;
    const QStringList keyValues = parameter.split(",", QString::SkipEmptyParts);
    foreach (const QString& keyValue, keyValues)
        values.insert(keyValue.section(":", 0, 0), keyValue.section(":", 1, 1).toInt());
    return values;
}

void IrcNetworkPrivate::setInfo(const QHash<QString, QString>& info)
{
    valid = !info.isEmpty();

    if (info.contains("NETWORK"))
        setName(info.value("NETWORK"));
    if (info.contains("PREFIX")) {
        const QString pfx = info.value("PREFIX");
        setModes(pfx.mid(1, pfx.indexOf(')') - 1).split("", QString::SkipEmptyParts));
        setPrefixes(pfx.mid(pfx.indexOf(')') + 1).split("", QString::SkipEmptyParts));
    }
    if (info.contains("CHANTYPES"))
        setChannelTypes(info.value("CHANTYPES").split("", QString::SkipEmptyParts));

    // TODO:
    if (info.contains("NICKLEN"))
        numericLimits.insert("NICKLEN", info.value("NICKLEN").toInt());
    if (info.contains("CHANNELLEN"))
        numericLimits.insert("CHANNELLEN", info.value("CHANNELLEN").toInt());
    if (info.contains("TOPICLEN"))
        numericLimits.insert("TOPICLEN", info.value("TOPICLEN").toInt());
    if (info.contains("KICKLEN"))
        numericLimits.insert("KICKLEN", info.value("KICKLEN").toInt());
    if (info.contains("AWAYLEN"))
        numericLimits.insert("AWAYLEN", info.value("AWAYLEN").toInt());
    if (info.contains("MODES"))
        numericLimits.insert("MODES", info.value("MODES").toInt());
    if (info.contains("CHANMODES"))
        channelModes = info.value("CHANMODES").split(",", QString::SkipEmptyParts);
    if (info.contains("MAXLIST"))
        modeLimits = numericValues(info.value("MAXLIST"));
    if (info.contains("CHANLIMIT"))
        channelLimits = numericValues(info.value("CHANLIMIT"));
    if (info.contains("TARGMAX"))
        targetLimits = numericValues(info.value("TARGMAX"));
}

void IrcNetworkPrivate::setAvailableCapabilities(const QSet<QString>& capabilities)
{
    Q_Q(IrcNetwork);
    if (availableCaps != capabilities) {
        availableCaps = capabilities;
        emit q->availableCapabilitiesChanged(availableCaps.toList());
    }
}

void IrcNetworkPrivate::setActiveCapabilities(const QSet<QString>& capabilities)
{
    Q_Q(IrcNetwork);
    if (activeCaps != capabilities) {
        activeCaps = capabilities;
        emit q->activeCapabilitiesChanged(activeCaps.toList());
    }
}

void IrcNetworkPrivate::setName(const QString& value)
{
    Q_Q(IrcNetwork);
    if (name != value) {
        name = value;
        emit q->nameChanged(value);
    }
}

void IrcNetworkPrivate::setModes(const QStringList& value)
{
    Q_Q(IrcNetwork);
    if (modes != value) {
        modes = value;
        emit q->modesChanged(value);
    }
}

void IrcNetworkPrivate::setPrefixes(const QStringList& value)
{
    Q_Q(IrcNetwork);
    if (prefixes != value) {
        prefixes = value;
        emit q->prefixesChanged(value);
    }
}

void IrcNetworkPrivate::setChannelTypes(const QStringList& value)
{
    Q_Q(IrcNetwork);
    if (channelTypes != value) {
        channelTypes = value;
        emit q->channelTypesChanged(value);
    }
}

/*!
    Constructs a new network object for IRC \a connection.
 */
IrcNetwork::IrcNetwork(IrcConnection* connection) : QObject(connection), d_ptr(new IrcNetworkPrivate)
{
    Q_D(IrcNetwork);
    d->q_ptr = this;
    d->connection = connection;
}

/*!
    Destructs the IRC connection info.
 */
IrcNetwork::~IrcNetwork()
{
}

/*!
    Returns \c true if the info object is valid, \c false otherwise.

    \note A valid IrcNetwork can only be constructed at any time
          after the IrcConnection::connectionInfoReceived() signal has been
          emitted. Constructing an IrcNetwork before the connection
          information has been received results to an invalid
          IrcNetwork.

    \sa isValid(), IrcConnection::connectionInfoReceived()
 */
bool IrcNetwork::isValid() const
{
    Q_D(const IrcNetwork);
    return d->connection && d->valid;
}

/*!
    Returns the IRC network name.
 */
QString IrcNetwork::name() const
{
    Q_D(const IrcNetwork);
    return d->name;
}

/*!
    Returns the supported channel user mode letters.

    \sa prefixes(), modeToPrefix()
 */
QStringList IrcNetwork::modes() const
{
    Q_D(const IrcNetwork);
    return d->modes;
}

/*!
    Returns the supported channel user mode prefix characters.

    \sa modes(), prefixToMode()
 */
QStringList IrcNetwork::prefixes() const
{
    Q_D(const IrcNetwork);
    return d->prefixes;
}

/*!
    Converts a channel user mode letter to a prefix character.

    \sa modes(), prefixToMode()
 */
QString IrcNetwork::modeToPrefix(const QString& mode) const
{
    Q_D(const IrcNetwork);
    return d->prefixes.value(d->modes.indexOf(mode));
}

/*!
    Converts a channel mode prefix character to a letter.

    \sa prefixes(), modeToPrefix()
 */
QString IrcNetwork::prefixToMode(const QString& prefix) const
{
    Q_D(const IrcNetwork);
    return d->modes.value(d->prefixes.indexOf(prefix));
}

/*!
    Returns the supported channel type prefix characters.
 */
QStringList IrcNetwork::channelTypes() const
{
    Q_D(const IrcNetwork);
    return d->channelTypes;
}

/*!
    Returns \c true if the \a name is a channel.

    This method is provided for convenience. It is equal to:
    \code
    !name.isEmpty() && network->channelTypes().contains(name.at(0))
    \endcode

    \sa channelTypes()
 */
bool IrcNetwork::isChannel(const QString& name) const
{
    Q_D(const IrcNetwork);
    return !name.isEmpty() && d->channelTypes.contains(name.at(0));
}

/*!
    Returns the supported channel modes for \a type.
 */
QStringList IrcNetwork::channelModes(IrcNetwork::ModeTypes types) const
{
    Q_D(const IrcNetwork);
    QStringList modes;
    if (types & TypeA)
        modes += d->channelModes.value(0).split("", QString::SkipEmptyParts);
    if (types & TypeB)
        modes += d->channelModes.value(1).split("", QString::SkipEmptyParts);
    if (types & TypeC)
        modes += d->channelModes.value(2).split("", QString::SkipEmptyParts);
    if (types & TypeD)
        modes += d->channelModes.value(3).split("", QString::SkipEmptyParts);
    return modes;
}

/*!
    Returns a numerical \a type of limit, or -1 if the limitation is not known.

    \sa modeLimit(), channelLimit(), targetLimit()
 */
int IrcNetwork::numericLimit(Limit limit) const
{
    Q_D(const IrcNetwork);
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
    return d->numericLimits.value(key, -1);
}

/*!
    Returns the limit of entries in the list per mode, or -1 if the limitation is not known.

    \sa numericLimit(), channelLimit(), targetLimit()
 */
int IrcNetwork::modeLimit(const QString& mode) const
{
    Q_D(const IrcNetwork);
    return d->modeLimits.value(mode);
}

/*!
    Returns the limit for a \a type of channels, or -1 if the limitation is not known.

    \sa numericLimit(), modeLimit(), targetLimit()
 */
int IrcNetwork::channelLimit(const QString& type) const
{
    Q_D(const IrcNetwork);
    return d->channelLimits.value(type);
}

/*!
    Returns the target limit for a \a command, or -1 if the limitation is not known.

    \sa numericLimit(), modeLimit(), channelLimit()
 */
int IrcNetwork::targetLimit(const QString& command) const
{
    Q_D(const IrcNetwork);
    return d->targetLimits.value(command);
}

/*!
    Returns the available capabilities.

    \sa IrcConnection::capabilities(), IrcCapabilityMessage, IrcCommand::createCapability()
 */
QStringList IrcNetwork::availableCapabilities() const
{
    Q_D(const IrcNetwork);
    return d->availableCaps.toList();
}

/*!
    Returns the active capabilities.

    \sa IrcConnection::capabilities(), IrcCapabilityMessage, IrcCommand::createCapability()
 */
QStringList IrcNetwork::activeCapabilities() const
{
    Q_D(const IrcNetwork);
    return d->activeCaps.toList();
}

/*!
    Returns \c true if the \a capability is \b available.

    This method is provided for convenience. It is equal to:
    \code
    network->availableCapabilities().contains(capability)
    \endcode

    \sa availableCapabilities()
 */
bool IrcNetwork::hasCapability(const QString& capability) const
{
    Q_D(const IrcNetwork);
    return d->availableCaps.contains(capability);
}

/*!
    Returns \c true if the \a capability is \b active.

    This method is provided for convenience. It is equal to:
    \code
    network->activeCapabilities().contains(capability)
    \endcode

    \sa activeCapabilities()
 */
bool IrcNetwork::isCapable(const QString& capability) const
{
    Q_D(const IrcNetwork);
    return d->activeCaps.contains(capability);
}

/*!
    Requests the specified \a capability.

    This method is provided for convenience. It is equal to:
    \code
    connection->sendCommand(IrcCommand::createCapability("REQ", capability))
    \endcode
 */
bool IrcNetwork::requestCapability(const QString& capability)
{
    Q_D(IrcNetwork);
    if (d->connection)
        return d->connection->sendCommand(IrcCommand::createCapability(QLatin1String("REQ"), capability));
    return false;
}

/*!
    Requests the specified \a capabilities.

    This method is provided for convenience. It is equal to:
    \code
    connection->sendCommand(IrcCommand::createCapability("REQ", capabilities))
    \endcode
 */
bool IrcNetwork::requestCapabilities(const QStringList& capabilities)
{
    Q_D(IrcNetwork);
    if (d->connection)
        return d->connection->sendCommand(IrcCommand::createCapability(QLatin1String("REQ"), capabilities));
    return false;
}

#include "moc_ircnetwork.cpp"

IRC_END_NAMESPACE
