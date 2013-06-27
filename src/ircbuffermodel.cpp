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

#include "ircbuffermodel.h"
#include "ircbuffermodel_p.h"
#include "ircsessioninfo.h"
#include "ircbuffer_p.h"
#include "ircchannel.h"
#include "ircmessage.h"
#include "ircsession.h"

/*!
    \file ircbuffermodel.h
    \brief #include &lt;IrcBufferModel&gt;
 */

/*!
    \class IrcBufferModel ircbuffermodel.h <IrcBufferModel>
    \ingroup models
    \brief Keeps track of buffers.

    IrcBufferModel automatically keeps track of channel and query buffers
    and manages IrcBuffer instances for them. It will notify via signals
    when channel and query buffers are added and/or removed. IrcBufferModel
    can be used directly as a data model for Qt's item views - both in C++
    and QML.

    \code
    IrcSession* session = new IrcSession(this);
    IrcBufferModel* model = new IrcBufferModel(session);
    connect(model, SIGNAL(bufferAdded(IrcBuffer*)), this, SLOT(onBufferAdded(IrcBuffer*)));
    connect(model, SIGNAL(bufferRemoved(IrcBuffer*)), this, SLOT(onBufferRemoved(IrcBuffer*)));
    listView->setModel(model);
    \endcode

    \sa models
 */

/*!
    \fn void IrcBufferModel::bufferAdded(IrcBuffer* buffer)

    This signal is emitted when a \a buffer is added to the list of buffers.
 */

/*!
    \fn void IrcBufferModel::bufferRemoved(IrcBuffer* buffer)

    This signal is emitted when a \a buffer is removed from the list of buffers.
 */

/*!
    \fn void IrcBufferModel::messageIgnored(IrcMessage* message)

    This signal is emitted when a message was ignored.

    IrcBufferModel handles only buffer specific messages and delivers
    them to the appropriate IrcBuffer instances. When applications decide
    to handle IrcBuffer::messageReceived(), this signal makes it easy to
    implement handling for the rest, non-buffer specific messages.

    \sa IrcSession::messageReceived(), IrcBuffer::messageReceived()
 */

IrcBufferModelPrivate::IrcBufferModelPrivate(IrcBufferModel* q) :
    q_ptr(q), role(Irc::TitleRole)
{
}

bool IrcBufferModelPrivate::messageFilter(IrcMessage* msg)
{
    Q_Q(IrcBufferModel);
    if (msg->type() == IrcMessage::Join && msg->flags() & IrcMessage::Own)
        addBuffer(static_cast<IrcJoinMessage*>(msg)->channel());

    bool processed = false;
    switch (msg->type()) {
        case IrcMessage::Nick:
        case IrcMessage::Quit:
            foreach (IrcBuffer* buffer, bufferList)
                IrcBufferPrivate::get(buffer)->processMessage(msg);
            processed = true;
            break;

        case IrcMessage::Join:
        case IrcMessage::Part:
        case IrcMessage::Kick:
        case IrcMessage::Names:
        case IrcMessage::Topic:
            processed = processMessage(msg->property("channel").toString(), msg);
            break;

        case IrcMessage::Mode:
        case IrcMessage::Notice:
        case IrcMessage::Private:
            processed = processMessage(msg->property("target").toString(), msg)
                     || processMessage(msg->sender().name(), msg, msg->type() == IrcMessage::Private);
            break;

        case IrcMessage::Numeric:
            // TODO: any other special cases besides RPL_NAMREPLY?
            if (static_cast<IrcNumericMessage*>(msg)->code() == Irc::RPL_NAMREPLY) {
                const int count = msg->parameters().count();
                const QString channel = msg->parameters().value(count - 2);
                processed = processMessage(channel, msg);
            } else {
                processed = processMessage(msg->parameters().value(1), msg);
            }
            break;

        default:
            break;
    }

    if (!processed)
        emit q->messageIgnored(msg);

    if (msg->type() == IrcMessage::Part && msg->flags() & IrcMessage::Own) {
        removeBuffer(static_cast<IrcPartMessage*>(msg)->channel());
    } else if (msg->type() == IrcMessage::Quit && msg->flags() & IrcMessage::Own) {
        foreach (const QString& buffer, bufferMap.keys())
            removeBuffer(buffer);
    } else if (msg->type() == IrcMessage::Kick) {
        const IrcKickMessage* kickMsg = static_cast<IrcKickMessage*>(msg);
        if (!kickMsg->user().compare(msg->session()->nickName(), Qt::CaseInsensitive))
            removeBuffer(kickMsg->channel());
    }

    return false;
}

