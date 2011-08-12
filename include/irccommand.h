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
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class IrcCommandPrivate;

class COMMUNI_EXPORT IrcCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList parameters READ parameters WRITE setParameters)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_ENUMS(Type)

public:
    enum Type
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

        // ctcp messages
        CtcpAction,
        CtcpRequest,
        CtcpReply,

        // user-based queries
        Who,
        Whois,
        Whowas,

        // miscellaneous commands
        Ping,
        Pong,
        Away
    };

    explicit IrcCommand(QObject* parent = 0);
    virtual ~IrcCommand();

    Type type() const;
    void setType(Type type);

    QStringList parameters() const;
    void setParameters(QStringList parameters);

    virtual QString toString() const;

    Q_INVOKABLE static IrcCommand* createPassword(const QString& password, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createNick(const QString& nick, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createUser(const QString& userName, const QString& realName, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createOperator(const QString& user, const QString& password, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createQuit(const QString& reason, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createJoin(const QString& channel, const QString& key, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createPart(const QString& channel, const QString& reason, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createTopic(const QString& channel, const QString& topic, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createNames(const QString& channel, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createList(const QString& channel, const QString& server, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createInvite(const QString& user, const QString& channel, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createKick(const QString& user, const QString& channel, const QString& reason, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createMode(const QString& target, const QString& mode, const QString& arg, const QString& mask, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createMessage(const QString& target, const QString& message, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createNotice(const QString& target, const QString& message, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createCtcpAction(const QString& target, const QString& action, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createCtcpRequest(const QString& target, const QString& request, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createCtcpReply(const QString& target, const QString& reply, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createWho(const QString& user, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createWhois(const QString& user, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createWhowas(const QString& user, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createPing(const QString& target, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createPong(const QString& target, QObject* parent = 0);
    Q_INVOKABLE static IrcCommand* createAway(const QString& reason, QObject* parent = 0);

private:
    QScopedPointer<IrcCommandPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcCommand)
    Q_DISABLE_COPY(IrcCommand)
};

#ifndef QT_NO_DEBUG_STREAM
COMMUNI_EXPORT QDebug operator<<(QDebug debug, const IrcCommand* command);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCCOMMAND_H
