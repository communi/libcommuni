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

static QVarLengthArray<QHash<QString, const QMetaObject*>, 3> irc_meta_init()
{
    QVarLengthArray<QHash<QString, const QMetaObject*>, 3> meta;
    meta.resize(3);
    meta[IrcMessage::FixedString].insert("PASS", &IrcPasswordMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("NICK", &IrcNickMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("OPER", &IrcOperatorMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("QUIT", &IrcQuitMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("JOIN", &IrcJoinMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("PART", &IrcPartMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("TOPIC", &IrcTopicMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("NAMES", &IrcNamesMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("LIST", &IrcListMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("INVITE", &IrcInviteMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("KICK", &IrcKickMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("MODE", &IrcModeMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("PRIVMSG", &IrcPrivateMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("NOTICE", &IrcNoticeMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("QUERY", &IrcQueryMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("WHO", &IrcWhoMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("WHOIS", &IrcWhoisMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("WHOWAS", &IrcWhowasMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("PING", &IrcPingMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("PONG", &IrcPongMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("ERROR", &IrcErrorMessage::staticMetaObject);
    meta[IrcMessage::FixedString].insert("AWAY", &IrcAwayMessage::staticMetaObject);
    meta[IrcMessage::RegExp].insert("\\d{1,3}", &IrcNumericMessage::staticMetaObject);
    return meta;
}
QVarLengthArray<QHash<QString, const QMetaObject*>, 3> IrcMessage::metaObjects = irc_meta_init();

void IrcMessage::unregisterCommand(const QString& command)
{
    for (int i = RegExp; i <= FixedString; ++i)
        metaObjects[i].remove(command);
}

static const QMetaObject* irc_command_match(const QString& command, const QHash<QString, const QMetaObject*>& meta, QRegExp::PatternSyntax syntax)
{
    QHash<QString, const QMetaObject*>::const_iterator it;
    for (it = meta.constBegin(); it != meta.constEnd(); ++it)
        if (QRegExp(it.key(), Qt::CaseSensitive, syntax).exactMatch(command))
            return it.value();
    return 0;
}

QStringList IrcMessage::availableCommands()
{
    QSet<QString> keys;
    keys += QSet<QString>::fromList(metaObjects[FixedString].keys());
    keys += QSet<QString>::fromList(metaObjects[Wildcard].keys());
    keys += QSet<QString>::fromList(metaObjects[RegExp].keys());
    return keys.toList();
}

IrcMessage* IrcMessage::create(const QString& command, QObject* parent)
{
    IrcMessage* message = 0;
    const QMetaObject* metaObject = metaObjects[FixedString].value(command);
    if (!metaObject)
        metaObject = irc_command_match(command, metaObjects[Wildcard], QRegExp::Wildcard);
    if (!metaObject)
        metaObject = irc_command_match(command, metaObjects[RegExp], QRegExp::RegExp);
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
