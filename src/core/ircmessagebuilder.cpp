/*
* Copyright (C) 2008-2013 The Communi Project
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

#include "ircmessagebuilder_p.h"
#include "ircmessage.h"
#include "irc.h"

IRC_BEGIN_NAMESPACE

#ifndef IRC_DOXYGEN
IrcMessageBuilder::IrcMessageBuilder(IrcConnection* connection)
{
    d.connection = connection;
    d.message = 0;
}

void IrcMessageBuilder::processMessage(IrcNumericMessage* message)
{
    switch (message->code()) {
    case Irc::RPL_MOTDSTART:
        d.message = new IrcMotdMessage(d.connection);
        d.message->setPrefix(message->prefix());
        d.message->setParameters(QStringList(message->parameters().value(0)));
        break;
    case Irc::RPL_MOTD:
        d.message->setParameters(d.message->parameters() << message->parameters().value(1));
        break;
    case Irc::RPL_ENDOFMOTD:
        d.message->setTimeStamp(message->timeStamp());
        emit messageReceived(d.message);
        d.message = 0;
        break;

    case Irc::RPL_NAMREPLY: {
        if (!d.message)
            d.message = new IrcNamesMessage(d.connection);
        d.message->setPrefix(message->prefix());
        int count = message->parameters().count();
        QString channel = message->parameters().value(count - 2);
        QStringList names = d.message->parameters().mid(1);
        names += message->parameters().value(count - 1).split(QLatin1Char(' '), QString::SkipEmptyParts);
        d.message->setParameters(QStringList() << channel << names);
        break;
    }
    case Irc::RPL_ENDOFNAMES:
        d.message->setTimeStamp(message->timeStamp());
        emit messageReceived(d.message);
        d.message = 0;
        break;

    case Irc::RPL_TOPIC:
    case Irc::RPL_NOTOPIC:
        d.message = new IrcTopicMessage(d.connection);
        d.message->setPrefix(message->prefix());
        d.message->setTimeStamp(message->timeStamp());
        d.message->setCommand(QString::number(message->code()));
        d.message->setParameters(QStringList() << message->parameters().value(1) << message->parameters().value(2));
        emit messageReceived(d.message);
        d.message = 0;
        break;

    case Irc::RPL_CHANNELMODEIS:
        d.message = new IrcModeMessage(d.connection);
        d.message->setPrefix(message->prefix());
        d.message->setTimeStamp(message->timeStamp());
        d.message->setCommand(QString::number(message->code()));
        d.message->setParameters(message->parameters().mid(1));
        emit messageReceived(d.message);
        d.message = 0;
        break;
    }
}
#endif // IRC_DOXYGEN

#include "moc_ircmessagebuilder_p.cpp"

IRC_END_NAMESPACE
