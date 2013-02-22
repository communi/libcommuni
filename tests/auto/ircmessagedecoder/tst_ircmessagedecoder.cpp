/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircmessagedecoder_p.h"
#include <QtTest/QtTest>
#include <QtCore/QTextCodec>

class tst_IrcMessageDecoder : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
};

void tst_IrcMessageDecoder::testDefaults()
{
    IrcMessageDecoder decoder;
    QCOMPARE(decoder.encoding(), QByteArray("ISO-8859-15"));
}

QTEST_MAIN(tst_IrcMessageDecoder)

#include "tst_ircmessagedecoder.moc"
