/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#include "ircmessage.h"
#include <QDebug>

class IrcMessagePrivate
{
public:
    uint type;
    QString prefix;
    QString command;
    QStringList params;
};

static const QMetaObject* irc_command_meta_object(const QString& command)
{
    static QHash<QString, const QMetaObject*> metaObjects;
    if (metaObjects.isEmpty())
    {
        metaObjects.insert("PASS", &IrcPasswordMessage::staticMetaObject);
        metaObjects.insert("NICK", &IrcNickMessage::staticMetaObject);
        metaObjects.insert("OPER", &IrcOperatorMessage::staticMetaObject);
        metaObjects.insert("QUIT", &IrcQuitMessage::staticMetaObject);
        metaObjects.insert("JOIN", &IrcJoinMessage::staticMetaObject);
        metaObjects.insert("PART", &IrcPartMessage::staticMetaObject);
        metaObjects.insert("TOPIC", &IrcTopicMessage::staticMetaObject);
        metaObjects.insert("NAMES", &IrcNamesMessage::staticMetaObject);
        metaObjects.insert("LIST", &IrcListMessage::staticMetaObject);
        metaObjects.insert("INVITE", &IrcInviteMessage::staticMetaObject);
        metaObjects.insert("KICK", &IrcKickMessage::staticMetaObject);
        metaObjects.insert("MODE", &IrcModeMessage::staticMetaObject);
        metaObjects.insert("PRIVMSG", &IrcPrivateMessage::staticMetaObject);
        metaObjects.insert("NOTICE", &IrcNoticeMessage::staticMetaObject);
        metaObjects.insert("QUERY", &IrcQueryMessage::staticMetaObject);
        metaObjects.insert("WHO", &IrcWhoMessage::staticMetaObject);
        metaObjects.insert("WHOIS", &IrcWhoisMessage::staticMetaObject);
        metaObjects.insert("WHOWAS", &IrcWhowasMessage::staticMetaObject);
        metaObjects.insert("PING", &IrcPingMessage::staticMetaObject);
        metaObjects.insert("PONG", &IrcPongMessage::staticMetaObject);
        metaObjects.insert("ERROR", &IrcErrorMessage::staticMetaObject);
        metaObjects.insert("AWAY", &IrcAwayMessage::staticMetaObject);
    }

    const QMetaObject* metaObject = metaObjects.value(command);
    if (!metaObject)
    {
        bool ok = false;
        command.toInt(&ok);
        if (ok)
            metaObject = &IrcNumericMessage::staticMetaObject;
    }
    return metaObject;
}

IrcMessage* IrcMessage::create(const QString& command, QObject* parent)
{
    IrcMessage* message = 0;
    const QMetaObject* metaObject = irc_command_meta_object(command);
    if (metaObject)
        message = qobject_cast<IrcMessage*>(metaObject->newInstance(Q_ARG(QObject*, parent)));
    if (message)
        message->setCommand(command);
    return message;
}

bool IrcMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    params = parameters;
    pfx = prefix;
    return !prefix.isEmpty();
}

bool IrcPasswordMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    passwd = parameters.value(0);
    return ret && !passwd.isEmpty();
}

bool IrcNickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    n = parameters.value(0);
    return ret && !n.isEmpty();
}

bool IrcUserMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    user = parameters.value(0);
    real = parameters.value(3);
    return ret && !user.isEmpty() && !real.isEmpty();
}

bool IrcOperatorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    passwd = parameters.value(1);
    return ret && !usr.isEmpty() && !passwd.isEmpty();
}

bool IrcQuitMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    rson = parameters.value(0);
    return ret;
}

bool IrcChannelMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    chan = parameters.value(0);
    return ret && !chan.isEmpty();
}

bool IrcJoinMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    k = parameters.value(1);
    return ret;
}

bool IrcPartMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    rson = parameters.value(1);
    return ret;
}

bool IrcTopicMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    tpc = parameters.value(1);
    return ret;
}

bool IrcNamesMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcChannelMessage::initFrom(prefix, parameters);
}

bool IrcListMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    srv = parameters.value(1);
    return ret;
}

bool IrcInviteMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
    return ret && !usr.isEmpty() && !chan.isEmpty();
}

bool IrcKickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
    rson = parameters.value(2);
    return ret && !usr.isEmpty() && !chan.isEmpty();
}

bool IrcModeMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    mod = parameters.value(1);
    arg = parameters.value(2);
    msk = parameters.value(3);
    return ret && !tgt.isEmpty();
}

bool IrcSendMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    msg = parameters.value(1);
    return ret && !tgt.isEmpty() && !msg.isEmpty();
}

bool IrcPrivateMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcSendMessage::initFrom(prefix, parameters);

    if (msg.startsWith("\1ACTION "))
    {
        act = true;
        msg.remove(0, 8);
    }
    else if (msg.startsWith('\1'))
    {
        req = true;
        msg.remove(0, 1);
    }

    if ((act || req) && msg.endsWith('\1'))
        msg.chop(1);

    return ret;
}

bool IrcNoticeMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcSendMessage::initFrom(prefix, parameters);
    if (msg.startsWith('\1'))
    {
        rpl = true;
        msg.remove(0, 1);
    }
    if (rpl && msg.endsWith('\1'))
        msg.chop(1);
    return ret;
}

bool IrcQueryMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    return ret && !usr.isEmpty();
}

bool IrcWhoMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcQueryMessage::initFrom(prefix, parameters);
}

bool IrcWhoisMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcQueryMessage::initFrom(prefix, parameters);
}

bool IrcWhowasMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcQueryMessage::initFrom(prefix, parameters);
}

bool IrcPingMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    return ret && !tgt.isEmpty();
}

bool IrcPongMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    return ret && !tgt.isEmpty();
}

bool IrcErrorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    err = parameters.value(0);
    return ret && !err.isEmpty();
}

bool IrcNumericMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    bool numeric = false;
    c = cmd.toInt(&numeric);
    params = parameters.mid(1);
    return ret && numeric;
}

bool IrcAwayMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    msg = parameters.value(0);
    return ret;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcMessage* message)
{
    if (!message)
        return debug << "IrcMessage(0x0) ";
    debug.nospace() << message->metaObject()->className() << '(' << (void*) message;
    if (!message->objectName().isEmpty())
        debug << ", name = " << message->objectName();
    if (!message->prefix().isEmpty())
        debug << ", prefix = " << message->prefix();
    if (!message->parameters().isEmpty())
        debug << ", params = " << message->parameters();
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM
