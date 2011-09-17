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
    void testNoticeMessage_data();
    void testNoticeMessage();
    void testNumericMessage_data();
    void testNumericMessage();
    void testModeMessage_data();
    void testModeMessage();
    void testPartMessage_data();
    void testPartMessage();
    void testPingMessage(); // <--
    void testPongMessage(); // <--
    void testPrivateMessage_data();
    void testPrivateMessage();
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

void tst_IrcMessage::testErrorMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("error");

    QTest::newRow("no sender") << false << QString("ERROR error1") << QString("error1");
    QTest::newRow("no params") << false << QString(":server ERROR") << QString();
    QTest::newRow("all ok") << true << QString(":server ERROR error1") << QString("error1");
}

void tst_IrcMessage::testErrorMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, error);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Error);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("error").toString(), error);

    IrcErrorMessage* errorMessage = qobject_cast<IrcErrorMessage*>(message);
    QVERIFY(errorMessage);
    QCOMPARE(errorMessage->isValid(), valid);
    QCOMPARE(errorMessage->error(), error);
}

void tst_IrcMessage::testInviteMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("user");
    QTest::addColumn<QString>("channel");

    QTest::newRow("no sender") << false << QString("INVITE Wiz #Dust") << QString("Wiz") << QString("#Dust");
    QTest::newRow("no params") << false << QString(":Angel INVITE") << QString() << QString();
    QTest::newRow("no channel") << false << QString(":Angel INVITE Wiz") << QString("Wiz") << QString();
    QTest::newRow("all ok") << true << QString(":Angel INVITE Wiz #Dust") << QString("Wiz") << QString("#Dust");
}

void tst_IrcMessage::testInviteMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, channel);
    QFETCH(QString, user);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Invite);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("channel").toString(), channel);
    QCOMPARE(message->property("user").toString(), user);

    IrcInviteMessage* inviteMessage = qobject_cast<IrcInviteMessage*>(message);
    QVERIFY(inviteMessage);
    QCOMPARE(inviteMessage->isValid(), valid);
    QCOMPARE(inviteMessage->channel(), channel);
    QCOMPARE(inviteMessage->user(), user);
}

void tst_IrcMessage::testJoinMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("channel");

    QTest::newRow("no sender") << false << QString("JOIN #Twilight_zone") << QString("#Twilight_zone");
    QTest::newRow("no params") << false << QString(":WiZ JOIN") << QString();
    QTest::newRow("all ok") << true << QString(":WiZ JOIN #Twilight_zone") << QString("#Twilight_zone");
}

void tst_IrcMessage::testJoinMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, channel);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Join);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("channel").toString(), channel);

    IrcJoinMessage* joinMessage = qobject_cast<IrcJoinMessage*>(message);
    QVERIFY(joinMessage);
    QCOMPARE(joinMessage->isValid(), valid);
    QCOMPARE(joinMessage->channel(), channel);
}

void tst_IrcMessage::testKickMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("user");
    QTest::addColumn<QString>("reason");

    QTest::newRow("no sender") << false << QString("KICK #Finnish John") << QString("#Finnish") << QString("John") << QString();
    QTest::newRow("no params") << false << QString(":WiZ KICK") << QString() << QString() << QString();
    QTest::newRow("no user") << false << QString(":WiZ KICK #Finnish") << QString("#Finnish") << QString() << QString();
    QTest::newRow("no reason") << true << QString(":WiZ KICK #Finnish John") << QString("#Finnish") << QString("John") << QString();
    QTest::newRow("all ok") << true << QString(":WiZ KICK #Finnish John :Another reason") << QString("#Finnish") << QString("John") << QString("Another reason");
}

void tst_IrcMessage::testKickMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, channel);
    QFETCH(QString, user);
    QFETCH(QString, reason);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Kick);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("channel").toString(), channel);
    QCOMPARE(message->property("user").toString(), user);
    QCOMPARE(message->property("reason").toString(), reason);

    IrcKickMessage* kickMessage = qobject_cast<IrcKickMessage*>(message);
    QVERIFY(kickMessage);
    QCOMPARE(kickMessage->isValid(), valid);
    QCOMPARE(kickMessage->channel(), channel);
    QCOMPARE(kickMessage->user(), user);
    QCOMPARE(kickMessage->reason(), reason);
}

void tst_IrcMessage::testNickMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("nick");

    QTest::newRow("no sender") << false << QString("NICK Kilroy") << QString("Kilroy");
    QTest::newRow("no params") << false << QString(":WiZ NICK") << QString();
    QTest::newRow("all ok") << true << QString(":WiZ NICK Kilroy") << QString("Kilroy");
}

void tst_IrcMessage::testNickMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, nick);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Nick);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("nick").toString(), nick);

    IrcNickMessage* nickMessage = qobject_cast<IrcNickMessage*>(message);
    QVERIFY(nickMessage);
    QCOMPARE(nickMessage->isValid(), valid);
    QCOMPARE(nickMessage->nick(), nick);
}

