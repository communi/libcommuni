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
#include "ircbot.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // enable debug output
    qputenv("IRC_DEBUG", "1");

    IrcBot bot;
    bot.setHost("irc.libera.chat");
    bot.setUserName("communi");
#if (QT_VERSION) >= (QT_VERSION_CHECK(5, 10, 0))
    bot.setNickName("Bot" + QString::number(QRandomGenerator::global()->bounded(1, 10000)));
#else
    qsrand(QTime::currentTime().msec());
    bot.setNickName("Bot" + QString::number(qrand() % 9999));
#endif
    bot.setRealName("Communi " + Irc::version() + " example bot");

    bool joined = false;
    QStringList args = app.arguments();
    for (int i = 0; i < args.count(); ++i) {
        QString arg = args.at(i);
        if (arg == "-help") {
            qDebug() << "Usage:" << argv[0] << "(-host <host>) (-port <port>) (-ssl) (-nick <nick>) (-user <user>) (-password <password>) (-channel <channel>)";
            return 0;
        } else if (arg == "-port") {
            bool ok = false;
            int port = args.value(++i).toInt(&ok);
            if (ok)
                bot.setPort(port);
        } else if (arg == "-ssl") {
            bot.setSecure(true);
        } else if (arg == "-host") {
            bot.setHost(args.value(++i));
        } else if (arg == "-user") {
            bot.setUserName(args.value(++i));
        } else if (arg == "-password") {
            bot.setPassword(args.value(++i));
        } else if (arg == "-nick") {
            bot.setNickName(args.value(++i));
        } else if (arg == "-channel") {
            bot.join(args.value(++i));
            joined = true;
        }
    }
    if (!joined)
        bot.join("#botwar");

    bot.open();
    return app.exec();
}
