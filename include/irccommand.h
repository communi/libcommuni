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

#ifndef IRCCOMMAND_H
#define IRCCOMMAND_H

#include <irc.h>
#include <ircglobal.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class COMMUNI_EXPORT IrcCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CommandType type READ type)
    Q_ENUMS(CommandType)

public:
    enum CommandType
    {
        Unknown,

        // connection registration
        Password,
        Nick,
        User,
        Operator,
        Quit,

        // channel operations
        Join,
        Part,
        Topic,
        Names,
        List,
        Invite,
        Kick,

        // mode operations
        Mode,

        // sending messages
        Message,
        Notice,

        // user-based queries
        Who,
        Whois,
        Whowas,

        // miscellaneous commands
        Ping,
        Pong,
        Away
    };

    Q_INVOKABLE explicit IrcCommand(QObject* parent = 0) :
        QObject(parent), t(Unknown) { }

    CommandType type() const { return t; }

    Q_INVOKABLE static IrcCommand* create(const QString& command, QObject* parent = 0);

    virtual QString toString() const;

protected:
    CommandType t;
    QString pfx;
    QString cmd;
    QStringList params;
};

// connection registration

class COMMUNI_EXPORT IrcPasswordCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    Q_INVOKABLE explicit IrcPasswordCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = Password; }

    QString password() const { return passwd; }
    void setPassword(const QString& password) { passwd = password; }

    QString toString() const;

protected:
    QString passwd;
};

class COMMUNI_EXPORT IrcNickCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString nick READ nick WRITE setNick)

public:
    Q_INVOKABLE explicit IrcNickCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = Nick; }

    QString nick() const { return n; }
    void setNick(const QString& nick) { n = nick; }

    QString toString() const;

protected:
    QString n;
};

class COMMUNI_EXPORT IrcUserCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString realName READ realName WRITE setRealName)

public:
    Q_INVOKABLE explicit IrcUserCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = User; }

    QString userName() const { return user; }
    void setUserName(const QString& userName) { user = userName; }

    QString realName() const { return real; }
    void setRealName(const QString& realName) { real = realName; }

    QString toString() const;

protected:
    QString user;
    QString real;
};

class COMMUNI_EXPORT IrcOperatorCommand : public IrcPasswordCommand
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)

public:
    Q_INVOKABLE explicit IrcOperatorCommand(QObject* parent = 0) :
        IrcPasswordCommand(parent) { t = Operator; }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString toString() const;

protected:
    QString usr;
};

class COMMUNI_EXPORT IrcQuitCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    Q_INVOKABLE explicit IrcQuitCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = Quit; }

    QString reason() const { return rson; }
    void setReason(const QString& reason) { rson = reason; }

    QString toString() const;

protected:
    QString rson;
};

// channel operations

class COMMUNI_EXPORT IrcChannelCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel WRITE setChannel)

public:
    Q_INVOKABLE explicit IrcChannelCommand(QObject* parent = 0) :
        IrcCommand(parent) { }

    QString channel() const { return chan; }
    void setChannel(const QString& channel) { chan = channel; }

protected:
    QString chan;
};

class COMMUNI_EXPORT IrcJoinCommand : public IrcChannelCommand
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey)

public:
    Q_INVOKABLE explicit IrcJoinCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = Join; }

    QString key() const { return k; }
    void setKey(const QString& key) { k = key; }

    QString toString() const;

protected:
    QString k;
};

class COMMUNI_EXPORT IrcPartCommand : public IrcChannelCommand
{
    Q_OBJECT
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    Q_INVOKABLE explicit IrcPartCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = Part; }

    QString reason() const { return rson; }
    void setReason(const QString& reason) { rson = reason; }

    QString toString() const;

protected:
    QString rson;
};

class COMMUNI_EXPORT IrcTopicCommand : public IrcChannelCommand
{
    Q_OBJECT
    Q_PROPERTY(QString topic READ topic WRITE setTopic)

public:
    Q_INVOKABLE explicit IrcTopicCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = Topic; }

    QString topic() const { return tpc; }
    void setTopic(const QString& topic) { tpc = topic; }

    QString toString() const;

protected:
    QString tpc;
};

class COMMUNI_EXPORT IrcNamesCommand : public IrcChannelCommand
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcNamesCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = Names; }

    QString toString() const;
};

class COMMUNI_EXPORT IrcListCommand : public IrcChannelCommand
{
    Q_OBJECT
    Q_PROPERTY(QString server READ server WRITE setServer)

public:
    Q_INVOKABLE explicit IrcListCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = List; }

    QString server() const { return srv; }
    void setServer(const QString& server) { srv = server; }

    QString toString() const;

protected:
    QString srv;
};

class COMMUNI_EXPORT IrcInviteCommand : public IrcChannelCommand
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)

public:
    Q_INVOKABLE explicit IrcInviteCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = Invite; }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString toString() const;