void tst_IrcMessage::testNoticeMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("target");
    QTest::addColumn<QString>("msg");
    QTest::addColumn<bool>("reply");

    QTest::newRow("no sender") << false << QString("NOTICE Wiz :Hello are you receiving this message ?") << QString("Wiz") << QString("Hello are you receiving this message ?") << false;
    QTest::newRow("no params") << false << QString(":Angel NOTICE Wiz") << QString("Wiz") << QString() << false;
    QTest::newRow("all ok") << true << QString(":Angel NOTICE Wiz :Hello are you receiving this message ?") << QString("Wiz") << QString("Hello are you receiving this message ?") << false;
    QTest::newRow("reply") << true << QString(":Angel NOTICE Wiz :\1Hello are you receiving this message ?\1") << QString("Wiz") << QString("Hello are you receiving this message ?") << true;
}

void tst_IrcMessage::testNoticeMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, target);
    QFETCH(QString, msg);
    QFETCH(bool, reply);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Notice);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("target").toString(), target);
    QCOMPARE(message->property("message").toString(), msg);
    QCOMPARE(message->property("reply").toBool(), reply);

    IrcNoticeMessage* noticeMessage = qobject_cast<IrcNoticeMessage*>(message);
    QVERIFY(noticeMessage);
    QCOMPARE(noticeMessage->isValid(), valid);
    QCOMPARE(noticeMessage->target(), target);
    QCOMPARE(noticeMessage->message(), msg);
    QCOMPARE(noticeMessage->isReply(), reply);
}

void tst_IrcMessage::testNumericMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<int>("code");

    QTest::newRow("no sender") << false << QString("123 Kilroy") << 123;
    QTest::newRow("no params") << true << QString(":WiZ 456") << 456;
    QTest::newRow("all ok") << true << QString(":WiZ 789 Kilroy") << 789;
}

void tst_IrcMessage::testNumericMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(int, code);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Numeric);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("code").toInt(), code);

    IrcNumericMessage* numericMessage = qobject_cast<IrcNumericMessage*>(message);
    QVERIFY(numericMessage);
    QCOMPARE(numericMessage->isValid(), valid);
    QCOMPARE(numericMessage->code(), code);
}

void tst_IrcMessage::testModeMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("target");
    QTest::addColumn<QString>("mode");
    QTest::addColumn<QString>("argument");

    QTest::newRow("no sender") << false << QString("MODE Kilroy -w") << QString("Kilroy") << QString("-w") << QString();
    QTest::newRow("no params") << false << QString(":WiZ MODE Kilroy") << QString("Kilroy") << QString() << QString();
    QTest::newRow("all ok") << true << QString(":WiZ MODE Kilroy -w") << QString("Kilroy") << QString("-w") << QString();

    QTest::newRow("1") << true << QString(":WiZ MODE #Finnish +im") << QString("#Finnish") << QString("+im") << QString();
    QTest::newRow("2") << true << QString(":Angel MODE #Finnish +o Kilroy") << QString("#Finnish") << QString("+o") << QString("Kilroy");
    QTest::newRow("3") << true << QString(":Kilroy MODE #Finnish +v Wiz") << QString("#Finnish") << QString("+v") << QString("Wiz");
    QTest::newRow("4") << false << QString("MODE #Fins -s") << QString("#Fins") << QString("-s") << QString();
    QTest::newRow("5") << true << QString(":WiZ MODE #42 +k oulu") << QString("#42") << QString("+k") << QString("oulu");
    QTest::newRow("6") << false << QString("MODE #eu-opers +l 10") << QString("#eu-opers") << QString("+l") << QString("10");
    QTest::newRow("7") << true << QString(":nobody MODE &oulu +b") << QString("&oulu") << QString("+b") << QString();
    QTest::newRow("8") << true << QString(":someone MODE &oulu +b *!*@*") << QString("&oulu") << QString("+b") << QString("*!*@*");
    QTest::newRow("9") << true << QString(":anyone MODE &oulu +b *!*@*.edu") << QString("&oulu") << QString("+b") << QString("*!*@*.edu");
    QTest::newRow("10") << false << QString("MODE WiZ -w") << QString("WiZ") << QString("-w") << QString();
    QTest::newRow("11") << true << QString(":Angel MODE Angel +i") << QString("Angel") << QString("+i") << QString();
    QTest::newRow("12") << true << QString(":WiZ MODE WiZ -o") << QString("WiZ") << QString("-o") << QString();
}

void tst_IrcMessage::testModeMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, target);
    QFETCH(QString, mode);
    QFETCH(QString, argument);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Mode);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("target").toString(), target);
    QCOMPARE(message->property("mode").toString(), mode);
    QCOMPARE(message->property("argument").toString(), argument);

    IrcModeMessage* modeMessage = qobject_cast<IrcModeMessage*>(message);
    QVERIFY(modeMessage);
    QCOMPARE(modeMessage->isValid(), valid);
    QCOMPARE(modeMessage->target(), target);
    QCOMPARE(modeMessage->mode(), mode);
    QCOMPARE(modeMessage->argument(), argument);
}

