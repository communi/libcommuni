/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "ircmessagedata_p.h"
#include <QtTest/QtTest>

class tst_IrcMessageData : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testFromData_data();
    void testFromData();
};

void tst_IrcMessageData::testDefaults()
{
    IrcMessageData data;
    QVERIFY(!data.valid);
    QVERIFY(data.data.isNull());
    QVERIFY(data.prefix.isNull());
    QVERIFY(data.command.isNull());
    QVERIFY(data.params.isEmpty());
}

typedef QList<QByteArray> QByteArrayList;
Q_DECLARE_METATYPE(QByteArrayList)
void tst_IrcMessageData::testFromData_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QByteArray>("line");
    QTest::addColumn<QByteArray>("prefix");
    QTest::addColumn<QByteArray>("command");
    QTest::addColumn<QList<QByteArray> >("params");

    QTest::newRow("null") << false << QByteArray() << QByteArray() << QByteArray() << QByteArrayList();
    QTest::newRow("empty") << false << QByteArray("") << QByteArray() << QByteArray() << QByteArrayList();
    QTest::newRow("space") << false << QByteArray(" ") << QByteArray() << QByteArray() << QByteArrayList();

    QTest::newRow(":pfx") << false << QByteArray(":pfx") << QByteArray("pfx") << QByteArray() << QByteArrayList();
    QTest::newRow("cmd") << true << QByteArray("cmd") << QByteArray() << QByteArray("cmd") << QByteArrayList();
    QTest::newRow(":pfx cmd") << true << QByteArray(":pfx cmd") << QByteArray("pfx") << QByteArray("cmd") << QByteArrayList();

    QTest::newRow(":pfx cmd 1 2 3") << true << QByteArray(":pfx cmd 1 2 3") << QByteArray("pfx") << QByteArray("cmd") << (QByteArrayList() << "1" << "2" << "3");
    QTest::newRow(":pfx cmd :1 2 3") << true << QByteArray(":pfx cmd :1 2 3") << QByteArray("pfx") << QByteArray("cmd") << (QByteArrayList() << "1 2 3");
    QTest::newRow(":pfx cmd 1 :2 3") << true << QByteArray(":pfx cmd 1 :2 3") << QByteArray("pfx") << QByteArray("cmd") << (QByteArrayList() << "1" << "2 3");
    QTest::newRow(":pfx cmd 1 2 :3") << true << QByteArray(":pfx cmd 1 2 :3") << QByteArray("pfx") << QByteArray("cmd") << (QByteArrayList() << "1" << "2" << "3");
}

void tst_IrcMessageData::testFromData()
{
    QFETCH(bool, valid);
    QFETCH(QByteArray, line);
    QFETCH(QByteArray, prefix);
    QFETCH(QByteArray, command);
    QFETCH(QByteArrayList, params);

    IrcMessageData data = IrcMessageData::fromData(line);
    QCOMPARE(data.valid, valid);
    QCOMPARE(data.prefix, prefix);
    QCOMPARE(data.command, command);
    QCOMPARE(data.params, params);
}

QTEST_MAIN(tst_IrcMessageData)

#include "tst_ircmessagedata.moc"
