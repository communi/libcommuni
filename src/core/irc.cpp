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

#include "irc.h"
#include "irccommand.h"
#include "ircconnection.h"
#include "ircmessage_p.h"
#include <QMetaEnum>
#include <QDebug>

IRC_BEGIN_NAMESPACE

/*!
    \file irc.h
    \brief \#include &lt;%Irc&gt;
 */

/*!
    \namespace Irc
    \ingroup core
    \brief Miscellaneous identifiers used throughout the library.
 */

/*!
    Returns the version number of Communi at run-time as a string (for example, "1.2.3").
    This may be a different version than the version the application was compiled against.

    \sa IRC_VERSION, IRC_VERSION_STR
 */
QString Irc::version()
{
    return QLatin1String(IRC_VERSION_STR);
}

/*!
    Returns the numeric \a code as a string or a null string if the code is unknown.

    \sa Irc::Code, IrcNumericMessage::code()
 */
QString Irc::codeToString(int code)
{
    const int index = Irc::staticMetaObject.indexOfEnumerator("Code");
    QMetaEnum enumerator = Irc::staticMetaObject.enumerator(index);
    return QLatin1String(enumerator.valueToKey(code));
}

/*!
    Returns the nick part of the specified \a prefix.

    Nick part of a prefix as specified in RFC 1459:
    <pre>
    <b>&lt;nick&gt;</b> [ '!' &lt;ident&gt; ] [ '@' &lt;host&gt; ]
    </pre>

    \sa IrcMessage::prefix, IrcMessage::nick
 */
QString Irc::nickFromPrefix(const QString& prefix)
{
    QString nick;
    IrcMessagePrivate::parsePrefix(prefix, &nick, 0, 0);
    return nick;
}

/*!
    Returns the ident part of the specified \a prefix.

    Ident part of a prefix as specified in RFC 1459:
    <pre>
    &lt;nick&gt; [ '!' <b>&lt;ident&gt;</b> ] [ '@' &lt;host&gt; ]
    </pre>

    \sa IrcMessage::prefix, IrcMessage::ident
 */
QString Irc::identFromPrefix(const QString& prefix)
{
    QString ident;
    IrcMessagePrivate::parsePrefix(prefix, 0, &ident, 0);
    return ident;
}

/*!
    Returns the host part of the specified \a prefix.

    Host part of a prefix as specified in RFC 1459:
    <pre>
    &lt;nick&gt; [ '!' &lt;ident&gt; ] [ '@' <b>&lt;host&gt;</b> ]
    </pre>

    \sa IrcMessage::prefix, IrcMessage::host
 */
QString Irc::hostFromPrefix(const QString& prefix)
{
    QString host;
    IrcMessagePrivate::parsePrefix(prefix, 0, 0, &host);
    return host;
}

/*!
    Registers IrcCore types to the %Qt meta-system.

    \sa qRegisterMetaType()
 */
void Irc::registerMetaTypes()
{
    qRegisterMetaType<IrcConnection*>("IrcConnection*");
    qRegisterMetaType<IrcConnection::Status>("IrcConnection::Status");

    qRegisterMetaType<IrcNetwork*>("IrcNetwork*");

    qRegisterMetaType<IrcCommand*>("IrcCommand*");
    qRegisterMetaType<IrcCommand::Type>("IrcCommand::Type");

    qRegisterMetaType<IrcMessage*>("IrcMessage*");
    qRegisterMetaType<IrcMessage::Type>("IrcMessage::Type");

    qRegisterMetaType<IrcCapabilityMessage*>("IrcCapabilityMessage*");
    qRegisterMetaType<IrcErrorMessage*>("IrcErrorMessage*");
    qRegisterMetaType<IrcInviteMessage*>("IrcInviteMessage*");
    qRegisterMetaType<IrcJoinMessage*>("IrcJoinMessage*");
    qRegisterMetaType<IrcKickMessage*>("IrcKickMessage*");
    qRegisterMetaType<IrcModeMessage*>("IrcModeMessage*");
    qRegisterMetaType<IrcNamesMessage*>("IrcNamesMessage*");
    qRegisterMetaType<IrcNickMessage*>("IrcNickMessage*");
    qRegisterMetaType<IrcNoticeMessage*>("IrcNoticeMessage*");
    qRegisterMetaType<IrcNumericMessage*>("IrcNumericMessage*");
    qRegisterMetaType<IrcMotdMessage*>("IrcMotdMessage*");
    qRegisterMetaType<IrcPartMessage*>("IrcPartMessage*");
    qRegisterMetaType<IrcPingMessage*>("IrcPingMessage*");
    qRegisterMetaType<IrcPongMessage*>("IrcPongMessage*");
    qRegisterMetaType<IrcPrivateMessage*>("IrcPrivateMessage*");
    qRegisterMetaType<IrcQuitMessage*>("IrcQuitMessage*");
    qRegisterMetaType<IrcTopicMessage*>("IrcTopicMessage*");

}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, Irc::Code code)
{
    const int index = Irc::staticMetaObject.indexOfEnumerator("Code");
    QMetaEnum enumerator = Irc::staticMetaObject.enumerator(index);
    const char* key = enumerator.valueToKey(code);
    debug << (key ? key : "Unknown");
    return debug;
}

QDebug operator<<(QDebug debug, Irc::DataRole role)
{
    const int index = Irc::staticMetaObject.indexOfEnumerator("DataRole");
    QMetaEnum enumerator = Irc::staticMetaObject.enumerator(index);
    const char* key = enumerator.valueToKey(role);
    debug << (key ? key : "Unknown");
    return debug;
}

QDebug operator<<(QDebug debug, Irc::Color color)
{
    const int index = Irc::staticMetaObject.indexOfEnumerator("Color");
    QMetaEnum enumerator = Irc::staticMetaObject.enumerator(index);
    const char* key = enumerator.valueToKey(color);
    debug << (key ? key : "Unknown");
    return debug;
}

QDebug operator<<(QDebug debug, Irc::SortMethod method)
{
    const int index = Irc::staticMetaObject.indexOfEnumerator("SortMethod");
    QMetaEnum enumerator = Irc::staticMetaObject.enumerator(index);
    const char* key = enumerator.valueToKey(method);
    debug << (key ? key : "Unknown");
    return debug;
}
#endif // QT_NO_DEBUG_STREAM

#include "moc_irc.cpp"

IRC_END_NAMESPACE
