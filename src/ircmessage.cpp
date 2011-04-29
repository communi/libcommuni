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

QString IrcMessage::syntax() const
{
    return QString();
}

QString IrcMessage::toString() const
{
    return QString("UNKNOWN %1").arg(params.join(" "));
}

bool IrcMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    params = parameters;
    pfx = prefix;
    return !prefix.isEmpty();
}

QString IrcPasswordMessage::syntax() const
{
    return QString("PASS <password>");
}

QString IrcPasswordMessage::toString() const
{
    return QString("PASS %1").arg(passwd);
}

bool IrcPasswordMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    passwd = parameters.value(0);
    return ret && !passwd.isEmpty();
}

QString IrcNickMessage::syntax() const
{
    return QString("NICK <nick>");
}

QString IrcNickMessage::toString() const
{
    return QString("NICK %1").arg(n);
}

bool IrcNickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    n = parameters.value(0);
    return ret && !n.isEmpty();
}

QString IrcUserMessage::syntax() const
{
    return QString("USER <username> <hostname> <servername> <realname>");
}

QString IrcUserMessage::toString() const
{
    // RFC 1459 states that "hostname and servername are normally
    // ignored by the IRC server when the USER command comes from
    // a directly connected client (for security reasons)", therefore
    // we don't need them.
    return QString("USER %1 hostname servername :%2").arg(user, real);
}

bool IrcUserMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    user = parameters.value(0);
    real = parameters.value(3);
    return ret && !user.isEmpty() && !real.isEmpty();
}

QString IrcOperatorMessage::syntax() const
{
    return QString("OPER <user> <password>");
}

QString IrcOperatorMessage::toString() const
{
    return QString("OPER %1 %2").arg(usr, passwd);
}

bool IrcOperatorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    passwd = parameters.value(1);
    return ret && !usr.isEmpty() && !passwd.isEmpty();
}

QString IrcQuitMessage::syntax() const
{
    return QString("QUIT (<message>)");
}

QString IrcQuitMessage::toString() const
{
    return QString("QUIT :%1").arg(rson);
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

QString IrcJoinMessage::syntax() const
{
    return QString("JOIN <channel> (<key>)");
}

QString IrcJoinMessage::toString() const
{
    if (k.isEmpty())
        return QString("JOIN %1").arg(chan);
    return QString("JOIN %1 %2").arg(chan, k);
}

bool IrcJoinMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    k = parameters.value(1);
    return ret;
}

QString IrcPartMessage::syntax() const
{
    return QString("PART <channel> (<reason>)");
}

QString IrcPartMessage::toString() const
{
    if (rson.isEmpty())
        return QString("PART %1").arg(chan);
    return QString("PART %1 :%2").arg(chan, rson);
}

bool IrcPartMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    rson = parameters.value(1);
    return ret;
}

QString IrcTopicMessage::syntax() const
{
    return QString("TOPIC <channel> (<topic>)");
}

QString IrcTopicMessage::toString() const
{
    if (tpc.isEmpty())
        return QString("TOPIC %1").arg(chan);
    return QString("TOPIC %1 :%2").arg(chan, tpc);
}

bool IrcTopicMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    tpc = parameters.value(1);
    return ret;
}

QString IrcNamesMessage::syntax() const
{
    return QString("NAMES <channel>");
}

QString IrcNamesMessage::toString() const
{
    return QString("NAMES %1").arg(chan);
}

bool IrcNamesMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcChannelMessage::initFrom(prefix, parameters);
}

QString IrcListMessage::syntax() const
{
    return QString("LIST <channel> (<server>)");
}

QString IrcListMessage::toString() const
{
    if (srv.isEmpty())
        return QString("LIST %1").arg(chan);
    return QString("LIST %1 %2").arg(chan, srv);
}

bool IrcListMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcChannelMessage::initFrom(prefix, parameters);
    srv = parameters.value(1);
    return ret;
}

