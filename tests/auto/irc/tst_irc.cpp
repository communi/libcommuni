/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irc.h"
#include <QtTest/QtTest>

class tst_Irc : public QObject
{
    Q_OBJECT

private slots:
    void testPrefix_data();
    void testPrefix();
};

void tst_Irc::testPrefix_data()
{
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QString>("expectedNick");
    QTest::addColumn<QString>("expectedIdent");
    QTest::addColumn<QString>("expectedHost");

    QTest::newRow("null") << false << QString() << QString() << QString() << QString();
    QTest::newRow("empty") << false << QString("") << QString("") << QString("") << QString("");
    QTest::newRow("trimmed") << true << QString(" n!u@h ") << QString("n") << QString("u") << QString("h");
    QTest::newRow("n!u@h") << true << QString("n!u@h") << QString("n") << QString("u") << QString("h");

    QTest::newRow("n@h") << true << QString("n@h") << QString("n") << QString() << QString("h");
    QTest::newRow("n!u") << true << QString("n!u") << QString("n") << QString("u") << QString();
    QTest::newRow("!u@h") << false << QString("!u@h") << QString() << QString() << QString();
    QTest::newRow("n!@h") << false << QString("n!@h") << QString() << QString() << QString();
    QTest::newRow("n!u@") << false << QString("n!u@") << QString() << QString() << QString();

    QTest::newRow("n !u@h") << false << QString("n !u@h") << QString() << QString() << QString();
    QTest::newRow("n! u@h") << false << QString("n! u@h") << QString() << QString() << QString();
    QTest::newRow("n!u @h") << false << QString("n!u @h") << QString() << QString() << QString();
    QTest::newRow("n!u@ h") << false << QString("n!u@ h") << QString() << QString() << QString();
    QTest::newRow("n ! u @ h") << false << QString("n ! u @ h") << QString() << QString() << QString();
}

void tst_Irc::testPrefix()
{
    QFETCH(bool, valid);
    QFETCH(QString, prefix);
    QFETCH(QString, expectedNick);
    QFETCH(QString, expectedIdent);
    QFETCH(QString, expectedHost);

    QString actualNick = Irc::nickFromPrefix(prefix);
    QString actualIdent = Irc::identFromPrefix(prefix);
    QString actualHost = Irc::hostFromPrefix(prefix);

    Q_UNUSED(valid);
    QCOMPARE(expectedNick, actualNick);
    QCOMPARE(expectedIdent, actualIdent);
    QCOMPARE(expectedHost, actualHost);
}

QTEST_MAIN(tst_Irc)

#include "tst_irc.moc"
