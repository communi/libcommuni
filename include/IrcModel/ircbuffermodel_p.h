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

#ifndef IRCBUFFERMODEL_P_H
#define IRCBUFFERMODEL_P_H

#include "ircbuffer.h"
#include "ircfilter.h"
#include "ircbuffermodel.h"
#include <qpointer.h>

IRC_BEGIN_NAMESPACE

class IrcBufferModelPrivate : public QObject, public IrcMessageFilter
{
    Q_OBJECT
    Q_INTERFACES(IrcMessageFilter)
    Q_DECLARE_PUBLIC(IrcBufferModel)

public:
    IrcBufferModelPrivate();

    bool messageFilter(IrcMessage* message);

    IrcBuffer* createBufferHelper(const QString& title);
    IrcChannel* createChannelHelper(const QString& title);

    IrcBuffer* createBuffer(const QString& title);
    void destroyBuffer(const QString& title, bool force = false);

    void addBuffer(IrcBuffer* buffer, bool notify = true);
    void insertBuffer(int index, IrcBuffer* buffer, bool notify = true);
    void removeBuffer(IrcBuffer* buffer, bool notify = true);
    bool renameBuffer(const QString& from, const QString& to);

    bool processMessage(const QString& title, IrcMessage* message, bool create = false);

    void _irc_connectionStatusChanged();
    void _irc_bufferDestroyed(IrcBuffer* buffer);

    static IrcBufferModelPrivate* get(IrcBufferModel* model)
    {
        return model->d_func();
    }

    IrcBufferModel* q_ptr;
    Irc::DataRole role;
    QPointer<IrcConnection> connection;
    QList<IrcBuffer*> bufferList;
    QMap<QString, IrcBuffer*> bufferMap;
    QStringList channels;
    Irc::SortMethod sortMethod;
    Qt::SortOrder sortOrder;
    IrcBuffer* bufferProto;
    IrcChannel* channelProto;
};

IRC_END_NAMESPACE

#endif // IRCBUFFERMODEL_P_H
