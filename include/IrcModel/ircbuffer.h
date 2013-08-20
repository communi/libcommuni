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

IRC_BEGIN_NAMESPACE

class IrcChannel;
class IrcCommand;
class IrcMessage;
class IrcConnection;
class IrcBufferModel;
class IrcBufferPrivate;

class IRC_MODEL_EXPORT IrcBuffer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)
    Q_PROPERTY(IrcConnection* connection READ connection CONSTANT)
    Q_PROPERTY(IrcBufferModel* model READ model CONSTANT)
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
    Q_PROPERTY(bool channel READ isChannel CONSTANT)
    Q_PROPERTY(bool sticky READ isSticky WRITE setSticky NOTIFY stickyChanged)
    Q_PROPERTY(bool persistent READ isPersistent WRITE setPersistent NOTIFY persistentChanged)

public:
    explicit IrcBuffer(QObject* parent = 0);
    virtual ~IrcBuffer();

    QString title() const;
    QString name() const;
    QString prefix() const;

    bool isChannel() const;
    Q_INVOKABLE IrcChannel* toChannel();

    IrcConnection* connection() const;
    IrcBufferModel* model() const;

    virtual bool isActive() const;

    bool isSticky() const;
    void setSticky(bool sticky);

    bool isPersistent() const;
    void setPersistent(bool persistent);

    Q_INVOKABLE bool sendCommand(IrcCommand* command);

public Q_SLOTS:
    void setName(const QString& name);
    void setPrefix(const QString& prefix);
    void receiveMessage(IrcMessage* message);

Q_SIGNALS:
    void titleChanged(const QString& title);
    void nameChanged(const QString& name);
    void prefixChanged(const QString& name);
    void messageReceived(IrcMessage* message);
    void destroyed(IrcBuffer* buffer);
    void activeChanged(bool active);
    void stickyChanged(bool sticky);
    void persistentChanged(bool persistent);

protected:
    IrcBuffer(IrcBufferPrivate& dd, QObject* parent);

    QScopedPointer<IrcBufferPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcBuffer)
    Q_DISABLE_COPY(IrcBuffer)
    Q_PRIVATE_SLOT(d_func(), void _irc_emitActiveChanged())
};

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcBuffer*))
Q_DECLARE_METATYPE(QList<IRC_PREPEND_NAMESPACE(IrcBuffer*)>)

#endif // IRCBUFFER_H
