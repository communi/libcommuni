/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "irc.h"
#include "irccommand.h"
#include "ircconnection.h"
#include "ircmessage.h"
#include <QtTest/QtTest>
#include <QtCore/QTextCodec>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#ifndef QT_NO_OPENSSL
#include <QtNetwork/QSslSocket>
#endif

class tst_IrcConnection : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testDefaults();

    void testHost_data();
    void testHost();

    void testPort_data();
    void testPort();

    void testUserName_data();
    void testUserName();

    void testNickName_data();
    void testNickName();

    void testRealName_data();
    void testRealName();

    void testPassword_data();
    void testPassword();

    void testDisplayName_data();
    void testDisplayName();

    void testEncoding_data();
    void testEncoding();

    void testSocket_data();
    void testSocket();

    void testSecure();
    void testConnection();
    void testSendCommand();
    void testSendData();

private:
    QTcpServer server;
};

void tst_IrcConnection::initTestCase()
{
    server.listen();
}

void tst_IrcConnection::cleanupTestCase()
{
    server.close();
}

void tst_IrcConnection::testDefaults()
{
    IrcConnection connection;
    QVERIFY(connection.host().isNull());
    QCOMPARE(connection.port(), 6667);
    QVERIFY(connection.userName().isNull());
    QVERIFY(connection.nickName().isNull());
    QVERIFY(connection.realName().isNull());
    QVERIFY(connection.password().isNull());
    QVERIFY(connection.displayName().isNull());
    QCOMPARE(connection.encoding(), QByteArray("ISO-8859-15"));
    QCOMPARE(connection.status(), IrcConnection::Inactive);
    QVERIFY(!connection.isActive());
    QVERIFY(!connection.isConnected());
    QCOMPARE(connection.reconnectDelay(), 0);
    QVERIFY(connection.socket());
    QVERIFY(!connection.isSecure());
    QVERIFY(connection.saslMechanism().isNull());
    QVERIFY(!IrcConnection::supportedSaslMechanisms().isEmpty());
    QVERIFY(connection.network());
}

void tst_IrcConnection::testHost_data()
{
    QTest::addColumn<QString>("host");

    QTest::newRow("null") << QString();
    QTest::newRow("empty") << QString("");
    QTest::newRow("space") << QString(" ");
    QTest::newRow("invalid") << QString("invalid");
    QTest::newRow(qPrintable(server.serverAddress().toString())) << server.serverAddress().toString();
}

void tst_IrcConnection::testHost()
{
    QFETCH(QString, host);

    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(hostChanged(QString)));
    QVERIFY(spy.isValid());
    connection.setHost(host);
    QCOMPARE(connection.host(), host);
    QCOMPARE(spy.count(), !host.isEmpty() ? 1 : 0);
    if (!spy.isEmpty())
        QCOMPARE(spy.first().first().toString(), host);
}

void tst_IrcConnection::testPort_data()
{
    QTest::addColumn<int>("port");

    QTest::newRow("-1") << -1;
    QTest::newRow("0") << 0;
    QTest::newRow("6666") << 6666;
    QTest::newRow("6667") << 6667;
    QTest::newRow("6668") << 6668;
    QTest::newRow(qPrintable(QString::number(server.serverPort()))) << static_cast<int>(server.serverPort());
}

void tst_IrcConnection::testPort()
{
    QFETCH(int, port);

    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(portChanged(int)));
    QVERIFY(spy.isValid());
    connection.setPort(port);
    QCOMPARE(connection.port(), port);
    QCOMPARE(spy.count(), port != 6667 ? 1 : 0);
    if (!spy.isEmpty())
        QCOMPARE(spy.first().first().toInt(), port);
}

void tst_IrcConnection::testUserName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("result");

    QTest::newRow("null") << QString() << QString();
    QTest::newRow("empty") << QString("") << QString("");
    QTest::newRow("space") << QString(" ") << QString("");
    QTest::newRow("spaces") << QString(" foo bar ") << QString("foo");
}

void tst_IrcConnection::testUserName()
{
    QFETCH(QString, name);
    QFETCH(QString, result);

    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(userNameChanged(QString)));
    QVERIFY(spy.isValid());
    connection.setUserName(name);
    QCOMPARE(connection.userName(), result);
    QCOMPARE(spy.count(), !result.isEmpty() ? 1 : 0);
    if (!spy.isEmpty())
        QCOMPARE(spy.first().first().toString(), result);
}

