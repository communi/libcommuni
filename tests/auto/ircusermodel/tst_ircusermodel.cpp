/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircusermodel.h"
#include <QtTest/QtTest>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class tst_IrcUserModel : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
};

void tst_IrcUserModel::testDefaults()
{
    IrcUserModel model;
    QCOMPARE(model.count(), 0);
    QVERIFY(model.names().isEmpty());
    QVERIFY(model.users().isEmpty());
    QCOMPARE(model.displayRole(), Irc::TitleRole);
    QVERIFY(!model.channel());
    QVERIFY(!model.dynamicSort());
}

QTEST_MAIN(tst_IrcUserModel)

#include "tst_ircusermodel.moc"
