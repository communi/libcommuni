/*
 * Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "ircmessage.h"
#include <QtTest/QtTest>

class tst_IrcMessage : public QObject
{
    Q_OBJECT

private slots:
    void testInviteMessage();
    void testKickMessage();
    void testJoinMessage();
    void testListMessage();
    void testNamesMessage();
    void testNickMessage();
    void testOperatorMessage();
    void testPartMessage();
    void testPasswordMessage();
    void testQuitMessage();
    void testUserMessage();
    void testTopicMessage();

    // TODO: mode, private, notice, query, who, whois,
    //       whowas, ping, pong, error, numeric, away
};

void tst_IrcMessage::testInviteMessage()
{
    IrcInviteMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Invite));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.user(), QString());
    message.setChannel("foo1");
    message.setUser("foo2");
    QCOMPARE(message.channel(), QString("foo1"));
    QCOMPARE(message.user(), QString("foo2"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(message.setProperty("user", QString("oof2")));
    QCOMPARE(message.property("user").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#channn");
    message.setUser("ussser");
    QCOMPARE(message.toString(), QString("INVITE ussser #channn"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "chan"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "user1" << "chan1" << "extra"));
    QCOMPARE(message.channel(), QString("chan1"));
    QCOMPARE(message.user(), QString("user1"));
}

void tst_IrcMessage::testJoinMessage()
{
    IrcJoinMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Join));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.key(), QString());
    message.setChannel("foo1");
    message.setKey("foo2");
    QCOMPARE(message.channel(), QString("foo1"));
    QCOMPARE(message.key(), QString("foo2"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(message.setProperty("key", QString("oof2")));
    QCOMPARE(message.property("key").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#chan");
    message.setKey("kkk");
    QCOMPARE(message.toString(), QString("JOIN #chan kkk"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "chan"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan1" << "key1" << "extra"));
    QCOMPARE(message.channel(), QString("chan1"));
    QCOMPARE(message.key(), QString("key1"));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan2"));
    QCOMPARE(message.channel(), QString("chan2"));
    QCOMPARE(message.key(), QString());
}

void tst_IrcMessage::testKickMessage()
{
    IrcKickMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Kick));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.user(), QString());
    QCOMPARE(message.reason(), QString());
    message.setChannel("foo1");
    message.setUser("foo2");
    message.setReason("foo3");
    QCOMPARE(message.channel(), QString("foo1"));
    QCOMPARE(message.user(), QString("foo2"));
    QCOMPARE(message.reason(), QString("foo3"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(message.setProperty("user", QString("oof2")));
    QCOMPARE(message.property("user").toString(), QString("oof2"));

    QVERIFY(message.setProperty("reason", QString("oof3")));
    QCOMPARE(message.property("reason").toString(), QString("oof3"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#channn");
    message.setUser("ussser");
    message.setReason("reassson");
    QCOMPARE(message.toString(), QString("KICK ussser #channn :reassson"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "user"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "user1" << "chan1" << "reason1"));
    QCOMPARE(message.channel(), QString("chan1"));
    QCOMPARE(message.user(), QString("user1"));
    QCOMPARE(message.reason(), QString("reason1"));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "user2" << "chan2"));
    QCOMPARE(message.channel(), QString("chan2"));
    QCOMPARE(message.user(), QString("user2"));
    QCOMPARE(message.reason(), QString());
}

void tst_IrcMessage::testListMessage()
{
    IrcListMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::List));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.server(), QString());
    message.setChannel("foo1");
    message.setServer("foo2");
    QCOMPARE(message.channel(), QString("foo1"));
    QCOMPARE(message.server(), QString("foo2"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(message.setProperty("server", QString("oof2")));
    QCOMPARE(message.property("server").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#chan");
    message.setServer("irc.serv.er");
    QCOMPARE(message.toString(), QString("LIST #chan irc.serv.er"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "chan"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan1" << "server1" << "extra"));
    QCOMPARE(message.channel(), QString("chan1"));
    QCOMPARE(message.server(), QString("server1"));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan2"));
    QCOMPARE(message.channel(), QString("chan2"));
    QCOMPARE(message.server(), QString());
}

void tst_IrcMessage::testNamesMessage()
{
    IrcNamesMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Names));
    QCOMPARE(message.channel(), QString());
    message.setChannel("foo1");
    QCOMPARE(message.channel(), QString("foo1"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#chan");
    QCOMPARE(message.toString(), QString("NAMES #chan"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "chan"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan"));
    QCOMPARE(message.channel(), QString("chan"));
}

void tst_IrcMessage::testNickMessage()
{
    IrcNickMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Nick));
    QCOMPARE(message.nick(), QString());
    message.setNick("foo");
    QCOMPARE(message.nick(), QString("foo"));

    QVERIFY(message.setProperty("nick", QString("oof")));
    QCOMPARE(message.property("nick").toString(), QString("oof"));

    QVERIFY(!message.syntax().isEmpty());
    message.setNick("name");
    QCOMPARE(message.toString(), QString("NICK name"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "bar"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "bar" << "baz"));
    QCOMPARE(message.prefix(), QString("nick!user@host"));
    QCOMPARE(message.nick(), QString("bar"));
}

void tst_IrcMessage::testOperatorMessage()
{
    IrcOperatorMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Operator));
    QCOMPARE(message.user(), QString());
    QCOMPARE(message.password(), QString());
    message.setUser("foo1");
    message.setPassword("foo2");
    QCOMPARE(message.user(), QString("foo1"));
    QCOMPARE(message.password(), QString("foo2"));

    QVERIFY(message.setProperty("user", QString("oof1")));
    QCOMPARE(message.property("user").toString(), QString("oof1"));

    QVERIFY(message.setProperty("password", QString("oof2")));
    QCOMPARE(message.property("password").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setUser("userrr");
    message.setPassword("passswd");
    QCOMPARE(message.toString(), QString("OPER userrr passswd"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "user"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));
    QVERIFY(!message.initFrom("nick!user@host", QStringList() << "user"));

    QCOMPARE(message.prefix(), QString("nick!user@host"));
    QVERIFY(message.initFrom("nick!user@host", QStringList() << "user" << "pass"));
    QCOMPARE(message.user(), QString("user"));
    QCOMPARE(message.password(), QString("pass"));
}

void tst_IrcMessage::testPartMessage()
{
    IrcPartMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Part));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.reason(), QString());
    message.setChannel("foo1");
    message.setReason("foo2");
    QCOMPARE(message.channel(), QString("foo1"));
    QCOMPARE(message.reason(), QString("foo2"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(message.setProperty("reason", QString("oof2")));
    QCOMPARE(message.property("reason").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#chan");
    message.setReason("rea son");
    QCOMPARE(message.toString(), QString("PART #chan :rea son"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "chan"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan1" << "rea son" << "extra"));
    QCOMPARE(message.channel(), QString("chan1"));
    QCOMPARE(message.reason(), QString("rea son"));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan2"));
    QCOMPARE(message.channel(), QString("chan2"));
    QCOMPARE(message.reason(), QString());
}

void tst_IrcMessage::testPasswordMessage()
{
    IrcPasswordMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Password));
    QCOMPARE(message.password(), QString());
    message.setPassword("foo");
    QCOMPARE(message.password(), QString("foo"));

    QVERIFY(message.setProperty("password", QString("oof")));
    QCOMPARE(message.property("password").toString(), QString("oof"));

    QVERIFY(!message.syntax().isEmpty());
    message.setPassword("secret");
    QCOMPARE(message.toString(), QString("PASS secret"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "bar"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "bar" << "baz"));
    QCOMPARE(message.prefix(), QString("nick!user@host"));
    QCOMPARE(message.password(), QString("bar"));
}

void tst_IrcMessage::testQuitMessage()
{
    IrcQuitMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Quit));
    QCOMPARE(message.reason(), QString());
    message.setReason("foo");
    QCOMPARE(message.reason(), QString("foo"));

    QVERIFY(message.setProperty("reason", QString("oof")));
    QCOMPARE(message.property("reason").toString(), QString("oof"));

    QVERIFY(!message.syntax().isEmpty());
    message.setReason("one two three");
    QCOMPARE(message.toString(), QString("QUIT :one two three"));

    QVERIFY(!message.initFrom(QString(), QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList()));
    QCOMPARE(message.prefix(), QString("nick!user@host"));
    QCOMPARE(message.reason(), QString());

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "huu haa" << "hoo"));
    QCOMPARE(message.prefix(), QString("nick!user@host"));
    QCOMPARE(message.reason(), QString("huu haa"));
}

void tst_IrcMessage::testUserMessage()
{
    IrcUserMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::User));
    QCOMPARE(message.userName(), QString());
    QCOMPARE(message.realName(), QString());
    message.setUserName("foo1");
    message.setRealName("foo2");
    QCOMPARE(message.userName(), QString("foo1"));
    QCOMPARE(message.realName(), QString("foo2"));

    QVERIFY(message.setProperty("userName", QString("oof1")));
    QCOMPARE(message.property("userName").toString(), QString("oof1"));

    QVERIFY(message.setProperty("realName", QString("oof2")));
    QCOMPARE(message.property("realName").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setUserName("userrr");
    message.setRealName("realll name");
    QVERIFY(QRegExp("USER userrr \\w+ \\w+ :realll name").exactMatch(message.toString()));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "1"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));
    QVERIFY(!message.initFrom("nick!user@host", QStringList() << "username" << "hostname"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList() << "username" << "hostname" << "servername"));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "username" << "hostname" << "servername" << "real name"));
    QCOMPARE(message.prefix(), QString("nick!user@host"));
    QCOMPARE(message.userName(), QString("username"));
    QCOMPARE(message.realName(), QString("real name"));
}

void tst_IrcMessage::testTopicMessage()
{
    IrcTopicMessage message;
    QCOMPARE(message.type(), static_cast<uint>(Irc::Topic));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.topic(), QString());
    message.setChannel("foo1");
    message.setTopic("foo2");
    QCOMPARE(message.channel(), QString("foo1"));
    QCOMPARE(message.topic(), QString("foo2"));

    QVERIFY(message.setProperty("channel", QString("oof1")));
    QCOMPARE(message.property("channel").toString(), QString("oof1"));

    QVERIFY(message.setProperty("topic", QString("oof2")));
    QCOMPARE(message.property("topic").toString(), QString("oof2"));

    QVERIFY(!message.syntax().isEmpty());
    message.setChannel("#chan");
    message.setTopic("t o p i c");
    QCOMPARE(message.toString(), QString("TOPIC #chan :t o p i c"));

    QVERIFY(!message.initFrom(QString(), QStringList()));
    QVERIFY(!message.initFrom(QString(), QStringList() << "chan"));
    QVERIFY(!message.initFrom("nick!user@host", QStringList()));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan1" << "topic" << "extra"));
    QCOMPARE(message.channel(), QString("chan1"));
    QCOMPARE(message.topic(), QString("topic"));

    QVERIFY(message.initFrom("nick!user@host", QStringList() << "chan2"));
    QCOMPARE(message.channel(), QString("chan2"));
    QCOMPARE(message.topic(), QString());
}

QTEST_MAIN(tst_IrcMessage)

#include "tst_ircmessage.moc"
