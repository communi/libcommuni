/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
*
* This library is free software you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation either version 2 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*
* $Id$
*/

#include "irchandler.h"

IrcHandler::~IrcHandler()
{
}

void IrcHandler::connected()
{
}

void IrcHandler::nickChanged(const QString& origin, const QString& nick)
{
    Q_UNUSED(origin);
    Q_UNUSED(nick);
}

void IrcHandler::quit(const QString& origin, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(message);
}

void IrcHandler::joined(const QString& origin, const QString& channel)
{
    Q_UNUSED(origin);
    Q_UNUSED(channel);
}

void IrcHandler::parted(const QString& origin, const QString& channel, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(channel);
    Q_UNUSED(message);
}

void IrcHandler::channelModeChanged(const QString& origin, const QString& channel, const QString& mode, const QString& args)
{
    Q_UNUSED(origin);
    Q_UNUSED(channel);
    Q_UNUSED(mode);
    Q_UNUSED(args);
}

void IrcHandler::userModeChanged(const QString& origin, const QString& mode)
{
    Q_UNUSED(origin);
    Q_UNUSED(mode);
}

void IrcHandler::topicChanged(const QString& origin, const QString& channel, const QString& topic)
{
    Q_UNUSED(origin);
    Q_UNUSED(channel);
    Q_UNUSED(topic);
}

void IrcHandler::kicked(const QString& origin, const QString& channel, const QString& nick, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(channel);
    Q_UNUSED(nick);
    Q_UNUSED(message);
}

void IrcHandler::channelMessageReceived(const QString& origin, const QString& channel, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(channel);
    Q_UNUSED(message);
}

void IrcHandler::privateMessageReceived(const QString& origin, const QString& receiver, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(receiver);
    Q_UNUSED(message);
}

void IrcHandler::noticeReceived(const QString& origin, const QString& receiver, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(receiver);
    Q_UNUSED(message);
}

void IrcHandler::invited(const QString& origin, const QString& nick, const QString& channel)
{
    Q_UNUSED(origin);
    Q_UNUSED(nick);
    Q_UNUSED(channel);
}

void IrcHandler::ctcpRequestReceived(const QString& origin, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(message);
}

void IrcHandler::ctcpReplyReceived(const QString& origin, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(message);
}

void IrcHandler::ctcpActionReceived(const QString& origin, const QString& message)
{
    Q_UNUSED(origin);
    Q_UNUSED(message);
}

void IrcHandler::unknownMessageReceived(const QString& origin, const QStringList& params)
{
    Q_UNUSED(origin);
    Q_UNUSED(params);
}

void IrcHandler::numericMessageReceived(const QString& origin, const QStringList& params)
{
    Q_UNUSED(origin);
    Q_UNUSED(params);
}
