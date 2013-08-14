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

#include "ircmessage.h"
#include "ircmessage_p.h"
#include "ircconnection.h"
#include "ircconnection_p.h"
#include "ircnetwork.h"
#include "irccommand.h"
#include "irc.h"
#include <QVariant>
#include <QDebug>

IRC_BEGIN_NAMESPACE

/*!
    \file ircmessage.h
    \brief #include &lt;IrcMessage&gt;
 */

/*!
    \class IrcMessage ircmessage.h <IrcMessage>
    \ingroup core
    \ingroup message
    \brief The base class of all messages.

    IRC messages are received from an IRC server. IrcConnection translates received
    messages into IrcMessage instances and emits the IrcConnection::messageReceived()
    signal upon message received.

    Subclasses of IrcMessage contain specialized accessors for parameters that
    are specific to the particular type of message. See IrcMessage::Type for
    the list of supported message types.

    \sa IrcConnection::messageReceived(), IrcMessage::Type
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

/*!
    \var IrcMessage::Motd
    \brief A message of the day (IrcMotdMessage).
 */

/*!
    \var IrcMessage::Names
    \brief A names message (IrcNamesMessage).
 */

/*!
    \enum IrcMessage::Flags
    This enum describes the supported message flags.
 */

/*!
    \var IrcMessage::None
    \brief The message has no flags.
 */

/*!
    \var IrcMessage::Own
    \brief The message is user's own message.
 */

/*!
    \var IrcMessage::Identified
    \brief The message is identified.
 */

/*!
    \var IrcMessage::Unidentified
    \brief The message is unidentified.
 */

