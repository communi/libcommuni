/*
 * Copyright (C) 2008-2020 The Communi Project
 *
 * This test is free, and not covered by the BSD license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irccompleter.h"
#include "ircbuffermodel.h"
#include "irccommandparser.h"
#include "ircchannel.h"
#include "ircbuffer.h"
#include <QtTest/QtTest>
#include "tst_ircclientserver.h"
#include "tst_ircdata.h"

class tst_IrcCompleter : public tst_IrcClientServer
{
    Q_OBJECT

private slots:
    void testSuffix();
    void testBuffer();
    void testParser();

    void testCompletion_data();
    void testCompletion();

    void testReset();
};

void tst_IrcCompleter::testSuffix()
{
    IrcCompleter completer;
    QCOMPARE(completer.suffix(), QString(":"));
    QCOMPARE(completer.property("suffix").toString(), QString(":"));

    QSignalSpy spy(&completer, SIGNAL(suffixChanged(QString)));
    QVERIFY(spy.isValid());

    completer.setSuffix(QStringLiteral(","));
    QCOMPARE(completer.suffix(), QString(","));
    QCOMPARE(completer.property("suffix").toString(), QString(","));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toString(), QString(","));
}

void tst_IrcCompleter::testBuffer()
{
    qRegisterMetaType<IrcBuffer*>("IrcBuffer*");

    IrcCompleter completer;
    QVERIFY(!completer.buffer());

    QSignalSpy spy(&completer, SIGNAL(bufferChanged(IrcBuffer*)));
    QVERIFY(spy.isValid());

    IrcBuffer* buffer = new IrcBuffer(&completer);
    completer.setBuffer(buffer);
    QCOMPARE(completer.buffer(), buffer);
    QCOMPARE(completer.property("buffer").value<IrcBuffer*>(), buffer);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).value<IrcBuffer*>(), buffer);

    completer.setBuffer(nullptr);
    QVERIFY(!completer.buffer());
    QCOMPARE(spy.count(), 2);
    QVERIFY(!spy.last().at(0).value<IrcBuffer*>());
}

void tst_IrcCompleter::testParser()
{
    qRegisterMetaType<IrcCommandParser*>("IrcCommandParser*");

    IrcCompleter completer;
    QVERIFY(!completer.parser());

    QSignalSpy spy(&completer, SIGNAL(parserChanged(IrcCommandParser*)));
    QVERIFY(spy.isValid());

    IrcCommandParser* parser = new IrcCommandParser(&completer);
    completer.setParser(parser);
    QCOMPARE(completer.parser(), parser);
    QCOMPARE(completer.property("parser").value<IrcCommandParser*>(), parser);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).value<IrcCommandParser*>(), parser);

    completer.setParser(nullptr);
    QVERIFY(!completer.parser());
    QCOMPARE(spy.count(), 2);
    QVERIFY(!spy.last().at(0).value<IrcCommandParser*>());
}

Q_DECLARE_METATYPE(QList<int>)
void tst_IrcCompleter::testCompletion_data()
{
    QTest::addColumn<QString>("suffix");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("cursor");
    QTest::addColumn<QStringList>("completions");
    QTest::addColumn<QList<int> >("positions");

    for (int i = -1; i <= 3; ++i)
        QTest::newRow("/j @ " + QByteArray::number(i)) << QString() << "/j" << i << QStringList("/JOIN ") << (QList<int>() << QStringLiteral("/JOIN ").length());

    QTest::newRow("/q #2") << QString() << "/q" << QStringLiteral("/q").length()
                       << (QStringList() << QStringLiteral("/QUERY ") << QStringLiteral("/QUIT "))
                       << (QList<int>() << QStringLiteral("/QUERY ").length() << QStringLiteral("/QUIT ").length());

    QTest::newRow("/QUERY q") << QString() << "/quer q " << QStringLiteral("/quer").length()
                       << (QStringList("/QUERY q "))
                       << (QList<int>() << QStringLiteral("/QUERY ").length());

    QTest::newRow("/query q") << QString() << "/query q" << QStringLiteral("/query q").length()
                       << (QStringList() << QStringLiteral("/query quackgyver ") << QStringLiteral("/query quelx "))
                       << (QList<int>() << QStringLiteral("/query quackgyver ").length() << QStringLiteral("/query quelx ").length());

    QTest::newRow("buffers") << QString() << "q" << QStringLiteral("q").length()
                       << (QStringList() << QStringLiteral("quackgyver ") << QStringLiteral("quelx "))
                       << (QList<int>() << QStringLiteral("quackgyver ").length() << QStringLiteral("quelx ").length());

    QTest::newRow("repeat") << QString() << "#libera " << QStringLiteral("#libera ").length()
                       << (QStringList() << QStringLiteral("#libera ") << QStringLiteral("#libera "))
                       << (QList<int>() << QStringLiteral("#libera ").length() << QStringLiteral("#libera ").length());

    QStringList names1;
    QStringList names2;
    QList<int> positions;
    foreach (const QString& name, tst_IrcData::names()) {
        if (name.startsWith(QLatin1String("je"), Qt::CaseInsensitive)) {
            names1 += name + ": ";
            names2 += name + ", ";
            positions += name.length() + 2;
        }
    }
    QTest::newRow("je...:") << ":" << "je" << 1 << names1 << positions;
    QTest::newRow("je...,") << "," << "je" << 1 << names2 << positions;

    names1.clear();
    names2.clear();
    positions.clear();
    foreach (const QString& name, tst_IrcData::names()) {
        if (name.startsWith(QLatin1String("sa"), Qt::CaseInsensitive)) {
            names1 += "... " + name + " ";
            positions += QStringLiteral("... ").length() + name.length() + QStringLiteral(" ").length();
        }
    }
    QTest::newRow("... sa") << QString() << "... sa" << QStringLiteral("... ").length() << names1 << positions;

    QTest::newRow("spaces") << QString() << "/quit  foo  #libe  rest... " << QStringLiteral("/quit  foo  #libe ").length()
                       << QStringList("/quit  foo  #libera  rest... ")
                       << (QList<int>() << QStringLiteral("/quit  foo  #libera ").length());
}

void tst_IrcCompleter::testCompletion()
{
    QFETCH(QString, suffix);
    QFETCH(QString, text);
    QFETCH(int, cursor);
    QFETCH(QStringList, completions);
    QFETCH(QList<int>, positions);

    IrcBufferModel model(connection);

    connection->open();
    waitForOpened();
    waitForWritten(tst_IrcData::welcome());
    waitForWritten(tst_IrcData::join());

    model.add(QStringLiteral("qout"));
    model.add(QStringLiteral("qtassistant"));

    IrcCommandParser parser;
    parser.setTriggers(QStringList("/"));
    parser.addCommand(IrcCommand::Join, QStringLiteral("JOIN <#channel> (<key>)"));
    parser.addCommand(IrcCommand::Part, QStringLiteral("PART (<#channel>) (<message...>)"));
    parser.addCommand(IrcCommand::Kick, QStringLiteral("KICK (<#channel>) <nick> (<reason...>)"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ME [target] <message...>"));
    parser.addCommand(IrcCommand::CtcpAction, QStringLiteral("ACTION <target> <message...>"));
    parser.addCommand(IrcCommand::Custom, QStringLiteral("QUERY <user>"));
    parser.addCommand(IrcCommand::Quit, QStringLiteral("QUIT (<message...>)"));

    IrcCompleter completer;
    completer.setSuffix(suffix);
    completer.setBuffer(model.get(0));
    completer.setParser(&parser);

    QSignalSpy spy(&completer, SIGNAL(completed(QString,int)));
    QVERIFY(spy.isValid());

    for (int i = 0; i < completions.count(); ++i) {
        completer.complete(text, cursor);
        QCOMPARE(spy.count(), i + 1);
        QCOMPARE(spy.last().at(0).toString(), completions.at(i));
        QCOMPARE(spy.last().at(1).toInt(), positions.at(i));
    }
}

void tst_IrcCompleter::testReset()
{
    IrcBufferModel model(connection);
    connection->open();
    waitForOpened();
    waitForWritten(tst_IrcData::welcome());
    waitForWritten(tst_IrcData::join());
    IrcChannel* channel = model.get(0)->toChannel();
    QVERIFY(channel);

    IrcCompleter completer;
    completer.setBuffer(channel);

    QSignalSpy spy(&completer, SIGNAL(completed(QString,int)));
    QVERIFY(spy.isValid());

    completer.complete(QStringLiteral("Guest"), 5);
    QCOMPARE(spy.count(), 1);
    QString guest1 = spy.last().at(0).toString();
    QVERIFY(guest1.startsWith("Guest"));

    completer.complete(QStringLiteral("Guest"), 5);
    QCOMPARE(spy.count(), 2);
    QString guest2 = spy.last().at(0).toString();
    QVERIFY(guest2.startsWith("Guest"));
    QVERIFY(guest2 != guest1);

    completer.reset();
    completer.complete(QStringLiteral("Guest"), 5);
    QCOMPARE(spy.count(), 3);
    QString guest3 = spy.last().at(0).toString();
    QCOMPARE(guest3, guest1);
}

QTEST_MAIN(tst_IrcCompleter)

#include "tst_irccompleter.moc"
