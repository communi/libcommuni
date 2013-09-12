/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irclagtimer.h"
#include "ircconnection.h"
#include <QtTest/QtTest>

class tst_IrcLagTimer : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testInterval();
    void testConnection();
};

void tst_IrcLagTimer::testDefaults()
{
    IrcLagTimer timer;
    QCOMPARE(timer.lag(), -1);
    QVERIFY(!timer.connection());
    QCOMPARE(timer.interval(), 60);
}

void tst_IrcLagTimer::testInterval()
{
    IrcLagTimer timer;
    timer.setInterval(INT_MIN);
    QCOMPARE(timer.interval(), INT_MIN);
    timer.setInterval(0);
    QCOMPARE(timer.interval(), 0);
    timer.setInterval(INT_MAX);
    QCOMPARE(timer.interval(), INT_MAX);
}

void tst_IrcLagTimer::testConnection()
{
    IrcLagTimer timer;
    IrcConnection* connection = new IrcConnection(&timer);
    timer.setConnection(connection);
    QCOMPARE(timer.connection(), connection);
    timer.setConnection(0);
    QVERIFY(!timer.connection());
}

QTEST_MAIN(tst_IrcLagTimer)

#include "tst_irclagtimer.moc"
