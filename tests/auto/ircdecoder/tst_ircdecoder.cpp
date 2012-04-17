/*
 * Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "ircdecoder_p.h"
#include <QtTest/QtTest>
#include <QtCore/QTextCodec>

class tst_IrcDecoder : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testEncoding_data();
    void testEncoding();
};

void tst_IrcDecoder::testDefaults()
{
    IrcDecoder decoder;
    QVERIFY(decoder.encoding().isNull());
}

void tst_IrcDecoder::testEncoding_data()
{
    QTest::addColumn<QByteArray>("encoding");

    QTest::newRow("null") << QByteArray();
    QTest::newRow("empty") << QByteArray("");
    QTest::newRow("space") << QByteArray(" ");
    QTest::newRow("invalid") << QByteArray("invalid");
    foreach (const QByteArray& codec, QTextCodec::availableCodecs())
        QTest::newRow(codec) << codec;
}

void tst_IrcDecoder::testEncoding()
{
    QFETCH(QByteArray, encoding);

    IrcDecoder decoder;
    decoder.setEncoding(encoding);
    QCOMPARE(decoder.encoding(), encoding);
}

QTEST_MAIN(tst_IrcDecoder)

#include "tst_ircdecoder.moc"