static const QMetaObject* irc_command_meta_object(const QString& command)
{
    static QHash<QString, const QMetaObject*> metaObjects;
    if (metaObjects.isEmpty()) {
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
        metaObjects.insert("CAP", &IrcCapabilityMessage::staticMetaObject);
    }

    const QMetaObject* metaObject = metaObjects.value(command.toUpper());
    if (!metaObject) {
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
    Constructs a new IrcMessage with \a connection.
 */
IrcMessage::IrcMessage(IrcConnection* connection) : QObject(connection), d_ptr(new IrcMessagePrivate)
{
    Q_D(IrcMessage);
    d->connection = connection;
}

/*!
    Destructs the message.
 */
IrcMessage::~IrcMessage()
{
}

/*!
    This property holds the message connection.

    \par Access functions:
    \li IrcConnection* <b>connection</b>() const
 */
IrcConnection* IrcMessage::connection() const
{
    Q_D(const IrcMessage);
    return d->connection;
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
    This property holds the message flags.

    \par Access functions:
    \li IrcMessage::Flags <b>flags</b>() const
 */
IrcMessage::Flags IrcMessage::flags() const
{
    Q_D(const IrcMessage);
    if (d->flags == -1) {
        d->flags = IrcMessage::None;
        IrcSender sender = d->sender();
        if (sender.isValid() && sender.name() == d->connection->nickName())
            d->flags |= IrcMessage::Own;

        if ((d->type == IrcMessage::Private || d->type == IrcMessage::Notice) &&
                IrcNetwork(d->connection).activeCapabilities().contains("identify-msg")) {
            QString msg = property("message").toString();
            if (msg.startsWith("+"))
                d->flags |= IrcMessage::Identified;
            else if (msg.startsWith("-"))
                d->flags |= IrcMessage::Unidentified;
        }
    }
    return IrcMessage::Flags(d->flags);
}

/*!
    This property holds the message command.

    \par Access functions:
    \li QString <b>command</b>() const
    \li void <b>setCommand</b>(const QString& command)
 */
QString IrcMessage::command() const
{
    Q_D(const IrcMessage);
    return d->command();
}

void IrcMessage::setCommand(const QString& command)
{
    Q_D(IrcMessage);
    d->setCommand(command);
}

/*!
    This property holds the message sender.

    \par Access functions:
    \li IrcSender <b>sender</b>() const
    \li void <b>setSender</b>(const IrcSender& sender)
 */
IrcSender IrcMessage::sender() const
{
    Q_D(const IrcMessage);
    return d->sender();
}

void IrcMessage::setSender(const IrcSender& sender)
{
    Q_D(IrcMessage);
    d->setSender(sender);
}

/*!
    This property holds the message parameters.

    \par Access functions:
    \li QStringList <b>parameters</b>() const
    \li void <b>setParameters</b>(const QStringList& parameters)
 */
QStringList IrcMessage::parameters() const
{
    Q_D(const IrcMessage);
    return d->params();
}

void IrcMessage::setParameters(const QStringList& parameters)
{
    Q_D(IrcMessage);
    d->setParams(parameters);
}

/*!
    This property holds the message time stamp.

    \par Access functions:
    \li QDateTime <b>timeStamp</b>() const
    \li void <b>setTimeStamp</b>(const QDateTime& timeStamp)
 */
QDateTime IrcMessage::timeStamp() const
{
    Q_D(const IrcMessage);
    return d->timeStamp;
}

void IrcMessage::setTimeStamp(const QDateTime& timeStamp)
{
    Q_D(IrcMessage);
    d->timeStamp = timeStamp;
}

/*!
    This property holds the FALLBACK encoding for the message.

    The fallback encoding is used when the message is detected not
    to be valid UTF-8 and the consequent auto-detection of message
    encoding fails. See QTextCodec::availableCodes() for the list of
    supported encodings.

    The default value is ISO-8859-15.

    \par Access functions:
    \li QByteArray <b>encoding</b>() const
    \li void <b>setEncoding</b>(const QByteArray& encoding)

    \sa QTextCodec::availableCodecs(), QTextCodec::codecForLocale()
 */
QByteArray IrcMessage::encoding() const
{
    Q_D(const IrcMessage);
    return d->encoding;
}

void IrcMessage::setEncoding(const QByteArray& encoding)
{
    Q_D(IrcMessage);
    extern bool irc_is_supported_encoding(const QByteArray& encoding); // ircmessagedecoder.cpp
    if (!irc_is_supported_encoding(encoding)) {
        qWarning() << "IrcMessage::setEncoding(): unsupported encoding" << encoding;
        return;
    }
    d->encoding = encoding;
    d->invalidate();
}

/*!
    Creates a new message from \a data and \a connection.
 */
IrcMessage* IrcMessage::fromData(const QByteArray& data, IrcConnection* connection)
{
    IrcMessage* message = 0;
    IrcMessageData md = IrcMessageData::fromData(data);
    const QMetaObject* metaObject = irc_command_meta_object(md.command);
    if (metaObject) {
        message = qobject_cast<IrcMessage*>(metaObject->newInstance(Q_ARG(IrcConnection*, connection)));
        Q_ASSERT(message);
        message->d_ptr->data = md;
    }
    return message;
}

/*!
    Creates a new message from \a sender and \a command with \a connection.
 */
IrcMessage* IrcMessage::fromCommand(const QString& sender, IrcCommand* command, IrcConnection* connection)
{
    return fromData(":" + sender.toUtf8() + " " + command->toString().toUtf8(), connection);
}

/*!
    Creates a new message from \a sender, \a command and \a parameters with \a connection.
 */
IrcMessage* IrcMessage::fromParameters(const QString& sender, const QString& command, const QStringList& parameters, IrcConnection* connection)
{
    IrcMessage* message = 0;
    const QMetaObject* metaObject = irc_command_meta_object(command);
    if (metaObject) {
        message = qobject_cast<IrcMessage*>(metaObject->newInstance(Q_ARG(IrcConnection*, connection)));
        Q_ASSERT(message);
        message->setSender(sender);
        message->setCommand(command);
        message->setParameters(parameters);
    }
    return message;
}

/*!
    \property bool IrcMessage::valid
    This property is \c true if the message is valid; otherwise \c false.

    A message is considered valid if the sender is valid
    and the parameters match the message.

    \par Access functions:
    \li bool <b>isValid</b>() const
 */
bool IrcMessage::isValid() const
{
    Q_D(const IrcMessage);
    return d->connection && sender().isValid();
}

/*!
    Returns the message as received from an IRC server.
 */
QByteArray IrcMessage::toData() const
{
    Q_D(const IrcMessage);
    return d->data.content;
}

/*!
    \class IrcNickMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a nick message.
 */

/*!
    Constructs a new IrcNickMessage with \a connection.
 */
IrcNickMessage::IrcNickMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Nick;
}

/*!
    This property holds the new nick.

    \par Access functions:
    \li QString <b>nick</b>() const
 */
QString IrcNickMessage::nick() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

bool IrcNickMessage::isValid() const
{
    return IrcMessage::isValid() && !nick().isEmpty();
}

/*!
    \class IrcQuitMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a quit message.
 */

/*!
    Constructs a new IrcQuitMessage with \a connection.
 */
IrcQuitMessage::IrcQuitMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Quit;
}

/*!
    This property holds the optional quit reason.

    \par Access functions:
    \li QString <b>reason</b>() const
 */
QString IrcQuitMessage::reason() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

bool IrcQuitMessage::isValid() const
{
    return IrcMessage::isValid();
}

/*!
    \class IrcJoinMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a join message.
 */

/*!
    Constructs a new IrcJoinMessage with \a connection.
 */
IrcJoinMessage::IrcJoinMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Join;
}

