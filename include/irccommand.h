/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
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

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class IrcCommandPrivate;

class COMMUNI_EXPORT IrcCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList parameters READ parameters WRITE setParameters)
    Q_PROPERTY(QByteArray encoding READ encoding WRITE setEncoding)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_ENUMS(Type)

public:
    enum Type {
        Admin,
        Away,
        Capability,
        CtcpAction,
        CtcpReply,
        CtcpRequest,
        Custom,
        Info,
        Invite,
        Join,
        Kick,
        Knock,
        List,
        Message,
        Mode,
        Motd,
        Names,
        Nick,
        Notice,
        Part,
        Quit,
        Quote,
        Stats,
        Time,
        Topic,
        Trace,
        Users,
        Version,
        Who,
        Whois,
        Whowas
    };

    explicit IrcCommand(QObject* parent = 0);
    virtual ~IrcCommand();

    Type type() const;
    void setType(Type type);

    QStringList parameters() const;
    void setParameters(const QStringList& parameters);

    QByteArray encoding() const;
    void setEncoding(const QByteArray& encoding);

    virtual QString toString() const;

    Q_INVOKABLE static IrcCommand* createAdmin(const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createAway(const QString& reason = QString());
    Q_INVOKABLE static IrcCommand* createCapability(const QString& subCommand, const QStringList& capabilities = QStringList());
    Q_INVOKABLE static IrcCommand* createCtcpAction(const QString& target, const QString& action);
    Q_INVOKABLE static IrcCommand* createCtcpReply(const QString& target, const QString& reply);
    Q_INVOKABLE static IrcCommand* createCtcpRequest(const QString& target, const QString& request);
    Q_INVOKABLE static IrcCommand* createInfo(const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createInvite(const QString& user, const QString& channel);
    Q_INVOKABLE static IrcCommand* createJoin(const QString& channel, const QString& key = QString());
    Q_INVOKABLE static IrcCommand* createJoin(const QStringList& channels, const QStringList& keys = QStringList());
    Q_INVOKABLE static IrcCommand* createKick(const QString& channel, const QString& user, const QString& reason = QString());
    Q_INVOKABLE static IrcCommand* createKnock(const QString& channel, const QString& message = QString());
    Q_INVOKABLE static IrcCommand* createList(const QStringList& channels = QStringList(), const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createMessage(const QString& target, const QString& message);
    Q_INVOKABLE static IrcCommand* createMode(const QString& target, const QString& mode = QString(), const QString& arg = QString());
    Q_INVOKABLE static IrcCommand* createMotd(const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createNames(const QString& channel = QString(), const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createNames(const QStringList& channels, const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createNick(const QString& nick);
    Q_INVOKABLE static IrcCommand* createNotice(const QString& target, const QString& notice);
    Q_INVOKABLE static IrcCommand* createPart(const QString& channel, const QString& reason = QString());
    Q_INVOKABLE static IrcCommand* createPart(const QStringList& channels, const QString& reason = QString());
    Q_INVOKABLE static IrcCommand* createQuit(const QString& reason = QString());
    Q_INVOKABLE static IrcCommand* createQuote(const QString& raw);
    Q_INVOKABLE static IrcCommand* createQuote(const QStringList& parameters);
    Q_INVOKABLE static IrcCommand* createStats(const QString& query, const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createTime(const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createTopic(const QString& channel, const QString& topic = QString());
    Q_INVOKABLE static IrcCommand* createTrace(const QString& target = QString());
    Q_INVOKABLE static IrcCommand* createUsers(const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createVersion(const QString& server = QString());
    Q_INVOKABLE static IrcCommand* createWho(const QString& mask, bool operators = false);
    Q_INVOKABLE static IrcCommand* createWhois(const QString& user);
    Q_INVOKABLE static IrcCommand* createWhowas(const QString& user, int count = 1);

private:
    QScopedPointer<IrcCommandPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcCommand)
    Q_DISABLE_COPY(IrcCommand)
};

#ifndef QT_NO_DEBUG_STREAM
COMMUNI_EXPORT QDebug operator<<(QDebug debug, const IrcCommand* command);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCCOMMAND_H
