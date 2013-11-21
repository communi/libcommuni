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

#ifndef IRCBUFFERMODEL_H
#define IRCBUFFERMODEL_H

#include <Irc>
#include <IrcGlobal>
#include <QtCore/qstringlist.h>
#include <QtCore/qabstractitemmodel.h>

IRC_BEGIN_NAMESPACE

class IrcBuffer;
class IrcChannel;
class IrcMessage;
class IrcNetwork;
class IrcConnection;
class IrcBufferModelPrivate;

class IRC_MODEL_EXPORT IrcBufferModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder)
    Q_PROPERTY(Irc::SortMethod sortMethod READ sortMethod WRITE setSortMethod)
    Q_PROPERTY(QStringList channels READ channels NOTIFY channelsChanged)
    Q_PROPERTY(Irc::DataRole displayRole READ displayRole WRITE setDisplayRole)
    Q_PROPERTY(QList<IrcBuffer*> buffers READ buffers NOTIFY buffersChanged)
    Q_PROPERTY(IrcConnection* connection READ connection WRITE setConnection NOTIFY connectionChanged)
    Q_PROPERTY(IrcNetwork* network READ network NOTIFY networkChanged)
    Q_PROPERTY(IrcBuffer* bufferPrototype READ bufferPrototype WRITE setBufferPrototype NOTIFY bufferPrototypeChanged)
    Q_PROPERTY(IrcChannel* channelPrototype READ channelPrototype WRITE setChannelPrototype NOTIFY channelPrototypeChanged)

public:
    explicit IrcBufferModel(QObject* parent = 0);
    virtual ~IrcBufferModel();

    IrcConnection* connection() const;
    void setConnection(IrcConnection* connection);

    IrcNetwork* network() const;

    int count() const;
    QStringList channels() const;
    QList<IrcBuffer*> buffers() const;
    Q_INVOKABLE IrcBuffer* get(int index) const;
    Q_INVOKABLE IrcBuffer* find(const QString& title) const;
    Q_INVOKABLE bool contains(const QString& title) const;
    Q_INVOKABLE int indexOf(IrcBuffer* buffer) const;

    Q_INVOKABLE IrcBuffer* add(const QString& title);
    Q_INVOKABLE void add(IrcBuffer* buffer);
    Q_INVOKABLE void remove(const QString& title);
    Q_INVOKABLE void remove(IrcBuffer* buffer);

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder order);

    Irc::SortMethod sortMethod() const;
    void setSortMethod(Irc::SortMethod method);

    Irc::DataRole displayRole() const;
    void setDisplayRole(Irc::DataRole role);

    QModelIndex index(IrcBuffer* buffer) const;
    IrcBuffer* buffer(const QModelIndex& index) const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;

    IrcBuffer* bufferPrototype() const;
    void setBufferPrototype(IrcBuffer* prototype);

    IrcChannel* channelPrototype() const;
    void setChannelPrototype(IrcChannel* prototype);

    Q_INVOKABLE QByteArray saveState(int version = 0) const;
    Q_INVOKABLE bool restoreState(const QByteArray& state, int version = 0);

public Q_SLOTS:
    void clear();
    void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder);
    void sort(Irc::SortMethod method, Qt::SortOrder order = Qt::AscendingOrder);

Q_SIGNALS:
    void countChanged(int count);
    void added(IrcBuffer* buffer);
    void removed(IrcBuffer* buffer);
    void aboutToBeAdded(IrcBuffer* buffer);
    void aboutToBeRemoved(IrcBuffer* buffer);
    void buffersChanged(const QList<IrcBuffer*>& buffers);
    void channelsChanged(const QStringList& channels);
    void connectionChanged(IrcConnection* connection);
    void networkChanged(IrcNetwork* network);
    void messageIgnored(IrcMessage* message);
    void bufferPrototypeChanged(IrcBuffer* prototype);
    void channelPrototypeChanged(IrcChannel* prototype);
    void destroyed(IrcBufferModel* model);

protected Q_SLOTS:
    virtual IrcBuffer* createBuffer(const QString& title);
    virtual IrcChannel* createChannel(const QString& title);

protected:
    virtual bool lessThan(IrcBuffer* one, IrcBuffer* another, Irc::SortMethod method) const;

private:
    friend class IrcBufferLessThan;
    friend class IrcBufferGreaterThan;
    QScopedPointer<IrcBufferModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcBufferModel)
    Q_DISABLE_COPY(IrcBufferModel)

    Q_PRIVATE_SLOT(d_func(), void _irc_connectionStatusChanged())
    Q_PRIVATE_SLOT(d_func(), void _irc_bufferDestroyed(IrcBuffer*))
};

IRC_END_NAMESPACE

#endif // IRCBUFFERMODEL_H
