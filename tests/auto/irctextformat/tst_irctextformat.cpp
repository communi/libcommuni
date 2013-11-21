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
#include "ircpalette.h"
#include <QtTest/QtTest>

class tst_IrcTextFormat : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
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
    QVERIFY(format.palette());
    QVERIFY(!format.urlPattern().isEmpty());
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
    QVERIFY(format.palette());
    IrcPalette* palette = format.palette();
    for (int i = Irc::White; i <= Irc::LightGray; ++i) {
        QString color = palette->colorName(i);
        QTest::newRow(color.toUtf8()) << QString("\x03%1%2\x0f").arg(i).arg(color) << color;
    }

    QTest::newRow("dummy \\x03") << "foo\x03 \02bold\x0f bar\x03" << "foo bold bar";
    QTest::newRow("extra \\x0f") << "foo\x0f \02bold\x0f bar\x0f" << "foo bold bar";
    QTest::newRow("background") << QString("foo \x03%1,%1red\x0f on \x03%1,%1red\x03 bar").arg(Irc::Red) << "foo red on red bar";
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

    QTest::newRow("bold") << "foo \02bold\x0f and \02bold\02 bar" << "foo <span style='font-weight: bold'>bold</span> and <span style='font-weight: bold'>bold</span> bar";
    QTest::newRow("strike-through") << "foo \x13strike\x0f and \x13through\x13 bar" << "foo <span style='text-decoration: line-through'>strike</span> and <span style='text-decoration: line-through'>through</span> bar";
    QTest::newRow("underline1") << "foo \x15under\x0f and \x15line\x15 bar" << "foo <span style='text-decoration: underline'>under</span> and <span style='text-decoration: underline'>line</span> bar";
    //TODO: QTest::newRow("inverse") << "\x16inverse\x0f" << "inverse";
    QTest::newRow("italic") << "foo \x1ditalic\x0f and \x1ditalic\x1d bar" << "foo <span style='font-style: italic'>italic</span> and <span style='font-style: italic'>italic</span> bar";
    QTest::newRow("underline2") << "foo \x1funder\x0f and \x1fline\x1f bar" << "foo <span style='text-decoration: underline'>under</span> and <span style='text-decoration: underline'>line</span> bar";

    IrcTextFormat format;
    QVERIFY(format.palette());
    IrcPalette* palette = format.palette();
    for (int i = Irc::White; i <= Irc::LightGray; ++i) {
        QString color = palette->colorName(i);
        QTest::newRow(color.toUtf8()) << QString("foo \x03%1%2\x0f and \x03%1%2\x03 bar").arg(i).arg(color) << QString("foo <span style='color:%1'>%1</span> and <span style='color:%1'>%1</span> bar").arg(color);
    }

    QTest::newRow("extra \\x0f") << "foo\x0f \02bold\x0f bar\x0f" << "foo <span style='font-weight: bold'>bold</span> bar";
    QTest::newRow("background") << QString("foo \x03%1,%1red\x0f on \x03%1,%1red\x03 bar").arg(Irc::Red) << "foo <span style='color:red;background-color:red'>red</span> on <span style='color:red;background-color:red'>red</span> bar";
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
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QString defaultPattern = IrcTextFormat().urlPattern();

    QTest::newRow("www.fi") << defaultPattern << "www.fi" << "<a href='http://www.fi'>www.fi</a>";
    QTest::newRow("ftp.funet.fi") << defaultPattern << "ftp.funet.fi" << "<a href='ftp://ftp.funet.fi'>ftp.funet.fi</a>";
    QTest::newRow("jpnurmi@gmail.com") << defaultPattern << "jpnurmi@gmail.com" << "<a href='mailto:jpnurmi@gmail.com'>jpnurmi@gmail.com</a>";
    QTest::newRow("quote") << defaultPattern << "http://en.wikipedia.org/wiki/Shamir's_Secret_Sharing" << "<a href='http://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing'>http://en.wikipedia.org/wiki/Shamir's_Secret_Sharing</a>";
    QTest::newRow("percent") << defaultPattern << "http://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing" << "<a href='http://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing'>http://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing</a>";
    QTest::newRow("parentheses") << defaultPattern << "http://en.wikipedia.org/wiki/Qt_(software)" << "<a href='http://en.wikipedia.org/wiki/Qt_%28software%29'>http://en.wikipedia.org/wiki/Qt_(software)</a>";
    QTest::newRow("hash & comma") << defaultPattern << "https://codereview.qt-project.org/#change,1" << "<a href='https://codereview.qt-project.org/#change%2C1'>https://codereview.qt-project.org/#change,1</a>";
    QTest::newRow("equal & question & ampersand") << defaultPattern << "https://www.google.no/imghp?hl=en&tab=wi" << "<a href='https://www.google.no/imghp?hl=en&tab=wi'>https://www.google.no/imghp?hl=en&tab=wi</a>";
    QTest::newRow("github commits") << defaultPattern << "https://github.com/communi/libcommuni/compare/ebf3c8ea47dc...19d66ddcb122" << "<a href='https://github.com/communi/libcommuni/compare/ebf3c8ea47dc...19d66ddcb122'>https://github.com/communi/libcommuni/compare/ebf3c8ea47dc...19d66ddcb122</a>";
    QTest::newRow("empty pattern") << QString() << "www.fi ftp.funet.fi jpnurmi@gmail.com" << "www.fi ftp.funet.fi jpnurmi@gmail.com";
}

void tst_IrcTextFormat::testUrls()
{
    QFETCH(QString, pattern);
    QFETCH(QString, input);
    QFETCH(QString, output);

    IrcTextFormat format;
    format.setUrlPattern(pattern);
    QCOMPARE(format.urlPattern(), pattern);
    QCOMPARE(format.toHtml(input), output);
}

QTEST_MAIN(tst_IrcTextFormat)

#include "tst_irctextformat.moc"
