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

#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <ircglobal.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class IRC_EXPORT IrcMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint type READ type WRITE setType)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QStringList parameters READ parameters WRITE setParameters)
    Q_ENUMS(Type)

public:
    enum Type
    {
        Unknown = 0,

        // connection registration
        Password = 1,
        NickName = 2,
        User = 3,
        Operator = 4,
        Quit = 5,

        // channel operations
        Join = 10,
        Part = 11,
        Topic = 12,
        Names = 13,
        List = 14,
        Invite = 15,
        Kick = 16,

        // mode operations
        ChannelMode = 20,
        UserMode = 21,

        // sending messages
        Private = 30,
        Notice = 31,

        // ctcp messages
        CtcpAction = 40,
        CtcpRequest = 41,
        CtcpReply = 42,

        // user-based queries
        Who = 50,
        Whois = 51,
        Whowas = 52,

        // miscellaneous messages
        Ping = 60,
        Pong = 61,
        Error = 62,
        Numeric = 63,
        Away = 64,

        // custom messages
        Custom = 100
    };

    Q_INVOKABLE explicit IrcMessage(QObject* parent = 0) :
        QObject(parent), t(Unknown) { }

    uint type() const { return t; }
    void setType(uint type) { t = type; }

    QString prefix() const { return pfx; }
    void setPrefix(const QString& prefix) { pfx = prefix; }

    QStringList parameters() const { return params; }
    void setParameters(const QStringList& parameters) { params = parameters; }

    template <typename T>
    static void registerCommand(const QString& command);
    static void unregisterCommand(const QString& command);

    static IrcMessage* create(const QString& command, QObject* parent = 0);

    virtual QString toString() const;
    virtual void initFrom(const QString& prefix, const QStringList& parameters);

protected:
    uint t;
    QString pfx;
    QStringList params;
    static QHash<QString, const QMetaObject*> metaObjects;
};

template <typename T>
void IrcMessage::registerCommand(const QString& command)
{
    metaObjects.insert(command, &T::staticMetaObject);
}

// connection registration

class IRC_EXPORT IrcPasswordMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    Q_INVOKABLE explicit IrcPasswordMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Password; }

    QString password() const { return passwd; }
    void setPassword(const QString& password) { passwd = password; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString passwd;
};

class IRC_EXPORT IrcNickNameMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString nickName READ nickName WRITE setNickName)

public:
    Q_INVOKABLE explicit IrcNickNameMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = NickName; }

    QString nickName() const { return nick; }
    void setNickName(const QString& nickName) { nick = nickName; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString nick;
};

class IRC_EXPORT IrcUserMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString realName READ realName WRITE setRealName)

public:
    Q_INVOKABLE explicit IrcUserMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = User; }

    QString userName() const { return user; }
    void setUserName(const QString& userName) { user = userName; }

    QString realName() const { return real; }
    void setRealName(const QString& realName) { real = realName; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString user;
    QString real;
};

class IRC_EXPORT IrcOperatorMessage : public IrcPasswordMessage
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)

public:
    Q_INVOKABLE explicit IrcOperatorMessage(QObject* parent = 0) :
        IrcPasswordMessage(parent) { t = Operator; }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString usr;
};

class IRC_EXPORT IrcQuitMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    Q_INVOKABLE explicit IrcQuitMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Quit; }

    QString reason() const { return rson; }
    void setReason(const QString& reason) { rson = reason; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString rson;
};

// channel operations

class IRC_EXPORT IrcChannelMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel WRITE setChannel)

public:
    Q_INVOKABLE explicit IrcChannelMessage(QObject* parent = 0) :
        IrcMessage(parent) { }

    QString channel() const { return chan; }
    void setChannel(const QString& channel) { chan = channel; }

    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString chan;
};

class IRC_EXPORT IrcJoinMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey)

public:
    Q_INVOKABLE explicit IrcJoinMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Join; }

    QString key() const { return k; }
    void setKey(const QString& key) { k = key; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString k;
};

class IRC_EXPORT IrcPartMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    Q_INVOKABLE explicit IrcPartMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Part; }

    QString reason() const { return rson; }
    void setReason(const QString& reason) { rson = reason; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString rson;
};

class IRC_EXPORT IrcTopicMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString topic READ topic WRITE setTopic)

public:
    Q_INVOKABLE explicit IrcTopicMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Topic; }

    QString topic() const { return tpc; }
    void setTopic(const QString& topic) { tpc = topic; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tpc;
};

class IRC_EXPORT IrcNamesMessage : public IrcChannelMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcNamesMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Names; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcListMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString server READ server WRITE setServer)

public:
    Q_INVOKABLE explicit IrcListMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = List; }

    QString server() const { return srv; }
    void setServer(const QString& server) { srv = server; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString srv;
};

class IRC_EXPORT IrcInviteMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)

public:
    Q_INVOKABLE explicit IrcInviteMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Invite; }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString usr;
};

