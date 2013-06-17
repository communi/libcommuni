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

#include "ircchannel.h"
#include "ircchannel_p.h"
#include "ircusermodel.h"
#include "ircbuffermodel.h"
#include "ircbuffermodel_p.h"
#include "ircsession.h"
#include "ircuser_p.h"
#include "ircsender.h"

/*!
    \file ircchannel.h
    \brief #include &lt;IrcChannel&gt;
 */

/*!
    \class IrcChannel ircchannel.h <IrcChannel>
    \ingroup models
    \brief Keeps track of channel status.

    \note IrcChannel does not work on its own. Use an instance managed by IrcBufferModel.

    \sa models
*/

static QString getPrefix(const QString& name, const QStringList& prefixes)
{
    int i = 0;
    while (i < name.length() && prefixes.contains(name.at(i)))
        ++i;
    return name.left(i);
}

static QString channelName(const QString& title, const QStringList& prefixes)
{
    int i = 0;
    while (i < title.length() && prefixes.contains(title.at(i)))
        ++i;
    return title.mid(i);
}

static QString userName(const QString& name, const QStringList& prefixes)
{
    QString copy = name;
    while (!copy.isEmpty() && prefixes.contains(copy.at(0)))
        copy.remove(0, 1);
    IrcSender sender(copy);
    if (sender.isValid())
        copy = sender.name();
    return copy;
}

IrcChannelPrivate::~IrcChannelPrivate()
{
}

void IrcChannelPrivate::init(const QString& title, IrcBufferModel* m)
{
    IrcBufferPrivate::init(title, m);

    info = IrcSessionInfo(model->session());
    const QStringList chanTypes = info.channelTypes();
    prefix = getPrefix(title, chanTypes);
    name = channelName(title, chanTypes);
}

void IrcChannelPrivate::changeMode(const QString& value)
{
    QString copy = mode;
    bool add = true;
    for (int i = 0; i < value.size(); ++i) {
        QChar c = value.at(i);
        if (c == QLatin1Char('+')) {
            add = true;
        } else if (c == QLatin1Char('-')) {
            add = false;
        } else {
            if (add) {
                if (!copy.contains(c))
                    copy += c;
            } else {
                copy.remove(c);
            }
        }
    }
    setMode(copy);
}

void IrcChannelPrivate::setMode(const QString& value)
{
    Q_Q(IrcChannel);
    if (mode != value) {
        mode = value;
        emit q->modeChanged(mode);
    }
}

void IrcChannelPrivate::setTopic(const QString& value)
{
    Q_Q(IrcChannel);
    if (topic != value) {
        topic = value;
        emit q->topicChanged(topic);
    }
}

void IrcChannelPrivate::addUsers(const QStringList& names)
{
    Q_Q(IrcChannel);
    QHash<QString, QString> unique;
    const QStringList prefixes = info.prefixes();
    foreach (const QString& name, names) {
        QString unprefixed = userName(name, prefixes);
        if (!userMap.contains(unprefixed))
            unique.insert(unprefixed, getPrefix(name, prefixes));
    }

    if (!unique.isEmpty()) {
        foreach (IrcUserModel* model, userModels)
            model->beginInsertRows(QModelIndex(), userList.count(), userList.count() + unique.count() - 1);

        QHash<QString, QString>::const_iterator it = unique.constBegin();
        while (it != unique.constEnd()) {
            IrcUser* user = new IrcUser(q);
            IrcUserPrivate* priv = IrcUserPrivate::get(user);
            priv->buffer = q;
            priv->setName(it.key());
            priv->setPrefix(it.value());
            priv->setMode(info.prefixToMode(it.value()));
            userList.append(user);
            userMap.insert(user->name(), user);
            foreach (IrcUserModel* model, userModels)
                emit model->userAdded(user);
            ++it;
        }

        const int count = userList.count();
        const QStringList names = userMap.keys();

        foreach (IrcUserModel* model, userModels) {
            model->endInsertRows();
            emit model->namesChanged(names);
            emit model->usersChanged(userList);
            emit model->countChanged(count);
        }
    }
}

bool IrcChannelPrivate::removeUser(const QString& name)
{
    if (IrcUser* user = userMap.value(name)) {
        int idx = userList.indexOf(user);
        if (idx != -1) {
            foreach (IrcUserModel* model, userModels)
                model->beginRemoveRows(QModelIndex(), idx, idx);

            delete userList.takeAt(idx);
            userMap.remove(name);

            const int count = userList.count();
            const QStringList names = userMap.keys();

            foreach (IrcUserModel* model, userModels) {
                model->endRemoveRows();
                emit model->userRemoved(user);
                emit model->namesChanged(names);
                emit model->usersChanged(userList);
                emit model->countChanged(count);
            }
            return true;
        }
    }
    return false;
}

