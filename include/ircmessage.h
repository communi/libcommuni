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
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class IRC_EXPORT IrcMessage
{
    Q_GADGET
    Q_ENUMS(Type)

public:
    enum Type
    {
        None = 0,

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
        Away = 64
    };

    IrcMessage(Type type);
    virtual ~IrcMessage();

    Type type() const { return t; }
    QString prefix() const { return pfx; }

    virtual QString toString() const = 0;

protected:
    Type t;
    QString pfx;
};

// connection registration

class IRC_EXPORT IrcPasswordMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcPasswordMessage(const QString& password);
    QString password() const { return passwd; }
    QString toString() const;
    static IrcPasswordMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString passwd;
};

class IRC_EXPORT IrcNickNameMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcNickNameMessage(const QString& nickName);
    QString nickName() const { return nick; }
    QString toString() const;
    static IrcNickNameMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString nick;
};

class IRC_EXPORT IrcUserMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcUserMessage(const QString& userName, const QString& realName);
    QString userName() const { return user; }
    QString realName() const { return real; }
    QString toString() const;
    static IrcUserMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString user;
    QString real;
};

class IRC_EXPORT IrcOperatorMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcOperatorMessage(const QString& user, const QString& password);
    QString user() const { return usr; }
    QString password() const { return passwd; }
    QString toString() const;
    static IrcOperatorMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString usr;
    QString passwd;
};

class IRC_EXPORT IrcQuitMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcQuitMessage(const QString& reason);
    QString reason() const { return rson; }
    QString toString() const;
    static IrcQuitMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString rson;
};

// channel operations

class IRC_EXPORT IrcChannelMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcChannelMessage(Type type, const QString& channel);
    QString channel() const { return chan; }
protected:
    QString chan;
};

class IRC_EXPORT IrcJoinMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcJoinMessage(const QString& channel, const QString& key = QString());
    QString key() const { return k; }
    QString toString() const;
    static IrcJoinMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString k;
};

class IRC_EXPORT IrcPartMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcPartMessage(const QString& channel, const QString& reason = QString());
    QString reason() const { return rson; }
    QString toString() const;
    static IrcPartMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString rson;
};

class IRC_EXPORT IrcTopicMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcTopicMessage(const QString& channel, const QString& topic = QString());
    QString topic() const { return tpc; }
    QString toString() const;
    static IrcTopicMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString tpc;
};

class IRC_EXPORT IrcNamesMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcNamesMessage(const QString& channel) :
        IrcChannelMessage(Names, channel) { }
    QString toString() const;
    static IrcNamesMessage fromString(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcListMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcListMessage(const QString& channel, const QString& server = QString());
    QString server() const { return srv; }
    QString toString() const;
    static IrcListMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString srv;
};

class IRC_EXPORT IrcInviteMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcInviteMessage(const QString& channel, const QString& user);
    QString user() const { return usr; }
    QString toString() const;
    static IrcInviteMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString usr;
};

class IRC_EXPORT IrcKickMessage : public IrcChannelMessage
{
    Q_GADGET
public:
    IrcKickMessage(const QString& channel, const QString& user, const QString& reason = QString());
    QString user() const { return usr; }
    QString reason() const { return rson; }
    QString toString() const;
    static IrcKickMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString usr;
    QString rson;
};

// mode operations

class IRC_EXPORT IrcModeMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcModeMessage(Type type, const QString& target, const QString& mode = QString());
    QString target() const { return tgt; }
    QString mode() const { return mod; }
protected:
    QString tgt;
    QString mod;
};

class IRC_EXPORT IrcChannelModeMessage : public IrcModeMessage
{
    Q_GADGET
public:
    IrcChannelModeMessage(const QString& channel, const QString& mode = QString(),
                          const QString& argument = QString(), const QString& mask = QString());
    QString mode() const { return mod; }
    QString argument() const { return arg; }
    QString mask() const { return msk; }
    QString toString() const;
    static IrcChannelModeMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString mod;
    QString arg;
    QString msk;
};

class IRC_EXPORT IrcUserModeMessage : public IrcModeMessage
{
    Q_GADGET
public:
    IrcUserModeMessage(const QString& user, const QString& mode) :
        IrcModeMessage(UserMode, user, mode) { }
    QString toString() const;
    static IrcUserModeMessage fromString(const QString& prefix, const QStringList& params);
};

// sending messages

