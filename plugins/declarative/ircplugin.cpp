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
#include <Irc>

QML_DECLARE_TYPE(IrcSession)
QML_DECLARE_TYPE(IrcMessage)
QML_DECLARE_TYPE(IrcInviteMessage)
QML_DECLARE_TYPE(IrcJoinMessage)
QML_DECLARE_TYPE(IrcKickMessage)
QML_DECLARE_TYPE(IrcModeMessage)
QML_DECLARE_TYPE(IrcNickMessage)
QML_DECLARE_TYPE(IrcNoticeMessage)
QML_DECLARE_TYPE(IrcNumericMessage)
QML_DECLARE_TYPE(IrcPartMessage)
QML_DECLARE_TYPE(IrcPrivateMessage)
QML_DECLARE_TYPE(IrcQuitMessage)
QML_DECLARE_TYPE(IrcTopicMessage)

class DeclarativeIrc : public Irc
{
    Q_OBJECT

public:
    explicit DeclarativeIrc(QObject* parent = 0) : Irc(parent) { }

    Q_INVOKABLE IrcMessage* createMessage(const QString& command, QObject* parent = 0)
    {
        return IrcMessage::create(command, parent);
    }
};

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
        context->setContextProperty("Irc", irc);
    }

    void registerTypes(const char *uri)
    {
        qmlRegisterType<IrcSession>(uri, 1, 0, "IrcSession");
        qmlRegisterType<IrcMessage>(uri, 1, 0, "IrcMessage");
        qmlRegisterType<IrcJoinMessage>(uri, 1, 0, "IrcJoinMessage");
    }
};

#include "ircplugin.moc"

Q_EXPORT_PLUGIN2(ircplugin, IrcPlugin);