/*!
    This property holds the channel in question.

    \par Access functions:
    \li QString <b>channel</b>() const
 */
QString IrcJoinMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

bool IrcJoinMessage::isValid() const
{
    return IrcMessage::isValid() && !channel().isEmpty();
}

/*!
    \class IrcPartMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a part message.
 */

/*!
    Constructs a new IrcPartMessage with \a connection.
 */
IrcPartMessage::IrcPartMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Part;
}

/*!
    This property holds the channel in question.

    \par Access functions:
    \li QString <b>channel</b>() const
 */
QString IrcPartMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the optional part reason.

    \par Access functions:
    \li QString <b>reason</b>() const
 */
QString IrcPartMessage::reason() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

bool IrcPartMessage::isValid() const
{
    return IrcMessage::isValid() && !channel().isEmpty();
}

/*!
    \class IrcTopicMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a topic message.
 */

/*!
    Constructs a new IrcTopicMessage with \a connection.
 */
IrcTopicMessage::IrcTopicMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Topic;
}

/*!
    This property holds the channel in question.

    \par Access functions:
    \li QString <b>channel</b>() const
 */
QString IrcTopicMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the new channel topic.

    \par Access functions:
    \li QString <b>topic</b>() const
 */
QString IrcTopicMessage::topic() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

/*!
    This property holds whether the message is a reply.

    Topic messages are sent in three situations:
    \li as a notification of a topic change (\c false),
    \li as a reply when joining a channel (\c true), or
    \li as a reply when explicitly querying the channel topic (\c true).

    \par Access functions:
    \li bool <b>isReply</b>() const

    \sa Irc::RPL_TOPIC, Irc::RPL_NOTOPIC, IrcTopicCommand
 */
bool IrcTopicMessage::isReply() const
{
    Q_D(const IrcMessage);
    int rpl = d->command().toInt();
    return rpl == Irc::RPL_TOPIC || rpl == Irc::RPL_NOTOPIC;
}

bool IrcTopicMessage::isValid() const
{
    return IrcMessage::isValid() && !channel().isEmpty();
}

/*!
    \class IrcInviteMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents an invite message.
 */

/*!
    Constructs a new IrcInviteMessage with \a connection.
 */
IrcInviteMessage::IrcInviteMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Invite;
}

/*!
    This property holds the user in question.

    \par Access functions:
    \li QString <b>user</b>() const
 */
QString IrcInviteMessage::user() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the channel in question.

    \par Access functions:
    \li QString <b>channel</b>() const
 */
QString IrcInviteMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