class IRC_EXPORT IrcSendMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcSendMessage(Type type, const QString& target, const QString& message);
    QString target() const { return tgt; }
    QString message() const { return msg; }
protected:
    QString tgt;
    QString msg;
};

class IRC_EXPORT IrcPrivateMessage : public IrcSendMessage
{
    Q_GADGET
public:
    IrcPrivateMessage(const QString& target, const QString& message) :
        IrcSendMessage(Private, target, message) { }
    QString toString() const;
    static IrcPrivateMessage fromString(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcNoticeMessage : public IrcSendMessage
{
    Q_GADGET
public:
    IrcNoticeMessage(const QString& target, const QString& message) :
        IrcSendMessage(Notice, target, message) { }
    QString toString() const;
    static IrcNoticeMessage fromString(const QString& prefix, const QStringList& params);
};

// ctcp messages

class IRC_EXPORT IrcCtcpMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcCtcpMessage(Type type, const QString& target, const QString& message);
    QString target() const { return tgt; }
    QString message() const { return msg; }
protected:
    QString tgt;
    QString msg;
};

class IRC_EXPORT IrcCtcpActionMessage : public IrcCtcpMessage
{
    Q_GADGET
public:
    IrcCtcpActionMessage(const QString& target, const QString& message) :
        IrcCtcpMessage(CtcpAction, target, message) { }
    QString toString() const;
    static IrcCtcpActionMessage fromString(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcCtcpRequestMessage : public IrcCtcpMessage
{
    Q_GADGET
public:
    IrcCtcpRequestMessage(const QString& target, const QString& message) :
        IrcCtcpMessage(CtcpRequest, target, message) { }
    QString toString() const;
    static IrcCtcpRequestMessage fromString(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcCtcpReplyMessage : public IrcCtcpMessage
{
    Q_GADGET
public:
    IrcCtcpReplyMessage(const QString& target, const QString& message) :
        IrcCtcpMessage(CtcpReply, target, message) { }
    QString toString() const;
    static IrcCtcpReplyMessage fromString(const QString& prefix, const QStringList& params);
};

// user-based queries

class IRC_EXPORT IrcQueryMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcQueryMessage(Type type, const QString& user);
    QString user() const { return usr; }
protected:
    QString usr;
};

class IRC_EXPORT IrcWhoMessage : public IrcQueryMessage
{
    Q_GADGET
public:
    IrcWhoMessage(const QString& user) :
        IrcQueryMessage(Who, user) { }
    QString toString() const;
    static IrcWhoMessage fromString(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcWhoisMessage : public IrcQueryMessage
{
    Q_GADGET
public:
    IrcWhoisMessage(const QString& user) :
        IrcQueryMessage(Whois, user) { }
    QString toString() const;
    static IrcWhoisMessage fromString(const QString& prefix, const QStringList& params);
};

class IRC_EXPORT IrcWhowasMessage : public IrcQueryMessage
{
    Q_GADGET
public:
    IrcWhowasMessage(const QString& user) :
        IrcQueryMessage(Whowas, user) { }
    QString toString() const;
    static IrcWhowasMessage fromString(const QString& prefix, const QStringList& params);
};

// miscellaneous messages

class IRC_EXPORT IrcPingMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcPingMessage(const QString& target);
    QString target() const { return tgt; }
    QString toString() const;
    static IrcPingMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString tgt;
};

class IRC_EXPORT IrcPongMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcPongMessage(const QString& target);
    QString target() const { return tgt; }
    QString toString() const;
    static IrcPongMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString tgt;
};

class IRC_EXPORT IrcErrorMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcErrorMessage(const QString& error);
    QString error() const { return err; }
    QString toString() const;
    static IrcErrorMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString err;
};

class IRC_EXPORT IrcNumericMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcNumericMessage(uint code, const QStringList& params);
    uint code() const { return c; }
    QStringList parameters() const { return p; }
    QString toString() const;
    static IrcNumericMessage fromString(const QString& prefix, const QStringList& params);
protected:
    uint c;
    QStringList p;
};

class IRC_EXPORT IrcAwayMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcAwayMessage(const QString& error);
    QString message() const { return msg; }
    QString toString() const;
    static IrcAwayMessage fromString(const QString& prefix, const QStringList& params);
protected:
    QString msg;
};

#ifndef QT_NO_DEBUG_STREAM
IRC_EXPORT QDebug operator<<(QDebug debug, const IrcMessage& message);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCMESSAGE_H
