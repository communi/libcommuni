/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircchannel.h"
#include <QtTest/QtTest>

class tst_IrcChannel : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSignals();
};

void tst_IrcChannel::testDefaults()
{
    IrcChannel channel;
    QVERIFY(channel.title().isEmpty());
    QVERIFY(channel.name().isEmpty());
    QVERIFY(channel.prefix().isEmpty());
    QVERIFY(channel.isChannel());
    QVERIFY(channel.toChannel());
    QVERIFY(!channel.connection());
    QVERIFY(!channel.network());
    QVERIFY(!channel.model());
    QVERIFY(!channel.isActive());
    QVERIFY(!channel.isSticky());
    QVERIFY(!channel.isPersistent());
    QVERIFY(channel.mode().isEmpty());
    QVERIFY(channel.topic().isEmpty());
}

void tst_IrcChannel::testSignals()
{
    IrcChannel channel;
    QSignalSpy modeSpy(&channel, SIGNAL(modeChanged(QString)));
    QSignalSpy topicSpy(&channel, SIGNAL(topicChanged(QString)));
    QVERIFY(modeSpy.isValid());
    QVERIFY(topicSpy.isValid());
}

QTEST_MAIN(tst_IrcChannel)

#include "tst_ircchannel.moc"
