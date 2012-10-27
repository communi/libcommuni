/*
 * Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "irccommand.h"
#include <QtTest/QtTest>

class tst_IrcCommand : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

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
    void testMotd();
    void testNames();
    void testNick();
    void testNotice();
    void testPart();
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
};

void tst_IrcCommand::testDefaults()
{
    IrcCommand cmd;
    QVERIFY(cmd.type() == IrcCommand::Custom);
    QVERIFY(cmd.parameters().isEmpty());
}

void tst_IrcCommand::testAdmin()
{
    IrcCommand* cmd = IrcCommand::createAdmin();
    QVERIFY(cmd->type() == IrcCommand::Admin);
    QVERIFY(cmd->toString().contains(QRegExp("\\bADMIN\\b")));
    delete cmd;
}

void tst_IrcCommand::testAway()
{
    IrcCommand* cmd = IrcCommand::createAway();
    QVERIFY(cmd->type() == IrcCommand::Away);
    QVERIFY(cmd->toString().contains(QRegExp("\\bAWAY\\b")));
    delete cmd;
}

void tst_IrcCommand::testCapability()
{
    IrcCommand* cmd = IrcCommand::createCapability("sub");
    QVERIFY(cmd->type() == IrcCommand::Capability);
    QVERIFY(cmd->toString().contains(QRegExp("\\bCAP\\b")));
    delete cmd;
}

void tst_IrcCommand::testCtcpAction()
{
    IrcCommand* cmd = IrcCommand::createCtcpAction("tgt", "act");
    QVERIFY(cmd->type() == IrcCommand::CtcpAction);
    QVERIFY(cmd->toString().contains(QRegExp("\\bPRIVMSG\\b")));
    delete cmd;
}

void tst_IrcCommand::testCtcpReply()
{
    IrcCommand* cmd = IrcCommand::createCtcpReply("tgt", "rpl");
    QVERIFY(cmd->type() == IrcCommand::CtcpReply);
    QVERIFY(cmd->toString().contains(QRegExp("\\bNOTICE\\b")));
    delete cmd;
}

void tst_IrcCommand::testCtcpRequest()
{
    IrcCommand* cmd = IrcCommand::createCtcpRequest("tgt", "req");
    QVERIFY(cmd->type() == IrcCommand::CtcpRequest);
    QVERIFY(cmd->toString().contains(QRegExp("\\bPRIVMSG\\b")));
    delete cmd;
}

void tst_IrcCommand::testInfo()
{
    IrcCommand* cmd = IrcCommand::createInfo();
    QVERIFY(cmd->type() == IrcCommand::Info);
    QVERIFY(cmd->toString().contains(QRegExp("\\bINFO\\b")));
    delete cmd;
}

void tst_IrcCommand::testInvite()
{
    IrcCommand* cmd = IrcCommand::createInvite("usr", "chan");
    QVERIFY(cmd->type() == IrcCommand::Invite);
    QVERIFY(cmd->toString().contains(QRegExp("\\bINVITE\\b")));
    delete cmd;
}

void tst_IrcCommand::testJoin()
{
    IrcCommand* cmd = IrcCommand::createJoin("chan");
    QVERIFY(cmd->type() == IrcCommand::Join);
    QVERIFY(cmd->toString().contains(QRegExp("\\bJOIN\\b")));
    delete cmd;
}

void tst_IrcCommand::testKick()
{
    IrcCommand* cmd = IrcCommand::createKick("chan", "usr");
    QVERIFY(cmd->type() == IrcCommand::Kick);
    QVERIFY(cmd->toString().contains(QRegExp("\\bKICK\\b")));
    delete cmd;
}

void tst_IrcCommand::testKnock()
{
    IrcCommand* cmd = IrcCommand::createKnock("chan");
    QVERIFY(cmd->type() == IrcCommand::Knock);
    QVERIFY(cmd->toString().contains(QRegExp("\\bKNOCK\\b")));
    delete cmd;
}

void tst_IrcCommand::testList()
{
    IrcCommand* cmd = IrcCommand::createList();
    QVERIFY(cmd->type() == IrcCommand::List);
    QVERIFY(cmd->toString().contains(QRegExp("\\bLIST\\b")));
    delete cmd;
}

void tst_IrcCommand::testMessage()
{
    IrcCommand* cmd = IrcCommand::createMessage("tgt", "msg");
    QVERIFY(cmd->type() == IrcCommand::Message);
    QVERIFY(cmd->toString().contains(QRegExp("\\bPRIVMSG\\b")));
    delete cmd;
}

void tst_IrcCommand::testMode()
{
    IrcCommand* cmd = IrcCommand::createMode("tgt", "mode");
    QVERIFY(cmd->type() == IrcCommand::Mode);
    QVERIFY(cmd->toString().contains(QRegExp("\\bMODE\\b")));
    delete cmd;
}

void tst_IrcCommand::testMotd()
{
    IrcCommand* cmd = IrcCommand::createMotd();
    QVERIFY(cmd->type() == IrcCommand::Motd);
    QVERIFY(cmd->toString().contains(QRegExp("\\bMOTD\\b")));
    delete cmd;
}

void tst_IrcCommand::testNames()
{
    IrcCommand* cmd = IrcCommand::createNames();
    QVERIFY(cmd->type() == IrcCommand::Names);
    QVERIFY(cmd->toString().contains(QRegExp("\\bNAMES\\b")));
    delete cmd;
}

void tst_IrcCommand::testNick()
{
    IrcCommand* cmd = IrcCommand::createNick("nick");
    QVERIFY(cmd->type() == IrcCommand::Nick);
    QVERIFY(cmd->toString().contains(QRegExp("\\bNICK\\b")));
    delete cmd;
}

void tst_IrcCommand::testNotice()
{
    IrcCommand* cmd = IrcCommand::createNotice("tgt", "msg");
    QVERIFY(cmd->type() == IrcCommand::Notice);
    QVERIFY(cmd->toString().contains(QRegExp("\\bNOTICE\\b")));
    delete cmd;
}

void tst_IrcCommand::testPart()
{
    IrcCommand* cmd = IrcCommand::createPart("chan");
    QVERIFY(cmd->type() == IrcCommand::Part);
    QVERIFY(cmd->toString().contains(QRegExp("\\bPART\\b")));
    delete cmd;
}

void tst_IrcCommand::testQuit()
{
    IrcCommand* cmd = IrcCommand::createQuit();
    QVERIFY(cmd->type() == IrcCommand::Quit);
    QVERIFY(cmd->toString().contains(QRegExp("\\bQUIT\\b")));
    delete cmd;
}

void tst_IrcCommand::testQuote()
{
    IrcCommand* cmd = IrcCommand::createQuote("CUSTOM");
    QVERIFY(cmd->type() == IrcCommand::Quote);
    QVERIFY(cmd->toString().contains(QRegExp("\\bCUSTOM\\b")));
    delete cmd;
}

void tst_IrcCommand::testStats()
{
    IrcCommand* cmd = IrcCommand::createStats("qry");
    QVERIFY(cmd->type() == IrcCommand::Stats);
    QVERIFY(cmd->toString().contains(QRegExp("\\bSTATS\\b")));
    delete cmd;
}

void tst_IrcCommand::testTime()
{
    IrcCommand* cmd = IrcCommand::createTime();
    QVERIFY(cmd->type() == IrcCommand::Time);
    QVERIFY(cmd->toString().contains(QRegExp("\\bTIME\\b")));
    delete cmd;
}

void tst_IrcCommand::testTopic()
{
    IrcCommand* cmd = IrcCommand::createTopic("chan");
    QVERIFY(cmd->type() == IrcCommand::Topic);
    QVERIFY(cmd->toString().contains(QRegExp("\\bTOPIC\\b")));
    delete cmd;
}

void tst_IrcCommand::testTrace()
{
    IrcCommand* cmd = IrcCommand::createTrace();
    QVERIFY(cmd->type() == IrcCommand::Trace);
    QVERIFY(cmd->toString().contains(QRegExp("\\bTRACE\\b")));
    delete cmd;
}

void tst_IrcCommand::testUsers()
{
    IrcCommand* cmd = IrcCommand::createUsers();
    QVERIFY(cmd->type() == IrcCommand::Users);
    QVERIFY(cmd->toString().contains(QRegExp("\\bUSERS\\b")));
    delete cmd;
}

void tst_IrcCommand::testVersion()
{
    IrcCommand* cmd = IrcCommand::createVersion();
    QVERIFY(cmd->type() == IrcCommand::Version);
    QVERIFY(cmd->toString().contains(QRegExp("\\bVERSION\\b")));
    delete cmd;
}

void tst_IrcCommand::testWho()
{
    IrcCommand* cmd = IrcCommand::createWho("msk");
    QVERIFY(cmd->type() == IrcCommand::Who);
    QVERIFY(cmd->toString().contains(QRegExp("\\bWHO\\b")));
    delete cmd;
}

void tst_IrcCommand::testWhois()
{
    IrcCommand* cmd = IrcCommand::createWhois("usr");
    QVERIFY(cmd->type() == IrcCommand::Whois);
    QVERIFY(cmd->toString().contains(QRegExp("\\bWHOIS\\b")));
    delete cmd;
}

void tst_IrcCommand::testWhowas()
{
    IrcCommand* cmd = IrcCommand::createWhowas("usr");
    QVERIFY(cmd->type() == IrcCommand::Whowas);
    QVERIFY(cmd->toString().contains(QRegExp("\\bWHOWAS\\b")));
    delete cmd;
}

QTEST_MAIN(tst_IrcCommand)

#include "tst_irccommand.moc"
