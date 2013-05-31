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
#include "ircsessioninfo.h"
#include "ircsession.h"
#include "ircmessage.h"
#include "ircuser_p.h"

/*!
    \file ircusermodel.h
    \brief #include &lt;IrcUserModel&gt;
 */

/*!
    \class IrcUserModel ircusermodel.h <IrcUserModel>
    \ingroup models
    \brief Keeps track of channel users.

    \note IrcUserModel does not work on its own. Use an instance managed by IrcChannel.

    \section sorting Sorting

    The order of \ref users is either kepts as sent from the server,
    or \ref activitySortEnabled "sorted by activity". Furthermore,
    an alphabetical list of \ref names is provided for convenience.

    Whilst these available options cater basic use cases, for more fine-
    grained sorting it is recommended to implement a proxy model. The
    following example illustrates how to sort users by prefix and name.

    \code
    class IrcUserProxyModel : public QSortFilterProxyModel
    {
    public:
        IrcUserProxyModel(IrcUserModel* userModel) : QSortFilterProxyModel(userModel)
        {
            setSourceModel(userModel);
        }

        // IrcSessionInfo::prefixes()
        void sortByPrefixes(const QStringList& prefixes)
        {
            m_prefixes = prefixes;
            sort(0, Qt::AscendingOrder);
            setDynamicSortFilter(true);
        }

    protected:
        bool lessThan(const QModelIndex& left, const QModelIndex& right) const
        {
            const QString p1 = left.data(Irc::PrefixRole).toString();
            const QString p2 = right.data(Irc::PrefixRole).toString();

            const int i1 = !p1.isEmpty() ? m_prefixes.indexOf(p1.at(0)) : -1;
            const int i2 = !p2.isEmpty() ? m_prefixes.indexOf(p2.at(0)) : -1;

            if (i1 >= 0 && i2 < 0)
                return true;
            if (i1 < 0 && i2 >= 0)
                return false;
            if (i1 >= 0 && i2 >= 0 && i1 != i2)
                return i1 < i2;

            const QString n1 = left.data(Irc::NameRole).toString();
            const QString n2 = right.data(Irc::NameRole).toString();
            return n1.compare(n2, Qt::CaseInsensitive) < 0;
        }

    private:
        QStringList m_prefixes;
    };
    \endcode

    \sa models
*/

/*!
    \enum IrcUserModel::DataRole
    This enum describes the available data roles.
    \sa roleNames()
 */

/*!
    \var IrcUserModel::UserRole
    \brief User object (IrcUser*)
 */

/*!
    \var IrcUserModel::NameRole
    \brief User name (QString)
 */

/*!
    \var IrcUserModel::PrefixRole
    \brief User prefix (QString)
 */

/*!
    \var IrcUserModel::ModeRole
    \brief User mode (QString)
 */

/*!
    \var IrcUserModel::TitleRole
    \brief User mode and name (QString)
 */

/*!
    \fn void IrcUserModel::userAdded(IrcUser* user)

    This signal is emitted when a \a user is added to the list of users.
 */

/*!
    \fn void IrcUserModel::userRemoved(IrcUser* user)

    This signal is emitted when a \a user is removed from the list of users.
 */

class IrcUserModelPrivate
{
    Q_DECLARE_PUBLIC(IrcUserModel)

public:
    void addUsers(const QStringList& users);
    void removeUser(const QString& user);
    void renameUser(const QString& from, const QString& to);
    void promoteUser(const QString& user);
    void setUserMode(const QString& user, const QString& mode);

    void _irc_processMessage(IrcMessage* message);

    IrcUserModel* q_ptr;
    bool activitySort;
    Irc::ItemDataRole role;
    QList<IrcUser*> userList;
    QMap<QString, IrcUser*> userMap;
    IrcSessionInfo info;
};

static QString namePrefix(const QString& name, const QStringList& prefixes)
{
    int i = 0;
    while (i < name.length() && prefixes.contains(name.at(i)))
        ++i;
    return name.left(i);
}

static QString unprefixedName(const QString& name, const QStringList& prefixes)
{
    QString copy = name;
    while (!copy.isEmpty() && prefixes.contains(copy.at(0)))
        copy.remove(0, 1);
    IrcSender sender(copy);
    if (sender.isValid())
        copy = sender.name();
    return copy;
}

void IrcUserModelPrivate::addUsers(const QStringList& names)
{
    Q_Q(IrcUserModel);
    QHash<QString, QString> unique;
    foreach (const QString& name, names) {
        QString unprefixed = unprefixedName(name, info.prefixes());
        if (!userMap.contains(unprefixed))
            unique.insert(unprefixed, namePrefix(name, info.prefixes()));
    }

    if (!unique.isEmpty()) {
        if (activitySort)
            q->beginInsertRows(QModelIndex(), 0, unique.count() - 1);
        else
            q->beginInsertRows(QModelIndex(), userList.count(), userList.count() + unique.count() - 1);
        QHash<QString, QString>::const_iterator it = unique.constBegin();
        while (it != unique.constEnd()) {
            IrcUser* user = new IrcUser(q);
            IrcUserPrivate* priv = user->d_func();
            priv->setName(it.key());
            priv->setPrefix(it.value());
            priv->setMode(info.prefixToMode(it.value()));
            if (activitySort)
                userList.prepend(user);
            else
                userList.append(user);
            userMap.insert(user->name(), user);
            emit q->userAdded(user);
            ++it;
        }
        q->endInsertRows();
        emit q->namesChanged(q->names());
        emit q->usersChanged(q->users());
        emit q->countChanged(q->count());
    }
}

