/*
* Copyright (C) 2008-2013 J-P Nurmi <jpnurmi@gmail.com>
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

Q_DECLARE_METATYPE(Irc::Code)
Q_DECLARE_METATYPE(IrcMessage::Type)
Q_DECLARE_METATYPE(IrcCommand::Type)

class CommuniPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Communi.QQmlExtensionInterface")

public:
    void registerTypes(const char* uri) {
        // Enums
        qRegisterMetaType<Irc::Code>();
        qRegisterMetaType<IrcMessage::Type>();
        qRegisterMetaType<IrcCommand::Type>();

        // IrcCore
        qmlRegisterType<Irc>(uri, 3, 0, "Irc");
        qmlRegisterType<IrcCommand>(uri, 3, 0, "IrcCommand");
        qmlRegisterType<IrcConnection>(uri, 3, 0, "IrcConnection");
        qmlRegisterUncreatableType<IrcMessage>(uri, 3, 0, "IrcMessage", "Cannot create an instance of IrcMessage. Use IrcConnection::messageReceived() signal instead.");
        qmlRegisterUncreatableType<IrcNetwork>(uri, 3, 0, "IrcNetwork", "Cannot create an instance of IrcNetwork. Use IrcConnection::network property instead.");
        // TODO: IrcMessageFilter

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
    }
};

#include "plugin.moc"
