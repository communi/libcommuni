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

#include <QtDeclarative>
#include <IrcSession>
#include <IrcCommand>
#include <IrcMessage>
#include <IrcPrefix>
#include <Irc>

class DeclarativeIrcPrefix : public QObject
{
    Q_OBJECT

public:
    explicit DeclarativeIrcPrefix(QObject* parent = 0) : QObject(parent) { }

    Q_INVOKABLE static QString nick(const QString& prefix)
    {
        return IrcPrefix(prefix).nick();
    }

    Q_INVOKABLE static QString user(const QString& prefix)
    {
        return IrcPrefix(prefix).user();
    }

    Q_INVOKABLE static QString host(const QString& prefix)
    {
        return IrcPrefix(prefix).host();
    }
};

class DeclarativeIrcSession : public IrcSession
{
    Q_OBJECT
    Q_PROPERTY(int port READ port WRITE setPort)

public:
    explicit DeclarativeIrcSession(QObject* parent = 0) : IrcSession(parent) { }

    // Invalid property assignment: unsupported type "ushort"
    int port() const { return IrcSession::port(); }
    void setPort(int port) { IrcSession::setPort(port); }

    Q_INVOKABLE static IrcCommand* createPasswordCommand(const QString& password) { return IrcCommand::createPassword(password); }
    Q_INVOKABLE static IrcCommand* createNickCommand(const QString& nick) { return IrcCommand::createNick(nick); }
    Q_INVOKABLE static IrcCommand* createUserCommand(const QString& userName, const QString& realName) { return IrcCommand::createUser(userName, realName); }
    Q_INVOKABLE static IrcCommand* createOperatorCommand(const QString& user, const QString& password) { return IrcCommand::createOperator(user, password); }
    Q_INVOKABLE static IrcCommand* createQuitCommand(const QString& reason) { return IrcCommand::createQuit(reason); }
    Q_INVOKABLE static IrcCommand* createJoinCommand(const QString& channel, const QString& key) { return IrcCommand::createJoin(channel, key); }
    Q_INVOKABLE static IrcCommand* createPartCommand(const QString& channel, const QString& reason) { return IrcCommand::createPart(channel, reason); }
    Q_INVOKABLE static IrcCommand* createTopicCommand(const QString& channel, const QString& topic) { return IrcCommand::createTopic(channel, topic); }
    Q_INVOKABLE static IrcCommand* createNamesCommand(const QString& channel) { return IrcCommand::createNames(channel); }
    Q_INVOKABLE static IrcCommand* createListCommand(const QString& channel, const QString& server) { return IrcCommand::createList(channel, server); }
    Q_INVOKABLE static IrcCommand* createInviteCommand(const QString& user, const QString& channel) { return IrcCommand::createInvite(user, channel); }
    Q_INVOKABLE static IrcCommand* createKickCommand(const QString& user, const QString& channel, const QString& reason) { return IrcCommand::createKick(user, channel, reason); }
    Q_INVOKABLE static IrcCommand* createModeCommand(const QString& target, const QString& mode, const QString& arg, const QString& mask) { return IrcCommand::createMode(target, mode, arg, mask); }
    Q_INVOKABLE static IrcCommand* createMessageCommand(const QString& target, const QString& message) { return IrcCommand::createMessage(target, message); }
    Q_INVOKABLE static IrcCommand* createNoticeCommand(const QString& target, const QString& message) { return IrcCommand::createNotice(target, message); }
    Q_INVOKABLE static IrcCommand* createCtcpActionCommand(const QString& target, const QString& action) { return IrcCommand::createCtcpAction(target, action); }
    Q_INVOKABLE static IrcCommand* createCtcpRequestCommand(const QString& target, const QString& request) { return IrcCommand::createCtcpRequest(target, request); }
    Q_INVOKABLE static IrcCommand* createCtcpReplyCommand(const QString& target, const QString& reply) { return IrcCommand::createCtcpReply(target, reply); }
    Q_INVOKABLE static IrcCommand* createWhoCommand(const QString& user) { return IrcCommand::createWho(user); }
    Q_INVOKABLE static IrcCommand* createWhoisCommand(const QString& user) { return IrcCommand::createWhois(user); }
    Q_INVOKABLE static IrcCommand* createWhowasCommand(const QString& user) { return IrcCommand::createWhowas(user); }
    Q_INVOKABLE static IrcCommand* createPingCommand(const QString& target) { return IrcCommand::createPing(target); }
    Q_INVOKABLE static IrcCommand* createPongCommand(const QString& target) { return IrcCommand::createPong(target); }
    Q_INVOKABLE static IrcCommand* createAwayCommand(const QString& reason) { return IrcCommand::createAway(reason); }
};

QML_DECLARE_TYPE(Irc)
QML_DECLARE_TYPE(IrcCommand)
QML_DECLARE_TYPE(IrcMessage)
QML_DECLARE_TYPE(DeclarativeIrcPrefix)
QML_DECLARE_TYPE(DeclarativeIrcSession)

class CommuniPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    virtual void initializeEngine(QDeclarativeEngine* engine, const char* uri)
    {
        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);
    }

    void registerTypes(const char *uri)
    {
        qmlRegisterType<DeclarativeIrcSession>(uri, 1, 0, "IrcSession");

        qmlRegisterUncreatableType<Irc>(uri, 1, 0, "Irc", "");
        qmlRegisterUncreatableType<IrcMessage>(uri, 1, 0, "IrcMessage", "");
        qmlRegisterUncreatableType<IrcCommand>(uri, 1, 0, "IrcCommand", "");
        qmlRegisterUncreatableType<DeclarativeIrcPrefix>(uri, 1, 0, "IrcPrefix", "");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(communiplugin, CommuniPlugin);
