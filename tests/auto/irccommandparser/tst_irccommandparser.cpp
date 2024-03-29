/*
 * Copyright (C) 2008-2020 The Communi Project
 *
 * This test is free, and not covered by the BSD license. There is no
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
    void testParse_data();
    void testParse();
    void testTriggers();
    void testTarget();
    void testChannels();
    void testCommands();
    void testClear();
    void testReset();
    void testAddRemove();
    void testSyntax_data();
    void testSyntax();
    void testTolerancy();
    void testCustom();
    void testWhitespace();
};

void tst_IrcCommandParser::testParse_data()
{
    QTest::addColumn<QString>("target");
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("msg") << QStringLiteral("#communi") << QStringLiteral("Hello all!") << QStringLiteral("PRIVMSG #communi :Hello all!");
    QTest::newRow("//msg") << QStringLiteral("#communi") << QStringLiteral("//msg test") << QStringLiteral("PRIVMSG #communi :/msg test");
    QTest::newRow("/ /msg") << QStringLiteral("#communi") << QStringLiteral("/ /msg test") << QStringLiteral("PRIVMSG #communi :/msg test");

    QTest::newRow("join1") << QStringLiteral("#communi") << QStringLiteral("/JOIN") << QString();
    QTest::newRow("join2") << QStringLiteral("#communi") << QStringLiteral("/JOIN #chan") << QStringLiteral("JOIN #chan");
    QTest::newRow("join3") << QStringLiteral("#communi") << QStringLiteral("/JOIN #chan secret") << QStringLiteral("JOIN #chan secret");
    QTest::newRow("join4") << QStringLiteral("#communi") << QStringLiteral("/JOIN #chan too secret") << QString();

    QTest::newRow("part1") << QStringLiteral("#communi") << QStringLiteral("/PART") << QStringLiteral("PART #communi");
    QTest::newRow("part2") << QStringLiteral("#communi") << QStringLiteral("/PART #communi") << QStringLiteral("PART #communi");
    QTest::newRow("part3") << QStringLiteral("#communi") << QStringLiteral("/PART #not-exist") << QStringLiteral("PART #communi :#not-exist");
    QTest::newRow("part4") << QStringLiteral("#communi") << QStringLiteral("/PART hasta la vista") << QStringLiteral("PART #communi :hasta la vista");
    QTest::newRow("part5") << QStringLiteral("#communi") << QStringLiteral("/PART #chan hasta la vista") << QStringLiteral("PART #communi :#chan hasta la vista");

    QTest::newRow("kick1") << QStringLiteral("#communi") << QStringLiteral("/KICK") << QString();
    QTest::newRow("kick2") << QStringLiteral("#communi") << QStringLiteral("/KICK #communi") << QString();
    QTest::newRow("kick3") << QStringLiteral("#communi") << QStringLiteral("/KICK jpnurmi") << QStringLiteral("KICK #communi jpnurmi");
    QTest::newRow("kick4") << QStringLiteral("jpnurmi")  << QStringLiteral("/KICK jpnurmi") << QString();
    QTest::newRow("kick5") << QStringLiteral("#communi") << QStringLiteral("/KICK #communi jpnurmi") << QStringLiteral("KICK #communi jpnurmi");
    QTest::newRow("kick6") << QStringLiteral("jpnurmi")  << QStringLiteral("/KICK jpnurmi jpnurmi") << QString();
    QTest::newRow("kick7") << QStringLiteral("#communi") << QStringLiteral("/KICK #communi jpnurmi hasta la vista") << QStringLiteral("KICK #communi jpnurmi :hasta la vista");
    QTest::newRow("kick8") << QStringLiteral("jpnurmi")  << QStringLiteral("/KICK jpnurmi jpnurmi hasta la vista") << QString();
    QTest::newRow("kick9") << QStringLiteral("#communi") << QStringLiteral("/KICK jpnurmi hasta la vista") << QStringLiteral("KICK #communi jpnurmi :hasta la vista");

    QTest::newRow("me1") << QStringLiteral("jpnurmi")  << QStringLiteral("/ME") << QString();
    QTest::newRow("me2") << QStringLiteral("#communi") << QStringLiteral("/ME loves communi") << QString("PRIVMSG #communi :\1ACTION loves communi\1");
    QTest::newRow("me3") << QStringLiteral("jpnurmi")  << QStringLiteral("/ME loves communi") << QString("PRIVMSG jpnurmi :\1ACTION loves communi\1");

    QTest::newRow("action1") << QStringLiteral("jpnurmi")  << QStringLiteral("/ACTION") << QString();
    QTest::newRow("action2") << QStringLiteral("#communi") << QStringLiteral("/ACTION #communi loves communi") << QString("PRIVMSG #communi :\1ACTION loves communi\1");
    QTest::newRow("action3") << QStringLiteral("jpnurmi")  << QStringLiteral("/ACTION jpnurmi loves communi") << QString("PRIVMSG jpnurmi :\1ACTION loves communi\1");
    QTest::newRow("action4") << QStringLiteral("jpnurmi")  << QStringLiteral("/ACTION #communi loves communi") << QString("PRIVMSG #communi :\1ACTION loves communi\1");
}

void tst_IrcCommandParser::testParse()
{
    QFETCH(QString, target);
    QFETCH(QString, input);
    QFETCH(QString, output);

    IrcCommandParser parser;
    parser.setTolerant(true);
    parser.setTriggers(QStringList("/"));
    QCOMPARE(parser.triggers(), QStringList("/"));

    parser.addCommand(IrcCommand::Join, QStringLiteral("JOIN <#channel> (<key>)"));
    parser.addCommand(IrcCommand::Part, QStringLiteral("PART (<#channel>) (<message...>)"));
    parser.addCommand(IrcCommand::Kick, QStringLiteral("KICK (<#channel>) <nick> (<reason...>)"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ME [target] <message...>"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ACTION <target> <message...>"));

    parser.setTarget(target);
    parser.setChannels(QStringList() << QStringLiteral("#libera") << QStringLiteral("#communi"));

    IrcCommand* cmd = parser.parse(input);
    QCOMPARE(cmd ? cmd->toString() : QString(), output);
}

void tst_IrcCommandParser::testTriggers()
{
    IrcCommandParser parser;
    parser.setTriggers(QStringList("/"));
    parser.addCommand(IrcCommand::Join, QStringLiteral("JOIN #channel"));
    parser.setTarget(QStringLiteral("#target"));

    QSignalSpy triggerSpy(&parser, SIGNAL(triggersChanged(QStringList)));
    QVERIFY(triggerSpy.isValid());

    parser.setTriggers(QStringList("!"));
    QCOMPARE(parser.triggers(), QStringList("!"));
    QCOMPARE(triggerSpy.count(), 1);
    QCOMPARE(triggerSpy.last().at(0).toStringList(), QStringList("!"));

    IrcCommand* cmd = parser.parse(QStringLiteral("!join #communi"));
    QVERIFY(cmd);
    QCOMPARE(cmd->type(), IrcCommand::Join);
    QCOMPARE(cmd->toString(), QString("JOIN #communi"));
    delete cmd;

    parser.setTriggers(QStringList());
    QCOMPARE(parser.triggers(), QStringList());
    QCOMPARE(triggerSpy.count(), 2);
    QCOMPARE(triggerSpy.last().at(0).toStringList(), QStringList());

    cmd = parser.parse(QStringLiteral("!join #communi"));
    QVERIFY(!cmd);

    parser.setTolerant(true);
    cmd = parser.parse(QStringLiteral("!join #communi"));
    QCOMPARE(cmd->type(), IrcCommand::Message);
    QCOMPARE(cmd->toString(), QString("PRIVMSG #target :!join #communi"));
    delete cmd;

    QVERIFY(!parser.parse(""));
}

void tst_IrcCommandParser::testTarget()
{
    IrcCommandParser parser;
    QVERIFY(parser.target().isEmpty());

    QSignalSpy targetSpy(&parser, SIGNAL(targetChanged(QString)));
    QVERIFY(targetSpy.isValid());

    parser.setTarget(QStringLiteral("#tgt"));
    QCOMPARE(parser.target(), QString("#tgt"));
    QCOMPARE(targetSpy.count(), 1);
    QCOMPARE(targetSpy.last().at(0).toString(), QString("#tgt"));

    parser.setTarget(QStringLiteral("#tgt"));
    QCOMPARE(targetSpy.count(), 1);

    parser.setTarget(QString());
    QCOMPARE(parser.target(), QString());
    QCOMPARE(targetSpy.count(), 2);
    QCOMPARE(targetSpy.last().at(0).toString(), QString());
}

void tst_IrcCommandParser::testChannels()
{
    IrcCommandParser parser;
    QVERIFY(parser.channels().isEmpty());

    QSignalSpy channelSpy(&parser, SIGNAL(channelsChanged(QStringList)));
    QVERIFY(channelSpy.isValid());

    parser.setChannels(QStringList() << QStringLiteral("#foo") << QStringLiteral("#bar"));
    QCOMPARE(parser.channels(), QStringList() << "#foo" << "#bar");
    QCOMPARE(channelSpy.count(), 1);
    QCOMPARE(channelSpy.last().at(0).toStringList(), QStringList() << "#foo" << "#bar");

    parser.setChannels(QStringList() << QStringLiteral("#foo") << QStringLiteral("#bar"));
    QCOMPARE(parser.channels(), QStringList() << "#foo" << "#bar");
    QCOMPARE(channelSpy.count(), 1);

    parser.setChannels(QStringList());
    QCOMPARE(parser.channels(), QStringList());
    QCOMPARE(channelSpy.count(), 2);
    QCOMPARE(channelSpy.last().at(0).toStringList(), QStringList());
}

void tst_IrcCommandParser::testCommands()
{
    IrcCommandParser parser;

    QSignalSpy commandSpy(&parser, SIGNAL(commandsChanged(QStringList)));
    QVERIFY(commandSpy.isValid());

    parser.addCommand(IrcCommand::Join, QStringLiteral("JOIN <#channel> (<key>)"));
    parser.addCommand(IrcCommand::Part, QStringLiteral("PART (<#channel>) (<message...>)"));
    parser.addCommand(IrcCommand::Kick, QStringLiteral("KICK (<#channel>) <nick> (<reason...>)"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ME [target] <message...>"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ACTION <target> <message...>"));

    QCOMPARE(parser.commands().count(), 5);
    QCOMPARE(parser.commands(), QStringList() << "ACTION" << "JOIN" << "KICK" << "ME" << "PART");

    QCOMPARE(commandSpy.count(), 5);
    QCOMPARE(commandSpy.at(0).at(0).toStringList(), QStringList() << "JOIN");
    QCOMPARE(commandSpy.at(1).at(0).toStringList(), QStringList() << "JOIN" << "PART");
    QCOMPARE(commandSpy.at(2).at(0).toStringList(), QStringList() << "JOIN" << "KICK" << "PART");
    QCOMPARE(commandSpy.at(3).at(0).toStringList(), QStringList() << "JOIN" << "KICK" << "ME" << "PART");
    QCOMPARE(commandSpy.at(4).at(0).toStringList(), QStringList() << "ACTION" << "JOIN" << "KICK" << "ME" << "PART");
}

void tst_IrcCommandParser::testClear()
{
    IrcCommandParser parser;
    parser.addCommand(IrcCommand::Join, QStringLiteral("JOIN <#channel> (<key>)"));
    parser.addCommand(IrcCommand::Part, QStringLiteral("PART (<#channel>) (<message...>)"));
    parser.addCommand(IrcCommand::Kick, QStringLiteral("KICK (<#channel>) <nick> (<reason...>)"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ME [target] <message...>"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ACTION <target> <message...>"));
    QCOMPARE(parser.commands().count(), 5);

    QSignalSpy commandSpy(&parser, SIGNAL(commandsChanged(QStringList)));
    QVERIFY(commandSpy.isValid());

    parser.clear();
    QVERIFY(parser.commands().isEmpty());
    QCOMPARE(commandSpy.count(), 1);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList());

    parser.clear();
    QVERIFY(parser.commands().isEmpty());
    QCOMPARE(commandSpy.count(), 1);
}

void tst_IrcCommandParser::testReset()
{
    IrcCommandParser parser;

    QSignalSpy targetSpy(&parser, SIGNAL(targetChanged(QString)));
    QVERIFY(targetSpy.isValid());

    QSignalSpy channelSpy(&parser, SIGNAL(channelsChanged(QStringList)));
    QVERIFY(channelSpy.isValid());

    parser.setTarget(QStringLiteral("#tgt"));
    QCOMPARE(targetSpy.count(), 1);
    QCOMPARE(targetSpy.last().at(0).toString(), QString("#tgt"));

    parser.setChannels(QStringList() << QStringLiteral("#foo") << QStringLiteral("#bar"));
    QCOMPARE(channelSpy.count(), 1);
    QCOMPARE(channelSpy.last().at(0).toStringList(), QStringList() << "#foo" << "#bar");

    parser.reset();

    QCOMPARE(targetSpy.count(), 2);
    QCOMPARE(targetSpy.last().at(0).toString(), QString());

    QCOMPARE(channelSpy.count(), 2);
    QCOMPARE(channelSpy.last().at(0).toStringList(), QStringList());

    parser.reset();
    QCOMPARE(targetSpy.count(), 2);
    QCOMPARE(channelSpy.count(), 2);
}

void tst_IrcCommandParser::testAddRemove()
{
    IrcCommandParser parser;
    QVERIFY(parser.commands().isEmpty());

    QSignalSpy commandSpy(&parser, SIGNAL(commandsChanged(QStringList)));
    QVERIFY(commandSpy.isValid());

    parser.addCommand(IrcCommand::Join, QStringLiteral("join <#channel> (<key>)"));
    QCOMPARE(parser.commands(), QStringList() << "JOIN");
    QCOMPARE(commandSpy.count(), 1);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList() << "JOIN");

    parser.addCommand(IrcCommand::Join, QStringLiteral("join <overload>"));
    QCOMPARE(parser.commands(), QStringList() << "JOIN");
    QCOMPARE(commandSpy.count(), 1);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList() << "JOIN");

    parser.addCommand(IrcCommand::Part, QStringLiteral("Part (<#channel>) (<message...>)"));
    QCOMPARE(parser.commands(), QStringList() << "JOIN" << "PART");
    QCOMPARE(commandSpy.count(), 2);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList() << "JOIN" << "PART");

    parser.addCommand(IrcCommand::Part, QStringLiteral("PART <overload>"));
    QCOMPARE(parser.commands(), QStringList() << "JOIN" << "PART");
    QCOMPARE(commandSpy.count(), 2);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList() << "JOIN" << "PART");

    parser.removeCommand(IrcCommand::Join);
    QCOMPARE(parser.commands(), QStringList() << "PART");
    QCOMPARE(commandSpy.count(), 3);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList() << "PART");

    parser.removeCommand(IrcCommand::Part, QStringLiteral("PART <overload>"));
    QCOMPARE(parser.commands(), QStringList() << "PART");
    QCOMPARE(commandSpy.count(), 3);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList() << "PART");

    parser.removeCommand(IrcCommand::Part, QStringLiteral("Part (<#channel>) (<message...>)"));
    QCOMPARE(parser.commands(), QStringList());
    QCOMPARE(commandSpy.count(), 4);
    QCOMPARE(commandSpy.last().at(0).toStringList(), QStringList());
    QVERIFY(parser.commands().isEmpty());
}

void tst_IrcCommandParser::testSyntax_data()
{
    QTest::addColumn<QString>("command");
    QTest::addColumn<QString>("syntax");
    QTest::addColumn<uint>("details");
    QTest::addColumn<QString>("expected");

    QTest::newRow("full")
            << QStringLiteral("foo")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::Full)
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)");

    QTest::newRow("no target")
            << QStringLiteral("fOO")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::NoTarget)
            << QStringLiteral("FOO <#chan> (<arg>) (<rest...>)");

    QTest::newRow("no ellipsis")
            << QStringLiteral("fOO")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::NoEllipsis)
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest>)");

    QTest::newRow("no prefix")
            << QStringLiteral("fOO")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::NoPrefix)
            << QStringLiteral("FOO [param] <chan> (<arg>) (<rest...>)");

    QTest::newRow("no parentheses")
            << QStringLiteral("Foo")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::NoParentheses)
            << QStringLiteral("FOO [param] <#chan> <arg> <rest...>");

    QTest::newRow("no brackets")
            << QStringLiteral("FOO")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::NoBrackets)
            << QStringLiteral("FOO param <#chan> (<arg>) (<rest...>)");

    QTest::newRow("no angles")
            << QStringLiteral("FOO")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::NoAngles)
            << QStringLiteral("FOO [param] #chan (arg) (rest...)");

    QTest::newRow("visual")
            << QStringLiteral("FOO")
            << QStringLiteral("FOO [param] <#chan> (<arg>) (<rest...>)")
            << uint(IrcCommandParser::Visual)
            << QStringLiteral("FOO <chan> (<arg>) (<rest>)");
}

void tst_IrcCommandParser::testSyntax()
{
    QFETCH(QString, command);
    QFETCH(QString, syntax);
    QFETCH(uint, details);
    QFETCH(QString, expected);

    IrcCommandParser parser;
    parser.addCommand(IrcCommand::Custom, syntax);
    QString actual = parser.syntax(command, IrcCommandParser::Details(details));
    QCOMPARE(actual, expected);
}

void tst_IrcCommandParser::testTolerancy()
{
    IrcCommandParser parser;
    parser.setTriggers(QStringList("/"));
    QVERIFY(!parser.isTolerant());

    IrcCommand* cmd = parser.parse(QStringLiteral("/NS help"));
    QVERIFY(!cmd);

    QSignalSpy tolerancySpy(&parser, SIGNAL(tolerancyChanged(bool)));
    QVERIFY(tolerancySpy.isValid());

    parser.setTolerant(true);
    QVERIFY(parser.isTolerant());
    QCOMPARE(tolerancySpy.count(), 1);
    QCOMPARE(tolerancySpy.last().at(0).toBool(), true);

    parser.setTolerant(true);
    QVERIFY(parser.isTolerant());
    QCOMPARE(tolerancySpy.count(), 1);

    cmd = parser.parse(QStringLiteral("/NS help"));
    QVERIFY(cmd);
    QCOMPARE(cmd->type(), IrcCommand::Quote);
    QCOMPARE(cmd->toString(), QString("NS help"));

    parser.setTolerant(false);
    QVERIFY(!parser.isTolerant());
    QCOMPARE(tolerancySpy.count(), 2);
    QCOMPARE(tolerancySpy.last().at(0).toBool(), false);
}

void tst_IrcCommandParser::testCustom()
{
    IrcCommandParser parser;
    parser.setTriggers(QStringList("/"));

    parser.addCommand(IrcCommand::Custom, QStringLiteral("Hello <a> <b> <c>"));
    QCOMPARE(parser.commands(), QStringList() << "HELLO");
    QCOMPARE(parser.syntax("HELLO"), QString("HELLO <a> <b> <c>"));

    QVERIFY(!parser.parse("/hello"));
    QVERIFY(!parser.parse("/hello foo"));
    QVERIFY(!parser.parse("/hello foo bar"));
    QVERIFY(!parser.parse("/hello foo bar foo baz"));

    IrcCommand* cmd = parser.parse(QStringLiteral("/hello foo bar baz"));
    QVERIFY(cmd);
    QCOMPARE(cmd->type(), IrcCommand::Custom);
    QCOMPARE(cmd->parameters(), QStringList() << "HELLO" << "foo" << "bar" << "baz");
    delete cmd;
}

void tst_IrcCommandParser::testWhitespace()
{
    IrcCommandParser parser;
    parser.setTriggers(QStringList("/"));
    parser.addCommand(IrcCommand::Custom, QStringLiteral("TEST <arg...>"));

    IrcCommand* cmd = parser.parse(QStringLiteral("/test foo  bar  baz"));
    QVERIFY(cmd);
    QCOMPARE(cmd->type(), IrcCommand::Custom);
    QCOMPARE(cmd->parameters(), QStringList() << "TEST" << "foo  bar  baz");
    delete cmd;
}

QTEST_MAIN(tst_IrcCommandParser)

#include "tst_irccommandparser.moc"
