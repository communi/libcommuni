/*
 * Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
 *
 * This test is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 */

#include "messageformatter.h"
#include <QtTest/QtTest>

static const QString MSG_32_5("Vestibulum eu libero eget metus.");
static const QString MSG_64_9("Phasellus enim dui, sodales sed tincidunt quis, ultricies metus.");
static const QString MSG_128_19("Ut porttitor volutpat tristique. Aenean semper ligula eget nulla condimentum tempor in quis felis. Sed sem diam, tincidunt amet.");
static const QString MSG_256_37("Vestibulum quis lorem velit, a varius augue. Suspendisse risus augue, ultricies at convallis in, elementum in velit. Fusce fermentum congue augue sit amet dapibus. Fusce ultrices urna ut tortor laoreet a aliquet elit lobortis. Suspendisse volutpat posuere.");
static const QString MSG_512_75("Nam leo risus, accumsan a sagittis eget, posuere eu velit. Morbi mattis auctor risus, vel consequat massa pulvinar nec. Proin aliquam convallis elit nec egestas. Pellentesque accumsan placerat augue, id volutpat nibh dictum vel. Aenean venenatis varius feugiat. Nullam molestie, ipsum id dignissim vulputate, eros urna vestibulum massa, in vehicula lacus nisi vitae risus. Ut nunc nunc, venenatis a mattis auctor, dictum et sem. Nulla posuere libero ut tortor elementum egestas. Aliquam egestas suscipit posuere.");

class TestMessageFormatter : public MessageFormatter
{
	friend class tst_MessageFormatter;
};

class tst_MessageFormatter : public QObject
{
    Q_OBJECT

private slots:
	void testFormatHtml_data();
    void testFormatHtml();
	
private:
	TestMessageFormatter formatter;
};

void tst_MessageFormatter::testFormatHtml_data()
{
    QTest::addColumn<QString>("message");
    // TODO: QTest::addColumn<QStringList>("users");

    QTest::newRow("empty") << QString();
	QTest::newRow("32 chars / 5 words / 0 users") << MSG_32_5;
	QTest::newRow("64 chars / 9 words / 0 users") << MSG_64_9;
	QTest::newRow("128 chars / 19 words / 0 users") << MSG_128_19;
	QTest::newRow("256 chars / 37 words / 0 users") << MSG_256_37;
	QTest::newRow("512 chars / 75 words / 0 users") << MSG_512_75;
}

void tst_MessageFormatter::testFormatHtml()
{
	QFETCH(QString, message);
	// TODO: QFETCH(QStringList, users);

	// TODO:
	// TestUserModel model;
	// model.addUsers(users);
	QBENCHMARK {
        formatter.formatHtml(message);
    }
}

QTEST_MAIN(tst_MessageFormatter)

#include "tst_messageformatter.moc"
