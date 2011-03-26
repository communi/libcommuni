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

#ifndef IRCBUFFER_H
#define IRCBUFFER_H

#include <ircglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>

class IrcSession;
class IrcBufferPrivate;

class IRC_EXPORT IrcBuffer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pattern READ pattern)

public:
    ~IrcBuffer();

    QString pattern() const;
    IrcSession* session() const;

Q_SIGNALS:
    void motdReceived(const QString& motd);
    void joined(const QString& origin);
    void parted(const QString& origin, const QString& message);
    void quit(const QString& origin, const QString& message);
    void namesReceived(const QStringList& names);
    void nickChanged(const QString& origin, const QString& nick);
    void modeChanged(const QString& origin, const QString& mode, const QString& args);
    void topicChanged(const QString& origin, const QString& topic);
    void invited(const QString& origin, const QString& receiver, const QString& channel);
    void kicked(const QString& origin, const QString& nick, const QString& message);
    void messageReceived(const QString& origin, const QString& message);
    void noticeReceived(const QString& origin, const QString& notice);
    void ctcpRequestReceived(const QString& origin, const QString& request);
    void ctcpReplyReceived(const QString& origin, const QString& reply);
    void ctcpActionReceived(const QString& origin, const QString& action);
    void numericMessageReceived(const QString& origin, uint code, const QStringList& params);
    void unknownMessageReceived(const QString& origin, const QStringList& params);

protected:
    IrcBuffer(const QString& pattern, IrcSession* parent);
    IrcBuffer(IrcBufferPrivate& dd, const QString& pattern, IrcSession* parent);
    QScopedPointer<IrcBufferPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(IrcBuffer)
    Q_DISABLE_COPY(IrcBuffer)
    friend class IrcSession;
};

#ifndef QT_NO_DEBUG_STREAM
IRC_EXPORT QDebug operator<<(QDebug debug, const IrcBuffer* buffer);
#endif // QT_NO_DEBUG_STREAM

#endif // IRCBUFFER_H
