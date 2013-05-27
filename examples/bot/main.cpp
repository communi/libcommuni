/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include <QCoreApplication>
#include <QSettings>
#include "ircbot.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    IrcBot bot;

    QSettings settings("settings.ini", QSettings::IniFormat);
    bot.setHost(settings.value("host", "irc.freenode.net").toString());
    bot.setPort(settings.value("port", 6667).toInt());
    bot.setUserName(settings.value("username", "communi").toString());
    bot.setNickName(settings.value("nickname", "communi-bot").toString());
    bot.setRealName(settings.value("realname", "communi bot").toString());
    bot.setChannel(settings.value("channel", "#communi").toString());

    bot.open();
    return app.exec();
}
