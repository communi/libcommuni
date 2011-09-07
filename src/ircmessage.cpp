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

/*!
    \class IrcMessage ircmessage.h <IrcMessage>
    \brief The IrcMessage class is the base class of all IRC message classes.

    IRC messages are received from an IRC server. IrcSession translates received
    messages into IrcMessage instances and emits the IrcSession::messageReceived()
    signal upon message received.

    Subclasses of IrcMessage contain specialized accessors for parameters that
    are specific to the particular type of message. See IrcMessage::Type for
    the list of supported message types.

    \sa IrcSession::messageReceived(), IrcMessage::Type
 */

/*!
    \enum IrcMessage::Type
    This enum describes the supported message types.
 */

/*!
    \var IrcMessage::Unknown
    \brief An unknown message (IrcMessage).
 */

/*!
    \var IrcMessage::Nick
    \brief A nick message (IrcNickMessage).
 */

/*!
    \var IrcMessage::Quit
    \brief A quit message (IrcQuitMessage).
 */

/*!
    \var IrcMessage::Join
    \brief A join message (IrcJoinMessage).
 */

/*!
    \var IrcMessage::Part
    \brief A part message (IrcPartMessage).
 */

/*!
    \var IrcMessage::Topic
    \brief A topic message (IrcTopicMessage).
 */

/*!
    \var IrcMessage::Invite
    \brief An invite message (IrcInviteMessage).
 */

/*!
    \var IrcMessage::Kick
    \brief A kick message (IrcKickMessage).
 */

/*!
    \var IrcMessage::Mode
    \brief A mode message (IrcModeMessage).
 */

/*!
    \var IrcMessage::Private
    \brief A private message (IrcPrivateMessage).
 */

/*!
    \var IrcMessage::Notice
    \brief A notice message (IrcNoticeMessage).
 */

/*!
    \var IrcMessage::Ping
    \brief A ping message (IrcPingMessage).
 */

/*!
    \var IrcMessage::Pong
    \brief A pong message (IrcPongMessage).
 */

/*!
    \var IrcMessage::Error
    \brief An error message (IrcErrorMessage).
 */

/*!
    \var IrcMessage::Numeric
    \brief A numeric message (IrcNumericMessage).
 */

class IrcMessagePrivate
{
public:
    IrcMessage::Type type;
    QString prefix;
    QString command;
    QStringList parameters;
};

static const QMetaObject* irc_command_meta_object(const QString& command)
{
    static QHash<QString, const QMetaObject*> metaObjects;
    if (metaObjects.isEmpty())
    {
        metaObjects.insert("NICK", &IrcNickMessage::staticMetaObject);
        metaObjects.insert("QUIT", &IrcQuitMessage::staticMetaObject);
        metaObjects.insert("JOIN", &IrcJoinMessage::staticMetaObject);
        metaObjects.insert("PART", &IrcPartMessage::staticMetaObject);
        metaObjects.insert("TOPIC", &IrcTopicMessage::staticMetaObject);
        metaObjects.insert("INVITE", &IrcInviteMessage::staticMetaObject);
        metaObjects.insert("KICK", &IrcKickMessage::staticMetaObject);
        metaObjects.insert("MODE", &IrcModeMessage::staticMetaObject);
        metaObjects.insert("PRIVMSG", &IrcPrivateMessage::staticMetaObject);
        metaObjects.insert("NOTICE", &IrcNoticeMessage::staticMetaObject);
        metaObjects.insert("PING", &IrcPingMessage::staticMetaObject);
        metaObjects.insert("PONG", &IrcPongMessage::staticMetaObject);
        metaObjects.insert("ERROR", &IrcErrorMessage::staticMetaObject);
    }

    const QMetaObject* metaObject = metaObjects.value(command);
    if (!metaObject)
    {
        bool ok = false;
        command.toInt(&ok);
        if (ok)
            metaObject = &IrcNumericMessage::staticMetaObject;
    }
    if (!metaObject)
        metaObject = &IrcMessage::staticMetaObject;
    return metaObject;
}

/*!
    Constructs a new IrcMessage with \a parent.
 */
IrcMessage::IrcMessage(QObject* parent) : QObject(parent), d_ptr(new IrcMessagePrivate)
{
    Q_D(IrcMessage);
    d->type = Unknown;
}

/*!
    Destructs the IRC message.
 */
IrcMessage::~IrcMessage()
{
}

/*!
    This property holds the message type.

    \par Access functions:
    \li IrcMessage::Type <b>type</b>() const
 */
IrcMessage::Type IrcMessage::type() const
{
    Q_D(const IrcMessage);
    return d->type;
}

/*!
    This property holds the message prefix.

    \par Access functions:
    \li QString <b>prefix</b>() const
 */
QString IrcMessage::prefix() const
{
    Q_D(const IrcMessage);
    return d->prefix;
}

/*!
    This property holds the message command.

    \par Access functions:
    \li QString <b>command</b>() const
 */
QString IrcMessage::command() const
{
    Q_D(const IrcMessage);
    return d->command;
}

