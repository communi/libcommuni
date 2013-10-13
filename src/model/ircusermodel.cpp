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

#include "ircusermodel.h"
#include "ircusermodel_p.h"
#include "ircbuffermodel.h"
#include "ircconnection.h"
#include "ircchannel_p.h"
#include "ircuser.h"
#include <qpointer.h>

IRC_BEGIN_NAMESPACE

/*!
    \file ircusermodel.h
    \brief \#include &lt;IrcUserModel&gt;
 */

/*!
    \class IrcUserModel ircusermodel.h <IrcUserModel>
    \ingroup models
    \brief Keeps track of channel users.

    In order to keep track of channel users, create an instance of IrcUserModel.
    It will notify via signals when users are added and/or removed. IrcUserModel
    can be used directly as a data model for Qt's item views - both in C++ and QML.

    \code
    void ChatView::setChannel(IrcChannel* channel)
    {
        IrcUserModel* model = new IrcUserModel(channel);
        connect(model, SIGNAL(added(IrcUser*)), this, SLOT(onUserAdded(IrcUser*)));
        connect(model, SIGNAL(removed(IrcUser*)), this, SLOT(onUserRemoved(IrcUser*)));
        nickCompleter->setModel(model);
        userListView->setModel(model);
    }
    \endcode

    \section sorting Sorting

    TODO
*/

/*!
    \fn void IrcUserModel::added(IrcUser* user)

    This signal is emitted when a \a user is added to the list of users.
 */

/*!
    \fn void IrcUserModel::removed(IrcUser* user)

    This signal is emitted when a \a user is removed from the list of users.
 */

/*!
    \fn void IrcUserModel::aboutToBeAdded(IrcUser* user)

    This signal is emitted just before a \a user is added to the list of users.
 */

/*!
    \fn void IrcUserModel::aboutToBeRemoved(IrcUser* user)

    This signal is emitted just before a \a user is removed from the list of users.
 */

#ifndef IRC_DOXYGEN
class IrcUserLessThan
{
public:
    IrcUserLessThan(IrcUserModel* model) : model(model) { }
    bool operator()(IrcUser* u1, IrcUser* u2) const { return model->lessThan(u1, u2); }
private:
    IrcUserModel* model;
};

class IrcUserGreaterThan
{
public:
    IrcUserGreaterThan(IrcUserModel* model) : model(model) { }
    bool operator()(IrcUser* u1, IrcUser* u2) const { return model->lessThan(u2, u1); }
private:
    IrcUserModel* model;
};

IrcUserModelPrivate::IrcUserModelPrivate() : q_ptr(0), role(Irc::TitleRole),
    sortMethod(Irc::SortByTitle), sortOrder(Qt::AscendingOrder), dynamicSort(false)
{
}

void IrcUserModelPrivate::addUser(IrcUser* user, bool notify)
{
    insertUser(-1, user, notify);
}

void IrcUserModelPrivate::insertUser(int index, IrcUser* user, bool notify)
{
    Q_Q(IrcUserModel);
    if (index == -1)
        index = userList.count();
    if (dynamicSort) {
        QList<IrcUser*>::iterator it;
        if (sortOrder == Qt::AscendingOrder)
            it = qUpperBound(userList.begin(), userList.end(), user, IrcUserLessThan(q));
        else
            it = qUpperBound(userList.begin(), userList.end(), user, IrcUserGreaterThan(q));
        index = it - userList.begin();
    }
    if (notify)
        emit q->aboutToBeAdded(user);
    q->beginInsertRows(QModelIndex(), index, index);
    userList.insert(index, user);
    q->endInsertRows();
    if (notify) {
        emit q->added(user);
        emit q->namesChanged(IrcChannelPrivate::get(channel)->userMap.keys());
        emit q->usersChanged(userList);
        emit q->countChanged(userList.count());
    }
}

void IrcUserModelPrivate::removeUser(IrcUser* user, bool notify)
{
    Q_Q(IrcUserModel);
    int idx = userList.indexOf(user);
    if (idx != -1) {
        if (notify)
            emit q->aboutToBeRemoved(user);
        q->beginRemoveRows(QModelIndex(), idx, idx);
        userList.removeAt(idx);
        q->endRemoveRows();
        if (notify) {
            emit q->removed(user);
            emit q->namesChanged(IrcChannelPrivate::get(channel)->userMap.keys());
            emit q->usersChanged(userList);
            emit q->countChanged(userList.count());
        }
    }
}

