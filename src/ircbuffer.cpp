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

#include "ircbuffer.h"
#include "ircbuffer_p.h"
#include "ircusermodel.h"
#include "ircsessioninfo.h"
#include "ircsession.h"
#include "ircuser_p.h"
#include "ircsender.h"

/*!
    \file ircbuffer.h
    \brief #include &lt;IrcBuffer&gt;
 */

/*!
    \class IrcBuffer ircbuffer.h <IrcBuffer>
    \ingroup models
    \brief Keeps track of buffer status.

    \note IrcBuffer does not work on its own. Use an instance managed by IrcBufferModel.

    \sa models
*/

/*!
    \fn void IrcBuffer::messageReceived(IrcMessage* message)

    This signal is emitted when a buffer specific message is received.

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

    \sa IrcSession::messageReceived(), IrcUser::messageReceived(), IrcBufferModel::messageIgnored()
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

IrcBufferPrivate::IrcBufferPrivate(IrcBuffer* q)
    : q_ptr(q), session(0), type(Irc::Channel)
{
}

void IrcBufferPrivate::init(const QString& title, IrcSession* s)
{
    const QStringList chanTypes = IrcSessionInfo(s).channelTypes();
    if (chanTypes.contains(title.at(0))) {
        type = Irc::Channel;
        prefix = getPrefix(title, chanTypes);
        name = channelName(title, chanTypes);
    } else {
        type = Irc::Query;
        prefix.clear();
        setName(userName(title, IrcSessionInfo(s).prefixes()));
    }
    session = s;
}

void IrcBufferPrivate::changeMode(const QString& value)
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

void IrcBufferPrivate::setMode(const QString& value)
{
    Q_Q(IrcBuffer);
    if (mode != value) {
        mode = value;
        emit q->modeChanged(mode);
    }
}

void IrcBufferPrivate::setTopic(const QString& value)
{
    Q_Q(IrcBuffer);
    if (topic != value) {
        topic = value;
        emit q->topicChanged(topic);
    }
}

void IrcBufferPrivate::setName(const QString& value)
{
    Q_Q(IrcBuffer);
    if (name != value) {
        name = value;
        emit q->nameChanged(name);
        emit q->titleChanged(q->title());
    }
}

void IrcBufferPrivate::addUsers(const QStringList& names)
{
    Q_Q(IrcBuffer);
    IrcSessionInfo info(session);
    QHash<QString, QString> unique;
    foreach (const QString& name, names) {
        QString unprefixed = userName(name, info.prefixes());
        if (!userMap.contains(unprefixed))
            unique.insert(unprefixed, getPrefix(name, info.prefixes()));
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

bool IrcBufferPrivate::removeUser(const QString& name)
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

bool IrcBufferPrivate::renameUser(const QString& from, const QString& to)
{
    if (IrcUser* user = userMap.take(from)) {
        IrcUserPrivate::get(user)->setName(to);
        int idx = userList.indexOf(user);
        if (idx != -1) {
            userMap.insert(to, user);

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

void IrcBufferPrivate::setUserMode(const QString& name, const QString& command)
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

void IrcBufferPrivate::clearUsers()
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

bool IrcBufferPrivate::processMessage(IrcMessage* message)
{
    Q_Q(IrcBuffer);
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
    case IrcMessage::Part:
        processed = processPartMessage(static_cast<IrcPartMessage*>(message));
        break;
    case IrcMessage::Quit:
        processed = processQuitMessage(static_cast<IrcQuitMessage*>(message));
        break;
    case IrcMessage::Topic:
        processed = processTopicMessage(static_cast<IrcTopicMessage*>(message));
        break;
    case IrcMessage::Notice:
    case IrcMessage::Numeric:
    case IrcMessage::Private:
        processed = true;
        break;
    default:
        break;
    }
    if (processed)
        emit q->messageReceived(message);
    return processed;
}

bool IrcBufferPrivate::processJoinMessage(IrcJoinMessage* message)
{
    if (message->flags() & IrcMessage::Own)
        clearUsers();
    else
        addUsers(QStringList() << message->sender().name());
    return true;
}

bool IrcBufferPrivate::processKickMessage(IrcKickMessage* message)
{
    if (!message->user().compare(message->session()->nickName(), Qt::CaseInsensitive)) {
        clearUsers();
        return true;
    }
    return removeUser(message->user());
}

bool IrcBufferPrivate::processModeMessage(IrcModeMessage* message)
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

bool IrcBufferPrivate::processNamesMessage(IrcNamesMessage* message)
{
    addUsers(message->names());
    return true;
}

bool IrcBufferPrivate::processNickMessage(IrcNickMessage* message)
{
    if (!message->sender().name().compare(name, Qt::CaseInsensitive)) {
        setName(message->nick());
        return true;
    }
    return renameUser(message->sender().name(), message->nick());
}

bool IrcBufferPrivate::processPartMessage(IrcPartMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        clearUsers();
        return true;
    }
    return removeUser(message->sender().name());
}

bool IrcBufferPrivate::processQuitMessage(IrcQuitMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        clearUsers();
        return true;
    }
    return removeUser(message->sender().name())
           || !message->sender().name().compare(name, Qt::CaseInsensitive);
}

bool IrcBufferPrivate::processTopicMessage(IrcTopicMessage* message)
{
    setTopic(message->topic());
    return true;
}

/*!
    Constructs a new buffer object with \a parent.

    \note IrcBuffer does not work on its own. Use an instance managed by IrcBufferModel.

    \sa IrcBufferModel::bufferAdded()
 */