/*!
    This property holds the message parameters.

    \par Access functions:
    \li QStringList <b>parameters</b>() const
 */
QStringList IrcMessage::parameters() const
{
    Q_D(const IrcMessage);
    return d->parameters;
}

/*!
    Creates a new message corresponding to \a command and with \a parent.
 */
IrcMessage* IrcMessage::create(const QString& command, QObject* parent)
{
    IrcMessage* message = 0;
    const QMetaObject* metaObject = irc_command_meta_object(command);
    Q_ASSERT(metaObject);
    message = qobject_cast<IrcMessage*>(metaObject->newInstance(Q_ARG(QObject*, parent)));
    Q_ASSERT(message);
    message->d_ptr->command = command;
    return message;
}

/*!
    Initializes the message from \a prefix and \a parameters and
    returns \c true on success or \c false if the parameters
    did not match the message.
 */
bool IrcMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    Q_D(IrcMessage);
    d->prefix = prefix;
    d->parameters = parameters;
    return !prefix.isEmpty();
}

IrcNickMessage::IrcNickMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Nick;
}

QString IrcNickMessage::nick() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

bool IrcNickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !nick().isEmpty();
}

IrcQuitMessage::IrcQuitMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Quit;
}

QString IrcQuitMessage::reason() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

bool IrcQuitMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters);
}

IrcJoinMessage::IrcJoinMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Join;
}

QString IrcJoinMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

bool IrcJoinMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !channel().isEmpty();
}

IrcPartMessage::IrcPartMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Part;
}

QString IrcPartMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcPartMessage::reason() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1);
}

bool IrcPartMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !channel().isEmpty();
}

IrcTopicMessage::IrcTopicMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Topic;
}

QString IrcTopicMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcTopicMessage::topic() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1);
}

bool IrcTopicMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !channel().isEmpty();
}

IrcInviteMessage::IrcInviteMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Invite;
}

QString IrcInviteMessage::user() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcInviteMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1);
}

bool IrcInviteMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !user().isEmpty() && !channel().isEmpty();
}

IrcKickMessage::IrcKickMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Kick;
}

QString IrcKickMessage::user() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcKickMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1);
}

QString IrcKickMessage::reason() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(2);
}

bool IrcKickMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !user().isEmpty() && !channel().isEmpty();
}

IrcModeMessage::IrcModeMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Mode;
}

QString IrcModeMessage::target() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcModeMessage::mode() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1);
}

QString IrcModeMessage::argument() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(2);
}

QString IrcModeMessage::mask() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(3);
}

bool IrcModeMessage::initFrom(const QString &prefix, const QStringList &parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !target().isEmpty();
}

IrcPrivateMessage::IrcPrivateMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Private;
}

QString IrcPrivateMessage::target() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcPrivateMessage::message() const
{
    Q_D(const IrcMessage);
    QString msg = d->parameters.value(1);
    const bool act = isAction();
    const bool req = isRequest();
    if (act) msg.remove(0, 8);
    if (req) msg.remove(0, 1);
    if (act || req) msg.chop(1);
    return msg;
}

bool IrcPrivateMessage::isAction() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1).startsWith("\1ACTION ") && d->parameters.value(1).endsWith('\1');
}

bool IrcPrivateMessage::isRequest() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1).startsWith('\1') && d->parameters.value(1).endsWith('\1') && !isAction();
}

bool IrcPrivateMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !target().isEmpty() && !message().isEmpty();
}

IrcNoticeMessage::IrcNoticeMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Notice;
}

QString IrcNoticeMessage::target() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

QString IrcNoticeMessage::message() const
{
    Q_D(const IrcMessage);
    QString msg = d->parameters.value(1);
    if (isReply())
    {
        msg.remove(0, 1);
        msg.chop(1);
    }
    return msg;
}

bool IrcNoticeMessage::isReply() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(1).startsWith('\1') && d->parameters.value(1).endsWith('\1');
}

bool IrcNoticeMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !target().isEmpty() && !message().isEmpty();
}

IrcPingMessage::IrcPingMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Ping;
}

QString IrcPingMessage::target() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

bool IrcPingMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !target().isEmpty();
}

IrcPongMessage::IrcPongMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Pong;
}

QString IrcPongMessage::target() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

bool IrcPongMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !target().isEmpty();
}

IrcErrorMessage::IrcErrorMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Error;
}

QString IrcErrorMessage::error() const
{
    Q_D(const IrcMessage);
    return d->parameters.value(0);
}

bool IrcErrorMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && !error().isEmpty();
}

IrcNumericMessage::IrcNumericMessage(QObject* parent) : IrcMessage(parent)
{
    Q_D(IrcMessage);
    d->type = Numeric;
}

int IrcNumericMessage::code() const
{
    Q_D(const IrcMessage);
    bool ok = false;
    int number = d->command.toInt(&ok);
    return ok ? number : -1;
}

bool IrcNumericMessage::initFrom(const QString& prefix, const QStringList& parameters)
{
    return IrcMessage::initFrom(prefix, parameters) && code() != -1;
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
