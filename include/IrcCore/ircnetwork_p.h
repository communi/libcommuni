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

#ifndef IRCNETWORK_P_H
#define IRCNETWORK_P_H

#include "ircnetwork.h"

#include <QHash>
#include <QString>
#include <QPointer>

class IrcNetworkPrivate
{
    Q_DECLARE_PUBLIC(IrcNetwork)

public:
    IrcNetworkPrivate(IrcNetwork* network);

    void setInfo(const QHash<QString, QString>& info);

    static IrcNetworkPrivate* get(const IrcNetwork* network)
    {
        return network->d_ptr.data();
    }

    IrcNetwork* q_ptr;
    QPointer<IrcSession> session;
    bool valid;
    mutable QHash<QString, QString> info;
    mutable QString network;
    mutable QStringList modes, prefixes, channelTypes, channelModes;
    mutable int modeLimit, channelLimit, targetLimit;
};

#endif // IRCNETWORK_P_H