IrcBuffer* IrcBufferModelPrivate::addBuffer(const QString& title)
{
    Q_Q(IrcBufferModel);
    const QString lower = title.toLower();
    IrcBuffer* buffer = bufferMap.value(lower);
    if (!buffer) {
        buffer = q->createBuffer(title);
        if (buffer) {
            const bool isChannel = qobject_cast<IrcChannel*>(buffer);
            IrcBufferPrivate::get(buffer)->init(title, q);
            q->beginInsertRows(QModelIndex(), bufferList.count(), bufferList.count());
            bufferList.append(buffer);
            bufferMap.insert(lower, buffer);
            if (isChannel)
                channels += title;
            q->connect(buffer, SIGNAL(destroyed(IrcBuffer*)), SLOT(_irc_bufferDestroyed(IrcBuffer*)));
            q->endInsertRows();
            emit q->bufferAdded(buffer);
            if (isChannel)
                emit q->channelsChanged(channels);
            emit q->buffersChanged(bufferList);
            emit q->countChanged(bufferList.count());
        }
    }
    return buffer;
}

void IrcBufferModelPrivate::removeBuffer(const QString& title)
{
    Q_Q(IrcBufferModel);
    IrcBuffer* buffer = bufferMap.value(title.toLower());
    if (buffer)
        q->destroyBuffer(buffer);
}

bool IrcBufferModelPrivate::renameBuffer(IrcBuffer* buffer, const QString& title)
{
    Q_Q(IrcBufferModel);
    int idx = bufferList.indexOf(buffer);
    if (idx != -1) {
        const QString lower = title.toLower();

        if (!bufferMap.contains(lower)) {
            bufferMap.remove(buffer->title().toLower());
            bufferMap.insert(lower, buffer);

            QModelIndex index = q->index(idx);
            emit q->dataChanged(index, index);

            return true;
        }
    }
    return false;
}

bool IrcBufferModelPrivate::processMessage(const QString& title, IrcMessage* message, bool create)
{
    IrcBuffer* buffer = bufferMap.value(title.toLower());
    if (!buffer && create)
        buffer = addBuffer(title);
    if (buffer)
        return IrcBufferPrivate::get(buffer)->processMessage(message);
    return false;
}

void IrcBufferModelPrivate::_irc_bufferDestroyed(IrcBuffer* buffer)
{
    Q_Q(IrcBufferModel);
    int idx = bufferList.indexOf(buffer);
    if (idx != -1) {
        const bool isChannel = qobject_cast<IrcChannel*>(buffer);
        q->beginRemoveRows(QModelIndex(), idx, idx);
        bufferList.removeAt(idx);
        bufferMap.remove(buffer->title().toLower());
        if (isChannel)
            channels.removeOne(buffer->title());
        q->endRemoveRows();
        emit q->bufferRemoved(buffer);
        if (isChannel)
            emit q->channelsChanged(channels);
        emit q->buffersChanged(bufferList);
        emit q->countChanged(bufferList.count());
    }
}

/*!
    Constructs a new model with \a parent.

    \note If \a parent is an instance of IrcSession, it will be
    automatically assigned to \ref IrcBufferModel::session "session".
 */
IrcBufferModel::IrcBufferModel(QObject* parent)
    : QAbstractListModel(parent), d_ptr(new IrcBufferModelPrivate(this))
{
    setSession(qobject_cast<IrcSession*>(parent));

    qRegisterMetaType<IrcBuffer*>();
    qRegisterMetaType<QList<IrcBuffer*> >();
}