void tst_IrcConnection::testNickName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("result");

    QTest::newRow("null") << QString() << QString();
    QTest::newRow("empty") << QString("") << QString("");
    QTest::newRow("space") << QString(" ") << QString("");
    QTest::newRow("spaces") << QString(" foo bar ") << QString("foo");
}

void tst_IrcConnection::testNickName()
{
    QFETCH(QString, name);
    QFETCH(QString, result);

    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(nickNameChanged(QString)));
    QVERIFY(spy.isValid());
    connection.setNickName(name);
    QCOMPARE(connection.nickName(), result);
    QCOMPARE(spy.count(), !result.isEmpty() ? 1 : 0);
    if (!spy.isEmpty())
        QCOMPARE(spy.first().first().toString(), result);
}

void tst_IrcConnection::testRealName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("result");

    QTest::newRow("null") << QString() << QString();
    QTest::newRow("empty") << QString("") << QString("");
    QTest::newRow("space") << QString(" ") << QString(" ");
    QTest::newRow("spaces") << QString(" foo bar ") << QString(" foo bar ");
}

void tst_IrcConnection::testRealName()
{
    QFETCH(QString, name);
    QFETCH(QString, result);

    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(realNameChanged(QString)));
    QVERIFY(spy.isValid());
    connection.setRealName(name);
    QCOMPARE(connection.realName(), result);
    QCOMPARE(spy.count(), !result.isEmpty() ? 1 : 0);
    if (!spy.isEmpty())
        QCOMPARE(spy.first().first().toString(), result);
}

void tst_IrcConnection::testPassword_data()
{
    QTest::addColumn<QString>("passwd");
    QTest::addColumn<QString>("result");

    QTest::newRow("null") << QString() << QString();
    QTest::newRow("empty") << QString("") << QString("");
    QTest::newRow("space") << QString(" ") << QString(" ");
    QTest::newRow("spaces") << QString(" foo bar ") << QString(" foo bar ");
}

void tst_IrcConnection::testPassword()
{
    QFETCH(QString, passwd);
    QFETCH(QString, result);

    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(passwordChanged(QString)));
    QVERIFY(spy.isValid());
    connection.setPassword(passwd);
    QCOMPARE(connection.password(), result);
    QCOMPARE(spy.count(), !result.isEmpty() ? 1 : 0);
    if (!spy.isEmpty())
        QCOMPARE(spy.first().first().toString(), result);
}

void tst_IrcConnection::testDisplayName_data()
{
    QTest::addColumn<QString>("host");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("result");

    QTest::newRow("null") << QString() << QString() << QString();
    QTest::newRow("empty") << QString() << QString("") << QString("");
    QTest::newRow("space") << QString() << QString(" ") << QString(" ");

    QTest::newRow("host") << QString("host") << QString() << QString("host");
    QTest::newRow("name") << QString() << QString("name") << QString("name");
    QTest::newRow("explicit") << QString("host") << QString("name") << QString("name");
}

void tst_IrcConnection::testDisplayName()
{
    QFETCH(QString, host);
    QFETCH(QString, name);
    QFETCH(QString, result);

    IrcConnection connection;
    connection.setHost(host);
    connection.setDisplayName(name);
    QCOMPARE(connection.displayName(), result);
}

void tst_IrcConnection::testEncoding_data()
{
    QTest::addColumn<QByteArray>("encoding");
    QTest::addColumn<QByteArray>("actual");
    QTest::addColumn<bool>("supported");

    QTest::newRow("null") << QByteArray() << QByteArray("ISO-8859-15") << false;
    QTest::newRow("empty") << QByteArray("") << QByteArray("ISO-8859-15") << false;
    QTest::newRow("space") << QByteArray(" ") << QByteArray("ISO-8859-15") << false;
    QTest::newRow("invalid") << QByteArray("invalid") << QByteArray("ISO-8859-15") << false;
    foreach (const QByteArray& codec, QTextCodec::availableCodecs())
        QTest::newRow(codec) << codec << codec << true;
}

void tst_IrcConnection::testEncoding()
{
    QFETCH(QByteArray, encoding);
    QFETCH(QByteArray, actual);
    QFETCH(bool, supported);

    if (!supported)
        QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setEncoding(): unsupported encoding \"" + encoding + "\" ");

    IrcConnection connection;
    connection.setEncoding(encoding);
    QCOMPARE(connection.encoding(), actual);
}

