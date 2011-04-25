/*
 * Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "ircprefix.h"
#include <QtTest/QtTest>

class tst_IrcPrefix : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testPrefix_data();
    void testPrefix();
};

void tst_IrcPrefix::testDefaults()
{
    IrcPrefix prefix;
    QVERIFY(prefix.prefix().isNull());
    QVERIFY(prefix.nick().isNull());
    QVERIFY(prefix.user().isNull());
    QVERIFY(prefix.host().isNull());
}

void tst_IrcPrefix::testPrefix_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QString>("nick");
    QTest::addColumn<QString>("user");
    QTest::addColumn<QString>("host");

    QTest::newRow("null") << false << QString() << QString() << QString() << QString();
    QTest::newRow("empty") << false << QString("") << QString("") << QString("") << QString("");
    QTest::newRow("trimmed") << true << QString(" n!u@h ") << QString("n") << QString("u") << QString("h");
    QTest::newRow("n!u@h") << true << QString("n!u@h") << QString("n") << QString("u") << QString("h");

    QTest::newRow("n@h") << false << QString("n@h") << QString() << QString() << QString();
    QTest::newRow("n!u") << false << QString("n!u") << QString() << QString() << QString();
    QTest::newRow("!u@h") << false << QString("!u@h") << QString() << QString() << QString();
    QTest::newRow("n!@h") << false << QString("n!@h") << QString() << QString() << QString();
    QTest::newRow("n!u@") << false << QString("n!u@") << QString() << QString() << QString();

    QTest::newRow("n !u@h") << false << QString("n !u@h") << QString() << QString() << QString();
    QTest::newRow("n! u@h") << false << QString("n! u@h") << QString() << QString() << QString();
    QTest::newRow("n!u @h") << false << QString("n!u @h") << QString() << QString() << QString();
    QTest::newRow("n!u@ h") << false << QString("n!u@ h") << QString() << QString() << QString();
    QTest::newRow("n ! u @ h") << false << QString("n ! u @ h") << QString() << QString() << QString();
}

void tst_IrcPrefix::testPrefix()
{
    QFETCH(bool, valid);
    QFETCH(QString, prefix);
    QFETCH(QString, nick);
    QFETCH(QString, user);
    QFETCH(QString, host);

    IrcPrefix pfx(prefix);
    QCOMPARE(pfx.isValid(), valid);
    QCOMPARE(pfx.nick(), nick);
    QCOMPARE(pfx.user(), user);
    QCOMPARE(pfx.host(), host);
}

QTEST_MAIN(tst_IrcPrefix)

#include "tst_ircprefix.moc"
