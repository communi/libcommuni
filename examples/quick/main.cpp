/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include <QtGui>
#include <QtQml>

#include <IrcCore>
#include <IrcModel>
#include <IrcUtil>

Q_DECLARE_METATYPE(Irc::Code)
Q_DECLARE_METATYPE(IrcMessage::Type)
Q_DECLARE_METATYPE(IrcCommand::Type)

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    // Enums
    qRegisterMetaType<Irc::Code>();
    qRegisterMetaType<IrcMessage::Type>();
    qRegisterMetaType<IrcCommand::Type>();

    // IrcCore
    qmlRegisterType<Irc>("Communi", 3, 0, "Irc");
    qmlRegisterType<IrcCommand>("Communi", 3, 0, "IrcCommand");
    qmlRegisterType<IrcConnection>("Communi", 3, 0, "IrcConnection");
    qmlRegisterUncreatableType<IrcMessage>("Communi", 3, 0, "IrcMessage", "Cannot create an instance of IrcMessage. Use IrcConnection::messageReceived() signal instead.");
    qmlRegisterUncreatableType<IrcNetwork>("Communi", 3, 0, "IrcNetwork", "Cannot create an instance of IrcNetwork. Use IrcConnection::network property instead.");
    // TODO: IrcMessageFilter

    // IrcModel
    qmlRegisterType<IrcBuffer>("Communi", 3, 0, "IrcBuffer");
    qmlRegisterType<IrcBufferModel>("Communi", 3, 0, "IrcBufferModel");
    qmlRegisterType<IrcChannel>("Communi", 3, 0, "IrcChannel");
    qmlRegisterType<IrcUser>("Communi", 3, 0, "IrcUser");
    qmlRegisterType<IrcUserModel>("Communi", 3, 0, "IrcUserModel");

    // IrcUtil
    qmlRegisterType<IrcCommandParser>("Communi", 3, 0, "IrcCommandParser");
    qmlRegisterType<IrcLagTimer>("Communi", 3, 0, "IrcLagTimer");
    qmlRegisterType<IrcTextFormat>("Communi", 3, 0, "IrcTextFormat");

    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
    return app.exec();
}