IrcBuffer::IrcBuffer(QObject* parent)
    : QObject(parent), d_ptr(new IrcBufferPrivate(this))
{
}

/*!
    Destructs the buffer object.

    \sa IrcBufferModel::bufferRemoved()
 */
IrcBuffer::~IrcBuffer()
{
    Q_D(IrcBuffer);
    qDeleteAll(d->userList);
    d->userList.clear();
    d->userMap.clear();
    d->models.clear();

    emit destroyed(this);
}

/*!
    This property holds the buffer type.

    \par Access function:
    \li Irc::BufferType <b>type</b>() const
 */
Irc::BufferType IrcBuffer::type() const
{
    Q_D(const IrcBuffer);
    return d->type;
}

/*!
    This property holds the buffer title.

    The title consists of prefix and name.

    \par Access function:
    \li QString <b>title</b>() const
 */
QString IrcBuffer::title() const
{
    Q_D(const IrcBuffer);
    return d->prefix + d->name;
}

/*!
    This property holds the buffer name.

    \par Access function:
    \li QString <b>name</b>() const
 */
QString IrcBuffer::name() const
{
    Q_D(const IrcBuffer);
    return d->name;
}

/*!
    This property holds the buffer prefix.

    \par Access function:
    \li QString <b>prefix</b>() const
 */
QString IrcBuffer::prefix() const
{
    Q_D(const IrcBuffer);
    return d->prefix;
}

/*!
    This property holds the buffer mode.

    \par Access function:
    \li QString <b>mode</b>() const

    \par Notifier signal:
    \li void <b>modeChanged</b>(const QString& mode)
 */
QString IrcBuffer::mode() const
{
    Q_D(const IrcBuffer);
    return d->mode;
}

/*!
    This property holds the buffer topic.

    \par Access function:
    \li QString <b>topic</b>() const

    \par Notifier signal:
    \li void <b>topicChanged</b>(const QString& topic)
 */
QString IrcBuffer::topic() const
{
    Q_D(const IrcBuffer);
    return d->topic;
}

/*!
    This property holds the session of the buffer.

    \par Access function:
    \li \ref IrcSession* <b>session</b>() const
 */
IrcSession* IrcBuffer::session() const
{
    Q_D(const IrcBuffer);
    return d->session;
}