void tst_IrcMessage::testPartMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("reason");

    QTest::newRow("no sender") << false << QString("PART #Twilight_zone") << QString("#Twilight_zone") << QString();
    QTest::newRow("no params") << false << QString(":WiZ PART") << QString() << QString();
    QTest::newRow("no reason") << true << QString(":WiZ PART #Twilight_zone") << QString("#Twilight_zone") << QString();
    QTest::newRow("all ok") << true << QString(":WiZ PART #Twilight_zone :Gone to have lunch") << QString("#Twilight_zone") << QString("Gone to have lunch");
}

void tst_IrcMessage::testPartMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, channel);
    QFETCH(QString, reason);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Part);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("channel").toString(), channel);
    QCOMPARE(message->property("reason").toString(), reason);

    IrcPartMessage* partMessage = qobject_cast<IrcPartMessage*>(message);
    QVERIFY(partMessage);
    QCOMPARE(partMessage->isValid(), valid);
    QCOMPARE(partMessage->channel(), channel);
    QCOMPARE(partMessage->reason(), reason);
}

void tst_IrcMessage::testPingMessage()
{
}

void tst_IrcMessage::testPongMessage()
{
}

void tst_IrcMessage::testPrivateMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("target");
    QTest::addColumn<QString>("msg");
    QTest::addColumn<bool>("action");
    QTest::addColumn<bool>("request");

    QTest::newRow("no sender") << false << QString("PRIVMSG Wiz :Hello are you receiving this message ?") << QString("Wiz") << QString("Hello are you receiving this message ?") << false << false;
    QTest::newRow("no params") << false << QString(":Angel PRIVMSG Wiz") << QString("Wiz") << QString() << false << false;
    QTest::newRow("all ok") << true << QString(":Angel PRIVMSG Wiz :Hello are you receiving this message ?") << QString("Wiz") << QString("Hello are you receiving this message ?") << false << false;
    QTest::newRow("action") << true << QString(":Angel PRIVMSG Wiz :\1ACTION Hello are you receiving this message ?\1") << QString("Wiz") << QString("Hello are you receiving this message ?") << true << false;
    QTest::newRow("request") << true << QString(":Angel PRIVMSG Wiz :\1Hello are you receiving this message ?\1") << QString("Wiz") << QString("Hello are you receiving this message ?") << false << true;
}

void tst_IrcMessage::testPrivateMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, target);
    QFETCH(QString, msg);
    QFETCH(bool, action);
    QFETCH(bool, request);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Private);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("target").toString(), target);
    QCOMPARE(message->property("message").toString(), msg);
    QCOMPARE(message->property("action").toBool(), action);
    QCOMPARE(message->property("request").toBool(), request);

    IrcPrivateMessage* privateMessage = qobject_cast<IrcPrivateMessage*>(message);
    QVERIFY(privateMessage);
    QCOMPARE(privateMessage->isValid(), valid);
    QCOMPARE(privateMessage->target(), target);
    QCOMPARE(privateMessage->message(), msg);
    QCOMPARE(privateMessage->isAction(), action);
    QCOMPARE(privateMessage->isRequest(), request);
}

void tst_IrcMessage::testQuitMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("reason");

    QTest::newRow("no sender") << false << QString("QUIT :Gone to have lunch") << QString("Gone to have lunch");
    QTest::newRow("no params") << true << QString(":WiZ QUIT") << QString();
    QTest::newRow("all ok") << true << QString(":WiZ QUIT :Gone to have lunch") << QString("Gone to have lunch");
}

void tst_IrcMessage::testQuitMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, reason);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Quit);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("reason").toString(), reason);

    IrcQuitMessage* quitMessage = qobject_cast<IrcQuitMessage*>(message);
    QVERIFY(quitMessage);
    QCOMPARE(quitMessage->isValid(), valid);
    QCOMPARE(quitMessage->reason(), reason);
}

void tst_IrcMessage::testTopicMessage_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("topic");

    QTest::newRow("no sender") << false << QString("TOPIC #test") << QString("#test") << QString();
    QTest::newRow("no params") << false << QString(":WiZ TOPIC") << QString() << QString();
    QTest::newRow("no topic") << true << QString(":WiZ TOPIC #test") << QString("#test") << QString();
    QTest::newRow("all ok") << true << QString(":WiZ TOPIC #test :another topic") << QString("#test") << QString("another topic");
}

void tst_IrcMessage::testTopicMessage()
{
    QFETCH(bool, valid);
    QFETCH(QString, str);
    QFETCH(QString, channel);
    QFETCH(QString, topic);

    IrcMessage* message = IrcMessage::fromString(str, this);
    QCOMPARE(message->type(), IrcMessage::Topic);
    QCOMPARE(message->property("valid").toBool(), valid);
    QCOMPARE(message->property("channel").toString(), channel);
    QCOMPARE(message->property("topic").toString(), topic);

    IrcTopicMessage* topicMessage = qobject_cast<IrcTopicMessage*>(message);
    QVERIFY(topicMessage);
    QCOMPARE(topicMessage->isValid(), valid);
    QCOMPARE(topicMessage->channel(), channel);
    QCOMPARE(topicMessage->topic(), topic);
}

QTEST_MAIN(tst_IrcMessage)

#include "tst_ircmessage.moc"
