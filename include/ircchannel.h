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

#ifndef IRCCHANNEL_H
#define IRCCHANNEL_H

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qscopedpointer.h>

class IrcMessage;
class IrcSession;
class IrcChannelPrivate;

class COMMUNI_EXPORT IrcChannel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString prefix READ prefix CONSTANT)
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(QString topic READ topic NOTIFY topicChanged)
    Q_PROPERTY(IrcSession* session READ session CONSTANT)

public:
    explicit IrcChannel(QObject* parent = 0);
    virtual ~IrcChannel();

    QString title() const;
    QString name() const;
    QString prefix() const;

    QString mode() const;
    QString topic() const;

    IrcSession* session() const;

Q_SIGNALS:
    void titleChanged(const QString& title);
    void nameChanged(const QString& name);
    void modeChanged(const QString& mode);
    void topicChanged(const QString& topic);
    void destroyed(IrcChannel* channel);
    void messageReceived(IrcMessage* message);

private:
    QScopedPointer<IrcChannelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcChannel)
    Q_DISABLE_COPY(IrcChannel)
};

Q_DECLARE_METATYPE(IrcChannel*)
Q_DECLARE_METATYPE(QList<IrcChannel*>)

#endif // IRCCHANNEL_H
