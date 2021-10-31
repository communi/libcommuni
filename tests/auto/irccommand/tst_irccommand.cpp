/*
 * Copyright (C) 2008-2020 The Communi Project
 *
 * This test is free, and not covered by the BSD license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irccommand.h"
#include "ircmessage.h"
#include "ircconnection.h"
#include <QtTest/QtTest>
#include <QTextCodec>
#include <QtCore/QScopedPointer>

class tst_IrcCommand : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testEncoding_data();
    void testEncoding();

    void testConversion();

    void testConnection();

    void testAdmin();
    void testAway();
    void testCapability();
    void testCtcpAction();
    void testCtcpReply();
    void testCtcpRequest();
    void testInfo();
    void testInvite();
    void testJoin();
    void testKick();
    void testKnock();
    void testList();
    void testMessage();
    void testMode();
    void testMonitor();
    void testMotd();
    void testNames();
    void testNick();
    void testNotice();
    void testPart();
    void testPing();
    void testPong();
    void testQuit();
    void testQuote();
    void testStats();
    void testTime();
    void testTopic();
    void testTrace();
    void testUsers();
    void testVersion();
    void testWho();
    void testWhois();
    void testWhowas();

    void testDebug();
};

void tst_IrcCommand::testDefaults()
{
    IrcCommand cmd;
    QVERIFY(cmd.parameters().isEmpty());
    QCOMPARE(cmd.type(), IrcCommand::Custom);
    QCOMPARE(cmd.encoding(), QByteArray("UTF-8"));
    QVERIFY(!cmd.connection());
    QVERIFY(!cmd.network());

    QTest::ignoreMessage(QtWarningMsg, "Reimplement IrcCommand::toString() for IrcCommand::Custom");
    QVERIFY(cmd.toString().isEmpty());
}

void tst_IrcCommand::testEncoding_data()
{
    QTest::addColumn<QByteArray>("encoding");
    QTest::addColumn<QByteArray>("actual");
    QTest::addColumn<bool>("supported");

    QTest::newRow("null") << QByteArray() << QByteArray("UTF-8") << false;
    QTest::newRow("empty") << QByteArray("") << QByteArray("UTF-8") << false;
    QTest::newRow("space") << QByteArray(" ") << QByteArray("UTF-8") << false;
    QTest::newRow("invalid") << QByteArray("invalid") << QByteArray("UTF-8") << false;
    foreach (const QByteArray& codec, QTextCodec::availableCodecs())
        QTest::newRow(codec) << codec << codec << true;
}

void tst_IrcCommand::testEncoding()
{
    QFETCH(QByteArray, encoding);
    QFETCH(QByteArray, actual);
    QFETCH(bool, supported);

    if (!supported)
        QTest::ignoreMessage(QtWarningMsg, "IrcCommand::setEncoding(): unsupported encoding \"" + encoding + "\" ");

    IrcCommand cmd;
    cmd.setEncoding(encoding);
    QCOMPARE(cmd.encoding(), actual);
}

void tst_IrcCommand::testConversion()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createMessage(QStringLiteral("target"), QStringLiteral("foo bar")));
    QVERIFY(cmd.data());
    QCOMPARE(cmd->type(), IrcCommand::Message);

    IrcConnection conn;
    QScopedPointer<IrcMessage> msg(cmd->toMessage(QStringLiteral("prefix"), &conn));
    QVERIFY(msg.data());

    QCOMPARE(msg->type(), IrcMessage::Private);
    QCOMPARE(msg->connection(), &conn);
    QCOMPARE(msg->prefix(), QString("prefix"));
    QCOMPARE(msg->property("target").toString(), QString("target"));
    QCOMPARE(msg->property("content").toString(), QString("foo bar"));
}

void tst_IrcCommand::testConnection()
{
    IrcConnection* connection = new IrcConnection(this);
    IrcCommand command(connection);
    QVERIFY(!command.connection());
    QVERIFY(!command.network());

    connection->sendCommand(&command);
    QCOMPARE(command.connection(), connection);
    QCOMPARE(command.network(), connection->network());

    command.setParent(nullptr);
    delete connection;
    QVERIFY(!command.connection());
    QVERIFY(!command.network());
}

void tst_IrcCommand::testAdmin()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createAdmin(QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Admin);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bADMIN\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testAway()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createAway(QStringLiteral("reason")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Away);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bAWAY\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\breason\\b")));
}

void tst_IrcCommand::testCapability()
{
    QScopedPointer<IrcCommand> cmd1(IrcCommand::createCapability(QStringLiteral("sub"), QStringLiteral("cap")));
    QVERIFY(cmd1.data());

    QCOMPARE(cmd1->type(), IrcCommand::Capability);
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bCAP\\b")));
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bsub\\b")));
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bcap\\b")));

    QScopedPointer<IrcCommand> cmd2(IrcCommand::createCapability(QStringLiteral("sub"), QStringList() << QStringLiteral("cap1") << QStringLiteral("cap2")));
    QVERIFY(cmd2.data());

    QCOMPARE(cmd2->type(), IrcCommand::Capability);
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bCAP\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bsub\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bcap1\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bcap2\\b")));
}

void tst_IrcCommand::testCtcpAction()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createCtcpAction(QStringLiteral("tgt"), QStringLiteral("act")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::CtcpAction);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bPRIVMSG\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btgt\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bact\\b")));
    QCOMPARE(cmd->toString().count("\01"), 2);
}

void tst_IrcCommand::testCtcpReply()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createCtcpReply(QStringLiteral("tgt"), QStringLiteral("rpl")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::CtcpReply);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bNOTICE\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btgt\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\brpl\\b")));
    QCOMPARE(cmd->toString().count("\01"), 2);
}

void tst_IrcCommand::testCtcpRequest()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createCtcpRequest(QStringLiteral("tgt"), QStringLiteral("req")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::CtcpRequest);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bPRIVMSG\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btgt\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\breq\\b")));
    QCOMPARE(cmd->toString().count("\01"), 2);
}

void tst_IrcCommand::testInfo()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createInfo(QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Info);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bINFO\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testInvite()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createInvite(QStringLiteral("usr"), QStringLiteral("chan")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Invite);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bINVITE\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\busr\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bchan\\b")));
}

void tst_IrcCommand::testJoin()
{
    QScopedPointer<IrcCommand> cmd1(IrcCommand::createJoin(QStringLiteral("chan")));
    QVERIFY(cmd1.data());

    QCOMPARE(cmd1->type(), IrcCommand::Join);
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bJOIN\\b")));
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bchan\\b")));

    QScopedPointer<IrcCommand> cmd2(IrcCommand::createJoin(QStringList() << QStringLiteral("chan1") << QStringLiteral("chan2")));
    QVERIFY(cmd2.data());

    QCOMPARE(cmd2->type(), IrcCommand::Join);
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bJOIN\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bchan1\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bchan2\\b")));

    QScopedPointer<IrcCommand> cmd3(IrcCommand::createJoin(QStringList() << QStringLiteral("chan1") << QStringLiteral("chan2"), QStringList() << QStringLiteral("key1") << QStringLiteral("key2")));
    QVERIFY(cmd3.data());

    QCOMPARE(cmd3->type(), IrcCommand::Join);
    QVERIFY(cmd3->toString().contains(QRegularExpression("\\bJOIN\\b")));
    QVERIFY(cmd3->toString().contains(QRegularExpression("\\bchan1,chan2\\b")));
    QVERIFY(cmd3->toString().contains(QRegularExpression("\\bkey1,key2\\b")));
}

void tst_IrcCommand::testKick()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createKick(QStringLiteral("chan"), QStringLiteral("usr")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Kick);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bKICK\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bchan\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\busr\\b")));
}

void tst_IrcCommand::testKnock()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createKnock(QStringLiteral("chan")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Knock);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bKNOCK\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bchan\\b")));
}

void tst_IrcCommand::testList()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createList(QStringList() << QStringLiteral("chan1") << QStringLiteral("chan2"), QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::List);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bLIST\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bchan1\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bchan2\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testMessage()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createMessage(QStringLiteral("tgt"), QStringLiteral("msg")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Message);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bPRIVMSG\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btgt\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bmsg\\b")));
}

void tst_IrcCommand::testMode()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createMode(QStringLiteral("tgt"), QStringLiteral("mode")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Mode);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bMODE\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btgt\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bmode\\b")));
}

void tst_IrcCommand::testMonitor()
{
    QScopedPointer<IrcCommand> cmd1(IrcCommand::createMonitor(QStringLiteral("+"), QStringLiteral("foo")));
    QVERIFY(cmd1.data());

    QCOMPARE(cmd1->type(), IrcCommand::Monitor);
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bMONITOR\\b")));
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bfoo\\b")));

    QScopedPointer<IrcCommand> cmd2(IrcCommand::createMonitor(QStringLiteral("+"), QStringList() << QStringLiteral("foo") << QStringLiteral("bar")));
    QVERIFY(cmd2.data());

    QCOMPARE(cmd2->type(), IrcCommand::Monitor);
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bMONITOR\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bfoo\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bbar\\b")));
}

void tst_IrcCommand::testMotd()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createMotd(QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Motd);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bMOTD\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testNames()
{
    QScopedPointer<IrcCommand> cmd1(IrcCommand::createNames(QStringLiteral("chan")));
    QVERIFY(cmd1.data());

    QCOMPARE(cmd1->type(), IrcCommand::Names);
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bNAMES\\b")));
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bchan\\b")));

    QScopedPointer<IrcCommand> cmd2(IrcCommand::createNames(QStringList() << QStringLiteral("chan1") << QStringLiteral("chan2")));
    QVERIFY(cmd2.data());

    QCOMPARE(cmd2->type(), IrcCommand::Names);
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bNAMES\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bchan1\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bchan2\\b")));
}

void tst_IrcCommand::testNick()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createNick(QStringLiteral("nick")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Nick);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bNICK\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bnick\\b")));
}

void tst_IrcCommand::testNotice()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createNotice(QStringLiteral("tgt"), QStringLiteral("msg")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Notice);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bNOTICE\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btgt\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bmsg\\b")));
}

void tst_IrcCommand::testPart()
{
    QScopedPointer<IrcCommand> cmd1(IrcCommand::createPart(QStringLiteral("chan")));
    QVERIFY(cmd1.data());

    QCOMPARE(cmd1->type(), IrcCommand::Part);
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bPART\\b")));
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bchan\\b")));

    QScopedPointer<IrcCommand> cmd2(IrcCommand::createPart(QStringList() << QStringLiteral("chan1") << QStringLiteral("chan2")));
    QVERIFY(cmd2.data());

    QCOMPARE(cmd2->type(), IrcCommand::Part);
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bPART\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bchan1\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bchan2\\b")));
}

void tst_IrcCommand::testPing()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createPing(QStringLiteral("arg")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Ping);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bPING\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\barg\\b")));
}

void tst_IrcCommand::testPong()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createPong(QStringLiteral("arg")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Pong);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bPONG\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\barg\\b")));
}

void tst_IrcCommand::testQuit()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createQuit(QStringLiteral("reason")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Quit);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bQUIT\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\breason\\b")));
}

void tst_IrcCommand::testQuote()
{
    QScopedPointer<IrcCommand> cmd1(IrcCommand::createQuote(QStringLiteral("CUSTOM")));
    QVERIFY(cmd1.data());

    QCOMPARE(cmd1->type(), IrcCommand::Quote);
    QVERIFY(cmd1->toString().contains(QRegularExpression("\\bCUSTOM\\b")));

    QScopedPointer<IrcCommand> cmd2(IrcCommand::createQuote(QStringList() << QStringLiteral("FOO") << QStringLiteral("BAR")));
    QVERIFY(cmd2.data());

    QCOMPARE(cmd2->type(), IrcCommand::Quote);
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bFOO\\b")));
    QVERIFY(cmd2->toString().contains(QRegularExpression("\\bBAR\\b")));
}

void tst_IrcCommand::testStats()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createStats(QStringLiteral("query"), QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Stats);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bSTATS\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bquery\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testTime()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createTime(QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Time);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bTIME\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testTopic()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createTopic(QStringLiteral("chan"), QStringLiteral("topic")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Topic);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bTOPIC\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bchan\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btopic\\b")));
}

void tst_IrcCommand::testTrace()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createTrace(QStringLiteral("target")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Trace);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bTRACE\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\btarget\\b")));
}

void tst_IrcCommand::testUsers()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createUsers(QStringLiteral("server")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Users);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bUSERS\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bserver\\b")));
}

void tst_IrcCommand::testVersion()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createVersion(QStringLiteral("user")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Version);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bVERSION\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\buser\\b")));
}

void tst_IrcCommand::testWho()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createWho(QStringLiteral("mask")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Who);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bWHO\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bmask\\b")));
}

void tst_IrcCommand::testWhois()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createWhois(QStringLiteral("mask")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Whois);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bWHOIS\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bmask\\b")));
}

void tst_IrcCommand::testWhowas()
{
    QScopedPointer<IrcCommand> cmd(IrcCommand::createWhowas(QStringLiteral("mask")));
    QVERIFY(cmd.data());

    QCOMPARE(cmd->type(), IrcCommand::Whowas);
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bWHOWAS\\b")));
    QVERIFY(cmd->toString().contains(QRegularExpression("\\bmask\\b")));
}

void tst_IrcCommand::testDebug()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcCommand*>(nullptr);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcCommand(0x0)"));
    str.clear();

    IrcCommand command;
    QTest::ignoreMessage(QtWarningMsg, "Reimplement IrcCommand::toString() for IrcCommand::Custom");
    dbg << &command;
    QVERIFY(QRegularExpression("IrcCommand\\(0x[0-9A-Fa-f]+, type=Custom\\) ").match(str).hasMatch());
    str.clear();

    command.setType(IrcCommand::Quit);
    dbg << &command;
    QVERIFY(QRegularExpression("IrcCommand\\(0x[0-9A-Fa-f]+, type=Quit, \"QUIT :\"\\) ").match(str).hasMatch());
    str.clear();

    command.setObjectName(QStringLiteral("foo"));
    dbg << &command;
    QVERIFY(QRegularExpression("IrcCommand\\(0x[0-9A-Fa-f]+, name=foo, type=Quit, \"QUIT :\"\\) ").match(str).hasMatch());
    str.clear();

    dbg << IrcCommand::Join;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Join"));
    str.clear();
}

QTEST_MAIN(tst_IrcCommand)

#include "tst_irccommand.moc"
