/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircuser.h"
#include <QtTest/QtTest>

class tst_IrcUser : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSignals();
};

void tst_IrcUser::testDefaults()
{
    IrcUser user;
    QVERIFY(user.name().isEmpty());
    QVERIFY(user.prefix().isEmpty());
    QVERIFY(user.mode().isEmpty());
    QVERIFY(!user.channel());
}

void tst_IrcUser::testSignals()
{
    IrcUser user;
    QSignalSpy nameSpy(&user, SIGNAL(nameChanged(QString)));
    QSignalSpy prefixSpy(&user, SIGNAL(prefixChanged(QString)));
    QSignalSpy modeSpy(&user, SIGNAL(modeChanged(QString)));
    QVERIFY(nameSpy.isValid());
    QVERIFY(prefixSpy.isValid());
    QVERIFY(modeSpy.isValid());
}

QTEST_MAIN(tst_IrcUser)

#include "tst_ircuser.moc"