void IrcUserModelPrivate::removeUser(const QString& name)
{
    Q_Q(IrcUserModel);
    if (IrcUser* user = userMap.value(name)) {
        int idx = userList.indexOf(user);
        if (idx != -1) {
            q->beginRemoveRows(QModelIndex(), idx, idx);
            delete userList.takeAt(idx);
            userMap.remove(name);
            q->endRemoveRows();
            emit q->userRemoved(user);
            emit q->namesChanged(q->names());
            emit q->usersChanged(q->users());
            emit q->countChanged(q->count());
        }
    }
}

void IrcUserModelPrivate::renameUser(const QString& from, const QString& to)
{
    Q_Q(IrcUserModel);
    if (IrcUser* user = userMap.value(from)) {
        user->d_func()->setName(to);
        int idx = userList.indexOf(user);
        if (idx != -1) {
            if (activitySort) {
                q->beginRemoveRows(QModelIndex(), idx, idx);
                userList.removeAt(idx);
                userMap.remove(from);
                q->endRemoveRows();

                q->beginInsertRows(QModelIndex(), 0, 0);
                userList.prepend(user);
                userMap.insert(to, user);
                q->endInsertRows();
            } else {
                emit q->dataChanged(q->index(idx, 0), q->index(idx, 0));
            }
            emit q->namesChanged(q->names());
            emit q->usersChanged(q->users());
        }
    }
}

void IrcUserModelPrivate::promoteUser(const QString& name)
{
    Q_Q(IrcUserModel);
    if (IrcUser* user = userMap.value(name)) {
        int idx = userList.indexOf(user);
        if (idx != -1) {
            // TODO: fix QCompleter to listen QAIM::rowsMoved()
            //q->beginMoveRows(QModelIndex(), idx, idx, QModelIndex(), 0);
            //names.move(idx, 0);
            //q->endMoveRows();

            q->beginRemoveRows(QModelIndex(), idx, idx);
            userList.removeAt(idx);
            q->endRemoveRows();

            q->beginInsertRows(QModelIndex(), 0, 0);
            userList.prepend(user);
            q->endInsertRows();

            emit q->namesChanged(q->names());
            emit q->usersChanged(q->users());
        }
    }
}

void IrcUserModelPrivate::setUserMode(const QString& name, const QString& command)
{
    Q_Q(IrcUserModel);
    if (IrcUser* user = userMap.value(name)) {
        int idx = userList.indexOf(user);
        if (idx != -1) {
            bool add = true;
            QString mode = user->mode();
            QString prefix = user->prefix();
            for (int i = 0; i < command.size(); ++i) {
                QChar c = command.at(i);
                if (c == QLatin1Char('+')) {
                    add = true;
                } else if (c == QLatin1Char('-')) {
                    add = false;
                } else {
                    QString p = info.modeToPrefix(c);
                    if (add) {
                        if (!mode.contains(c))
                            mode += c;
                        if (!prefix.contains(p))
                            prefix += p;
                    } else {
                        mode.remove(c);
                        prefix.remove(p);
                    }
                }
            }
            IrcUserPrivate* priv = user->d_func();
            priv->setPrefix(prefix);
            priv->setMode(mode);
            emit q->dataChanged(q->index(idx, 0), q->index(idx, 0));
        }
    }
}

void IrcUserModelPrivate::_irc_processMessage(IrcMessage* message)
{
    Q_Q(IrcUserModel);
    if (!info.isValid())
        info = IrcSessionInfo(message->session());

    if (message->type() == IrcMessage::Nick) {
        const QString nick = static_cast<IrcNickMessage*>(message)->nick();
        renameUser(message->sender().name(), nick);
    } else if (message->type() == IrcMessage::Join) {
        if (message->flags() & IrcMessage::Own)
            q->clear();
        else
            addUsers(QStringList() << message->sender().name());
    } else if (message->type() == IrcMessage::Part || message->type() == IrcMessage::Quit) {
        if (message->flags() & IrcMessage::Own)
            q->clear();
        else
            removeUser(message->sender().name());
    } else if (message->type() == IrcMessage::Kick) {
        const QString user = static_cast<IrcKickMessage*>(message)->user();
        if (!user.compare(message->session()->nickName(), Qt::CaseInsensitive))
            q->clear();
        else
            removeUser(user);
    } else if (message->type() == IrcMessage::Mode) {
        const IrcModeMessage* modeMsg = static_cast<IrcModeMessage*>(message);
        if (!modeMsg->argument().isEmpty())
            setUserMode(modeMsg->argument(), modeMsg->mode());
    } else if (message->type() == IrcMessage::Names) {
        const QStringList names = static_cast<IrcNamesMessage*>(message)->names();
        addUsers(names);
    } else if (activitySort) {
        promoteUser(message->sender().name());
    }
}

