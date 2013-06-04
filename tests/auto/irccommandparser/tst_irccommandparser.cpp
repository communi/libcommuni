/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irccommandparser.h"
#include <QtTest/QtTest>

class tst_IrcCommandParser : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testParse_data();
    void testParse();

private:
    IrcCommandParser parser;
};

void tst_IrcCommandParser::initTestCase()
{
    parser.addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser.addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
    parser.addCommand(IrcCommand::Kick, "KICK (<#channel>) <nick> (<reason...>)");
    parser.addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser.addCommand(IrcCommand::CtcpAction, "ACTION <target> <message...>");

    parser.setChannels(QStringList() << "#freenode" << "#communi");
}

void tst_IrcCommandParser::testParse_data()
{
    QTest::addColumn<QString>("target");
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("join1") << QString("#communi") << QString("/JOIN") << QString();
    QTest::newRow("join2") << QString("#communi") << QString("/JOIN #chan") << QString("JOIN #chan");
    QTest::newRow("join3") << QString("#communi") << QString("/JOIN #chan secret") << QString("JOIN #chan secret");
    QTest::newRow("join4") << QString("#communi") << QString("/JOIN #chan too secret") << QString();

    QTest::newRow("part1") << QString("#communi") << QString("/PART") << QString("PART #communi");
    QTest::newRow("part2") << QString("#communi") << QString("/PART #communi") << QString("PART #communi");
    QTest::newRow("part3") << QString("#communi") << QString("/PART #not-exist") << QString("PART #communi :#not-exist");
    QTest::newRow("part4") << QString("#communi") << QString("/PART hasta la vista") << QString("PART #communi :hasta la vista");
    QTest::newRow("part5") << QString("#communi") << QString("/PART #chan hasta la vista") << QString("PART #communi :#chan hasta la vista");

    QTest::newRow("kick1") << QString("#communi") << QString("/KICK") << QString();
    QTest::newRow("kick2") << QString("#communi") << QString("/KICK #communi") << QString();
    QTest::newRow("kick3") << QString("#communi") << QString("/KICK jpnurmi") << QString("KICK #communi jpnurmi");
    QTest::newRow("kick4") << QString("jpnurmi")  << QString("/KICK jpnurmi") << QString();
    QTest::newRow("kick5") << QString("#communi") << QString("/KICK #communi jpnurmi") << QString("KICK #communi jpnurmi");
    QTest::newRow("kick6") << QString("jpnurmi")  << QString("/KICK jpnurmi jpnurmi") << QString();
    QTest::newRow("kick7") << QString("#communi") << QString("/KICK #communi jpnurmi hasta la vista") << QString("KICK #communi jpnurmi :hasta la vista");
    QTest::newRow("kick8") << QString("jpnurmi")  << QString("/KICK jpnurmi jpnurmi hasta la vista") << QString();
    QTest::newRow("kick9") << QString("#communi") << QString("/KICK jpnurmi hasta la vista") << QString("KICK #communi jpnurmi :hasta la vista");

    QTest::newRow("me1") << QString("jpnurmi")  << QString("/ME") << QString();
    QTest::newRow("me2") << QString("#communi") << QString("/ME loves communi") << QString("PRIVMSG #communi :\1ACTION loves communi\1");
    QTest::newRow("me3") << QString("jpnurmi")  << QString("/ME loves communi") << QString("PRIVMSG jpnurmi :\1ACTION loves communi\1");

    QTest::newRow("action1") << QString("jpnurmi")  << QString("/ACTION") << QString();
    QTest::newRow("action2") << QString("#communi") << QString("/ACTION #communi loves communi") << QString("PRIVMSG #communi :\1ACTION loves communi\1");
    QTest::newRow("action3") << QString("jpnurmi")  << QString("/ACTION jpnurmi loves communi") << QString("PRIVMSG jpnurmi :\1ACTION loves communi\1");
    QTest::newRow("action4") << QString("jpnurmi")  << QString("/ACTION #communi loves communi") << QString("PRIVMSG #communi :\1ACTION loves communi\1");
}

void tst_IrcCommandParser::testParse()
{
    QFETCH(QString, target);
    QFETCH(QString, input);
    QFETCH(QString, output);

    parser.setCurrentTarget(target);

    IrcCommand* cmd = parser.parse(input);
    QCOMPARE(cmd ? cmd->toString() : QString(), output);
}

QTEST_MAIN(tst_IrcCommandParser)

#include "tst_irccommandparser.moc"
