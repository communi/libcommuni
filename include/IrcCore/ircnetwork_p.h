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

#include <QSet>
#include <QHash>
#include <QString>
#include <QPointer>

IRC_BEGIN_NAMESPACE

class IrcNetworkPrivate
{
    Q_DECLARE_PUBLIC(IrcNetwork)

public:
    IrcNetworkPrivate();

    void setInfo(const QHash<QString, QString>& info);
    void setAvailableCapabilities(const QSet<QString>& capabilities);
    void setActiveCapabilities(const QSet<QString>& capabilities);

    void setName(const QString& name);
    void setModes(const QStringList& modes);
    void setPrefixes(const QStringList& prefixes);
    void setChannelTypes(const QStringList& types);

    static IrcNetworkPrivate* get(const IrcNetwork* network)
    {
        return network->d_ptr.data();
    }

    IrcNetwork* q_ptr;
    QPointer<IrcConnection> connection;
    bool valid;
    QString name;
    QStringList modes, prefixes, channelTypes, channelModes;
    QHash<QString, int> numericLimits, modeLimits, channelLimits, targetLimits;
    QSet<QString> availableCaps, requestedCaps, activeCaps;
};

IRC_END_NAMESPACE

#endif // IRCNETWORK_P_H