void IrcUserModelPrivate::setUsers(const QList<IrcUser*>& users, bool reset)
{
    Q_Q(IrcUserModel);
    if (reset)
        q->beginResetModel();
    userList = users;
    if (dynamicSort) {
        if (sortOrder == Qt::AscendingOrder)
            qSort(userList.begin(), userList.end(), IrcUserLessThan(q));
        else
            qSort(userList.begin(), userList.end(), IrcUserGreaterThan(q));
    }
    if (reset)
        q->endResetModel();
    QStringList names;
    if (channel)
        names = IrcChannelPrivate::get(channel)->userMap.keys();
    emit q->namesChanged(names);
    emit q->usersChanged(userList);
    emit q->countChanged(userList.count());
}

void IrcUserModelPrivate::renameUser(IrcUser* user)
{
    Q_Q(IrcUserModel);
    const int idx = userList.indexOf(user);
    if (idx != -1) {
        QModelIndex index = q->index(idx, 0);
        emit q->dataChanged(index, index);

        if (dynamicSort) {
            QList<IrcUser*> users = userList;
            const bool notify = false;
            removeUser(user, notify);
            insertUser(-1, user, notify);
            if (users != userList)
                emit q->usersChanged(userList);
        }
    }
}

void IrcUserModelPrivate::setUserMode(IrcUser* user)
{
    Q_Q(IrcUserModel);
    const int idx = userList.indexOf(user);
    if (idx != -1) {
        QModelIndex index = q->index(idx, 0);
        emit q->dataChanged(index, index);

        if (dynamicSort && sortMethod == Irc::SortByTitle) {
            const bool notify = false;
            removeUser(user, notify);
            insertUser(0, user, notify);
            emit q->usersChanged(userList);
        }
    }
}

void IrcUserModelPrivate::promoteUser(IrcUser* user)
{
    Q_Q(IrcUserModel);
    if (dynamicSort && sortMethod == Irc::SortByActivity) {
        const bool notify = false;
        removeUser(user, notify);
        insertUser(0, user, notify);
        emit q->usersChanged(userList);
    }
}
#endif // IRC_DOXYGEN

/*!
    Constructs a new model with \a parent.

    \note If \a parent is an instance of IrcChannel, it will be
    automatically assigned to \ref IrcUserModel::channel "channel".
 */
IrcUserModel::IrcUserModel(QObject* parent) : QAbstractListModel(parent), d_ptr(new IrcUserModelPrivate)
{
    Q_D(IrcUserModel);
    d->q_ptr = this;
    setChannel(qobject_cast<IrcChannel*>(parent));

    qRegisterMetaType<IrcUser*>();
    qRegisterMetaType<QList<IrcUser*> >();
}

/*!
    Destructs the model.
 */
IrcUserModel::~IrcUserModel()
{
    Q_D(IrcUserModel);
    if (d->channel)
        IrcChannelPrivate::get(d->channel)->userModels.removeOne(this);
}

/*!
    This property holds the channel.

    \par Access functions:
    \li \ref IrcChannel* <b>channel</b>() const
    \li void <b>setChannel</b>(\ref IrcChannel* channel)
 */
IrcChannel* IrcUserModel::channel() const
{
    Q_D(const IrcUserModel);
    return d->channel;
}

void IrcUserModel::setChannel(IrcChannel* channel)
{
    Q_D(IrcUserModel);
    if (d->channel != channel) {
        beginResetModel();
        if (d->channel)
            IrcChannelPrivate::get(d->channel)->userModels.removeOne(this);

        d->channel = channel;

        QList<IrcUser*> users;
        if (d->channel) {
            IrcChannelPrivate::get(d->channel)->userModels.append(this);
            if (d->sortMethod == Irc::SortByActivity)
                users = IrcChannelPrivate::get(d->channel)->activeUsers;
            else
                users = IrcChannelPrivate::get(d->channel)->userList;
        }
        const bool reset = false;
        d->setUsers(users, reset);
        endResetModel();

        emit channelChanged(channel);
    }
}

/*!
    This property holds the number of users on the channel.

    \par Access function:
    \li int <b>count</b>() const

    \par Notifier signal:
    \li void <b>countChanged</b>(int count)
 */
int IrcUserModel::count() const
{
    return rowCount();
}

/*!
    This property holds the list of names in alphabetical order.

    \par Access function:
    \li QStringList <b>names</b>() const

    \par Notifier signal:
    \li void <b>namesChanged</b>(const QStringList& names)
 */
QStringList IrcUserModel::names() const
{
    Q_D(const IrcUserModel);
    if (d->channel && !d->userList.isEmpty())
        return IrcChannelPrivate::get(d->channel)->userMap.keys();
    return QStringList();
}

/*!
    This property holds the list of users.

    The order of users is kept as sent from the server.

    \par Access function:
    \li QList<\ref IrcUser*> <b>users</b>() const

    \par Notifier signal:
    \li void <b>usersChanged</b>(const QList<\ref IrcUser*>& users)
 */
QList<IrcUser*> IrcUserModel::users() const
{
    Q_D(const IrcUserModel);
    return d->userList;
}

/*!
    Returns the user object at \a index.
 */