QString IrcInviteMessage::syntax() const
{
    return QString("INVITE <user> <channel>");
}

QString IrcInviteMessage::toString() const
{
    return QString("INVITE %1 %2").arg(usr, chan);
}

bool IrcInviteMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
    return ret && !usr.isEmpty() && !chan.isEmpty();
}

QString IrcKickMessage::syntax() const
{
    return QString("KICK <user> <channel> (<reason>)");
}

QString IrcKickMessage::toString() const
{
    if (rson.isEmpty())
        return QString("KICK %1 %2").arg(usr, chan);
    return QString("KICK %1 %2 :%3").arg(usr, chan, rson);
}

bool IrcKickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    chan = parameters.value(1);
    rson = parameters.value(2);
    return ret && !usr.isEmpty() && !chan.isEmpty();
}

QString IrcModeMessage::syntax() const
{
    return QString("MODE <target> <mode> (<arg>) (<mask>)");
}

QString IrcModeMessage::toString() const
{
    QStringList lst;
    lst << QString("MODE") << tgt << mod << arg << msk;
    while (lst.last().isEmpty())
        lst.removeLast();
    return lst.join(" ");
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

QString IrcPrivateMessage::syntax() const
{
    return QString("PRIVMSG <target> <message>");
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

#include <QDebug>
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

QString IrcNoticeMessage::syntax() const
{
    return QString("NOTICE <target> <message>");
}

QString IrcNoticeMessage::toString() const
{
    QString copy(msg);
    if (rpl)
        copy = QString("\1%1\1").arg(msg);
    return QString("NOTICE %1 :%2").arg(tgt, copy);
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

QString IrcQueryMessage::syntax() const
{
    return QString("QUERY <user>");
}

QString IrcQueryMessage::toString() const
{
    return QString("QUERY %1").arg(usr);
}

bool IrcQueryMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    usr = parameters.value(0);
    return ret && !usr.isEmpty();
}

QString IrcWhoMessage::syntax() const
{
    return QString("WHO <user>");
}

QString IrcWhoMessage::toString() const
{
    return QString("WHO %1").arg(usr);
}

bool IrcWhoMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcWhoisMessage::syntax() const
{
    return QString("WHOIS <user>");
}

QString IrcWhoisMessage::toString() const
{
    return QString("WHOIS %1 %1").arg(usr);
}

bool IrcWhoisMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcWhowasMessage::syntax() const
{
    return QString("WHOWAS <user>");
}

QString IrcWhowasMessage::toString() const
{
    return QString("WHOWAS %1 %1").arg(usr);
}

bool IrcWhowasMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcQueryMessage::initFrom(prefix, parameters);
}

QString IrcPingMessage::syntax() const
{
    return QString("PING <target>");
}

QString IrcPingMessage::toString() const
{
    return QString("PING %1").arg(tgt);
}

bool IrcPingMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    return ret && !tgt.isEmpty();
}

QString IrcPongMessage::toString() const
{
    return QString("PONG %1").arg(tgt);
}

bool IrcPongMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    tgt = parameters.value(0);
    return ret && !tgt.isEmpty();
}

QString IrcErrorMessage::toString() const
{
    return QString("ERROR :%1").arg(err);
}

bool IrcErrorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    err = parameters.value(0);
    return ret && !err.isEmpty();
}

QString IrcNumericMessage::toString() const
{
    return QString("%1 :%2").arg(c).arg(params.join(" "));
}

bool IrcNumericMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    bool ret = IrcMessage::initFrom(prefix, parameters);
    bool numeric = false;
    c = cmd.toInt(&numeric);
    params = parameters.mid(1);
    return ret && numeric;
}

QString IrcAwayMessage::syntax() const
{
    return QString("AWAY <message>");
}

QString IrcAwayMessage::toString() const
{
    return QString("AWAY :%1").arg(msg);
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
