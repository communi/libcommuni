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
    void testDefaults();
    void testCreate_data();
    void testCreate();

    void testErrorMessage_data();
    void testErrorMessage();
    void testInviteMessage_data();
    void testInviteMessage();
    void testJoinMessage_data();
    void testJoinMessage();
    void testKickMessage_data();
    void testKickMessage();
    void testNickMessage_data();
    void testNickMessage();
    void testNoticeMessage(); // <--
    void testNumericMessage(); // <--
    void testModeMessage(); // <--
    void testPartMessage_data();
    void testPartMessage();
    void testPingMessage(); // <--
    void testPongMessage(); // <--
    void testPrivateMessage(); // <--
    void testQuitMessage_data();
    void testQuitMessage();
    void testTopicMessage_data();
    void testTopicMessage();
};

void tst_IrcMessage::testDefaults()
{
    IrcMessage msg;
    QVERIFY(msg.type() == IrcMessage::Unknown);
    QVERIFY(msg.sender().prefix().isNull());
    QVERIFY(msg.command().isNull());
    QVERIFY(msg.parameters().isEmpty());
}

void tst_IrcMessage::testCreate_data()
{
    QTest::addColumn<QString>("command");
    QTest::addColumn<int>("type");

    QTest::newRow("NICK") << QString("NICK") << static_cast<int>(IrcMessage::Nick);
    QTest::newRow("quit") << QString("quit") << static_cast<int>(IrcMessage::Quit);
    QTest::newRow("joIN") << QString("joIN") << static_cast<int>(IrcMessage::Join);
    QTest::newRow("PArt") << QString("PArt") << static_cast<int>(IrcMessage::Part);
    QTest::newRow("TOPIC") << QString("TOPIC") << static_cast<int>(IrcMessage::Topic);
    QTest::newRow("invite") << QString("invite") << static_cast<int>(IrcMessage::Invite);
    QTest::newRow("KiCK") << QString("KiCK") << static_cast<int>(IrcMessage::Kick);
    QTest::newRow("Mode") << QString("Mode") << static_cast<int>(IrcMessage::Mode);
    QTest::newRow("PrivMsg") << QString("PrivMsg") << static_cast<int>(IrcMessage::Private);
    QTest::newRow("NOTICE") << QString("NOTICE") << static_cast<int>(IrcMessage::Notice);
    QTest::newRow("PING") << QString("PING") << static_cast<int>(IrcMessage::Ping);
    QTest::newRow("pong") << QString("pong") << static_cast<int>(IrcMessage::Pong);
    QTest::newRow("ERROR") << QString("ERROR") << static_cast<int>(IrcMessage::Error);

    QTest::newRow("null") << QString() << static_cast<int>(IrcMessage::Unknown);
    QTest::newRow("empty") << QString("") << static_cast<int>(IrcMessage::Unknown);
    QTest::newRow("space") << QString(" ") << static_cast<int>(IrcMessage::Unknown);
    QTest::newRow("BOGIE") << QString("BOGIE") << static_cast<int>(IrcMessage::Unknown);
}

void tst_IrcMessage::testCreate()
{
    QFETCH(QString, command);
    QFETCH(int, type);

    IrcMessage* msg = IrcMessage::create(command, this);
    QCOMPARE(msg->parent(), this);
    QCOMPARE(msg->command(), command);
    QCOMPARE(type, static_cast<int>(msg->type()));
    delete msg;
}

void tst_IrcMessage::testErrorMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("error");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "error1") << QString("error1");
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "error1") << QString("error1");
}

void tst_IrcMessage::testErrorMessage()
{
    IrcErrorMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Error));
    QCOMPARE(message.error(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, error);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.error(), error);
}

void tst_IrcMessage::testInviteMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("user");
    QTest::addColumn<QString>("channel");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString() << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "user1" << "chan1") << QString("user1") << QString("chan1");
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString() << QString();
    QTest::newRow("missing chan") << false << QString("nick!user@host") << (QStringList() << "user1") << QString("user1") << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "user1" << "chan1") << QString("user1") << QString("chan1");
}

void tst_IrcMessage::testInviteMessage()
{
    IrcInviteMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Invite));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.user(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, user);
    QFETCH(QString, channel);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.user(), user);
    QCOMPARE(message.channel(), channel);
}

