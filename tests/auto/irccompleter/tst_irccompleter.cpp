/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irccompleter.h"
#include "ircbuffermodel.h"
#include "irccommandparser.h"
#include "ircbuffer.h"
#include <QtTest/QtTest>
#include "tst_ircclientserver.h"
#include "tst_ircdata.h"

class tst_IrcCompleter : public tst_IrcClientServer
{
    Q_OBJECT

private slots:
    void testBuffer();
    void testParser();

    void testCompletion_data();
    void testCompletion();
};

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

    completer.setBuffer(0);
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

    completer.setParser(0);
    QVERIFY(!completer.parser());
    QCOMPARE(spy.count(), 2);
    QVERIFY(!spy.last().at(0).value<IrcCommandParser*>());
}

Q_DECLARE_METATYPE(QList<int>)
void tst_IrcCompleter::testCompletion_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("cursor");
    QTest::addColumn<QStringList>("completions");
    QTest::addColumn<QList<int> >("positions");


    for (int i = -1; i <= 3; ++i)
        QTest::newRow("/j @ " + QByteArray::number(i)) << "/j" << i << QStringList("/JOIN ") << (QList<int>() << QString("/JOIN ").length());

    QTest::newRow("/q #2") << "/q" << QString("/q").length()
                       << (QStringList() << "/QUERY " << "/QUIT ")
                       << (QList<int>() << QString("/QUERY ").length() << QString("/QUIT ").length());

    QTest::newRow("/QUERY q") << "/quer q " << QString("/quer").length()
                       << (QStringList("/QUERY q "))
                       << (QList<int>() << QString("/QUERY ").length());

    QTest::newRow("/query q") << "/query q" << QString("/query q").length()
                       << (QStringList() << "/query qout " << "/query qtassistant ")
                       << (QList<int>() << QString("/query qout ").length() << QString("/query qtassistant ").length());

    QTest::newRow("buffers") << "q" << QString("q").length()
                       << (QStringList() << "qout " << "qtassistant ")
                       << (QList<int>() << QString("qout ").length() << QString("qtassistant ").length());

    QTest::newRow("repeat") << "qtassistant " << QString("qtassistant ").length()
                       << (QStringList() << "qtassistant " << "qtassistant ")
                       << (QList<int>() << QString("qtassistant ").length() << QString("qtassistant ").length());

    QStringList names;
    QList<int> positions;
    foreach (const QString& name, tst_IrcData::names()) {
        if (name.startsWith("je", Qt::CaseInsensitive)) {
            names += name + ": ";
            positions += name.length() + 2;
        }
    }
    QTest::newRow("je...:") << "je" << 1 << names << positions;

    names.clear();
    positions.clear();
    foreach (const QString& name, tst_IrcData::names()) {
        if (name.startsWith("sa", Qt::CaseInsensitive)) {
            names += "... " + name + " ";
            positions += QString("... ").length() + name.length() + QString(" ").length();
        }
    }
    QTest::newRow("... sa") << "... sa" << QString("... ").length() << names << positions;

    QTest::newRow("spaces") << "/quit  foo  qt  rest... " << QString("/quit  foo  qt ").length()
                       << QStringList("/quit  foo  qtassistant  rest... ")
                       << (QList<int>() << QString("/quit  foo  qtassistant ").length());
}

void tst_IrcCompleter::testCompletion()
{
    QFETCH(QString, text);
    QFETCH(int, cursor);
    QFETCH(QStringList, completions);
    QFETCH(QList<int>, positions);

    IrcBufferModel model(connection);

    connection->open();
    waitForOpened();
    waitForWritten(tst_IrcData::welcome());
    waitForWritten(tst_IrcData::join());

    model.add("qout");
    model.add("qtassistant");

    IrcCommandParser parser;
    parser.setTriggers(QStringList("/"));
    parser.addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser.addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
    parser.addCommand(IrcCommand::Kick, "KICK (<#channel>) <nick> (<reason...>)");
    parser.addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser.addCommand(IrcCommand::CtcpAction, "ACTION <target> <message...>");
    parser.addCommand(IrcCommand::Custom, "QUERY <user>");
    parser.addCommand(IrcCommand::Quit, "QUIT (<message...>)");

    IrcCompleter completer;
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

QTEST_MAIN(tst_IrcCompleter)

#include "tst_irccompleter.moc"
