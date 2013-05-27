/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircbot.h"
#include <IrcCommand>
#include <IrcMessage>

IrcBot::IrcBot(QObject* parent) : IrcSession(parent)
{
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(this, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(onMessageReceived(IrcMessage*)));
}

QString IrcBot::channel() const
{
    return m_channel;
}

void IrcBot::setChannel(const QString& channel)
{
    m_channel = channel;
}

void IrcBot::onConnected()
{
    sendCommand(IrcCommand::createJoin(m_channel));
}

void IrcBot::onMessageReceived(IrcMessage* message)
{
    if (message->type() == IrcMessage::Private) {
        IrcPrivateMessage* msg = static_cast<IrcPrivateMessage*>(message);

        if (!msg->target().compare(nickName(), Qt::CaseInsensitive)) {
            // echo private message
            sendCommand(IrcCommand::createMessage(msg->sender().name(), msg->message()));
        } else if (msg->message().startsWith(nickName(), Qt::CaseInsensitive)) {
            // echo prefixed channel message
            QString reply = msg->message().mid(msg->message().indexOf(" "));
            sendCommand(IrcCommand::createMessage(m_channel, msg->sender().name() + ":" + reply));
        }
    }
}