Q_DECLARE_METATYPE(QAbstractSocket*)
void tst_IrcConnection::testSocket_data()
{
    QTest::addColumn<QAbstractSocket*>("socket");

    QTest::newRow("null") << static_cast<QAbstractSocket*>(0);
    QTest::newRow("tcp") << static_cast<QAbstractSocket*>(new QTcpSocket(this));
#ifndef QT_NO_OPENSSL
    QTest::newRow("ssl") << static_cast<QAbstractSocket*>(new QSslSocket(this));
#endif
}

void tst_IrcConnection::testSocket()
{
    QFETCH(QAbstractSocket*, socket);

    IrcConnection connection;
    connection.setSocket(socket);
    QCOMPARE(connection.socket(), socket);
    QCOMPARE(connection.isSecure(), socket && socket->inherits("QSslSocket"));
}

void tst_IrcConnection::testSecure()
{
    IrcConnection connection;
    QSignalSpy spy(&connection, SIGNAL(secureChanged(bool)));
    QVERIFY(spy.isValid());
    QVERIFY(!connection.isSecure());

    connection.setSecure(true);
    QVERIFY(connection.isSecure());
    QVERIFY(connection.socket()->inherits("QSslSocket"));
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.first().first().toBool());

    connection.setSecure(false);
    QVERIFY(!connection.isSecure());
    QVERIFY(!connection.socket()->inherits("QSslSocket"));
    QCOMPARE(spy.count(), 2);
    QVERIFY(!spy.last().last().toBool());
}

Q_DECLARE_METATYPE(QString*)
Q_DECLARE_METATYPE(IrcMessage*)
void tst_IrcConnection::testConnection()
{
    qRegisterMetaType<QString*>();
    qRegisterMetaType<IrcMessage*>();

    IrcConnection connection;
    QSignalSpy connectingSpy(&connection, SIGNAL(connecting()));
    QSignalSpy connectedSpy(&connection, SIGNAL(connected()));
    QSignalSpy disconnectedSpy(&connection, SIGNAL(disconnected()));
    QSignalSpy messageReceivedSpy(&connection, SIGNAL(messageReceived(IrcMessage*)));

    QVERIFY(connectingSpy.isValid());
    QVERIFY(connectedSpy.isValid());
    QVERIFY(disconnectedSpy.isValid());
    QVERIFY(messageReceivedSpy.isValid());

    connection.setUserName("user");
    connection.setNickName("nick");
    connection.setRealName("real");
    connection.setHost(server.serverAddress().toString());
    connection.setPort(server.serverPort());

    connection.open();
    QVERIFY(server.waitForNewConnection(2000));
    QTcpSocket* serverSocket = server.nextPendingConnection();
    QVERIFY(serverSocket);

    QVERIFY(connection.socket()->waitForConnected());
    QCOMPARE(connectingSpy.count(), 1);

    QVERIFY(serverSocket->write(":irc.ser.ver 001 nick :Welcome to the Internet Relay Chat Network nick\r\n"));
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(connection.socket()->waitForReadyRead());
    QCOMPARE(connectedSpy.count(), 1);

    connection.close();
    QCOMPARE(disconnectedSpy.count(), 1);
}

void tst_IrcConnection::testSendCommand()
{
    IrcConnection connection;
    QVERIFY(!connection.sendCommand(0));
    QVERIFY(!connection.sendCommand(IrcCommand::createQuit()));

    connection.setUserName("user");
    connection.setNickName("nick");
    connection.setRealName("real");
    connection.setHost(server.serverAddress().toString());
    connection.setPort(server.serverPort());
    connection.open();
    QVERIFY(connection.socket()->waitForConnected());

    QVERIFY(!connection.sendCommand(0));
    QVERIFY(connection.sendCommand(IrcCommand::createQuit()));
    connection.close();
}

void tst_IrcConnection::testSendData()
{
    IrcConnection connection;
    QVERIFY(!connection.sendData("QUIT"));

    connection.setUserName("user");
    connection.setNickName("nick");
    connection.setRealName("real");
    connection.setHost(server.serverAddress().toString());
    connection.setPort(server.serverPort());
    connection.open();
    QVERIFY(connection.socket()->waitForConnected());

    QVERIFY(connection.sendData("QUIT"));
    connection.close();
}

QTEST_MAIN(tst_IrcConnection)

#include "tst_ircconnection.moc"