void tst_IrcMessage::testJoinMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("channel");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "chan1") << QString("chan1");
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "chan1") << QString("chan1");
}

void tst_IrcMessage::testJoinMessage()
{
    IrcJoinMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Join));
    QCOMPARE(message.channel(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, channel);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.channel(), channel);
}

void tst_IrcMessage::testKickMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("user");
    QTest::addColumn<QString>("reason");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString() << QString() << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "chan1" << "user1") << QString("chan1") << QString("user1") << QString();
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString() << QString() << QString();
    QTest::newRow("missing user") << false << QString("nick!user@host") << (QStringList() << "chan1") << QString("chan1") << QString() << QString();
    QTest::newRow("no reason") << true << QString("nick!user@host") << (QStringList() << "chan1" << "user1") << QString("chan1") << QString("user1") << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "chan1" << "user1" << "reason1") << QString("chan1") << QString("user1") << QString("reason1");
}

void tst_IrcMessage::testKickMessage()
{
    IrcKickMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Kick));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.user(), QString());
    QCOMPARE(message.reason(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, channel);
    QFETCH(QString, user);
    QFETCH(QString, reason);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.channel(), channel);
    QCOMPARE(message.user(), user);
    QCOMPARE(message.reason(), reason);
}

void tst_IrcMessage::testNickMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("nick");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "nick1") << QString("nick1");
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "nick1") << QString("nick1");
}

void tst_IrcMessage::testNickMessage()
{
    IrcNickMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Nick));
    QCOMPARE(message.nick(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, nick);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.nick(), nick);
}

void tst_IrcMessage::testNoticeMessage()
{
}

void tst_IrcMessage::testNumericMessage()
{
}

void tst_IrcMessage::testModeMessage()
{
}

void tst_IrcMessage::testPartMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("reason");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString() << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "chan1" << "reason1") << QString("chan1") << QString("reason1");
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString() << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "chan1" << "reason1") << QString("chan1") << QString("reason1");
}

void tst_IrcMessage::testPartMessage()
{
    IrcPartMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Part));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.reason(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, channel);
    QFETCH(QString, reason);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.channel(), channel);
    QCOMPARE(message.reason(), reason);
}

void tst_IrcMessage::testPingMessage()
{
}

void tst_IrcMessage::testPongMessage()
{
}

void tst_IrcMessage::testPrivateMessage()
{
}

void tst_IrcMessage::testQuitMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("reason");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "reason1") << QString("reason1");
    QTest::newRow("no params") << true << QString("nick!user@host") << (QStringList()) << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "reason1") << QString("reason1");
}

void tst_IrcMessage::testQuitMessage()
{
    IrcQuitMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Quit));
    QCOMPARE(message.reason(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, reason);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.reason(), reason);
}

void tst_IrcMessage::testTopicMessage_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QStringList>("params");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("topic");

    QTest::newRow("null") << false << QString() << (QStringList()) << QString() << QString();
    QTest::newRow("no sender") << false << QString() << (QStringList() << "chan1" << "topic1") << QString("chan1") << QString("topic1");
    QTest::newRow("no params") << false << QString("nick!user@host") << (QStringList()) << QString() << QString();
    QTest::newRow("all ok") << true << QString("nick!user@host") << (QStringList() << "chan1" << "topic1") << QString("chan1") << QString("topic1");
}

void tst_IrcMessage::testTopicMessage()
{
    IrcTopicMessage message;
    QCOMPARE(static_cast<int>(message.type()), static_cast<int>(IrcMessage::Topic));
    QCOMPARE(message.channel(), QString());
    QCOMPARE(message.topic(), QString());

    QFETCH(bool, result);
    QFETCH(QString, sender);
    QFETCH(QStringList, params);
    QFETCH(QString, channel);
    QFETCH(QString, topic);

    QCOMPARE(result, message.initFrom(sender, params));
    QCOMPARE(message.sender().prefix(), sender);
    QCOMPARE(message.parameters(), params);
    QCOMPARE(message.channel(), channel);
    QCOMPARE(message.topic(), topic);
}

QTEST_MAIN(tst_IrcMessage)

#include "tst_ircmessage.moc"
