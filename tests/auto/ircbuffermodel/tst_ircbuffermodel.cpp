/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircbuffermodel.h"
#include <QtTest/QtTest>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class tst_IrcBufferModel : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
};

void tst_IrcBufferModel::testDefaults()
{
    IrcBufferModel model;
    QCOMPARE(model.count(), 0);
    QVERIFY(!model.dynamicSort());
    QVERIFY(model.channels().isEmpty());
    QCOMPARE(model.displayRole(), Irc::TitleRole);
    QVERIFY(model.buffers().isEmpty());
    QVERIFY(!model.connection());
    QVERIFY(!model.network());
    QVERIFY(model.bufferPrototype());
    QVERIFY(model.channelPrototype());
}

QTEST_MAIN(tst_IrcBufferModel)

#include "tst_ircbuffermodel.moc"
