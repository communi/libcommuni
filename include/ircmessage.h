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

class IRC_EXPORT IrcMessage
{
    Q_GADGET
    Q_ENUMS(Type)

public:
    enum Type
    {
        None = 0,

        // TODO: connection registration
        Password = 1,
        Nickname = 2,
        User = 3,
        Server = 4,
        Operator = 5,
        Quit = 6,
        ServerQuit = 7,

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

        // server queries and commands
        Version = 30,
        Stats = 31,
        Links = 32,
        Time = 33,
        Connect = 34,
        Trace = 35,
        Admin = 36,
        Info = 37,
        Motd = 38,

        // sending messages
        Private = 40,
        Notice = 41,

        // ctcp messages
        CtcpAction = 50,
        CtcpRequest = 51,
        CtcpReply = 52,

        // user-based queries
        Who = 60,
        Whois = 61,
        Whowas = 62,

        // TODO: miscellaneous messages
        Kill = 70,
        Ping = 71,
        Pong = 72,
        Error = 73,

        // TODO: optional messages
        Away = 80,
        Rehash = 81,
        Restart = 82
    };

    IrcMessage(Type type);
    virtual ~IrcMessage();

    inline Type type() const { return t; }

    virtual QString toString() const = 0;

protected:
    Type t;
};

// channel operations

class IRC_EXPORT IrcChannelMessage : public IrcMessage
{
public:
    IrcChannelMessage(Type type, const QString& channel);
    QString channel() const { return chan; }
protected:
    QString chan;
};

class IRC_EXPORT IrcJoinMessage : public IrcChannelMessage
{
public:
    IrcJoinMessage(const QString& channel, const QString& key = QString());
    QString key() const { return k; }
    QString toString() const;
    static IrcJoinMessage fromString(const QString& str);
protected:
    QString k;
};

class IRC_EXPORT IrcPartMessage : public IrcChannelMessage
{
public:
    IrcPartMessage(const QString& channel, const QString& reason = QString());
    QString reason() const { return rson; }
    QString toString() const;
    static IrcPartMessage fromString(const QString& str);
protected:
    QString rson;
};

class IRC_EXPORT IrcTopicMessage : public IrcChannelMessage
{
public:
    IrcTopicMessage(const QString& channel, const QString& topic = QString());
    QString topic() const { return tpc; }
    QString toString() const;
    static IrcTopicMessage fromString(const QString& str);
protected:
    QString tpc;
};

class IRC_EXPORT IrcNamesMessage : public IrcChannelMessage
{
public:
    IrcNamesMessage(const QString& channel) :
        IrcChannelMessage(Names, channel) { }
    QString toString() const;
    static IrcNamesMessage fromString(const QString& str);
};

class IRC_EXPORT IrcListMessage : public IrcChannelMessage
{
public:
    IrcListMessage(const QString& channel, const QString& server = QString());
    QString server() const { return srv; }
    QString toString() const;
    static IrcListMessage fromString(const QString& str);
protected:
    QString srv;
};

class IRC_EXPORT IrcInviteMessage : public IrcChannelMessage
{
public:
    IrcInviteMessage(const QString& channel, const QString& user);
    QString user() const { return usr; }
    QString toString() const;
    static IrcInviteMessage fromString(const QString& str);
protected:
    QString usr;
};

class IRC_EXPORT IrcKickMessage : public IrcChannelMessage
{
public:
    IrcKickMessage(const QString& channel, const QString& user, const QString& reason = QString());
    QString user() const { return usr; }
    QString reason() const { return rson; }
    QString toString() const;
    static IrcKickMessage fromString(const QString& str);
protected:
    QString usr;
    QString rson;
};

// mode operations

class IRC_EXPORT IrcModeMessage : public IrcMessage
{
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
public:
    IrcChannelModeMessage(const QString& channel, const QString& mode = QString(),
                          const QString& argument = QString(), const QString& mask = QString());
    QString mode() const { return mod; }
    QString argument() const { return arg; }
    QString mask() const { return msk; }
    QString toString() const;
    static IrcChannelModeMessage fromString(const QString& str);
protected:
    QString mod;
    QString arg;
    QString msk;
};

class IRC_EXPORT IrcUserModeMessage : public IrcModeMessage
{
public:
    IrcUserModeMessage(const QString& user, const QString& mode) :
        IrcModeMessage(UserMode, user, mode) { }
    QString toString() const;
    static IrcUserModeMessage fromString(const QString& str);
};

// sending messages

class IRC_EXPORT IrcSendMessage : public IrcMessage
{
public:
    IrcSendMessage(Type type, const QString& message);
    QString target() const { return tgt; }
    QString message() const { return msg; }
protected:
    QString tgt;
    QString msg;
};

class IRC_EXPORT IrcPrivateMessage : public IrcSendMessage
{
public:
    IrcPrivateMessage(const QString& message) :
        IrcSendMessage(Private, message) { }
    QString toString() const;
    static IrcPrivateMessage fromString(const QString& str);
};

class IRC_EXPORT IrcNoticeMessage : public IrcSendMessage
{
public:
    IrcNoticeMessage(const QString& message) :
        IrcSendMessage(Notice, message) { }
    QString toString() const;
    static IrcNoticeMessage fromString(const QString& str);
};

// ctcp messages

class IRC_EXPORT IrcCtcpMessage : public IrcMessage
{
public:
    IrcCtcpMessage(Type type, const QString& message);
    QString target() const { return tgt; }
    QString message() const { return msg; }
protected:
    QString tgt;
    QString msg;
};

class IRC_EXPORT IrcCtcpActionMessage : public IrcCtcpMessage
{
public:
    IrcCtcpActionMessage(const QString& message) :
        IrcCtcpMessage(CtcpAction, message) { }
    QString toString() const;
    static IrcCtcpActionMessage fromString(const QString& str);
};

class IRC_EXPORT IrcCtcpRequestMessage : public IrcCtcpMessage
{
public:
    IrcCtcpRequestMessage(const QString& message) :
        IrcCtcpMessage(CtcpRequest, message) { }
    QString toString() const;
    static IrcCtcpRequestMessage fromString(const QString& str);
};

class IRC_EXPORT IrcCtcpReplyMessage : public IrcCtcpMessage
{
public:
    IrcCtcpReplyMessage(const QString& message) :
        IrcCtcpMessage(CtcpReply, message) { }
    QString toString() const;
    static IrcCtcpReplyMessage fromString(const QString& str);
};

// user-based queries

class IRC_EXPORT IrcQueryMessage : public IrcMessage
{
public:
    IrcQueryMessage(Type type, const QString& user);
    QString user() const { return usr; }
protected:
    QString usr;
};

class IRC_EXPORT IrcWhoMessage : public IrcQueryMessage
{
public:
    IrcWhoMessage(const QString& user) :
        IrcQueryMessage(Who, user) { }
    QString toString() const;
    static IrcWhoMessage fromString(const QString& str);
};

class IRC_EXPORT IrcWhoisMessage : public IrcQueryMessage
{
public:
    IrcWhoisMessage(const QString& user) :
        IrcQueryMessage(Whois, user) { }
    QString toString() const;
    static IrcWhoisMessage fromString(const QString& str);
};

class IRC_EXPORT IrcWhowasMessage : public IrcQueryMessage
{
public:
    IrcWhowasMessage(const QString& user) :
        IrcQueryMessage(Whowas, user) { }
    QString toString() const;
    static IrcWhowasMessage fromString(const QString& str);
};

#endif // IRCMESSAGE_H
