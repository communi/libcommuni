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

#include "ircmessagebuilder_p.h"
#include "ircmessage.h"
#include "irc.h"
#include <QVariant>
#include <QDebug>

IrcMessageBuilder::IrcMessageBuilder(QObject* parent) : QObject(parent)
{
}

bool IrcMessageBuilder::handleMessage(IrcNumericMessage* message)
{
    IrcMessage* reply = 0;

    /*TODO:
    switch (message->code())
    {
    case Irc::RPL_MOTD:
        motdLines += message->parameters().value(0);
        break;
    case Irc::RPL_ENDOFMOTD: {
        reply = new IrcMotdMessage(this);
        reply->setProperty("lines", motdLines);
        motdLines.clear();
        break;
    }
    default:
        break;
    }
    */

    if (reply)
    {
        emit messageBuilt(reply);
        reply->deleteLater();
    }
}
