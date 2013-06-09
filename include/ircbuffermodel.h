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

class IrcBuffer;
class IrcSession;
class IrcMessage;
class IrcBufferModelPrivate;

class COMMUNI_EXPORT IrcBufferModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QStringList titles READ titles NOTIFY titlesChanged)
    Q_PROPERTY(Irc::ItemDataRole displayRole READ displayRole WRITE setDisplayRole)
    Q_PROPERTY(QList<IrcBuffer*> buffers READ buffers NOTIFY buffersChanged)
    Q_PROPERTY(IrcSession* session READ session WRITE setSession NOTIFY sessionChanged)

public:
    explicit IrcBufferModel(QObject* parent = 0);
    virtual ~IrcBufferModel();

    IrcSession* session() const;
    void setSession(IrcSession* session);

    int count() const;
    QStringList titles() const;
    QList<IrcBuffer*> buffers() const;
    Q_INVOKABLE IrcBuffer* get(int index) const;
    Q_INVOKABLE IrcBuffer* buffer(const QString& title) const;
    Q_INVOKABLE bool contains(const QString& title) const;
    Q_INVOKABLE IrcBuffer* addBuffer(const QString& title);
    Q_INVOKABLE void removeBuffer(const QString& title);

    Irc::ItemDataRole displayRole() const;
    void setDisplayRole(Irc::ItemDataRole role);

public Q_SLOTS:
    void clear();

Q_SIGNALS:
    void countChanged(int count);
    void bufferAdded(IrcBuffer* buffer);
    void bufferRemoved(IrcBuffer* buffer);
    void titlesChanged(const QStringList& titles);
    void buffersChanged(const QList<IrcBuffer*>& buffers);
    void sessionChanged(IrcSession* session);
    void messageIgnored(IrcMessage* message);

protected:
    virtual IrcBuffer* createBuffer(const QString& title);
    virtual void destroyBuffer(IrcBuffer* buffer);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QScopedPointer<IrcBufferModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcBufferModel)
    Q_DISABLE_COPY(IrcBufferModel)

    Q_PRIVATE_SLOT(d_func(), void _irc_bufferDestroyed(IrcBuffer*))
};

#endif // IRCBUFFERMODEL_H