bool IrcInviteMessage::isValid() const
{
    return IrcMessage::isValid() && !user().isEmpty() && !channel().isEmpty();
}

/*!
    \class IrcKickMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a kick message.
 */

/*!
    Constructs a new IrcKickMessage with \a connection.
 */
IrcKickMessage::IrcKickMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Kick;
}

/*!
    This property holds the channel in question.

    \par Access functions:
    \li QString <b>channel</b>() const
 */
QString IrcKickMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the user in question.

    \par Access functions:
    \li QString <b>user</b>() const
 */
QString IrcKickMessage::user() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

/*!
    This property holds the optional kick reason.

    \par Access functions:
    \li QString <b>reason</b>() const
 */
QString IrcKickMessage::reason() const
{
    Q_D(const IrcMessage);
    return d->param(2);
}

bool IrcKickMessage::isValid() const
{
    return IrcMessage::isValid() && !channel().isEmpty() && !user().isEmpty();
}

/*!
    \class IrcModeMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a mode message.
 */

/*!
    \enum IrcModeMessage::Kind
    This enum describes the kind of modes.
 */

/*!
    \var IrcModeMessage::Channel
    \brief Channel mode
 */

/*!
    \var IrcModeMessage::User
    \brief User mode
 */

/*!
    Constructs a new IrcModeMessage with \a connection.
 */
IrcModeMessage::IrcModeMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Mode;
}

/*!
    This property holds the target channel or user in question.

    \par Access functions:
    \li QString <b>target</b>() const
 */
QString IrcModeMessage::target() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the channel or user mode.

    \par Access functions:
    \li QString <b>mode</b>() const
 */
QString IrcModeMessage::mode() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

/*!
    This property holds the mode argument.

    \par Access functions:
    \li QString <b>argument</b>() const
 */
QString IrcModeMessage::argument() const
{
    Q_D(const IrcMessage);
    return d->param(2);
}

/*!
    This property holds whether the message is a reply.

    Mode messages are sent when a mode changes (\c false)
    and when joining a channel (\c true).

    \par Access functions:
    \li bool <b>isReply</b>() const

    \sa Irc::RPL_CHANNELMODEIS
 */
bool IrcModeMessage::isReply() const
{
    Q_D(const IrcMessage);
    int rpl = d->command().toInt();
    return rpl == Irc::RPL_CHANNELMODEIS;
}

/*!
    This property holds the kind of the mode.

    \par Access functions:
    \li Kind <b>kind</b>() const
 */
IrcModeMessage::Kind IrcModeMessage::kind() const
{
    Q_D(const IrcMessage);
    IrcNetwork info(d->connection);
    QStringList channelModes = info.channelModes(IrcNetwork::AllTypes);
    QString m = mode();
    if (m.startsWith(QLatin1Char('+')) || m.startsWith(QLatin1Char('-')))
        m.remove(0, 1);
    while (!m.isEmpty()) {
        if (!channelModes.contains(m.at(0)))
            return User;
        m.remove(0, 1);
    }
    return Channel;
}

bool IrcModeMessage::isValid() const
{
    return IrcMessage::isValid() && !target().isEmpty() && !mode().isEmpty();
}

/*!
    \class IrcPrivateMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a private message.
 */

/*!
    Constructs a new IrcPrivateMessage with \a connection.
 */
IrcPrivateMessage::IrcPrivateMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Private;
}

/*!
    This property holds the target channel or user in question.

    \par Access functions:
    \li QString <b>target</b>() const
 */
QString IrcPrivateMessage::target() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the message.

    \par Access functions:
    \li QString <b>message</b>() const
 */
QString IrcPrivateMessage::message() const
{
    Q_D(const IrcMessage);
    QString msg = d->param(1);
    if (flags() & (Identified | Unidentified))
        msg.remove(0, 1);
    const bool act = isAction();
    const bool req = isRequest();
    if (act) msg.remove(0, 8);
    if (req) msg.remove(0, 1);
    if (act || req) msg.chop(1);
    return msg;
}

