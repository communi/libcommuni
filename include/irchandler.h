/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#ifndef IRCHANDLER_H
#define IRCHANDLER_H

#include "global.h"
#include <QString>
#include <QStringList>

class IrcHandlerPrivate;

class LIBIRCCLIENT_QT_EXPORT IrcHandler
{
    friend class IrcSessionPrivate;

public:
    virtual ~IrcHandler();

protected:
    virtual void connected();
    virtual void nickChanged(const QString& origin, const QString& nick);
    virtual void quit(const QString& origin, const QString& message);
    virtual void joined(const QString& origin, const QString& channel);
    virtual void parted(const QString& origin, const QString& channel, const QString& message);
    virtual void channelModeChanged(const QString& origin, const QString& channel, const QString& mode, const QString& args);
    virtual void userModeChanged(const QString& origin, const QString& mode);
    virtual void topicChanged(const QString& origin, const QString& channel, const QString& topic);
    virtual void kicked(const QString& origin, const QString& channel, const QString& nick, const QString& message);
    virtual void channelMessageReceived(const QString& origin, const QString& channel, const QString& message);
    virtual void privateMessageReceived(const QString& origin, const QString& receiver, const QString& message);
    virtual void noticeReceived(const QString& origin, const QString& receiver, const QString& message);
    virtual void invited(const QString& origin, const QString& nick, const QString& channel);
    virtual void ctcpRequestReceived(const QString& origin, const QString& message);
    virtual void ctcpReplyReceived(const QString& origin, const QString& message);
    virtual void ctcpActionReceived(const QString& origin, const QString& message);
    virtual void unknownMessageReceived(const QString& origin, const QStringList& params);
    virtual void numericMessageReceived(const QString& origin, const QStringList& params);
};

#endif // IRCHANDLER_H
