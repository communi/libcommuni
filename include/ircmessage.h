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

#include <irc.h>
#include <ircglobal.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvarlengtharray.h>

class IRC_EXPORT IrcMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint type READ type WRITE setType)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString command READ command WRITE setCommand)
    Q_PROPERTY(QStringList parameters READ parameters WRITE setParameters)

public:
    Q_INVOKABLE explicit IrcMessage(QObject* parent = 0) :
        QObject(parent), t(Irc::Unknown) { }

    uint type() const { return t; }
    void setType(uint type) { t = type; }

    QString prefix() const { return pfx; }
    void setPrefix(const QString& prefix) { pfx = prefix; }

    QString command() const { return cmd; }
    void setCommand(const QString& command) { cmd = command; }

    QStringList parameters() const { return params; }
    void setParameters(const QStringList& parameters) { params = parameters; }

    enum CommandSyntax { RegExp, Wildcard, FixedString };

    template <typename T>
    static void registerCommand(const QString& command, CommandSyntax syntax = FixedString);
    static void unregisterCommand(const QString& command);

    Q_INVOKABLE static IrcMessage* create(const QString& command, QObject* parent = 0);

    virtual QString toString() const;
    virtual void initFrom(const QString& prefix, const QStringList& parameters);

protected:
    uint t;
    QString pfx;
    QString cmd;
    QStringList params;
    static QVarLengthArray<QHash<QString, const QMetaObject*>, 3> metaObjects;
};

template <typename T>
void IrcMessage::registerCommand(const QString& command, IrcMessage::CommandSyntax syntax)
{
    metaObjects[syntax].insert(command, &T::staticMetaObject);
}

// connection registration

class IRC_EXPORT IrcPasswordMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    Q_INVOKABLE explicit IrcPasswordMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Irc::Password; }

    QString password() const { return passwd; }
    void setPassword(const QString& password) { passwd = password; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString passwd;
};

class IRC_EXPORT IrcNickMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString nick READ nick WRITE setNick)

public:
    Q_INVOKABLE explicit IrcNickMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Irc::Nick; }

    QString nick() const { return n; }
    void setNick(const QString& nick) { n = nick; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString n;
};

class IRC_EXPORT IrcUserMessage : public IrcMessage
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString realName READ realName WRITE setRealName)

public:
    Q_INVOKABLE explicit IrcUserMessage(QObject* parent = 0) :
        IrcMessage(parent) { t = Irc::User; }

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
        IrcPasswordMessage(parent) { t = Irc::Operator; }

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
        IrcMessage(parent) { t = Irc::Quit; }

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
        IrcChannelMessage(parent) { t = Irc::Join; }

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
        IrcChannelMessage(parent) { t = Irc::Part; }

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
        IrcChannelMessage(parent) { t = Irc::Topic; }

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
        IrcChannelMessage(parent) { t = Irc::Names; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcListMessage : public IrcChannelMessage
{
    Q_OBJECT
    Q_PROPERTY(QString server READ server WRITE setServer)

public:
    Q_INVOKABLE explicit IrcListMessage(QObject* parent = 0) :
        IrcChannelMessage(parent) { t = Irc::List; }

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
        IrcChannelMessage(parent) { t = Irc::Invite; }

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
        IrcChannelMessage(parent) { t = Irc::Kick; }

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
    Q_PROPERTY(QString argument READ argument WRITE setArgument)
    Q_PROPERTY(QString mask READ mask WRITE setMask)

public:
    Q_INVOKABLE explicit IrcModeMessage(QObject* parent = 0) :
        IrcMessage(parent) { }

    QString target() const { return tgt; }
    void setTarget(const QString& target) { tgt = target; }

    QString mode() const { return mod; }
    void setMode(const QString& mode) { mod = mode; }

    QString argument() const { return arg; }
    void setArgument(const QString& argument) { arg = argument; }

    QString mask() const { return msk; }
    void setMask(const QString& mask) { msk = mask; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    QString tgt;
    QString mod;
    QString arg;
    QString msk;
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
    Q_PROPERTY(bool action READ isAction WRITE setAction)
    Q_PROPERTY(bool request READ isRequest WRITE setRequest)

public:
    Q_INVOKABLE explicit IrcPrivateMessage(QObject* parent = 0) :
        IrcSendMessage(parent), act(false), req(false) { t = Irc::Private; }

    bool isAction() const { return act; }
    void setAction(bool action) { act = action; }

    bool isRequest() const { return req; }
    void setRequest(bool request) { req = request; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    bool act;
    bool req;
};

class IRC_EXPORT IrcNoticeMessage : public IrcSendMessage
{
    Q_OBJECT
    Q_PROPERTY(bool reply READ isReply WRITE setReply)

public:
    Q_INVOKABLE explicit IrcNoticeMessage(QObject* parent = 0) :
        IrcSendMessage(parent), rpl(false) { t = Irc::Notice; }

    bool isReply() const { return rpl; }
    void setReply(bool reply) { rpl = reply; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);

protected:
    bool rpl;
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
        IrcQueryMessage(parent) { t = Irc::Who; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcWhoisMessage : public IrcQueryMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhoisMessage(QObject* parent = 0) :
        IrcQueryMessage(parent) { t = Irc::Whois; }

    QString toString() const;
    void initFrom(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcWhowasMessage : public IrcQueryMessage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit IrcWhowasMessage(QObject* parent = 0) :
        IrcQueryMessage(parent) { t = Irc::Whowas; }

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
        IrcMessage(parent) { t = Irc::Ping; }

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
        IrcMessage(parent) { t = Irc::Pong; }

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
        IrcMessage(parent) { t = Irc::Error; }

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
        IrcMessage(parent), c(0) { t = Irc::Numeric; }

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
        IrcMessage(parent) { t = Irc::Away; }

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
