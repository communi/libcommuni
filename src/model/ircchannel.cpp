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
#include "ircusermodel_p.h"
#include "ircbuffermodel.h"
#include "ircbuffermodel_p.h"
#include "irccommand.h"
#include "ircconnection.h"
#include "ircuser_p.h"
#include "irc.h"

IRC_BEGIN_NAMESPACE

/*!
    \file ircchannel.h
    \brief \#include &lt;IrcChannel&gt;
 */

/*!
    \class IrcChannel ircchannel.h <IrcChannel>
    \ingroup models
    \brief Keeps track of channel status.

    \sa IrcBufferModel
*/

#ifndef IRC_DOXYGEN
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
    return Irc::nickFromPrefix(copy);
}

IrcChannelPrivate::IrcChannelPrivate() : joined(0), left(0)
{
}

IrcChannelPrivate::~IrcChannelPrivate()
{
}

void IrcChannelPrivate::init(const QString& title, IrcBufferModel* m)
{
    IrcBufferPrivate::init(title, m);

    const QStringList chanTypes = m->network()->channelTypes();
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

void IrcChannelPrivate::addUser(const QString& name)
{
    Q_Q(IrcChannel);
    const QStringList prefixes = q->network()->prefixes();

    IrcUser* user = new IrcUser(q);
    IrcUserPrivate* priv = IrcUserPrivate::get(user);
    priv->channel = q;
    priv->setName(userName(name, prefixes));
    priv->setPrefix(getPrefix(name, prefixes));
    priv->setMode(q->network()->prefixToMode(user->prefix()));
    activeUsers.prepend(user);
    userList.append(user);
    userMap.insert(user->name(), user);

    foreach (IrcUserModel* model, userModels)
        IrcUserModelPrivate::get(model)->addUser(user);
}

bool IrcChannelPrivate::removeUser(const QString& name)
{
    if (IrcUser* user = userMap.value(name)) {
        userMap.remove(name);
        userList.removeOne(user);
        activeUsers.removeOne(user);
        foreach (IrcUserModel* model, userModels)
            IrcUserModelPrivate::get(model)->removeUser(user);
        user->deleteLater();
        return true;
    }
    return false;
}

void IrcChannelPrivate::setUsers(const QStringList& names)
{
    Q_Q(IrcChannel);
    const QStringList prefixes = q->network()->prefixes();

    qDeleteAll(userList);
    userMap.clear();
    userList.clear();
    activeUsers.clear();

    QList<IrcUser*> users;
    foreach (const QString& name, names) {
        IrcUser* user = new IrcUser(q);
        IrcUserPrivate* priv = IrcUserPrivate::get(user);
        priv->channel = q;
        priv->setName(userName(name, prefixes));
        priv->setPrefix(getPrefix(name, prefixes));
        priv->setMode(q->network()->prefixToMode(user->prefix()));
        activeUsers.append(user);
        userList.append(user);
        userMap.insert(user->name(), user);
        users.append(user);
    }

    foreach (IrcUserModel* model, userModels)
        IrcUserModelPrivate::get(model)->setUsers(users);
}

bool IrcChannelPrivate::renameUser(const QString& from, const QString& to)
{
    if (IrcUser* user = userMap.take(from)) {
        IrcUserPrivate::get(user)->setName(to);
        userMap.insert(to, user);

        QStringList names = userMap.keys();
        foreach (IrcUserModel* model, userModels) {
            IrcUserModelPrivate::get(model)->renameUser(user);
            emit model->namesChanged(names);
        }
        return true;
    }
    return false;
}

void IrcChannelPrivate::setUserMode(const QString& name, const QString& command)
{
    if (IrcUser* user = userMap.value(name)) {
        bool add = true;
        QString mode = user->mode();
        QString prefix = user->prefix();
        const IrcNetwork* network = model->network();
        for (int i = 0; i < command.size(); ++i) {
            QChar c = command.at(i);
            if (c == QLatin1Char('+')) {
                add = true;
            } else if (c == QLatin1Char('-')) {
                add = false;
            } else {
                QString p = network->modeToPrefix(c);
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

        foreach (IrcUserModel* model, userModels)
            IrcUserModelPrivate::get(model)->setUserMode(user);
    }
}

void IrcChannelPrivate::promoteUser(const QString& name)
{
    if (IrcUser* user = userMap.value(name)) {
        const int idx = activeUsers.indexOf(user);
        Q_ASSERT(idx != -1);
        activeUsers.move(idx, 0);
        foreach (IrcUserModel* model, userModels)
            IrcUserModelPrivate::get(model)->promoteUser(user);
    }
}

bool IrcChannelPrivate::processJoinMessage(IrcJoinMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        ++joined;
        emitActiveChanged();
    } else {
        addUser(message->nick());
    }
    return true;
}

bool IrcChannelPrivate::processKickMessage(IrcKickMessage* message)
{
    if (!message->user().compare(message->connection()->nickName(), Qt::CaseInsensitive)) {
        ++left;
        emitActiveChanged();
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
    setUsers(message->names());
    return true;
}

bool IrcChannelPrivate::processNickMessage(IrcNickMessage* message)
{
    const bool renamed = renameUser(message->oldNick(), message->newNick());
    if (renamed)
        promoteUser(message->newNick());
    return renamed;
}

bool IrcChannelPrivate::processNoticeMessage(IrcNoticeMessage* message)
{
    promoteUser(message->nick());
    return true;
}

bool IrcChannelPrivate::processNumericMessage(IrcNumericMessage* message)
{
    promoteUser(message->nick());
    return true;
}

bool IrcChannelPrivate::processPartMessage(IrcPartMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        ++left;
        emitActiveChanged();
        return true;
    }
    return removeUser(message->nick());
}

bool IrcChannelPrivate::processPrivateMessage(IrcPrivateMessage* message)
{
    const QString msg = message->message();
    const bool prefixed = !msg.isEmpty() && message->network()->prefixes().contains(msg.at(0));
    foreach (IrcUser* user, activeUsers) {
        const QString str = prefixed ? user->title() : user->name();
        if (msg.startsWith(str)) {
            promoteUser(user->name());
            break;
        }
    }
    promoteUser(message->nick());
    return true;
}

bool IrcChannelPrivate::processQuitMessage(IrcQuitMessage* message)
{
    if (message->flags() & IrcMessage::Own) {
        ++left;
        emitActiveChanged();
        return true;
    }
    return removeUser(message->nick()) || IrcBufferPrivate::processQuitMessage(message);
}

bool IrcChannelPrivate::processTopicMessage(IrcTopicMessage* message)
{
    setTopic(message->topic());
    return true;
}
#endif // IRC_DOXYGEN

/*!
    Constructs a new channel object with \a parent.
 */
IrcChannel::IrcChannel(QObject* parent)
    : IrcBuffer(*new IrcChannelPrivate, parent)
{
}

/*!
    Destructs the channel object.
 */
IrcChannel::~IrcChannel()
{
    Q_D(IrcChannel);
    qDeleteAll(d->userList);
    d->userList.clear();
    d->userMap.clear();
    d->userModels.clear();
    emit destroyed(this);
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

bool IrcChannel::isActive() const
{
    Q_D(const IrcChannel);
    return IrcBuffer::isActive() && d->joined > d->left;
}

/*!
    Parts the channel with an optional \a reason.

    This method is provided for convenience. It is equal to:
    \code
    IrcCommand* command = IrcCommand::createPart(channel->title(), reason);
    channel->sendCommand(command);
    \endcode

    \sa IrcBuffer::sendCommand(), IrcCommand::createPart()
 */
void IrcChannel::part(const QString& reason)
{
    sendCommand(IrcCommand::createPart(title(), reason));
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcChannel* channel)
{
    if (!channel)
        return debug << "IrcChannel(0x0) ";
    debug.nospace() << channel->metaObject()->className() << '(' << (void*) channel;
    if (!channel->objectName().isEmpty())
        debug.nospace() << ", name=" << qPrintable(channel->objectName());
    if (!channel->title().isEmpty())
        debug.nospace() << ", title=" << qPrintable(channel->title());
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_ircchannel.cpp"

IRC_END_NAMESPACE
