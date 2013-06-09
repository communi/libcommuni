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

#ifndef IRCUSERMODEL_H
#define IRCUSERMODEL_H

#include <Irc>
#include <IrcGlobal>
#include <QtCore/qstringlist.h>
#include <QtCore/qabstractitemmodel.h>

class IrcUser;
class IrcBuffer;
class IrcMessage;
class IrcUserModelPrivate;

class COMMUNI_EXPORT IrcUserModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QStringList names READ names NOTIFY namesChanged)
    Q_PROPERTY(QList<IrcUser*> users READ users NOTIFY usersChanged)
    Q_PROPERTY(Irc::ItemDataRole displayRole READ displayRole WRITE setDisplayRole)
    Q_PROPERTY(IrcBuffer* buffer READ buffer WRITE setBuffer NOTIFY bufferChanged)

public:
    explicit IrcUserModel(QObject* parent = 0);
    virtual ~IrcUserModel();

    IrcBuffer* buffer() const;
    void setBuffer(IrcBuffer* buffer);

    int count() const;
    QStringList names() const;
    QList<IrcUser*> users() const;
    Q_INVOKABLE IrcUser* get(int index) const;
    Q_INVOKABLE IrcUser* user(const QString& name) const;
    Q_INVOKABLE bool contains(const QString& name) const;

    Irc::ItemDataRole displayRole() const;
    void setDisplayRole(Irc::ItemDataRole role);

Q_SIGNALS:
    void countChanged(int count);
    void userAdded(IrcUser* user);
    void userRemoved(IrcUser* user);
    void namesChanged(const QStringList& names);
    void usersChanged(const QList<IrcUser*>& users);
    void activitySortEnabledChanged(bool enabled);
    void bufferChanged(IrcBuffer* buffer);

protected:
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    friend class IrcBufferPrivate;
    QScopedPointer<IrcUserModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcUserModel)
    Q_DISABLE_COPY(IrcUserModel)
};

#endif // IRCUSERMODEL_H
