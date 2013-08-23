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
class IrcConnection;
class IrcMessage;
class IrcBufferModelPrivate;

class IRC_MODEL_EXPORT IrcBufferModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool dynamicSort READ dynamicSort WRITE setDynamicSort)
    Q_PROPERTY(QStringList channels READ channels NOTIFY channelsChanged)
    Q_PROPERTY(Irc::DataRole displayRole READ displayRole WRITE setDisplayRole)
    Q_PROPERTY(QList<IrcBuffer*> buffers READ buffers NOTIFY buffersChanged)
    Q_PROPERTY(IrcConnection* connection READ connection WRITE setConnection NOTIFY connectionChanged)

public:
    explicit IrcBufferModel(QObject* parent = 0);
    virtual ~IrcBufferModel();

    IrcConnection* connection() const;
    void setConnection(IrcConnection* connection);

    int count() const;
    QStringList channels() const;
    QList<IrcBuffer*> buffers() const;
    Q_INVOKABLE IrcBuffer* get(int index) const;
    Q_INVOKABLE IrcBuffer* buffer(const QString& title) const;
    Q_INVOKABLE bool contains(const QString& title) const;
    Q_INVOKABLE int indexOf(IrcBuffer* buffer) const;

    Q_INVOKABLE IrcBuffer* add(const QString& title);
    Q_INVOKABLE void add(IrcBuffer* buffer);
    Q_INVOKABLE void remove(const QString& title);

    bool dynamicSort() const;
    void setDynamicSort(bool dynamic);

    Irc::DataRole displayRole() const;
    void setDisplayRole(Irc::DataRole role);

    QModelIndex index(IrcBuffer* buffer) const;
    IrcBuffer* buffer(const QModelIndex& index) const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;

public Q_SLOTS:
    void clear();
    void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder);

Q_SIGNALS:
    void countChanged(int count);
    void added(IrcBuffer* buffer);
    void removed(IrcBuffer* buffer);
    void aboutToBeAdded(IrcBuffer* buffer);
    void aboutToBeRemoved(IrcBuffer* buffer);
    void buffersChanged(const QList<IrcBuffer*>& buffers);
    void channelsChanged(const QStringList& channels);
    void connectionChanged(IrcConnection* connection);
    void messageIgnored(IrcMessage* message);

protected:
    virtual IrcBuffer* create(const QString& title);

    virtual bool lessThan(IrcBuffer* one, IrcBuffer* another) const;

private:
    QScopedPointer<IrcBufferModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcBufferModel)
    Q_DISABLE_COPY(IrcBufferModel)

    Q_PRIVATE_SLOT(d_func(), void _irc_bufferDestroyed(IrcBuffer*))
};

IRC_END_NAMESPACE

#endif // IRCBUFFERMODEL_H
