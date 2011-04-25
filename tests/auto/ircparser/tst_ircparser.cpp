/*
 * Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "ircparser_p.h"
#include <QtTest/QtTest>
#include <QtCore/QTextCodec>

class tst_IrcParser : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testParse_data();
    void testParse();

    void testEncoding_data();
    void testEncoding();
};

void tst_IrcParser::testDefaults()
{
    IrcParser parser;
    QVERIFY(parser.prefix().isNull());
    QVERIFY(parser.command().isNull());
    QVERIFY(parser.params().isEmpty());
    QVERIFY(parser.encoding().isNull());
}

void tst_IrcParser::testParse_data()
{
    QTest::addColumn<bool>("result");
    QTest::addColumn<QByteArray>("line");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QString>("command");
    QTest::addColumn<QStringList>("params");

    QTest::newRow("null") << false << QByteArray() << QString() << QString() << QStringList();
    QTest::newRow("empty") << false << QByteArray("") << QString() << QString() << QStringList();
    QTest::newRow("space") << false << QByteArray(" ") << QString() << QString() << QStringList();

    QTest::newRow(":pfx") << false << QByteArray(":pfx") << QString("pfx") << QString() << QStringList();
    QTest::newRow("cmd") << true << QByteArray("cmd") << QString() << QString("cmd") << QStringList();
    QTest::newRow(":pfx cmd") << true << QByteArray(":pfx cmd") << QString("pfx") << QString("cmd") << QStringList();

    QTest::newRow(":pfx cmd 1 2 3") << true << QByteArray(":pfx cmd 1 2 3") << QString("pfx") << QString("cmd") << (QStringList() << "1" << "2" << "3");
    QTest::newRow(":pfx cmd :1 2 3") << true << QByteArray(":pfx cmd :1 2 3") << QString("pfx") << QString("cmd") << (QStringList() << "1 2 3");
    QTest::newRow(":pfx cmd 1 :2 3") << true << QByteArray(":pfx cmd 1 :2 3") << QString("pfx") << QString("cmd") << (QStringList() << "1" << "2 3");
    QTest::newRow(":pfx cmd 1 2 :3") << true << QByteArray(":pfx cmd 1 2 :3") << QString("pfx") << QString("cmd") << (QStringList() << "1" << "2" << "3");
}

void tst_IrcParser::testParse()
{
    QFETCH(bool, result);
    QFETCH(QByteArray, line);
    QFETCH(QString, prefix);
    QFETCH(QString, command);
    QFETCH(QStringList, params);

    IrcParser parser;
    QCOMPARE(parser.parse(line), result);
    QCOMPARE(parser.prefix(), prefix);
    QCOMPARE(parser.command(), command);
    QCOMPARE(parser.params(), params);
}

void tst_IrcParser::testEncoding_data()
{
    QTest::addColumn<QByteArray>("encoding");

    QTest::newRow("null") << QByteArray();
    QTest::newRow("empty") << QByteArray("");
    QTest::newRow("space") << QByteArray(" ");
    QTest::newRow("invalid") << QByteArray("invalid");
    foreach (const QByteArray& codec, QTextCodec::availableCodecs())
        QTest::newRow(codec) << codec;
}

void tst_IrcParser::testEncoding()
{
    QFETCH(QByteArray, encoding);

    IrcParser parser;
    parser.setEncoding(encoding);
    QCOMPARE(parser.encoding(), encoding);
}

QTEST_MAIN(tst_IrcParser)

#include "tst_ircparser.moc"