IrcUser* IrcUserModel::get(int index) const
{
    Q_D(const IrcUserModel);
    return d->userList.value(index);
}

/*!
    Returns the user object for \a name or \c 0 if not found.
 */
IrcUser* IrcUserModel::find(const QString& name) const
{
    Q_D(const IrcUserModel);
    if (d->channel && !d->userList.isEmpty())
        return IrcChannelPrivate::get(d->channel)->userMap.value(name);
    return 0;
}

/*!
    Returns \c true if the model contains \a name.
 */
bool IrcUserModel::contains(const QString& name) const
{
    Q_D(const IrcUserModel);
    if (d->channel && !d->userList.isEmpty())
        return IrcChannelPrivate::get(d->channel)->userMap.contains(name);
    return false;
}

/*!
    Returns the index of the specified \a user,
    or \c -1 if the model does not contain the \a user.
 */
int IrcUserModel::indexOf(IrcUser* user) const
{
    Q_D(const IrcUserModel);
    return d->userList.indexOf(user);
}

/*!
    This property holds the model sort method.

    The default value is \c Irc::SortByTitle.

    Method              | Description                                                                                       | Example
    --------------------|---------------------------------------------------------------------------------------------------|----------------------------------------------
    Irc::SortByName     | Users are sorted alphabetically, ignoring any mode prefix.                                        | "bot", "@ChanServ", "jpnurmi", "+qtassistant"
    Irc::SortByTitle    | Users are sorted alphabetically, and special users (operators, voiced users) before normal users. | "@ChanServ", "+qtassistant", "bot", "jpnurmi"
    Irc::SortByActivity | Users are sorted based on their activity, last active and mentioned (1) users first.              | -

    1) For performance reasons, IrcUserModel does \b not scan the whole channel
       messages to find out if a channel user was mentioned. IrcUserModel merely
       checks if channel messages \b begin with the name of a user in the model.

    \par Access functions:
    \li Irc::SortMethod <b>sortMethod</b>() const
    \li void <b>setSortMethod</b>(Irc::SortMethod method)

    \sa sort(), lessThan(), dynamicSort
 */
Irc::SortMethod IrcUserModel::sortMethod() const
{
    Q_D(const IrcUserModel);
    return d->sortMethod;
}

void IrcUserModel::setSortMethod(Irc::SortMethod method)
{
    Q_D(IrcUserModel);
    if (d->sortMethod != method) {
        d->sortMethod = method;
        if (method == Irc::SortByActivity && d->channel)
            d->userList = IrcChannelPrivate::get(d->channel)->activeUsers;
        if (d->dynamicSort && !d->userList.isEmpty())
            sort(0, d->sortOrder);
    }
}

/*!
    This property holds whether the model is dynamically sorted.

    The default value is \c false.

    \par Access functions:
    \li bool <b>dynamicSort</b>() const
    \li void <b>setDynamicSort</b>(bool dynamic)

    \sa sort(), lessThan(), sortMethod
 */
bool IrcUserModel::dynamicSort() const
{
    Q_D(const IrcUserModel);
    return d->dynamicSort;
}

void IrcUserModel::setDynamicSort(bool dynamic)
{
    Q_D(IrcUserModel);
    if (d->dynamicSort != dynamic) {
        d->dynamicSort = dynamic;
        if (d->dynamicSort && !d->userList.isEmpty())
            sort(0, d->sortOrder);
    }
}

/*!
    This property holds the display role.

    The specified data role is returned for Qt::DisplayRole.

    The default value is \ref Irc::TitleRole.

    \par Access functions:
    \li \ref Irc::DataRole <b>displayRole</b>() const
    \li void <b>setDisplayRole</b>(\ref Irc::DataRole role)
 */
Irc::DataRole IrcUserModel::displayRole() const
{
    Q_D(const IrcUserModel);
    return d->role;
}

void IrcUserModel::setDisplayRole(Irc::DataRole role)
{
    Q_D(IrcUserModel);
    d->role = role;
}

/*!
    Returns the model index for \a user.
 */
QModelIndex IrcUserModel::index(IrcUser* user) const
{
    Q_D(const IrcUserModel);
    return index(d->userList.indexOf(user));
}

/*!
    Returns the user for model \a index.
 */
IrcUser* IrcUserModel::user(const QModelIndex& index) const
{
    if (!hasIndex(index.row(), index.column()))
        return 0;

    return static_cast<IrcUser*>(index.internalPointer());
}

/*!
    The following role names are provided by default:

    Role            | Name      | Type     | Example
    --------------- | ----------|----------|--------
    Qt::DisplayRole | "display" | 1)       | -
    Irc::UserRole   | "user"    | IrcUser* | &lt;object&gt;
    Irc::NameRole   | "name"    | QString  | "jpnurmi"
    Irc::PrefixRole | "prefix"  | QString  | "@"
    Irc::ModeRole   | "mode"    | QString  | "o"
    Irc::TitleRole  | "title"   | QString  | "@jpnurmi"

    1) The type depends on \ref displayRole.
 */
