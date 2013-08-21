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

#include "ircmessage_p.h"
#include "ircmessagedecoder_p.h"
#include "ircsender.h"

IRC_BEGIN_NAMESPACE

IrcMessagePrivate::IrcMessagePrivate() :
    connection(0), type(IrcMessage::Unknown), timeStamp(QDateTime::currentDateTime()), encoding("ISO-8859-15"), flags(-1)
{
}

QString IrcMessagePrivate::prefix() const
{
    if (!m_prefix.isExplicit() && m_prefix.isNull() && !data.prefix.isNull())
        m_prefix = decode(data.prefix, encoding);
    return m_prefix.value();
}

void IrcMessagePrivate::setPrefix(const QString& prefix)
{
    m_prefix.setValue(prefix);
    m_nick.clear();
    m_ident.clear();
    m_host.clear();
}

QString IrcMessagePrivate::nick() const
{
    if (m_nick.isNull())
        m_nick = IrcSender(prefix()).name(); // TODO
    return m_nick;
}

QString IrcMessagePrivate::ident() const
{
    if (m_ident.isNull())
        m_ident = IrcSender(prefix()).user(); // TODO
    return m_ident;
}

QString IrcMessagePrivate::host() const
{
    if (m_host.isNull())
        m_host = IrcSender(prefix()).host(); // TODO
    return m_host;
}

QString IrcMessagePrivate::command() const
{
    if (!m_command.isExplicit() && m_command.isNull() && !data.command.isNull())
        m_command = decode(data.command, encoding);
    return m_command.value();
}

void IrcMessagePrivate::setCommand(const QString& command)
{
    m_command.setValue(command);
}

QStringList IrcMessagePrivate::params() const
{
    if (!m_params.isExplicit() && m_params.isNull() && !data.params.isEmpty()) {
        QStringList params;
        foreach (const QByteArray& param, data.params)
            params += decode(param, encoding);
        m_params = params;
    }
    return m_params.value();
}

QString IrcMessagePrivate::param(int index) const
{
    return params().value(index);
}

void IrcMessagePrivate::setParams(const QStringList& params)
{
    m_params.setValue(params);
}

void IrcMessagePrivate::invalidate()
{
    m_nick.clear();
    m_ident.clear();
    m_host.clear();

    m_prefix.clear();
    m_command.clear();
    m_params.clear();
}

IrcMessageData IrcMessageData::fromData(const QByteArray& data)
{
    IrcMessageData message;
    message.content = data;

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

    return message;
}

QString IrcMessagePrivate::decode(const QByteArray& data, const QByteArray& encoding)
{
    // TODO: not thread safe
    static IrcMessageDecoder decoder;
    decoder.setEncoding(encoding);
    return decoder.decode(data);
}

IRC_END_NAMESPACE
