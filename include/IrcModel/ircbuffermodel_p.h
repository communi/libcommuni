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
#include "ircbuffermodel.h"
#include "ircmessagefilter.h"
#include <qpointer.h>

IRC_BEGIN_NAMESPACE

class IrcBufferModelPrivate : public IrcMessageFilter
{
    Q_DECLARE_PUBLIC(IrcBufferModel)

public:
    IrcBufferModelPrivate();

    bool messageFilter(IrcMessage* message);

    IrcBuffer* addBuffer(const QString& title);
    void addBuffer(IrcBuffer* buffer);
    void removeBuffer(const QString& title, bool force = false);
    bool renameBuffer(IrcBuffer* buffer, const QString& title);
    bool processMessage(const QString& title, IrcMessage* message, bool create = false);

    void _irc_bufferDestroyed(IrcBuffer* buffer);

    static IrcBufferModelPrivate* get(IrcBufferModel* model)
    {
        return model->d_func();
    }

    static bool bufferLessThan(IrcBuffer* b1, IrcBuffer* b2)
    {
        return b1->model()->lessThan(b1, b2);
    }

    static bool bufferGreaterThan(IrcBuffer* b1, IrcBuffer* b2)
    {
        return b2->model()->lessThan(b2, b1);
    }

    IrcBufferModel* q_ptr;
    Irc::DataRole role;
    QPointer<IrcConnection> connection;
    QList<IrcBuffer*> bufferList;
    QMap<QString, IrcBuffer*> bufferMap;
    QStringList channels;
    Qt::SortOrder sortOrder;
    bool dynamicSort;
};

IRC_END_NAMESPACE

#endif // IRCBUFFERMODEL_P_H