QHash<int, QByteArray> IrcUserModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Irc::UserRole] = "user";
    roles[Irc::NameRole] = "name";
    roles[Irc::PrefixRole] = "prefix";
    roles[Irc::ModeRole] = "mode";
    roles[Irc::TitleRole] = "title";
    return roles;
}

/*!
    Returns the number of users on the channel.
 */
int IrcUserModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const IrcUserModel);
    if (parent.isValid() || !d->channel)
        return 0;

    return d->userList.count();
}

/*!
    Returns the data for specified \a role referred to by the \a index.

    \sa Irc::DataRole, roleNames()
 */
QVariant IrcUserModel::data(const QModelIndex& index, int role) const
{
    Q_D(const IrcUserModel);
    if (!d->channel || !hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();

    IrcUser* user = static_cast<IrcUser*>(index.internalPointer());
    Q_ASSERT(user);

    switch (role) {
    case Qt::DisplayRole:
        return data(index, d->role);
    case Irc::UserRole:
        return QVariant::fromValue(user);
    case Irc::NameRole:
        return user->name();
    case Irc::PrefixRole:
        return user->prefix().left(1);
    case Irc::ModeRole:
        return user->mode().left(1);
    case Irc::TitleRole:
        return user->title();
    }

    return QVariant();
}

/*!
    Returns the index of the item in the model specified by the given \a row, \a column and \a parent index.
 */
QModelIndex IrcUserModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_D(const IrcUserModel);
    if (!d->channel || !hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, d->userList.value(row));
}

/*!
    Clears the model.
 */
void IrcUserModel::clear()
{
    Q_D(IrcUserModel);
    if (!d->userList.isEmpty()) {
        beginResetModel();
        d->userList.clear();
        endResetModel();
        emit namesChanged(QStringList());
        emit usersChanged(QList<IrcUser*>());
        emit countChanged(0);
    }
}

/*!
    Sorts the model in the given \a order.

    \sa lessThan(), dynamicSort
 */
void IrcUserModel::sort(int column, Qt::SortOrder order)
{
    Q_D(IrcUserModel);
    if (column != 0)
        return;

    emit layoutAboutToBeChanged();

    QList<IrcUser*> persistentUsers;
    QModelIndexList oldPersistentIndexes = persistentIndexList();
    foreach (const QModelIndex& index, oldPersistentIndexes)
        persistentUsers += static_cast<IrcUser*>(index.internalPointer());

    d->sortOrder = order;
    if (order == Qt::AscendingOrder)
        qSort(d->userList.begin(), d->userList.end(), IrcUserLessThan(this));
    else
        qSort(d->userList.begin(), d->userList.end(), IrcUserGreaterThan(this));

    QModelIndexList newPersistentIndexes;
    foreach (IrcUser* user, persistentUsers)
        newPersistentIndexes += index(d->userList.indexOf(user));
    changePersistentIndexList(oldPersistentIndexes, newPersistentIndexes);

    emit layoutChanged();
}

/*!
    Returns \c true if \a one buffer is "less than" \a another,
    otherwise returns \c false.

    The default implementation sorts according to the specified sort method.
    Reimplement this function in order to customize the sort order.

    \sa sort(), dynamicSort, sortMethod
 */
bool IrcUserModel::lessThan(IrcUser* one, IrcUser* another) const
{
    Q_D(const IrcUserModel);
    if (d->sortMethod == Irc::SortByActivity) {
        QList<IrcUser*> activeUsers = IrcChannelPrivate::get(one->channel())->activeUsers;
        const int i1 = activeUsers.indexOf(one);
        const int i2 = activeUsers.indexOf(another);
        return i1 < i2;
    } else if (d->sortMethod == Irc::SortByTitle) {
        const IrcNetwork* network = one->channel()->network();
        const QStringList prefixes = network->prefixes();

        const QString p1 = one->prefix();
        const QString p2 = another->prefix();

        const int i1 = !p1.isEmpty() ? prefixes.indexOf(p1.at(0)) : -1;
        const int i2 = !p2.isEmpty() ? prefixes.indexOf(p2.at(0)) : -1;

        if (i1 >= 0 && i2 < 0)
            return true;
        if (i1 < 0 && i2 >= 0)
            return false;
        if (i1 >= 0 && i2 >= 0 && i1 != i2)
            return i1 < i2;
    }

    // Irc::SortByName
    const QString n1 = one->name();
    const QString n2 = another->name();
    return n1.compare(n2, Qt::CaseInsensitive) < 0;
}

#include "moc_ircusermodel.cpp"

IRC_END_NAMESPACE