Q_DECLARE_METATYPE(IrcUser*)
Q_DECLARE_METATYPE(QList<IrcUser*>)

/*!
    Constructs a new model with \a parent.

    \note IrcUserModel does not work on its own. Use an instance managed by IrcChannel.
 */
IrcUserModel::IrcUserModel(QObject* parent)
    : QAbstractListModel(parent), d_ptr(new IrcUserModelPrivate)
{
    Q_D(IrcUserModel);
    d->q_ptr = this;
    d->activitySort = false;
    d->role = Irc::TitleRole;

    qRegisterMetaType<IrcUser*>();
    qRegisterMetaType<QList<IrcUser*> >();
}

/*!
    Destructs the model.

    \warning Do not delete an instance managed by IrcChannel.
 */
IrcUserModel::~IrcUserModel()
{
    Q_D(IrcUserModel);
    qDeleteAll(d->userList);
    d->userList.clear();
    d->userMap.clear();
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
    return d->userMap.keys();
}

/*!
    This property holds the list of users.

    The list order depends on the value of \ref activitySortEnabled.

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
    Returns the user object for \a name.
 */
IrcUser* IrcUserModel::user(const QString& name) const
{
    Q_D(const IrcUserModel);
    return d->userMap.value(name);
}

/*!
    Returns \c true if the model contains \a name.
 */
bool IrcUserModel::contains(const QString& name) const
{
    Q_D(const IrcUserModel);
    return d->userMap.contains(name);
}

/*!
    This property holds the display role.

    The specified data role is returned for Qt::DisplayRole.

    The default value is \ref Irc::TitleRole.

    \par Access functions:
    \li \ref Irc::ItemDataRole <b>displayRole</b>() const
    \li void <b>setDisplayRole</b>(\ref Irc::ItemDataRole role)
 */
Irc::ItemDataRole IrcUserModel::displayRole() const
{
    Q_D(const IrcUserModel);
    return d->role;
}

void IrcUserModel::setDisplayRole(Irc::ItemDataRole role)
{
    Q_D(IrcUserModel);
    d->role = role;
}

/*!
    \property bool IrcUserModel::activitySortEnabled

    This property holds whether users are sorted by activity.

    The default value is \c false, and the order of users is
    kept as sent from the server.

    When enabled, active users will be promoted to the top of
    the users list. This is particularly useful for nick completion
    models and such, that want to prioritize last active users.

    \par Access functions:
    \li bool <b>isActivitySortEnabled</b>() const
    \li void <b>setActivitySortEnabled</b>(bool enabled)

    \par Notifier signal:
    \li void <b>activitySortEnabledChanged</b>(bool enabled)
 */
bool IrcUserModel::isActivitySortEnabled() const
{
    Q_D(const IrcUserModel);
    return d->activitySort;
}

void IrcUserModel::setActivitySortEnabled(bool enabled)
{
    Q_D(IrcUserModel);
    if (d->activitySort != enabled) {
        d->activitySort = enabled;
        emit activitySortEnabledChanged(enabled);
    }
}

/*!
    Clears the model.
 */
void IrcUserModel::clear()
{
    Q_D(IrcUserModel);
    if (!d->userList.isEmpty()) {
        beginResetModel();
        qDeleteAll(d->userList);
        d->userList.clear();
        d->userMap.clear();
        endResetModel();
    }
}

/*!
    The following role names are provided by default:

    Role            | Name      | Type     | Example
    --------------- | ----------|----------|--------
    Qt::DisplayRole | "display" | 1)       | -
    Irc::UserRole   | "user"    | IrcUser* | <object>
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
    if (parent.isValid())
        return 0;

    Q_D(const IrcUserModel);
    return d->userList.count();
}

/*!
    Returns the data for specified \a role referred to by the \a index.

    \sa IrcUserModel::DataRole, roleNames()
 */
QVariant IrcUserModel::data(const QModelIndex& index, int role) const
{
    Q_D(const IrcUserModel);
    if (!hasIndex(index.row(), index.column()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return data(index, d->role);
    case Irc::UserRole:
        return QVariant::fromValue(d->userList.at(index.row()));
    case Irc::NameRole:
        if (IrcUser* user = d->userList.at(index.row()))
            return user->name();
        break;
    case Irc::PrefixRole:
        if (IrcUser* user = d->userList.at(index.row()))
            return user->prefix();
        break;
    case Irc::ModeRole:
        if (IrcUser* user = d->userList.at(index.row()))
            return user->mode();
        break;
    case Irc::TitleRole:
        if (IrcUser* user = d->userList.at(index.row()))
            return user->prefix() + user->name();
        break;
    }

    return QVariant();
}

#include "moc_ircusermodel.cpp"
