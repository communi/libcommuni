/*
  Copyright (C) 2008-2014 The Communi Project

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    case Irc::RPL_WHOREPLY: {
        d.message = new IrcWhoReplyMessage(d.connection);
        d.message->setPrefix(message->parameters().value(5) // nick
                             + QLatin1Char('!') + message->parameters().value(2) // ident
                             + QLatin1Char('@') + message->parameters().value(3)); // host
        d.message->setTimeStamp(message->timeStamp());
        d.message->setCommand(QString::number(message->code()));
        d.message->setParameters(QStringList() << message->parameters().value(1) // mask
                                               << message->parameters().value(4) // server
                                               << message->parameters().value(6)); // status
        QString last = message->parameters().value(7);
        int index = last.indexOf(QLatin1Char(' ')); // ignore hopcount
        if (index != -1)
            d.message->setParameters(d.message->parameters() << last.mid(index + 1)); // real name
        emit messageReceived(d.message);
        d.message = 0;
        break;
    }

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
