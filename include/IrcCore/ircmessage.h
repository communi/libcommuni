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

#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <Irc>
#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstringlist.h>

IRC_BEGIN_NAMESPACE

class IrcCommand;
class IrcNetwork;
class IrcConnection;
class IrcMessagePrivate;

class IRC_CORE_EXPORT IrcMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(IrcConnection* connection READ connection)
    Q_PROPERTY(IrcNetwork* network READ network)
    Q_PROPERTY(Type type READ type)
    Q_PROPERTY(Flags flags READ flags)
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(QString command READ command)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString nick READ nick)
    Q_PROPERTY(QString ident READ ident)
    Q_PROPERTY(QString host READ host)
    Q_PROPERTY(QStringList parameters READ parameters WRITE setParameters)
    Q_PROPERTY(QDateTime timeStamp READ timeStamp WRITE setTimeStamp)
    Q_ENUMS(Type Flag)
    Q_FLAGS(Flags)

public:
    enum Type {
        Unknown,
        Capability,
        Error,
        Invite,
        Join,
        Kick,
        Mode,
        Motd,
        Names,
        Nick,
        Notice,
        Numeric,
        Part,
        Ping,
        Pong,
        Private,
        Quit,
        Topic
    };

    enum Flag {
        None = 0x0,
        Own = 0x1,
        Identified = 0x2,
        Unidentified = 0x4
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    Q_INVOKABLE explicit IrcMessage(IrcConnection* connection);
    virtual ~IrcMessage();

    IrcConnection* connection() const;
    IrcNetwork* network() const;

    Type type() const;
    Flags flags() const;

    QString command() const;
    void setCommand(const QString& command);

    QString prefix() const;
    void setPrefix(const QString& prefix);

    QString nick() const;
    QString ident() const;
    QString host() const;

    QStringList parameters() const;
    void setParameters(const QStringList& parameters);

    virtual bool isValid() const;

    QDateTime timeStamp() const;
    void setTimeStamp(const QDateTime& timeStamp);

    QByteArray encoding() const;
    void setEncoding(const QByteArray& encoding);

    Q_INVOKABLE QByteArray toData() const;
    Q_INVOKABLE static IrcMessage* fromData(const QByteArray& data, IrcConnection* connection);
    Q_INVOKABLE static IrcMessage* fromParameters(const QString& prefix, const QString& command, const QStringList& parameters, IrcConnection* connection);

protected:
    QScopedPointer<IrcMessagePrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcMessage)
    Q_DISABLE_COPY(IrcMessage)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IrcMessage::Flags)

class IRC_CORE_EXPORT IrcCapabilityMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString subCommand READ subCommand)
    Q_PROPERTY(QStringList capabilities READ capabilities)

public:
    Q_INVOKABLE explicit IrcCapabilityMessage(IrcConnection* connection);

    QString subCommand() const;
    QStringList capabilities() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcCapabilityMessage)
};

class IRC_CORE_EXPORT IrcErrorMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error)

public:
    Q_INVOKABLE explicit IrcErrorMessage(IrcConnection* connection);

    QString error() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcErrorMessage)
};

class IRC_CORE_EXPORT IrcInviteMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user)
    Q_PROPERTY(QString channel READ channel)

public:
    Q_INVOKABLE explicit IrcInviteMessage(IrcConnection* connection);

    QString user() const;
    QString channel() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcInviteMessage)
};

class IRC_CORE_EXPORT IrcJoinMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel)

public:
    Q_INVOKABLE explicit IrcJoinMessage(IrcConnection* connection);

    QString channel() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcJoinMessage)
};

class IRC_CORE_EXPORT IrcKickMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel)
    Q_PROPERTY(QString user READ user)
    Q_PROPERTY(QString reason READ reason)

public:
    Q_INVOKABLE explicit IrcKickMessage(IrcConnection* connection);

    QString channel() const;
    QString user() const;
    QString reason() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcKickMessage)
};

class IRC_CORE_EXPORT IrcModeMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target)
    Q_PROPERTY(QString mode READ mode)
    Q_PROPERTY(QString argument READ argument)
    Q_PROPERTY(QStringList arguments READ arguments)
    Q_PROPERTY(bool reply READ isReply)
    Q_PROPERTY(Kind kind READ kind)
    Q_ENUMS(Kind)

