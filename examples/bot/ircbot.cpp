/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircbot.h"
#include <IrcMessage>
#include <IrcCommand>
#include <QCoreApplication>
#include <QTimer>

IrcBot::IrcBot(QObject* parent) : IrcConnection(parent)
{
    connect(this, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(onMessageReceived(IrcMessage*)));

    parser.setPrefix("!");
    parser.addCommand(IrcCommand::CtcpAction, "ACT [target] <message...>");
    parser.addCommand(IrcCommand::Custom, "HELP (<command...>)");
    parser.addCommand(IrcCommand::Nick, "NICK <nick>");
    parser.addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser.addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
    parser.addCommand(IrcCommand::Quit, "QUIT (<message...>)");
    parser.addCommand(IrcCommand::Message, "SAY [target] <message...>");

    model.setConnection(this);
    connect(&model, SIGNAL(channelsChanged(QStringList)), &parser, SLOT(setChannels(QStringList)));
}

void IrcBot::join(QString channel)
{
    sendCommand(IrcCommand::createJoin(channel));
}

void IrcBot::onMessageReceived(IrcMessage* message)
{
    qDebug() << QString::fromUtf8(message->toData());

    if (message->type() == IrcMessage::Private) {
        IrcPrivateMessage* msg = static_cast<IrcPrivateMessage*>(message);

        QString content = msg->message();
        if (content.startsWith(nickName(), Qt::CaseInsensitive))
            content = content.mid(content.indexOf(" ")).trimmed();

        if (!msg->target().compare(nickName(), Qt::CaseInsensitive))
            parser.setCurrentTarget(msg->nick());
        else
            parser.setCurrentTarget(msg->target());

        IrcCommand* cmd = parser.parse(content);
        if (cmd) {
            if (cmd->type() == IrcCommand::Custom && cmd->parameters().value(0) == "HELP") {
                help(cmd->parameters().mid(1));
            } else {
                sendCommand(cmd);

                if (cmd->type() == IrcCommand::Quit) {
                    connect(this, SIGNAL(disconnected()), qApp, SLOT(quit()));
                    QTimer::singleShot(1000, qApp, SLOT(quit()));
                }
            }
        }
    }
}

void IrcBot::help(QStringList commands)
{
    if (commands.isEmpty())
        commands = parser.commands();

    QString target = parser.currentTarget();
    foreach (const QString& command, commands) {
        QString syntax = parser.syntax(command);
        sendCommand(IrcCommand::createMessage(target, syntax));
    }
}