/*!
    Destructs the model.
 */
IrcBufferModel::~IrcBufferModel()
{
    Q_D(IrcBufferModel);
    foreach (IrcBuffer* buffer, d->bufferList) {
        buffer->blockSignals(true);
        delete buffer;
    }
    d->bufferList.clear();
    d->bufferMap.clear();
    d->channels.clear();
}

/*!
    This property holds the session.

    \par Access functions:
    \li \ref IrcSession* <b>session</b>() const
    \li void <b>setSession</b>(\ref IrcSession* session)

    \warning Changing the session on the fly is not supported.
 */
IrcSession* IrcBufferModel::session() const
{
    Q_D(const IrcBufferModel);
    return d->session;
}

void IrcBufferModel::setSession(IrcSession* session)
{
    Q_D(IrcBufferModel);
    if (d->session != session) {
        if (d->session)
            qFatal("IrcBufferModel::setSession(): changing the session on the fly is not supported.");
        d->session = session;
        d->session->installMessageFilter(d);
        emit sessionChanged(session);
    }
}

/*!
    This property holds the number of buffers.

    \par Access function:
    \li int <b>count</b>() const

    \par Notifier signal:
    \li void <b>countChanged</b>(int count)
 */
int IrcBufferModel::count() const
{
    return rowCount();
}

/*!
    This property holds the list of channel names.

    \par Access function:
    \li QStringList <b>channels</b>() const

    \par Notifier signal:
    \li void <b>channelsChanged</b>(const QStringList& channels)
 */
QStringList IrcBufferModel::channels() const
{
    Q_D(const IrcBufferModel);
    return d->channels;
}

/*!
    This property holds the list of buffers.

    \par Access function:
    \li QList<\ref IrcBuffer*> <b>buffers</b>() const

    \par Notifier signal:
    \li void <b>buffersChanged</b>(const QList<\ref IrcBuffer*>& buffers)
 */
QList<IrcBuffer*> IrcBufferModel::buffers() const
{
    Q_D(const IrcBufferModel);
    return d->bufferList;
}

/*!
    Returns the buffer object at \a index.
 */
IrcBuffer* IrcBufferModel::get(int index) const
{
    Q_D(const IrcBufferModel);
    return d->bufferList.value(index);
}

/*!
    Returns the buffer object for \a title.
 */
IrcBuffer* IrcBufferModel::buffer(const QString& title) const
{
    Q_D(const IrcBufferModel);
    return d->bufferMap.value(title.toLower());
}

/*!
    Returns \c true if the model contains \a title.
 */
bool IrcBufferModel::contains(const QString& title) const
{
    Q_D(const IrcBufferModel);
    return d->bufferMap.contains(title.toLower());
}

/*!
    Adds a buffer with \a title to the model and returns it.

    \note IrcBufferModel automatically keeps track of the buffers.
    Normally you do not need to manually alter the list of buffers.
 */
IrcBuffer* IrcBufferModel::addBuffer(const QString& title)
{
    Q_D(IrcBufferModel);
    return d->addBuffer(title);
}

/*!
    Removes a buffer with \a title from the model.

    \note IrcBufferModel automatically keeps track of the buffers.
    Normally you do not need to manually alter the list of buffers.
 */
void IrcBufferModel::removeBuffer(const QString& title)
{
    Q_D(IrcBufferModel);
    d->removeBuffer(title);
}

/*!
    This property holds the display role.

    The specified data role is returned for Qt::DisplayRole.

    The default value is \ref Irc::TitleRole.

    \par Access functions:
    \li \ref Irc::ItemDataRole <b>displayRole</b>() const
    \li void <b>setDisplayRole</b>(\ref Irc::ItemDataRole role)
 */
Irc::ItemDataRole IrcBufferModel::displayRole() const
{
    Q_D(const IrcBufferModel);
    return d->role;
}

