/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#include <QtDeclarative>
#include <IrcSession>
#include <IrcMessage>
#include <IrcPrefix>
#include <Irc>

class DeclarativeIrc : public Irc
{
    Q_OBJECT

public:
    explicit DeclarativeIrc(QObject* parent = 0) : Irc(parent) { }

    Q_INVOKABLE static IrcMessage* createMessage(const QString& command, QObject* parent = 0)
    {
        return IrcMessage::create(command, parent);
    }

    Q_INVOKABLE static QString nick(const QString& prefix)
    {
        return IrcPrefix(prefix).nick();
    }

    Q_INVOKABLE static QString user(const QString& prefix)
    {
        return IrcPrefix(prefix).user();
    }

    Q_INVOKABLE static QString host(const QString& prefix)
    {
        return IrcPrefix(prefix).host();
    }
};

QML_DECLARE_TYPE(IrcSession)
QML_DECLARE_TYPE(DeclarativeIrc)

class IrcPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    virtual void initializeEngine(QDeclarativeEngine* engine, const char* uri)
    {
        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);
        QDeclarativeContext *context = engine->rootContext();
        DeclarativeIrc* irc = new DeclarativeIrc(context);
        context->setContextProperty("irc", irc);
    }

    void registerTypes(const char *uri)
    {
        qmlRegisterType<IrcSession>(uri, 1, 0, "IrcSession");
        qmlRegisterUncreatableType<DeclarativeIrc>(uri, 1, 0, "Irc", "");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(communiplugin, IrcPlugin);