bool IrcChannelPrivate::renameUser(const QString& from, const QString& to)
{
    if (IrcUser* user = userMap.take(from)) {
        IrcUserPrivate::get(user)->setName(to);
        int idx = userList.indexOf(user);
        if (idx != -1) {
            userMap.insert(to, user);

            const QStringList names = userMap.keys();
            foreach (IrcUserModel* model, userModels) {
                QModelIndex index = model->index(idx, 0);
                emit model->dataChanged(index, index);
                emit model->namesChanged(names);
                emit model->usersChanged(userList);
            }
            return true;
        }
    }
    return false;
}

void IrcChannelPrivate::setUserMode(const QString& name, const QString& command)
{
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
            IrcUserPrivate* priv = IrcUserPrivate::get(user);
            priv->setPrefix(prefix);
            priv->setMode(mode);
            foreach (IrcUserModel* model, userModels) {
                QModelIndex index = model->index(idx, 0);
                emit model->dataChanged(index, index);
            }
        }
    }
}

void IrcChannelPrivate::clearUsers()
{
    if (!userList.isEmpty()) {
        foreach (IrcUserModel* model, userModels)
            model->beginResetModel();

        qDeleteAll(userList);
        userList.clear();
        userMap.clear();

        foreach (IrcUserModel* model, userModels) {
            model->endResetModel();

            emit model->namesChanged(QStringList());
            emit model->usersChanged(QList<IrcUser*>());
            emit model->countChanged(0);
        }
    }
}

bool IrcChannelPrivate::processJoinMessage(IrcJoinMessage* message)
{
    if (message->flags() & IrcMessage::Own)
        clearUsers();
    else
        addUsers(QStringList() << message->sender().name());
    return true;
}

bool IrcChannelPrivate::processKickMessage(IrcKickMessage* message)
{
    if (!message->user().compare(message->session()->nickName(), Qt::CaseInsensitive)) {
        clearUsers();
        return true;
    }
    return removeUser(message->user());
}

bool IrcChannelPrivate::processModeMessage(IrcModeMessage* message)
{
    if (message->kind() == IrcModeMessage::Channel) {
        if (message->isReply())
            setMode(message->mode());
        else
            changeMode(message->mode());
        return true;
    } else if (!message->argument().isEmpty()) {
        setUserMode(message->argument(), message->mode());
    }
    return true;
}

bool IrcChannelPrivate::processNamesMessage(IrcNamesMessage* message)
{
    addUsers(message->names());
    return true;
}

bool IrcChannelPrivate::processNickMessage(IrcNickMessage* message)
{
    return renameUser(message->sender().name(), message->nick());
}

bool IrcChannelPrivate::processPartMessage(IrcPartMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        clearUsers();
        return true;
    }
    return removeUser(message->sender().name());
}

bool IrcChannelPrivate::processQuitMessage(IrcQuitMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        clearUsers();
        return true;
    }
    return removeUser(message->sender().name()) || IrcBufferPrivate::processQuitMessage(message);
}

bool IrcChannelPrivate::processTopicMessage(IrcTopicMessage* message)
{
    setTopic(message->topic());
    return true;
}

/*!
    Constructs a new channel object with \a parent.

    \note IrcChannel does not work on its own. Use an instance managed by IrcBufferModel.

    \sa IrcBufferModel::bufferAdded()
 */
IrcChannel::IrcChannel(QObject* parent)
    : IrcBuffer(*new IrcChannelPrivate, parent)
{
}

/*!
    Destructs the channel object.

    \sa IrcBufferModel::bufferRemoved()
 */
IrcChannel::~IrcChannel()
{
    Q_D(IrcChannel);
    qDeleteAll(d->userList);
    d->userList.clear();
    d->userMap.clear();
    d->userModels.clear();
}

/*!
    This property holds the channel mode.

    \par Access function:
    \li QString <b>mode</b>() const

    \par Notifier signal:
    \li void <b>modeChanged</b>(const QString& mode)
 */
QString IrcChannel::mode() const
{
    Q_D(const IrcChannel);
    return d->mode;
}

/*!
    This property holds the channel topic.

    \par Access function:
    \li QString <b>topic</b>() const

    \par Notifier signal:
    \li void <b>topicChanged</b>(const QString& topic)
 */
QString IrcChannel::topic() const
{
    Q_D(const IrcChannel);
    return d->topic;
}
