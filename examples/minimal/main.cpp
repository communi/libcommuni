/*
 * Copyright (C) 2008-2020 The Communi Project
 *
 * This example is free, and not covered by the BSD license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include <QtCore>
#if (QT_VERSION) >= (QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif
#include <IrcConnection>
#include <IrcCommand>
#include <Irc>

#ifndef IRC_DOXYGEN
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // enable debug output
    qputenv("IRC_DEBUG", "1");

//! [minimal]
    IrcConnection connection(QStringLiteral("irc.libera.chat"));
    connection.setUserName(QStringLiteral("communi"));
#if (QT_VERSION) >= (QT_VERSION_CHECK(5, 10, 0))
    connection.setNickName(QStringLiteral("Minimal%1").arg(QRandomGenerator::global()->bounded(1, 10000)));
#else
    qsrand(QTime::currentTime().msec());
    connection.setNickName(QString("Minimal%1").arg(qrand() % 9999));
#endif
    connection.setRealName(QStringLiteral("Communi %1 minimal example").arg(Irc::version()));
    connection.sendCommand(IrcCommand::createJoin(QStringLiteral("#botwar")));
    connection.sendCommand(IrcCommand::createMessage(QStringLiteral("#botwar"), QStringLiteral("Hi, kthxbye!")));
    connection.sendCommand(IrcCommand::createQuit());
    connection.open();
//! [minimal]

    QObject::connect(&connection, SIGNAL(disconnected()), &app, SLOT(quit()));
    return app.exec();
}
#endif
