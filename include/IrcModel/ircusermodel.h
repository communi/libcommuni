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

IRC_BEGIN_NAMESPACE

class IrcUser;
class IrcChannel;
class IrcMessage;
class IrcUserModelPrivate;

class IRC_MODEL_EXPORT IrcUserModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QStringList names READ names NOTIFY namesChanged)
    Q_PROPERTY(QList<IrcUser*> users READ users NOTIFY usersChanged)
    Q_PROPERTY(Irc::ItemDataRole displayRole READ displayRole WRITE setDisplayRole)
    Q_PROPERTY(IrcChannel* channel READ channel WRITE setChannel NOTIFY channelChanged)

public:
    explicit IrcUserModel(QObject* parent = 0);
    virtual ~IrcUserModel();

    IrcChannel* channel() const;
    void setChannel(IrcChannel* channel);

    int count() const;
    QStringList names() const;
    QList<IrcUser*> users() const;
    Q_INVOKABLE IrcUser* get(int index) const;
    Q_INVOKABLE IrcUser* user(const QString& name) const;
    Q_INVOKABLE bool contains(const QString& name) const;

    Irc::ItemDataRole displayRole() const;
    void setDisplayRole(Irc::ItemDataRole role);

    QModelIndex index(IrcUser* user) const;
    IrcUser* user(const QModelIndex& index) const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;

Q_SIGNALS:
    void added(IrcUser* user);
    void removed(IrcUser* user);
    void countChanged(int count);
    void namesChanged(const QStringList& names);
    void usersChanged(const QList<IrcUser*>& users);
    void activitySortEnabledChanged(bool enabled);
    void channelChanged(IrcChannel* channel);

private:
    friend class IrcChannelPrivate;
    QScopedPointer<IrcUserModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcUserModel)
    Q_DISABLE_COPY(IrcUserModel)
};

IRC_END_NAMESPACE

#endif // IRCUSERMODEL_H