/*!
    \property bool IrcPrivateMessage::action
    This property is \c true if the message is an action; otherwise \c false.

    \par Access functions:
    \li bool <b>isAction</b>() const
 */
bool IrcPrivateMessage::isAction() const
{
    Q_D(const IrcMessage);
    QString msg = d->param(1);
    if (flags() & (Identified | Unidentified))
        msg.remove(0, 1);
    return msg.startsWith("\1ACTION ") && msg.endsWith('\1');
}

/*!
    \property bool IrcPrivateMessage::request
    This property is \c true if the message is a request; otherwise \c false.

    \par Access functions:
    \li bool <b>isRequest</b>() const
 */
bool IrcPrivateMessage::isRequest() const
{
    Q_D(const IrcMessage);
    QString msg = d->param(1);
    if (flags() & (Identified | Unidentified))
        msg.remove(0, 1);
    return msg.startsWith('\1') && msg.endsWith('\1') && !isAction();
}

bool IrcPrivateMessage::isValid() const
{
    return IrcMessage::isValid() && !target().isEmpty() && !message().isEmpty();
}

/*!
    \class IrcNoticeMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a notice message.
 */

/*!
    Constructs a new IrcNoticeMessage with \a connection.
 */
IrcNoticeMessage::IrcNoticeMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Notice;
}

/*!
    This property holds the target channel or user in question.

    \par Access functions:
    \li QString <b>target</b>() const
 */
QString IrcNoticeMessage::target() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the message.

    \par Access functions:
    \li QString <b>message</b>() const
 */
QString IrcNoticeMessage::message() const
{
    Q_D(const IrcMessage);
    QString msg = d->param(1);
    if (flags() & (Identified | Unidentified))
        msg.remove(0, 1);
    if (isReply()) {
        msg.remove(0, 1);
        msg.chop(1);
    }
    return msg;
}

/*!
    \property bool IrcNoticeMessage::reply
    This property is \c true if the message is a reply; otherwise \c false.

    \par Access functions:
    \li bool <b>isReply</b>() const
 */
bool IrcNoticeMessage::isReply() const
{
    Q_D(const IrcMessage);
    QString msg = d->param(1);
    return msg.startsWith('\1') && msg.endsWith('\1');
}

bool IrcNoticeMessage::isValid() const
{
    return IrcMessage::isValid() && !target().isEmpty() && !message().isEmpty();
}

/*!
    \class IrcPingMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a ping message.
 */

/*!
    Constructs a new IrcPingMessage with \a connection.
 */
IrcPingMessage::IrcPingMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Ping;
}

/*!
    This property holds the optional message argument.

    \par Access functions:
    \li QString <b>argument</b>() const
 */
QString IrcPingMessage::argument() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

bool IrcPingMessage::isValid() const
{
    return IrcMessage::isValid();
}

/*!
    \class IrcPongMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a pong message.
 */

/*!
    Constructs a new IrcPongMessage with \a connection.
 */
IrcPongMessage::IrcPongMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Pong;
}

/*!
    This property holds the optional message argument.

    \par Access functions:
    \li QString <b>argument</b>() const
 */
QString IrcPongMessage::argument() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

bool IrcPongMessage::isValid() const
{
    return IrcMessage::isValid();
}

/*!
    \class IrcErrorMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents an error message.
 */

/*!
    Constructs a new IrcErrorMessage with \a connection.
 */
IrcErrorMessage::IrcErrorMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Error;
}

/*!
    This property holds the error.

    \par Access functions:
    \li QString <b>error</b>() const
 */
QString IrcErrorMessage::error() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

bool IrcErrorMessage::isValid() const
{
    return IrcMessage::isValid() && !error().isEmpty();
}

/*!
    \class IrcNumericMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a numeric message.
 */

/*!
    Constructs a new IrcNumericMessage with \a connection.
 */
IrcNumericMessage::IrcNumericMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Numeric;
}

/*!
    This property holds the numeric code.

    \par Access functions:
    \li int <b>code</b>() const
 */