class IRC_EXPORT IrcKickMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    Q_INVOKABLE explicit IrcKickMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Kick; }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString reason() const { return rson; }
    void setReason(const QString& reason) { rson = reason; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString usr;
    QString rson;
};

// mode operations

class IRC_EXPORT IrcModeMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)
    Q_PROPERTY(QString mode READ mode WRITE setMode)

public:
    Q_INVOKABLE explicit IrcModeMessage(QObject* parent = 0) :
        IrcMessage(parent) { }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString mode() const { return mod; }
    void setMode(const QString& mode) { mod = mode; }

    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tgt;
    QString mod;
};

class IRC_EXPORT IrcChannelModeMessage : public IrcModeMessage
{
    Q_OBJECT
    Q_PROPERTY(QString argument READ argument WRITE setArgument)
    Q_PROPERTY(QString mask READ mask WRITE setMask)

public:
    Q_INVOKABLE explicit IrcChannelModeMessage(QObject* parent = 0) :
        IrcModeMessage(parent) { t = ChannelMode; }

    QString argument() const { return arg; }
    void setArgument(const QString& argument) { arg = argument; }

    QString mask() const { return msk; }
    void setMask(const QString& mask) { msk = mask; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString arg;
    QString msk;
};

class IRC_EXPORT IrcUserModeMessage : public IrcModeMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcUserModeMessage(QObject* parent = 0) :
        IrcModeMessage(parent) { t = UserMode; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

// sending messages

class IRC_EXPORT IrcSendMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)
    Q_PROPERTY(QString message READ message WRITE setMessage)

public:
    Q_INVOKABLE explicit IrcSendMessage(QObject* parent = 0) :
        IrcMessage(parent) { }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString message() const { return msg; }
    void setMessage(const QString& message) { msg = message; }

    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tgt;
    QString msg;
};

class IRC_EXPORT IrcPrivateMessage : public IrcSendMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcPrivateMessage(QObject* parent = 0) :
        IrcSendMessage(parent) { t = Private; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcNoticeMessage : public IrcSendMessage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit IrcNoticeMessage(QObject* parent = 0) :
        IrcSendMessage(parent) { t = Notice; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

// ctcp messages

class IRC_EXPORT IrcCtcpMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)
    Q_PROPERTY(QString message READ message WRITE setMessage)

public:
    Q_INVOKABLE explicit IrcCtcpMessage(QObject* parent = 0) :
        IrcMessage(parent) { }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString message() const { return msg; }
    void setMessage(const QString& message) { msg = message; }

    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tgt;
    QString msg;
};

class IRC_EXPORT IrcCtcpActionMessage : public IrcCtcpMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcCtcpActionMessage(QObject* parent = 0) :
        IrcCtcpMessage(parent) { t = CtcpAction; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcCtcpRequestMessage : public IrcCtcpMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcCtcpRequestMessage(QObject* parent = 0) :
        IrcCtcpMessage(parent) { t = CtcpRequest; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcCtcpReplyMessage : public IrcCtcpMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcCtcpReplyMessage(QObject* parent = 0) :
        IrcCtcpMessage(parent) { t = CtcpReply; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

// user-based queries

class IRC_EXPORT IrcQueryMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)

public:
    Q_INVOKABLE explicit IrcQueryMessage(QObject* parent = 0) :
        IrcMessage(parent) { }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString usr;
};

class IRC_EXPORT IrcWhoMessage : public IrcQueryMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhoMessage(QObject* parent = 0) :
        IrcQueryMessage(parent) { t = Who; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcWhoisMessage : public IrcQueryMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhoisMessage(QObject* parent = 0) :
        IrcQueryMessage(parent) { t = Whois; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcWhowasMessage : public IrcQueryMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhowasMessage(QObject* parent = 0) :
        IrcQueryMessage(parent) { t = Whowas; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

// miscellaneous messages

class IRC_EXPORT IrcPingMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)

public:
    Q_INVOKABLE explicit IrcPingMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Ping; }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tgt;
};

class IRC_EXPORT IrcPongMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)

public:
    Q_INVOKABLE explicit IrcPongMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Pong; }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tgt;
};

class IRC_EXPORT IrcErrorMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error WRITE setError)

public:
    Q_INVOKABLE explicit IrcErrorMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Error; }

    QString error() const { return err; }
    void setError(const QString& error) { err = error; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString err;
};

class IRC_EXPORT IrcNumericMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(uint code READ code WRITE setCode)

public:
    Q_INVOKABLE explicit IrcNumericMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Numeric; }

    uint code() const { return c; }
    void setCode(uint code) { c = code; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    uint c;
};

class IRC_EXPORT IrcAwayMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage)

public:
    Q_INVOKABLE explicit IrcAwayMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Away; }

    QString message() const { return msg; }
    void setMessage(const QString& message) { msg = message; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString msg;
};

#ifndef QT_NO_DEBUG_STREAM
IRC_EXPORT QDebug operator<<(QDebug debug, const IrcMessage* message);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCMESSAGE_H
