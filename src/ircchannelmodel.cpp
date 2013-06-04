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

#include "ircchannelmodel.h"
#include "ircmessagefilter.h"
#include "ircsessioninfo.h"
#include "ircchannel_p.h"
#include "ircmessage.h"
#include "ircsession.h"
#include <qpointer.h>

/*!
    \file ircchannelmodel.h
    \brief #include &lt;IrcChannelModel&gt;
 */

/*!
    \class IrcChannelModel ircchannelmodel.h <IrcChannelModel>
    \ingroup models
    \brief Keeps track of channels.

    IrcChannelModel automatically keeps track of channels
    and manages IrcChannel instances for them.

    \sa models
 */

/*!
    \fn void IrcChannelModel::channelAdded(IrcChannel* channel)

    This signal is emitted when a \a channel is added to the list of channels.
 */

/*!
    \fn void IrcChannelModel::channelRemoved(IrcChannel* channel)

    This signal is emitted when a \a channel is removed from the list of channels.
 */

/*!
    \fn void IrcChannelModel::messageIgnored(IrcMessage* message)

    This signal is emitted when a message was ignored.

    IrcChannelModel handles only channel specific messages and delivers
    them to the appropriate IrcChannel instances. When applications decide
    to handle IrcChannel::messageReceived(), this signal makes it easy to
    implement handling for the rest, non-channel specific messages.

    \sa IrcSession::messageReceived(), IrcChannel::messageReceived()
 */

class IrcChannelModelPrivate : public IrcMessageFilter
{
    Q_DECLARE_PUBLIC(IrcChannelModel)

public:
    IrcChannelModelPrivate(IrcChannelModel* q);

    bool messageFilter(IrcMessage* message);

    void addChannel(const QString& title);
    void removeChannel(const QString& title);
    bool processMessage(const QString& title, IrcMessage* message);

    void _irc_channelDestroyed(IrcChannel* channel);

    IrcChannelModel* q_ptr;
    Irc::ItemDataRole role;
    QPointer<IrcSession> session;
    QList<IrcChannel*> channelList;
    QMap<QString, IrcChannel*> channelMap;
};

IrcChannelModelPrivate::IrcChannelModelPrivate(IrcChannelModel* q) :
    q_ptr(q), role(Irc::TitleRole)
{
}

bool IrcChannelModelPrivate::messageFilter(IrcMessage* msg)
{
    Q_Q(IrcChannelModel);
    if (msg->type() == IrcMessage::Join && msg->flags() & IrcMessage::Own)
        addChannel(static_cast<IrcJoinMessage*>(msg)->channel().toLower());

    switch (msg->type()) {
        case IrcMessage::Nick:
        case IrcMessage::Quit:
            foreach (IrcChannel* channel, channelList)
                IrcChannelPrivate::get(channel)->processChannelMessage(msg);
            break;

        case IrcMessage::Join:
        case IrcMessage::Part:
        case IrcMessage::Kick:
        case IrcMessage::Names:
        case IrcMessage::Topic:
            if (!processMessage(msg->property("channel").toString().toLower(), msg))
                emit q->messageIgnored(msg);
            break;

        case IrcMessage::Mode:
        case IrcMessage::Notice:
        case IrcMessage::Private:
            if (!processMessage(msg->property("target").toString().toLower(), msg))
                emit q->messageIgnored(msg);
            break;

        default:
            emit q->messageIgnored(msg);
            break;
    }

    if (msg->type() == IrcMessage::Part && msg->flags() & IrcMessage::Own) {
        removeChannel(static_cast<IrcPartMessage*>(msg)->channel().toLower());
    } else if (msg->type() == IrcMessage::Quit && msg->flags() & IrcMessage::Own) {
        foreach (const QString& channel, channelMap.keys())
            removeChannel(channel);
    } else if (msg->type() == IrcMessage::Kick) {
        const IrcKickMessage* kickMsg = static_cast<IrcKickMessage*>(msg);
        if (!kickMsg->user().compare(msg->session()->nickName(), Qt::CaseInsensitive))
            removeChannel(kickMsg->channel().toLower());
    }

    return false;
}

