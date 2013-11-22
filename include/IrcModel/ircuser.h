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

#ifndef IRCUSER_H
#define IRCUSER_H

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qscopedpointer.h>

IRC_BEGIN_NAMESPACE

class IrcChannel;
class IrcUserPrivate;

class IRC_MODEL_EXPORT IrcUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString prefix READ prefix NOTIFY prefixChanged)
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(bool servOp READ isServOp NOTIFY servOpChanged)
    Q_PROPERTY(bool away READ isAway NOTIFY awayChanged)
    Q_PROPERTY(IrcChannel* channel READ channel CONSTANT)

public:
    explicit IrcUser(QObject* parent = 0);
    virtual ~IrcUser();

    QString title() const;
    QString name() const;
    QString prefix() const;
    QString mode() const;
    bool isServOp() const;
    bool isAway() const;

    IrcChannel* channel() const;

Q_SIGNALS:
    void titleChanged(const QString& title);
    void nameChanged(const QString& name);
    void prefixChanged(const QString& prefix);
    void modeChanged(const QString& mode);
    void servOpChanged(const bool& servOp);
    void awayChanged(const bool& away);

private:
    QScopedPointer<IrcUserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcUser)
    Q_DISABLE_COPY(IrcUser)
};

#ifndef QT_NO_DEBUG_STREAM
IRC_MODEL_EXPORT QDebug operator<<(QDebug debug, const IrcUser* user);
#endif // QT_NO_DEBUG_STREAM

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcUser*))
Q_DECLARE_METATYPE(QList<IRC_PREPEND_NAMESPACE(IrcUser*)>)

#endif // IRCUSER_H