protected:
    QString usr;
};

class COMMUNI_EXPORT IrcKickCommand : public IrcChannelCommand
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)
    Q_PROPERTY(QString reason READ reason WRITE setReason)

public:
    Q_INVOKABLE explicit IrcKickCommand(QObject* parent = 0) :
        IrcChannelCommand(parent) { t = Kick; }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString reason() const { return rson; }
    void setReason(const QString& reason) { rson = reason; }

    QString toString() const;

protected:
    QString usr;
    QString rson;
};

// mode operations

class COMMUNI_EXPORT IrcModeCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)
    Q_PROPERTY(QString mode READ mode WRITE setMode)
    Q_PROPERTY(QString argument READ argument WRITE setArgument)
    Q_PROPERTY(QString mask READ mask WRITE setMask)

public:
    Q_INVOKABLE explicit IrcModeCommand(QObject* parent = 0) :
        IrcCommand(parent) { }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString mode() const { return mod; }
    void setMode(const QString& mode) { mod = mode; }

    QString argument() const { return arg; }
    void setArgument(const QString& argument) { arg = argument; }

    QString mask() const { return msk; }
    void setMask(const QString& mask) { msk = mask; }

    QString toString() const;

protected:
    QString tgt;
    QString mod;
    QString arg;
    QString msk;
};

// sending messages

class COMMUNI_EXPORT IrcSendCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)
    Q_PROPERTY(QString message READ message WRITE setMessage)

public:
    Q_INVOKABLE explicit IrcSendCommand(QObject* parent = 0) :
        IrcCommand(parent) { }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString message() const { return msg; }
    void setMessage(const QString& message) { msg = message; }

protected:
    QString tgt;
    QString msg;
};

class COMMUNI_EXPORT IrcMessageCommand : public IrcSendCommand
{
    Q_OBJECT
    Q_PROPERTY(bool action READ isAction WRITE setAction)
    Q_PROPERTY(bool request READ isRequest WRITE setRequest)

public:
    Q_INVOKABLE explicit IrcMessageCommand(QObject* parent = 0) :
        IrcSendCommand(parent), act(false), req(false) { t = Message; }

    bool isAction() const { return act; }
    void setAction(bool action) { act = action; }

    bool isRequest() const { return req; }
    void setRequest(bool request) { req = request; }

    QString toString() const;

protected:
    bool act;
    bool req;
};

class COMMUNI_EXPORT IrcNoticeCommand : public IrcSendCommand
{
    Q_OBJECT
    Q_PROPERTY(bool reply READ isReply WRITE setReply)

public:
    Q_INVOKABLE explicit IrcNoticeCommand(QObject* parent = 0) :
        IrcSendCommand(parent), rpl(false) { t = Notice; }

    bool isReply() const { return rpl; }
    void setReply(bool reply) { rpl = reply; }

    QString toString() const;

protected:
    bool rpl;
};

// user-based queries

class COMMUNI_EXPORT IrcQueryCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser)

public:
    Q_INVOKABLE explicit IrcQueryCommand(QObject* parent = 0) :
        IrcCommand(parent) { }

    QString user() const { return usr; }
    void setUser(const QString& user) { usr = user; }

    QString toString() const;

protected:
    QString usr;
};

class COMMUNI_EXPORT IrcWhoCommand : public IrcQueryCommand
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhoCommand(QObject* parent = 0) :
        IrcQueryCommand(parent) { t = Who; }

    QString toString() const;
};

class COMMUNI_EXPORT IrcWhoisCommand : public IrcQueryCommand
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhoisCommand(QObject* parent = 0) :
        IrcQueryCommand(parent) { t = Whois; }

    QString toString() const;
};

class COMMUNI_EXPORT IrcWhowasCommand : public IrcQueryCommand
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhowasCommand(QObject* parent = 0) :
        IrcQueryCommand(parent) { t = Whowas; }

    QString toString() const;
};

// miscellaneous commands

class COMMUNI_EXPORT IrcPingCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)

public:
    Q_INVOKABLE explicit IrcPingCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = Ping; }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString toString() const;

protected:
    QString tgt;
};

class COMMUNI_EXPORT IrcPongCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget)

public:
    Q_INVOKABLE explicit IrcPongCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = Pong; }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString toString() const;

protected:
    QString tgt;
};

class COMMUNI_EXPORT IrcAwayCommand : public IrcCommand
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setCommand)

public:
    Q_INVOKABLE explicit IrcAwayCommand(QObject* parent = 0) :
        IrcCommand(parent) { t = Away; }

    QString message() const { return msg; }
    void setCommand(const QString& message) { msg = message; }

    QString toString() const;

protected:
    QString msg;
};

#ifndef QT_NO_DEBUG_STREAM
COMMUNI_EXPORT QDebug operator<<(QDebug debug, const IrcCommand* command);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCCOMMAND_H