public:
    Q_INVOKABLE explicit IrcModeMessage(IrcConnection* connection);

    QString target() const;
    QString mode() const;
    QString argument() const;
    QStringList arguments() const;
    bool isReply() const;

    enum Kind { Channel, User };
    Kind kind() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcModeMessage)
};

class IRC_CORE_EXPORT IrcMotdMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QStringList lines READ lines)

public:
    Q_INVOKABLE explicit IrcMotdMessage(IrcConnection* connection);

    QStringList lines() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcMotdMessage)
};

class IRC_CORE_EXPORT IrcNamesMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel)
    Q_PROPERTY(QStringList names READ names)

public:
    Q_INVOKABLE explicit IrcNamesMessage(IrcConnection* connection);

    QString channel() const;
    QStringList names() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcNamesMessage)
};

class IRC_CORE_EXPORT IrcNickMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString oldNick READ oldNick)
    Q_PROPERTY(QString newNick READ newNick)

public:
    Q_INVOKABLE explicit IrcNickMessage(IrcConnection* connection);

    QString oldNick() const;
    QString newNick() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcNickMessage)
};

class IRC_CORE_EXPORT IrcNoticeMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target)
    Q_PROPERTY(QString content READ content)
    Q_PROPERTY(bool private READ isPrivate)
    Q_PROPERTY(bool reply READ isReply)

public:
    Q_INVOKABLE explicit IrcNoticeMessage(IrcConnection* connection);

    QString target() const;
    QString content() const;
    bool isPrivate() const;
    bool isReply() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcNoticeMessage)
};

class IRC_CORE_EXPORT IrcNumericMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(int code READ code)

public:
    Q_INVOKABLE explicit IrcNumericMessage(IrcConnection* connection);

    int code() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcNumericMessage)
};

class IRC_CORE_EXPORT IrcPartMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel)
    Q_PROPERTY(QString reason READ reason)

public:
    Q_INVOKABLE explicit IrcPartMessage(IrcConnection* connection);

    QString channel() const;
    QString reason() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcPartMessage)
};

class IRC_CORE_EXPORT IrcPingMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString argument READ argument)

public:
    Q_INVOKABLE explicit IrcPingMessage(IrcConnection* connection);

    QString argument() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcPingMessage)
};

class IRC_CORE_EXPORT IrcPongMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString argument READ argument)

public:
    Q_INVOKABLE explicit IrcPongMessage(IrcConnection* connection);

    QString argument() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcPongMessage)
};

class IRC_CORE_EXPORT IrcPrivateMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target)
    Q_PROPERTY(QString content READ content)
    Q_PROPERTY(bool private READ isPrivate)
    Q_PROPERTY(bool action READ isAction)
    Q_PROPERTY(bool request READ isRequest)

public:
    Q_INVOKABLE explicit IrcPrivateMessage(IrcConnection* connection);

    QString target() const;
    QString content() const;
    bool isPrivate() const;
    bool isAction() const;
    bool isRequest() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcPrivateMessage)
};

class IRC_CORE_EXPORT IrcQuitMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString reason READ reason)

public:
    Q_INVOKABLE explicit IrcQuitMessage(IrcConnection* connection);

    QString reason() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcQuitMessage)
};

class IRC_CORE_EXPORT IrcTopicMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel)
    Q_PROPERTY(QString topic READ topic)
    Q_PROPERTY(bool reply READ isReply)

public:
    Q_INVOKABLE explicit IrcTopicMessage(IrcConnection* connection);

    QString channel() const;
    QString topic() const;
    bool isReply() const;

    bool isValid() const;

private:
    Q_DISABLE_COPY(IrcTopicMessage)
};

#ifndef QT_NO_DEBUG_STREAM
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, IrcMessage::Type type);
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, IrcMessage::Flag flag);
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, IrcMessage::Flags flags);
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, IrcModeMessage::Kind kind);
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, const IrcMessage* message);
#endif // QT_NO_DEBUG_STREAM

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcMessage::Type))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcCapabilityMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcErrorMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcInviteMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcJoinMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcKickMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcModeMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcMotdMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcNamesMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcNickMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcNoticeMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcNumericMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcPartMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcPingMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcPongMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcPrivateMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcQuitMessage*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcTopicMessage*))

#endif // IRCMESSAGE_H
