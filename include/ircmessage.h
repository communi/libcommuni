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

        // TODO: server queries and commands
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
        Numeric = 74,

        // TODO: optional messages
        Away = 80,
        Rehash = 81,
        Restart = 82
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

class IRC_EXPORT IrcNumericMessage : public IrcMessage
{
    Q_GADGET
public:
    IrcNumericMessage(uint code, const QStringList& params);
    uint code() const { return c; }
    QString toString() const;
    static IrcNumericMessage fromString(const QString& prefix, const QStringList& params);
protected:
    uint c;
    QStringList p;
};

#ifndef QT_NO_DEBUG_STREAM
IRC_EXPORT QDebug operator<<(QDebug debug, const IrcMessage& message);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCMESSAGE_H
