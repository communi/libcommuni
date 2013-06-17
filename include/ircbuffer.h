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

#ifndef IRCBUFFER_H
#define IRCBUFFER_H

#include <Irc>
#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qscopedpointer.h>

class IrcCommand;
class IrcMessage;
class IrcSession;
class IrcBufferModel;
class IrcBufferPrivate;

class COMMUNI_EXPORT IrcBuffer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString prefix READ prefix CONSTANT)
    Q_PROPERTY(IrcSession* session READ session CONSTANT)
    Q_PROPERTY(IrcBufferModel* model READ model CONSTANT)

public:
    explicit IrcBuffer(QObject* parent = 0);
    virtual ~IrcBuffer();

    QString title() const;
    QString name() const;
    QString prefix() const;

    IrcSession* session() const;
    IrcBufferModel* model() const;

    Q_INVOKABLE bool sendCommand(IrcCommand* command);

Q_SIGNALS:
    void titleChanged(const QString& title);
    void nameChanged(const QString& name);
    void messageReceived(IrcMessage* message);
    void destroyed(IrcBuffer* buffer);

protected:
    IrcBuffer(IrcBufferPrivate& dd, QObject* parent);

    QScopedPointer<IrcBufferPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcBuffer)
    Q_DISABLE_COPY(IrcBuffer)
};

Q_DECLARE_METATYPE(IrcBuffer*)
Q_DECLARE_METATYPE(QList<IrcBuffer*>)

#endif // IRCBUFFER_H
