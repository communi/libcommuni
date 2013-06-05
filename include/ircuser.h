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

class IrcMessage;
class IrcUserPrivate;

class COMMUNI_EXPORT IrcUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString prefix READ prefix NOTIFY prefixChanged)
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)

public:
    explicit IrcUser(QObject* parent = 0);
    virtual ~IrcUser();

    QString name() const;
    QString prefix() const;
    QString mode() const;

Q_SIGNALS:
    void nameChanged(const QString& name);
    void prefixChanged(const QString& prefix);
    void modeChanged(const QString& mode);
    void messageReceived(IrcMessage* message);

private:
    QScopedPointer<IrcUserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcUser)
    Q_DISABLE_COPY(IrcUser)
};

#endif // IRCUSER_H
