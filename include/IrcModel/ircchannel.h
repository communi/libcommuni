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
#include <IrcBuffer>
#include <QtCore/qmetatype.h>

IRC_BEGIN_NAMESPACE

class IrcChannelPrivate;

class IRC_MODEL_EXPORT IrcChannel : public IrcBuffer
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key NOTIFY keyChanged)
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(QString topic READ topic NOTIFY topicChanged)

public:
    Q_INVOKABLE explicit IrcChannel(QObject* parent = 0);
    virtual ~IrcChannel();

    QString key() const;
    QString mode() const;
    QString topic() const;

    virtual bool isActive() const;

public Q_SLOTS:
    void join(const QString& key = QString());
    void part(const QString& reason = QString());
    void close(const QString& reason = QString());

Q_SIGNALS:
    void keyChanged(const QString& key);
    void modeChanged(const QString& mode);
    void topicChanged(const QString& topic);
    void destroyed(IrcChannel* channel);
private:
    Q_DECLARE_PRIVATE(IrcChannel)
    Q_DISABLE_COPY(IrcChannel)
};

#ifndef QT_NO_DEBUG_STREAM
IRC_MODEL_EXPORT QDebug operator<<(QDebug debug, const IrcChannel* channel);
#endif // QT_NO_DEBUG_STREAM

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcChannel*))
Q_DECLARE_METATYPE(QList<IRC_PREPEND_NAMESPACE(IrcChannel*)>)

#endif // IRCCHANNEL
