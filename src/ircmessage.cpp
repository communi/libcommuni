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

QString IrcMessage::toString() const
{
    return QString("UNKNOWN %1").arg(params.join(" "));
}

void IrcMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    params = parameters;
    pfx = prefix;
}

QString IrcPasswordMessage::toString() const
{
    return QString("PASS %1").arg(passwd);
}

void IrcPasswordMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    passwd = parameters.value(0);
}

QString IrcNickMessage::toString() const
{
    return QString("NICK %1").arg(n);
}

void IrcNickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    n = parameters.value(0);
}

QString IrcUserMessage::toString() const
{
    // RFC 1459 states that "hostname and servername are normally
    // ignored by the IRC server when the USER command comes from
    // a directly connected client (for security reasons)", therefore
    // we don't need them.
    return QString("USER %1 hostname servername :%2").arg(user, real);
}

void IrcUserMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    user = parameters.value(0);
    real = parameters.value(3);
}

QString IrcOperatorMessage::toString() const
{
    return QString("OPER %1 %2").arg(usr, passwd);
}

void IrcOperatorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    passwd = parameters.value(1);
}

QString IrcQuitMessage::toString() const
{
    return QString("QUIT :%1").arg(rson);
}

void IrcQuitMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    rson = parameters.value(0);
}

void IrcChannelMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    chan = parameters.value(0);
}

QString IrcJoinMessage::toString() const
{
    if (k.isEmpty())
        return QString("JOIN %1").arg(chan);
    return QString("JOIN %1 %2").arg(chan, k);
}

void IrcJoinMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
}

QString IrcPartMessage::toString() const
{
    if (rson.isEmpty())
        return QString("PART %1").arg(chan);
    return QString("PART %1 %2").arg(chan, rson);
}

void IrcPartMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
    rson = parameters.value(1);
}

QString IrcTopicMessage::toString() const
{
    if (tpc.isEmpty())
        return QString("TOPIC %1").arg(chan);
    return QString("TOPIC %1 :%2").arg(chan, tpc);
}

void IrcTopicMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
    tpc = parameters.value(1);
}

QString IrcNamesMessage::toString() const
{
    return QString("NAMES %1").arg(chan);
}

void IrcNamesMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
}

QString IrcListMessage::toString() const
{
    if (srv.isEmpty())
        return QString("LIST %1").arg(chan);
    return QString("LIST %1 %2").arg(chan, srv);
}

void IrcListMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcChannelMessage::initFrom(prefix, parameters);
    srv = parameters.value(1);
}

QString IrcInviteMessage::toString() const
{
    return QString("INVITE %1 %2").arg(usr, chan);
}

void IrcInviteMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
}

QString IrcKickMessage::toString() const
{
    if (rson.isEmpty())
        return QString("KICK %1 %2").arg(usr, chan);
    return QString("KICK %1 %2 :%3").arg(usr, chan, rson);
}

void IrcKickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
    rson = parameters.value(2);
}

QString IrcModeMessage::toString() const
{
    QStringList lst;
    lst << QString("MODE") << tgt << mod << arg << msk;
    while (lst.last().isEmpty())
        lst.removeLast();
    return lst.join(" ");
}

void IrcModeMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    mod = parameters.value(1);
    arg = parameters.value(2);
    msk = parameters.value(3);
}

void IrcSendMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    msg = parameters.value(1);
}

QString IrcPrivateMessage::toString() const
{
    QString copy(msg);
    if (act)
        copy = QString("\1ACTION %1\x01").arg(msg);
    else if (req)
        copy = QString("\1%1\1").arg(msg);
    return QString("PRIVMSG %1 :%2").arg(tgt, copy);
}

void IrcPrivateMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcSendMessage::initFrom(prefix, parameters);

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
}

QString IrcNoticeMessage::toString() const
{
    QString copy(msg);
    if (rpl)
        copy = QString("\1%1\1").arg(msg);
    return QString("NOTICE %1 :%2").arg(tgt, copy);
}

void IrcNoticeMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcSendMessage::initFrom(prefix, parameters);
    if (rpl)
    {
        if (msg.startsWith('\1'))
            msg.remove(0, 1);
        if (msg.endsWith('\1'))
            msg.chop(1);
    }
}

void IrcQueryMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
}

QString IrcWhoMessage::toString() const
{
    return QString("WHO %1").arg(usr);
}

void IrcWhoMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcWhoisMessage::toString() const
{
    return QString("WHOIS %1 %1").arg(usr);
}

void IrcWhoisMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcWhowasMessage::toString() const
{
    return QString("WHOWAS %1 %1").arg(usr);
}

void IrcWhowasMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcPingMessage::toString() const
{
    return QString("PING %1").arg(tgt);
}

void IrcPingMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
}

QString IrcPongMessage::toString() const
{
    return QString("PONG %1").arg(tgt);
}

void IrcPongMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
}

QString IrcErrorMessage::toString() const
{
    return QString("ERROR :%1").arg(err);
}

void IrcErrorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    err = parameters.value(0);
}

QString IrcNumericMessage::toString() const
{
    return QString("%1 :%2").arg(c).arg(params.join(" "));
}

void IrcNumericMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    c = cmd.toInt();
    params = parameters.mid(1);
}

QString IrcAwayMessage::toString() const
{
    return QString("AWAY :%1").arg(msg);
}

void IrcAwayMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    IrcMessage::initFrom(prefix, parameters);
    msg = parameters.value(0);
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
