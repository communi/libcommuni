/*
* Copyright (C) 2008-2013 J-P Nurmi <jpnurmi@gmail.com>
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

#include "ircmessage_p.h"
#include "ircmessagedecoder_p.h"

IrcMessagePrivate::IrcMessagePrivate() :
    session(0), type(IrcMessage::Unknown), timeStamp(QDateTime::currentDateTime()), encoding("ISO-8859-15"), flags(-1)
{
}

IrcSender IrcMessagePrivate::sender() const
{
    if (content.dirty)
        content = IrcMessageContent::fromData(message, encoding);
    return content.sender;
}

QString IrcMessagePrivate::command() const
{
    if (content.dirty)
        content = IrcMessageContent::fromData(message, encoding);
    return content.command;
}

QStringList IrcMessagePrivate::params() const
{
    if (content.dirty)
        content = IrcMessageContent::fromData(message, encoding);
    return content.params;
}

QString IrcMessagePrivate::param(int index) const
{
    if (content.dirty)
        content = IrcMessageContent::fromData(message, encoding);
    return content.params.value(index);
}

IrcMessageData IrcMessageData::fromData(const QByteArray& data)
{
    IrcMessageData message;

    // From RFC 1459:
    //  <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
    //  <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
    //  <command>  ::= <letter> { <letter> } | <number> <number> <number>
    //  <SPACE>    ::= ' ' { ' ' }
    //  <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
    //  <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
    //                 or NUL or CR or LF, the first of which may not be ':'>
    //  <trailing> ::= <Any, possibly *empty*, sequence of octets not including
    //                   NUL or CR or LF>
    QByteArray process = data;

    // parse <prefix>
    if (process.startsWith(':')) {
        message.prefix = process.mid(1, process.indexOf(' ') - 1);
        process.remove(0, message.prefix.length() + 2);
    }

    // parse <command>
    message.command = process.mid(0, process.indexOf(' '));
    process.remove(0, message.command.length() + 1);

    // parse <params>
    while (!process.isEmpty()) {
        if (process.startsWith(':')) {
            process.remove(0, 1);
            message.params += process;
            process.clear();
        } else {
            QByteArray param = process.mid(0, process.indexOf(' '));
            process.remove(0, param.length() + 1);
            message.params += param;
        }
    }

    message.valid = !message.command.isEmpty() && process.trimmed().isEmpty();
    return message;
}

IrcMessageContent IrcMessageContent::fromData(const IrcMessageData& data, const QByteArray& encoding)
{
    IrcMessageContent content;
    if (data.valid) {
        // TODO: not thread safe
        static IrcMessageDecoder decoder;
        decoder.setEncoding(encoding);

        content.sender = IrcSender(decoder.decode(data.prefix));
        content.command = decoder.decode(data.command);
        foreach (const QByteArray& param, data.params)
            content.params += decoder.decode(param);
        content.dirty = false;
    }
    return content;
}