int IrcNumericMessage::code() const
{
    Q_D(const IrcMessage);
    bool ok = false;
    int number = d->command().toInt(&ok);
    return ok ? number : -1;
}

bool IrcNumericMessage::isValid() const
{
    return IrcMessage::isValid() && code() != -1;
}

/*!
    \class IrcCapabilityMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a capability message.
 */

/*!
    Constructs a new IrcCapabilityMessage with \a connection.
 */
IrcCapabilityMessage::IrcCapabilityMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Capability;
}

/*!
    This property holds the subcommand.

    The following capability subcommands are defined:
    LS, LIST, REQ, ACK, NAK, CLEAR, END

    \par Access functions:
    \li QString <b>subCommand</b>() const
 */
QString IrcCapabilityMessage::subCommand() const
{
    Q_D(const IrcMessage);
    return d->param(1);
}

/*!
    This property holds the capabilities.

    A list of capabilities may exist for the following
    subcommands: LS, LIST, REQ, ACK and NAK.

    \par Access functions:
    \li QStringList <b>capabilities</b>() const
 */
QStringList IrcCapabilityMessage::capabilities() const
{
    Q_D(const IrcMessage);
    QStringList caps;
    QStringList params = d->params();
    if (params.count() > 2)
        caps = params.last().split(QLatin1Char(' '), QString::SkipEmptyParts);
    return caps;
}

bool IrcCapabilityMessage::isValid() const
{
    return IrcMessage::isValid();
}

/*!
    \class IrcMotdMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a message of the day.
 */

/*!
    Constructs a new IrcMotdMessage with \a connection.
 */
IrcMotdMessage::IrcMotdMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Motd;
}

/*!
    This property holds the message of the day lines.

    \par Access functions:
    \li QStringList <b>lines</b>() const
 */
QStringList IrcMotdMessage::lines() const
{
    Q_D(const IrcMessage);
    return d->params().mid(1);
}

bool IrcMotdMessage::isValid() const
{
    Q_D(const IrcMessage);
    return d->connection && sender().isValid() && !d->params().isEmpty();
}

/*!
    \class IrcNamesMessage ircmessage.h <IrcMessage>
    \ingroup message
    \brief Represents a names list message.
 */

/*!
    Constructs a new IrcNamesMessage with \a connection.
 */
IrcNamesMessage::IrcNamesMessage(IrcConnection* connection) : IrcMessage(connection)
{
    Q_D(IrcMessage);
    d->type = Names;
}

/*!
    This property holds the channel.

    \par Access functions:
    \li QString <b>channel</b>() const
 */
QString IrcNamesMessage::channel() const
{
    Q_D(const IrcMessage);
    return d->param(0);
}

/*!
    This property holds the list of names.

    \par Access functions:
    \li QStringList <b>names</b>() const
 */
QStringList IrcNamesMessage::names() const
{
    Q_D(const IrcMessage);
    return d->params().mid(1);
}

bool IrcNamesMessage::isValid() const
{
    Q_D(const IrcMessage);
    return d->connection && sender().isValid() && !d->params().isEmpty();
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const IrcMessage* message)
{
    if (!message)
        return debug << "IrcMessage(0x0) ";
    debug.nospace() << message->metaObject()->className() << '(' << (void*) message;
    QStringList flags;
    if (message->flags() == IrcMessage::None)
        flags << "None";
    else if (message->flags() & IrcMessage::Identified)
        flags << "Identified";
    else if (message->flags() & IrcMessage::Unidentified)
        flags << "Unidentified";
    debug << ", flags = " << flags;
    if (!message->objectName().isEmpty())
        debug << ", name = " << message->objectName();
    if (message->sender().isValid())
        debug << ", sender = " << message->sender().name();
    if (!message->command().isEmpty())
        debug << ", command = " << message->command();
    if (!message->parameters().isEmpty())
        debug << ", params = " << message->parameters();
    debug.nospace() << ')';
    return debug.space();
}
#endif // QT_NO_DEBUG_STREAM

IRC_END_NAMESPACE