void IrcChannelModelPrivate::addChannel(const QString& title)
{
    Q_Q(IrcChannelModel);
    if (!channelMap.contains(title)) {
        IrcChannel* channel = q->createChannel(title);
        if (channel) {
            IrcChannelPrivate::get(channel)->init(title, session);
            q->beginInsertRows(QModelIndex(), channelList.count(), channelList.count());
            channelList.append(channel);
            channelMap.insert(title, channel);
            q->connect(channel, SIGNAL(destroyed(IrcChannel*)), SLOT(_irc_channelDestroyed(IrcChannel*)));
            q->endInsertRows();
            emit q->channelAdded(channel);
            emit q->channelsChanged(channelList);
            emit q->countChanged(channelList.count());
        }
    }
}

void IrcChannelModelPrivate::removeChannel(const QString& title)
{
    Q_Q(IrcChannelModel);
    IrcChannel* channel = channelMap.value(title);
    if (channel)
        q->destroyChannel(channel);
}

bool IrcChannelModelPrivate::processMessage(const QString& title, IrcMessage* message)
{
    IrcChannel* channel = channelMap.value(title);
    if (channel)
        return IrcChannelPrivate::get(channel)->processChannelMessage(message);
    return false;
}

void IrcChannelModelPrivate::_irc_channelDestroyed(IrcChannel* channel)
{
    Q_Q(IrcChannelModel);
    int idx = channelList.indexOf(channel);
    if (idx != -1) {
        q->beginRemoveRows(QModelIndex(), idx, idx);
        channelList.removeAt(idx);
        channelMap.remove(channel->title());
        q->endRemoveRows();
        emit q->channelRemoved(channel);
        emit q->channelsChanged(channelList);
        emit q->countChanged(channelList.count());
    }
}

Q_DECLARE_METATYPE(IrcChannel*)
Q_DECLARE_METATYPE(QList<IrcChannel*>)

/*!
    Constructs a new model with \a parent.

    \note If \a parent is an instance of IrcSession, it will be
    automatically assigned to \ref IrcChannelModel::session "session".
 */
IrcChannelModel::IrcChannelModel(QObject* parent)
    : QAbstractListModel(parent), d_ptr(new IrcChannelModelPrivate(this))
{
    setSession(qobject_cast<IrcSession*>(parent));

    qRegisterMetaType<IrcChannel*>();
    qRegisterMetaType<QList<IrcChannel*> >();
}

/*!
    Destructs the model.
 */
IrcChannelModel::~IrcChannelModel()
{
    Q_D(IrcChannelModel);
    foreach (IrcChannel* channel, d->channelList) {
        channel->blockSignals(true);
        delete channel;
    }
    d->channelList.clear();
    d->channelMap.clear();
}

/*!
    This property holds the session.

    \par Access functions:
    \li \ref IrcSession* <b>session</b>() const
    \li void <b>setSession</b>(\ref IrcSession* session)

    \warning Changing the session on the fly is not supported.
 */
IrcSession* IrcChannelModel::session() const
{
    Q_D(const IrcChannelModel);
    return d->session;
}

void IrcChannelModel::setSession(IrcSession* session)
{
    Q_D(IrcChannelModel);
    if (d->session != session) {
        if (d->session)
            qFatal("IrcChannelModel::setSession(): changing the session on the fly is not supported.");
        d->session = session;
        d->session->installMessageFilter(d);
        emit sessionChanged(session);
    }
}

/*!
    This property holds the number of channels.

    \par Access function:
    \li int <b>count</b>() const

    \par Notifier signal:
    \li void <b>countChanged</b>(int count)
 */
int IrcChannelModel::count() const
{
    return rowCount();
}

/*!
    This property holds the list of channels.

    \par Access function:
    \li QList<\ref IrcChannel*> <b>channels</b>() const

    \par Notifier signal:
    \li void <b>channelsChanged</b>(const QList<\ref IrcChannel*>& channels)
 */
QList<IrcChannel*> IrcChannelModel::channels() const
{
    Q_D(const IrcChannelModel);
    return d->channelList;
}

/*!
    Returns the channel object at \a index.
 */
IrcChannel* IrcChannelModel::get(int index) const
{
    Q_D(const IrcChannelModel);
    return d->channelList.value(index);
}

