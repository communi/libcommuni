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
#include "ircsessioninfo.h"
#include "ircsession.h"
#include "ircmessage.h"
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

    \note IrcChannel does not work on its own. Use an instance managed by IrcChannelModel.

    \sa models
*/

/*!
    \fn void IrcChannel::messageReceived(IrcMessage* message)

    This signal is emitted when a channel specific message is received.

    The message may one of the following types:
    - IrcMessage::Join
    - IrcMessage::Kick
    - IrcMessage::Mode
    - IrcMessage::Names
    - IrcMessage::Nick
    - IrcMessage::Notice
    - IrcMessage::Part
    - IrcMessage::Private
    - IrcMessage::Quit
    - IrcMessage::Topic

    \sa IrcSession::messageReceived(), IrcChannelModel::messageIgnored()
 */

static QString namePrefix(const QString& name, const QStringList& prefixes)
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

void IrcChannelPrivate::init(const QString& title, IrcSession* s)
{
    const QStringList prefixes = IrcSessionInfo(s).channelTypes();
    prefix = namePrefix(title, prefixes);
    name = channelName(title, prefixes);
    session = s;
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
    IrcSessionInfo info(session);
    QHash<QString, QString> unique;
    foreach (const QString& name, names) {
        QString unprefixed = userName(name, info.prefixes());
        if (!userMap.contains(unprefixed))
            unique.insert(unprefixed, namePrefix(name, info.prefixes()));
    }

    if (!unique.isEmpty()) {
        foreach (IrcUserModel* model, models)
            model->beginInsertRows(QModelIndex(), userList.count(), userList.count() + unique.count() - 1);

        QHash<QString, QString>::const_iterator it = unique.constBegin();
        while (it != unique.constEnd()) {
            IrcUser* user = new IrcUser(q);
            IrcUserPrivate* priv = IrcUserPrivate::get(user);
            priv->setName(it.key());
            priv->setPrefix(it.value());
            priv->setMode(info.prefixToMode(it.value()));
            userList.append(user);
            userMap.insert(user->name(), user);
            foreach (IrcUserModel* model, models)
                emit model->userAdded(user);
            ++it;
        }

        const int count = userList.count();
        const QStringList names = userMap.keys();

        foreach (IrcUserModel* model, models) {
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
            foreach (IrcUserModel* model, models)
                model->beginRemoveRows(QModelIndex(), idx, idx);

            delete userList.takeAt(idx);
            userMap.remove(name);

            const int count = userList.count();
            const QStringList names = userMap.keys();

            foreach (IrcUserModel* model, models) {
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
    if (IrcUser* user = userMap.value(from)) {
        IrcUserPrivate::get(user)->setName(to);
        int idx = userList.indexOf(user);
        if (idx != -1) {
            const QStringList names = userMap.keys();
            foreach (IrcUserModel* model, models) {
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
            const IrcSessionInfo info(session);
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
            foreach (IrcUserModel* model, models) {
                QModelIndex index = model->index(idx, 0);
                emit model->dataChanged(index, index);
            }
        }
    }
}

void IrcChannelPrivate::clearUsers()
{
    if (!userList.isEmpty()) {
        foreach (IrcUserModel* model, models)
            model->beginResetModel();

        qDeleteAll(userList);
        userList.clear();
        userMap.clear();

        foreach (IrcUserModel* model, models) {
            model->endResetModel();

            emit model->namesChanged(QStringList());
            emit model->usersChanged(QList<IrcUser*>());
            emit model->countChanged(0);
        }
    }
}

bool IrcChannelPrivate::processChannelMessage(IrcMessage* message)
{
    Q_Q(IrcChannel);
    bool handled = true;
    switch (message->type()) {
    case IrcMessage::Mode: {
        IrcModeMessage* modeMsg = static_cast<IrcModeMessage*>(message);
        if (modeMsg->kind() != IrcModeMessage::Channel)
            processUserMessage(message);
        else if (modeMsg->isReply())
            setMode(modeMsg->mode());
        else
            changeMode(modeMsg->mode());
        break;
    }
    case IrcMessage::Topic:
        setTopic(static_cast<IrcTopicMessage*>(message)->topic());
        break;
    default:
        handled = processUserMessage(message);
        break;
    }
    if (handled)
        emit q->messageReceived(message);
    return handled;
}

bool IrcChannelPrivate::processUserMessage(IrcMessage* message)
{
    if (message->type() == IrcMessage::Nick) {
        const QString nick = static_cast<IrcNickMessage*>(message)->nick();
        return renameUser(message->sender().name(), nick);
    } else if (message->type() == IrcMessage::Join) {
        if (message->flags() & IrcMessage::Own)
            clearUsers();
        else
            addUsers(QStringList() << message->sender().name());
    } else if (message->type() == IrcMessage::Part || message->type() == IrcMessage::Quit) {
        if (message->flags() & IrcMessage::Own)
            clearUsers();
        else
            return removeUser(message->sender().name());
    } else if (message->type() == IrcMessage::Kick) {
        const QString user = static_cast<IrcKickMessage*>(message)->user();
        if (!user.compare(message->session()->nickName(), Qt::CaseInsensitive))
            clearUsers();
        else
            return removeUser(user);
    } else if (message->type() == IrcMessage::Mode) {
        const IrcModeMessage* modeMsg = static_cast<IrcModeMessage*>(message);
        if (!modeMsg->argument().isEmpty())
            setUserMode(modeMsg->argument(), modeMsg->mode());
    } else if (message->type() == IrcMessage::Names) {
        const QStringList names = static_cast<IrcNamesMessage*>(message)->names();
        addUsers(names);
    }
    return true;
}

/*!
    Constructs a new channel object with \a parent.

    \note IrcChannel does not work on its own. Use an instance managed by IrcChannelModel.

    \sa IrcChannelModel::channelAdded()
 */
IrcChannel::IrcChannel(QObject* parent)
    : QObject(parent), d_ptr(new IrcChannelPrivate)
{
    Q_D(IrcChannel);
    d->q_ptr = this;
    d->session = 0;
}

/*!
    Destructs the channel object.

    \sa IrcChannelModel::channelRemoved()
 */
IrcChannel::~IrcChannel()
{
    Q_D(IrcChannel);
    qDeleteAll(d->userList);
    d->userList.clear();
    d->userMap.clear();
    d->models.clear();

    emit destroyed(this);
}

/*!
    This property holds the channel title.

    The title consists of prefix and name.

    \par Access function:
    \li QString <b>title</b>() const
 */
QString IrcChannel::title() const
{
    Q_D(const IrcChannel);
    return d->prefix + d->name;
}

/*!
    This property holds the channel name.

    \par Access function:
    \li QString <b>name</b>() const
 */
QString IrcChannel::name() const
{
    Q_D(const IrcChannel);
    return d->name;
}

/*!
    This property holds the channel prefix.

    \par Access function:
    \li QString <b>prefix</b>() const
 */
QString IrcChannel::prefix() const
{
    Q_D(const IrcChannel);
    return d->prefix;
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

/*!
    This property holds the session of the channel.

    \par Access function:
    \li \ref IrcSession* <b>session</b>() const
 */
IrcSession* IrcChannel::session() const
{
    Q_D(const IrcChannel);
    return d->session;
}
