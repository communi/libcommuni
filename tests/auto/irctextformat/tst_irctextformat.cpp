/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irc.h"
#include "irctextformat.h"
#include <QtTest/QtTest>

class tst_IrcTextFormat : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testColorName();
    void testPlainText_data();
    void testPlainText();
    void testHtml_data();
    void testHtml();
    void testUrls_data();
    void testUrls();
};

void tst_IrcTextFormat::testDefaults()
{
    IrcTextFormat format;
    QVERIFY(!format.urlPattern().isEmpty());
    for (int i = Irc::White; i <= Irc::LightGray; ++i)
        QVERIFY(!format.colorName(i).isEmpty());
    QCOMPARE(format.colorName(-1, "fallback"), QString("fallback"));
}

void tst_IrcTextFormat::testColorName()
{
    IrcTextFormat format;
    for (int i = -1; i <= 123; ++i) {
        format.setColorName(i, QString::number(i));
        QCOMPARE(format.colorName(i), QString::number(i));
    }
}

void tst_IrcTextFormat::testPlainText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("bold") << "\02bold\x0f" << "bold";
    QTest::newRow("strike-through") << "\x13strike-through\x0f" << "strike-through";
    QTest::newRow("underline") << "\x15underline\x0f" << "underline";
    QTest::newRow("inverse") << "\x16inverse\x0f" << "inverse";
    QTest::newRow("italic") << "\x1ditalic\x0f" << "italic";
    QTest::newRow("underline") << "\x1funderline\x0f" << "underline";

    IrcTextFormat format;
    for (int i = Irc::White; i <= Irc::LightGray; ++i) {
        QString color = format.colorName(i);
        QTest::newRow(color.toUtf8()) << QString("\x03%1%2\x0f").arg(i).arg(color) << color;
    }
}

void tst_IrcTextFormat::testPlainText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    IrcTextFormat format;
    QCOMPARE(format.toPlainText(input), output);
}

void tst_IrcTextFormat::testHtml_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("bold") << "\02bold\x0f" << "<span style='font-weight: bold'>bold</span>";
    QTest::newRow("strike-through") << "\x13strike-through\x0f" << "<span style='text-decoration: line-through'>strike-through</span>";
    QTest::newRow("underline") << "\x15underline\x0f" << "<span style='text-decoration: underline'>underline</span>";
    //TODO: QTest::newRow("inverse") << "\x16inverse\x0f" << "inverse";
    QTest::newRow("italic") << "\x1ditalic\x0f" << "<span style='font-style: italic'>italic</span>";
    QTest::newRow("underline") << "\x1funderline\x0f" << "<span style='text-decoration: underline'>underline</span>";

    IrcTextFormat format;
    for (int i = Irc::White; i <= Irc::LightGray; ++i) {
        QString color = format.colorName(i);
        QTest::newRow(color.toUtf8()) << QString("\x03%1%2\x0f").arg(i).arg(color) << QString("<span style='color:%1'>%1</span>").arg(color);
    }
}

void tst_IrcTextFormat::testHtml()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    IrcTextFormat format;
    QCOMPARE(format.toHtml(input), output);
}

void tst_IrcTextFormat::testUrls_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("www.fi") << "www.fi" << "<a href='http://www.fi'>www.fi</a>";
    QTest::newRow("ftp.funet.fi") << "ftp.funet.fi" << "<a href='ftp://ftp.funet.fi'>ftp.funet.fi</a>";
    QTest::newRow("jpnurmi@gmail.com") << "jpnurmi@gmail.com" << "<a href='mailto:jpnurmi@gmail.com'>jpnurmi@gmail.com</a>";
    QTest::newRow("github commits") << "https://github.com/communi/libcommuni/compare/ebf3c8ea47dc...19d66ddcb122" << "<a href='https://github.com/communi/libcommuni/compare/ebf3c8ea47dc...19d66ddcb122'>https://github.com/communi/libcommuni/compare/ebf3c8ea47dc...19d66ddcb122</a>";
}

void tst_IrcTextFormat::testUrls()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    IrcTextFormat format;
    QCOMPARE(format.toHtml(input), output);
}


QTEST_MAIN(tst_IrcTextFormat)

#include "tst_irctextformat.moc"
