/*
 * Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "ircencoder_p.h"
#include <QtTest/QtTest>
#include <QtCore/QTextCodec>

class tst_IrcEncoder : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testEncoding_data();
    void testEncoding();
};

void tst_IrcEncoder::testDefaults()
{
    IrcEncoder encoder;
    QVERIFY(encoder.encoding().isNull());
}

void tst_IrcEncoder::testEncoding_data()
{
    QTest::addColumn<QByteArray>("encoding");

    QTest::newRow("null") << QByteArray();
    QTest::newRow("empty") << QByteArray("");
    QTest::newRow("space") << QByteArray(" ");
    QTest::newRow("invalid") << QByteArray("invalid");
    foreach (const QByteArray& codec, QTextCodec::availableCodecs())
        QTest::newRow(codec) << codec;
}

void tst_IrcEncoder::testEncoding()
{
    QFETCH(QByteArray, encoding);

    IrcEncoder encoder;
    encoder.setEncoding(encoding);
    QCOMPARE(encoder.encoding(), encoding);
}

QTEST_MAIN(tst_IrcEncoder)

#include "tst_ircencoder.moc"