/*!
    Returns the channel object for \a title.
 */
IrcChannel* IrcChannelModel::channel(const QString& title) const
{
    Q_D(const IrcChannelModel);
    return d->channelMap.value(title);
}

/*!
    Returns \c true if the model contains \a title.
 */
bool IrcChannelModel::contains(const QString& title) const
{
    Q_D(const IrcChannelModel);
    return d->channelMap.contains(title);
}

/*!
    This property holds the display role.

    The specified data role is returned for Qt::DisplayRole.

    The default value is \ref Irc::TitleRole.

    \par Access functions:
    \li \ref Irc::ItemDataRole <b>displayRole</b>() const
    \li void <b>setDisplayRole</b>(\ref Irc::ItemDataRole role)
 */
Irc::ItemDataRole IrcChannelModel::displayRole() const
{
    Q_D(const IrcChannelModel);
    return d->role;
}

void IrcChannelModel::setDisplayRole(Irc::ItemDataRole role)
{
    Q_D(IrcChannelModel);
    d->role = role;
}

/*!
    Clears the model.
 */
void IrcChannelModel::clear()
{
    Q_D(IrcChannelModel);
    if (!d->channelList.isEmpty()) {
        beginResetModel();
        qDeleteAll(d->channelList);
        d->channelList.clear();
        d->channelMap.clear();
        endResetModel();
        emit channelsChanged(QList<IrcChannel*>());
        emit countChanged(0);
    }
}

/*!
    Creates a channel object for channel \a title.

    IrcChannelModel will automatically call this factory method when a
    need for the channel object occurs ie. the channel is being joined.

    The default implementation creates a new instance of IrcChannel.
    Reimplement this function in order to alter the default behavior,
    for example to provide a custom IrcChannel subclass.
 */
IrcChannel* IrcChannelModel::createChannel(const QString& title)
{
    Q_UNUSED(title);
    return new IrcChannel(this);
}

/*!
    Destroys the channel \a model.

    IrcChannelModel will automatically call this method when the channel
    object is no longer needed ie. the user quit, the channel was parted,
    or the user was kicked from the channel.

    The default implementation deletes the channel object.
    Reimplement this function in order to alter the default behavior,
    for example to keep the channel object alive.
 */
void IrcChannelModel::destroyChannel(IrcChannel* channel)
{
    delete channel;
}

/*!
    The following role names are provided by default:

    Role             | Name      | Type        | Example
    -----------------|-----------|-------------|--------
    Qt::DisplayRole  | "display" | 1)          | -
    Irc::ChannelRole | "channel" | IrcChannel* | <object>
    Irc::NameRole    | "name"    | QString     | "communi"
    Irc::PrefixRole  | "prefix"  | QString     | "#"
    Irc::TitleRole   | "title"   | QString     | "#communi"

    1) The type depends on \ref displayRole.
 */
QHash<int, QByteArray> IrcChannelModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Irc::ChannelRole] = "channel";
    roles[Irc::NameRole] = "name";
    roles[Irc::PrefixRole] = "prefix";
    roles[Irc::TitleRole] = "title";
    return roles;
}

/*!
    Returns the number of channels.
 */
int IrcChannelModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    Q_D(const IrcChannelModel);
    return d->channelList.count();
}

/*!
    Returns the data for specified \a role and user referred to by by the \a index.
 */
QVariant IrcChannelModel::data(const QModelIndex& index, int role) const
{
    Q_D(const IrcChannelModel);
    if (!hasIndex(index.row(), index.column()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return data(index, d->role);
    case Irc::ChannelRole:
        return QVariant::fromValue(d->channelList.at(index.row()));
    case Irc::NameRole:
        if (IrcChannel* channel =  d->channelList.at(index.row()))
            return channel->name();
        break;
    case Irc::PrefixRole:
        if (IrcChannel* channel =  d->channelList.at(index.row()))
            return channel->prefix();
        break;
    case Irc::TitleRole:
        if (IrcChannel* channel =  d->channelList.at(index.row()))
            return channel->title();
        break;
    }

    return QVariant();
}

#include "moc_ircchannelmodel.cpp"