void IrcBufferModel::setDisplayRole(Irc::ItemDataRole role)
{
    Q_D(IrcBufferModel);
    d->role = role;
}

/*!
    Clears the model.
 */
void IrcBufferModel::clear()
{
    Q_D(IrcBufferModel);
    if (!d->bufferList.isEmpty()) {
        beginResetModel();
        qDeleteAll(d->bufferList);
        d->bufferList.clear();
        d->bufferMap.clear();
        d->channels.clear();
        endResetModel();
        emit channelsChanged(QStringList());
        emit buffersChanged(QList<IrcBuffer*>());
        emit countChanged(0);
    }
}

/*!
    Creates a buffer object with \a title.

    IrcBufferModel will automatically call this factory method when a
    need for the buffer object occurs ie. a channel is being joined
    or a private message is received.

    The default implementation creates an instance of IrcChannel if
    \a title is detected to be a channel, and otherwise an instance
    of IrcBuffer. Reimplement this function in order to alter the
    default behavior, for example to provide a custom IrcBuffer or
    IrcChannel subclass.
 */
IrcBuffer* IrcBufferModel::createBuffer(const QString& title)
{
    Q_D(IrcBufferModel);
    IrcSessionInfo info(d->session);
    const QStringList chanTypes = info.channelTypes();
    if (!title.isEmpty() && chanTypes.contains(title.at(0)))
        return new IrcChannel(this);
    return new IrcBuffer(this);
}

/*!
    Destroys the buffer \a model.

    IrcBufferModel will automatically call this method when the buffer
    object is no longer needed ie. the user quit, a channel was left,
    or the user was kicked from the channel.

    The default implementation deletes the buffer object.
    Reimplement this function in order to alter the default behavior,
    for example to keep the buffer object alive.
 */
void IrcBufferModel::destroyBuffer(IrcBuffer* buffer)
{
    delete buffer;
}

/*!
    The following role names are provided by default:

    Role             | Name       | Type        | Example
    -----------------|------------|-------------|--------
    Qt::DisplayRole  | "display"  | 1)          | -
    Irc::BufferRole  | "buffer"   | IrcBuffer*  | &lt;object&gt;
    Irc::ChannelRole | "channel"  | IrcChannel* | &lt;object&gt;
    Irc::NameRole    | "name"     | QString     | "communi"
    Irc::PrefixRole  | "prefix"   | QString     | "#"
    Irc::TitleRole   | "title"    | QString     | "#communi"

    1) The type depends on \ref displayRole.
 */
QHash<int, QByteArray> IrcBufferModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Irc::BufferRole] = "buffer";
    roles[Irc::ChannelRole] = "channel";
    roles[Irc::NameRole] = "name";
    roles[Irc::PrefixRole] = "prefix";
    roles[Irc::TitleRole] = "title";
    return roles;
}

/*!
    Returns the number of buffers.
 */
int IrcBufferModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    Q_D(const IrcBufferModel);
    return d->bufferList.count();
}

/*!
    Returns the data for specified \a role and user referred to by by the \a index.
 */
QVariant IrcBufferModel::data(const QModelIndex& index, int role) const
{
    Q_D(const IrcBufferModel);
    if (!hasIndex(index.row(), index.column()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return data(index, d->role);
    case Irc::BufferRole:
        return QVariant::fromValue(d->bufferList.at(index.row()));
    case Irc::ChannelRole:
        return QVariant::fromValue(qobject_cast<IrcChannel*>(d->bufferList.at(index.row())));
    case Irc::NameRole:
        if (IrcBuffer* buffer =  d->bufferList.at(index.row()))
            return buffer->name();
        break;
    case Irc::PrefixRole:
        if (IrcBuffer* buffer =  d->bufferList.at(index.row()))
            return buffer->prefix();
        break;
    case Irc::TitleRole:
        if (IrcBuffer* buffer =  d->bufferList.at(index.row()))
            return buffer->title();
        break;
    }

    return QVariant();
}

#include "moc_ircbuffermodel.cpp"
