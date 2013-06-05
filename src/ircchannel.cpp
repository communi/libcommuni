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
    - IrcMessage::Numeric
    - IrcMessage::Part
    - IrcMessage::Private
    - IrcMessage::Quit
    - IrcMessage::Topic

    \sa IrcSession::messageReceived(), IrcUser::messageReceived(), IrcChannelModel::messageIgnored()
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

void IrcChannelPrivate::addUsers(const QStringList& names, IrcMessage* message)
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
            if (message)
                priv->receiveMessage(message);
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

bool IrcChannelPrivate::removeUser(const QString& name, IrcMessage* message)
{
    if (IrcUser* user = userMap.value(name)) {
        int idx = userList.indexOf(user);
        if (idx != -1) {
            IrcUserPrivate::get(user)->receiveMessage(message);

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

bool IrcChannelPrivate::renameUser(const QString& from, const QString& to, IrcMessage* message)
{
    if (IrcUser* user = userMap.value(from)) {
        IrcUserPrivate::get(user)->setName(to);
        int idx = userList.indexOf(user);
        if (idx != -1) {
            IrcUserPrivate::get(user)->receiveMessage(message);

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

void IrcChannelPrivate::setUserMode(const QString& name, const QString& command, IrcMessage* message)
{
    if (IrcUser* user = userMap.value(name)) {
        int idx = userList.indexOf(user);
        if (idx != -1) {
            IrcUserPrivate::get(user)->receiveMessage(message);

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

bool IrcChannelPrivate::processMessage(IrcMessage* message)
{
    Q_Q(IrcChannel);
    bool processed = false;
    switch (message->type()) {
    case IrcMessage::Join:
        processed = processJoinMessage(static_cast<IrcJoinMessage*>(message));
        break;
    case IrcMessage::Kick:
        processed = processKickMessage(static_cast<IrcKickMessage*>(message));
        break;
    case IrcMessage::Mode:
        processed = processModeMessage(static_cast<IrcModeMessage*>(message));
        break;
    case IrcMessage::Names:
        processed = processNamesMessage(static_cast<IrcNamesMessage*>(message));
        break;
    case IrcMessage::Nick:
        processed = processNickMessage(static_cast<IrcNickMessage*>(message));
        break;
    case IrcMessage::Notice:
        processed = processNoticeMessage(static_cast<IrcNoticeMessage*>(message));
        break;
    case IrcMessage::Numeric:
        processed = processNumericMessage(static_cast<IrcNumericMessage*>(message));
        break;
    case IrcMessage::Part:
        processed = processPartMessage(static_cast<IrcPartMessage*>(message));
        break;
    case IrcMessage::Private:
        processed = processPrivateMessage(static_cast<IrcPrivateMessage*>(message));
        break;
    case IrcMessage::Quit:
        processed = processQuitMessage(static_cast<IrcQuitMessage*>(message));
        break;
    case IrcMessage::Topic:
        processed = processTopicMessage(static_cast<IrcTopicMessage*>(message));
        break;
    default:
        break;
    }
    if (processed)
        emit q->messageReceived(message);
    return processed;
}

bool IrcChannelPrivate::processJoinMessage(IrcJoinMessage* message)
{
    if (message->flags() & IrcMessage::Own)
        clearUsers();
    else
        addUsers(QStringList() << message->sender().name(), message);
    return true;
}

bool IrcChannelPrivate::processKickMessage(IrcKickMessage* message)
{
    if (!message->user().compare(message->session()->nickName(), Qt::CaseInsensitive)) {
        clearUsers();
        return true;
    }
    return removeUser(message->user(), message);
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
        setUserMode(message->argument(), message->mode(), message);
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
    return renameUser(message->sender().name(), message->nick(), message);
}

bool IrcChannelPrivate::processNoticeMessage(IrcNoticeMessage* message)
{
    IrcUser* user = userMap.value(message->sender().name());
    if (user)
        IrcUserPrivate::get(user)->receiveMessage(message);
    return true;
}

bool IrcChannelPrivate::processNumericMessage(IrcNumericMessage* message)
{
    Q_UNUSED(message);
    return true;
}

bool IrcChannelPrivate::processPartMessage(IrcPartMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        clearUsers();
        return true;
    }
    return removeUser(message->sender().name(), message);
}

bool IrcChannelPrivate::processPrivateMessage(IrcPrivateMessage* message)
{
    IrcUser* user = userMap.value(message->sender().name());
    if (user)
        IrcUserPrivate::get(user)->receiveMessage(message);
    return true;
}

bool IrcChannelPrivate::processQuitMessage(IrcQuitMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        clearUsers();
        return true;
    }
    return removeUser(message->sender().name(), message);
}

bool IrcChannelPrivate::processTopicMessage(IrcTopicMessage* message)
{
    setTopic(message->topic());
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
