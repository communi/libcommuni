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

IrcMessageBuilder::IrcMessageBuilder(IrcSession* session)
{
    d.session = session;
    d.message = 0;
}

void IrcMessageBuilder::processMessage(IrcNumericMessage* message)
{
    switch (message->code()) {
    case Irc::RPL_MOTDSTART:
        d.message = new IrcMotdMessage(d.session);
        d.message->setSender(message->sender());
        d.message->setCommand(QLatin1String("MOTD"));
        d.message->setParameters(QStringList(message->parameters().value(0)));
        break;
    case Irc::RPL_MOTD:
        d.message->setParameters(d.message->parameters() << message->parameters().value(1));
        break;
    case Irc::RPL_ENDOFMOTD:
        d.message->setTimeStamp(message->timeStamp());
        emit messageReceived(d.message);
        break;
    }
}
