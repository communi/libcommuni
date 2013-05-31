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

#ifndef IRCCHANNELMODEL_H
#define IRCCHANNELMODEL_H

#include <Irc>
#include <IrcGlobal>
#include <QtCore/qabstractitemmodel.h>

class IrcSession;
class IrcMessage;
class IrcChannel;
class IrcChannelModelPrivate;

class COMMUNI_EXPORT IrcChannelModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Irc::ItemDataRole displayRole READ displayRole WRITE setDisplayRole)
    Q_PROPERTY(QList<IrcChannel*> channels READ channels NOTIFY channelsChanged)
    Q_PROPERTY(IrcSession* session READ session WRITE setSession NOTIFY sessionChanged)

public:
    explicit IrcChannelModel(QObject* parent = 0);
    virtual ~IrcChannelModel();

    IrcSession* session() const;
    void setSession(IrcSession* session);

    int count() const;
    QList<IrcChannel*> channels() const;
    Q_INVOKABLE IrcChannel* get(int index) const;
    Q_INVOKABLE IrcChannel* channel(const QString& name) const;
    Q_INVOKABLE bool contains(const QString& name) const;

    Irc::ItemDataRole displayRole() const;
    void setDisplayRole(Irc::ItemDataRole role);

public Q_SLOTS:
    void clear();

Q_SIGNALS:
    void countChanged(int count);
    void channelAdded(IrcChannel* channel);
    void channelRemoved(IrcChannel* channel);
    void channelsChanged(const QList<IrcChannel*>& channels);
    void sessionChanged(IrcSession* session);
    void messageIgnored(IrcMessage* message);

protected:
    virtual IrcChannel* createChannel(const QString& name);
    virtual void destroyChannel(IrcChannel* channel);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QScopedPointer<IrcChannelModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcChannelModel)
    Q_DISABLE_COPY(IrcChannelModel)

    Q_PRIVATE_SLOT(d_func(), void _irc_channelDestroyed(IrcChannel*))
};

#endif // IRCCHANNELMODEL_H
