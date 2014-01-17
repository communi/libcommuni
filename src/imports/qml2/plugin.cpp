/*
* Copyright (C) 2008-2014 The Communi Project
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

#include <QtQml>

#include <IrcCore>
#include <IrcModel>
#include <IrcUtil>

class IrcQmlFilter : public QObject,
                     public IrcCommandFilter,
                     public IrcMessageFilter
{
    Q_OBJECT
    Q_INTERFACES(IrcCommandFilter IrcMessageFilter)
    Q_PROPERTY(IrcConnection* connection READ connection WRITE setConnection NOTIFY connectionChanged)

public:
    IrcQmlFilter(QObject* parent = 0) : QObject(parent), conn(0) { }

    IrcConnection* connection() const { return conn; }
    void setConnection(IrcConnection* connection)
    {
        if (conn != connection) {
            if (conn) {
                conn->removeCommandFilter(this);
                conn->removeMessageFilter(this);
            }
            conn = connection;
            if (conn) {
                conn->installCommandFilter(this);
                conn->installMessageFilter(this);
            }
            emit connectionChanged();
        }
    }

    bool commandFilter(IrcCommand* cmd)
    {
        // QML: QVariant commandFilter(QVariant)
        const QMetaObject* mo = metaObject();
        int idx = mo->indexOfMethod("commandFilter(QVariant)");
        if (idx != -1) {
            QVariant ret;
            QMetaMethod method = mo->method(idx);
            method.invoke(this, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(cmd)));
            return ret.toBool();
        }
        return false;
    }

    bool messageFilter(IrcMessage* msg)
    {
        // QML: QVariant messageFilter(QVariant)
        const QMetaObject* mo = metaObject();
        int idx = mo->indexOfMethod("messageFilter(QVariant)");
        if (idx != -1) {
            QVariant ret;
            QMetaMethod method = mo->method(idx);
            method.invoke(this, Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(msg)));
            return ret.toBool();
        }
        return false;
    }

signals:
    void connectionChanged();

private:
    QPointer<IrcConnection> conn;
};

class CommuniPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Communi.QQmlExtensionInterface")

public:
    void registerTypes(const char* uri) {
//! [qml-register-types]
        // IrcCore
        Irc::registerMetaTypes();
        qmlRegisterType<Irc>(uri, 3, 0, "Irc");
        qmlRegisterType<IrcCommand>(uri, 3, 0, "IrcCommand");
        qmlRegisterType<IrcConnection>(uri, 3, 0, "IrcConnection");
        qmlRegisterUncreatableType<IrcMessage>(uri, 3, 0, "IrcMessage", "Cannot create an instance of IrcMessage. Use IrcConnection::messageReceived() signal instead.");
        qmlRegisterUncreatableType<IrcNetwork>(uri, 3, 0, "IrcNetwork", "Cannot create an instance of IrcNetwork. Use IrcConnection::network property instead.");
        qmlRegisterType<IrcQmlFilter>(uri, 3, 0, "IrcMessageFilter");
        qmlRegisterType<IrcQmlFilter>(uri, 3, 0, "IrcCommandFilter");
        qmlRegisterType<IrcQmlFilter>(uri, 3, 0, "IrcFilter");

        // IrcModel
        qmlRegisterType<IrcBuffer>(uri, 3, 0, "IrcBuffer");
        qmlRegisterType<IrcBufferModel>(uri, 3, 0, "IrcBufferModel");
        qmlRegisterType<IrcChannel>(uri, 3, 0, "IrcChannel");
        qmlRegisterType<IrcUser>(uri, 3, 0, "IrcUser");
        qmlRegisterType<IrcUserModel>(uri, 3, 0, "IrcUserModel");

        // IrcUtil
        qmlRegisterType<IrcCommandParser>(uri, 3, 0, "IrcCommandParser");
        qmlRegisterType<IrcLagTimer>(uri, 3, 0, "IrcLagTimer");
        qmlRegisterType<IrcTextFormat>(uri, 3, 0, "IrcTextFormat");
        qmlRegisterUncreatableType<IrcPalette>(uri, 3, 0, "IrcPalette", "Cannot create an instance of IrcPalette. Use IrcTextFormat::palette property instead.");
//! [qml-register-types]
    }
};

#include "plugin.moc"
