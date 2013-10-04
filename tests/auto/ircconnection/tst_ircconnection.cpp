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
#include "ircprotocol.h"
#include "ircconnection.h"
#include "ircmessage.h"
#include "ircfilter.h"
#include <QtTest/QtTest>
#include <QtCore/QTextCodec>
#include <QtCore/QScopedPointer>
#ifndef QT_NO_OPENSSL
#include <QtNetwork/QSslSocket>
#endif

#include "tst_data.h"
#include "tst_clientserver.h"

class FriendlyConnection : public IrcConnection
{
    friend class tst_IrcConnection;
};

class TestProtocol : public IrcProtocol
{
public:
    TestProtocol(IrcConnection* connection) : IrcProtocol(connection)
    {
    }

    virtual bool write(const QByteArray& data)
    {
        written = data;
        return IrcProtocol::write(data);
    }

    QByteArray written;
};

class tst_IrcConnection : public tst_ClientServer
{
    Q_OBJECT

private slots:
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
    void testSasl();

    void testOpen();

    void testStatus();
    void testConnection();
    void testMessages();
    void testMessageFlags();

    void testSendCommand();
    void testSendData();

    void testMessageFilter();
    void testCommandFilter();

    void testWarnings();

    void testCtcp();
};

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
    QTest::newRow("local") << QString("127.0.0.1");
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

void tst_IrcConnection::testSasl()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    IrcProtocol* protocol = static_cast<FriendlyConnection*>(connection.data())->protocol();
    QVERIFY(protocol);

    QVERIFY(!IrcConnection::supportedSaslMechanisms().contains("UNKNOWN"));
    QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setSaslMechanism(): unsupported mechanism: 'UNKNOWN'");
    connection->setSaslMechanism("UNKNOWN");
    QVERIFY(connection->saslMechanism().isEmpty());

    QVERIFY(IrcConnection::supportedSaslMechanisms().contains("PLAIN"));
    connection->setSaslMechanism("PLAIN");
    QCOMPARE(connection->saslMechanism(), QString("PLAIN"));

    waitForWritten(":irc.freenode.net CAP * LS :sasl\r\n");
    QVERIFY(clientSocket->waitForBytesWritten());
    serverSocket->waitForReadyRead();
    QVERIFY(serverSocket->readAll().contains("CAP REQ :sasl"));

    // do not resume handshake too early
    QCoreApplication::sendPostedEvents(protocol, QEvent::MetaCall);
    QVERIFY(!clientSocket->waitForBytesWritten());

    waitForWritten(":irc.freenode.net CAP user ACK :sasl\r\n");
    QVERIFY(clientSocket->waitForBytesWritten());
    serverSocket->waitForReadyRead();
    QVERIFY(serverSocket->readAll().contains("AUTHENTICATE PLAIN"));

    waitForWritten("AUTHENTICATE +\r\n");
    QVERIFY(clientSocket->waitForBytesWritten());
    serverSocket->waitForReadyRead();

    QByteArray response = serverSocket->readAll();
    int index = response.indexOf("AUTHENTICATE");
    QVERIFY(index != -1);
    QByteArray secret = response.mid(index + 13);
    index = secret.indexOf("\r\n");
    QVERIFY(index != -1);
    secret.truncate(index + 1);
    secret = QByteArray::fromBase64(secret);
    QByteArray expected = connection->userName().toUtf8() + '\0' +
                          connection->userName().toUtf8() + '\0' +
                          connection->password().toUtf8();
    QCOMPARE(secret, expected);

    // resume handshake
    QCoreApplication::sendPostedEvents(protocol, QEvent::MetaCall);

    QVERIFY(clientSocket->waitForBytesWritten());
    serverSocket->waitForReadyRead();
    QVERIFY(serverSocket->readAll().contains("CAP END"));

    waitForWritten(":irc.freenode.net 900 user nick!user@host nick :You are now logged in as user.\r\n");
    waitForWritten(":irc.freenode.net 903 user :SASL authentication successful\r\n");
    waitForWritten(":irc.freenode.net 001 user :Welcome to the freenode Internet Relay Chat Network user\r\n");
}

void tst_IrcConnection::testOpen()
{
    IrcConnection connection;
    QTest::ignoreMessage(QtCriticalMsg, "IrcConnection::open(): host is empty!");
    connection.open();
    QCOMPARE(connection.status(), IrcConnection::Inactive);

    connection.setHost("irc.ser.ver");
    QTest::ignoreMessage(QtCriticalMsg, "IrcConnection::open(): userName is empty!");
    connection.open();
    QCOMPARE(connection.status(), IrcConnection::Inactive);

    connection.setUserName("user");
    QTest::ignoreMessage(QtCriticalMsg, "IrcConnection::open(): nickName is empty!");
    connection.open();
    QCOMPARE(connection.status(), IrcConnection::Inactive);

    connection.setNickName("nick");
    QTest::ignoreMessage(QtCriticalMsg, "IrcConnection::open(): realName is empty!");
    connection.open();
    QCOMPARE(connection.status(), IrcConnection::Inactive);

    connection.setRealName("real");
    connection.open();
    QVERIFY(connection.status() != IrcConnection::Inactive);
}

void tst_IrcConnection::testStatus()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    Irc::registerMetaTypes();

    // tst_ClientServer::init() opens the connection
    QCOMPARE(connection->status(), IrcConnection::Connecting);
    QVERIFY(!connection->isConnected());
    QVERIFY(connection->isActive());

    QSignalSpy statusSpy(connection, SIGNAL(statusChanged(IrcConnection::Status)));
    QSignalSpy connectingSpy(connection, SIGNAL(connecting()));
    QSignalSpy connectedSpy(connection, SIGNAL(connected()));
    QSignalSpy disconnectedSpy(connection, SIGNAL(disconnected()));

    QVERIFY(statusSpy.isValid());
    QVERIFY(connectingSpy.isValid());
    QVERIFY(connectedSpy.isValid());
    QVERIFY(disconnectedSpy.isValid());

    int statusCount = 0;
    int connectingCount = 0;
    int connectedCount = 0;
    int disconnectedCount = 0;

    waitForWritten(tst_Data::welcome());
    QVERIFY(connection->isActive());
    QVERIFY(connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connected);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Connected);
    QCOMPARE(connectedSpy.count(), ++connectedCount);

    clientSocket->close();
    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closing);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Closing);

    connection->close();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closed);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Closed);
    QCOMPARE(disconnectedSpy.count(), ++disconnectedCount);

    connection->open();
    waitForOpened();
    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connecting);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Connecting);
    QCOMPARE(connectingSpy.count(), ++connectingCount);

    waitForWritten(tst_Data::welcome());
    QVERIFY(connection->isActive());
    QVERIFY(connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connected);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Connected);
    QCOMPARE(connectedSpy.count(), ++connectedCount);

    // trigger an error
    serverSocket->close();
    QVERIFY(clientSocket->waitForDisconnected(100));
    QVERIFY(!connection->isConnected());
    QVERIFY(!connection->isActive());

    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Error);
    QCOMPARE(statusSpy.count(), statusCount);

    connection->close();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closed);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Closed);
    QCOMPARE(disconnectedSpy.count(), ++disconnectedCount);

    connection->open();
    waitForOpened();
    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connecting);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Connecting);
    QCOMPARE(connectingSpy.count(), ++connectingCount);

    waitForWritten(tst_Data::welcome());
    QVERIFY(connection->isActive());
    QVERIFY(connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connected);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Connected);
    QCOMPARE(connectedSpy.count(), ++connectedCount);

    // trigger an error - automatic reconnect
    connection->setReconnectDelay(1);
    serverSocket->close();
    QVERIFY(clientSocket->waitForDisconnected(100));
    QVERIFY(!connection->isConnected());
    QVERIFY(!connection->isActive());

    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Error);
    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Waiting);
    QCOMPARE(statusSpy.count(), statusCount);

    QEventLoop reconnectLoop;
    QTimer::singleShot(2000, &reconnectLoop, SLOT(quit()));
    connect(connection, SIGNAL(statusChanged(IrcConnection::Status)), &reconnectLoop, SLOT(quit()));
    reconnectLoop.exec();

    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connecting);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Connecting);

    waitForOpened();
    QCOMPARE(connectingSpy.count(), ++connectingCount);

    // trigger an error _after_ quit -> no automatic reconnect
    connection->quit();
    serverSocket->close();
    QVERIFY(clientSocket->waitForDisconnected(100));
    QVERIFY(!connection->isConnected());
    QVERIFY(!connection->isActive());
    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Closing);
    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Closed);
    QCOMPARE(statusSpy.count(), statusCount);
}

void tst_IrcConnection::testConnection()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    Irc::registerMetaTypes();

    TestProtocol* protocol = new TestProtocol(connection);
    static_cast<FriendlyConnection*>(connection.data())->setProtocol(protocol);

    // tst_ClientServer::init() opens the connection
    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connecting);

    waitForWritten(":irc.ser.ver 001 nick :Welcome to the Internet Relay Chat Network nick\r\n");
    QVERIFY(connection->isActive());
    QVERIFY(connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connected);

    connection->close();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closed);

    // does nothing when explicitly closed
    connection->resume();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closed);

    connection->open();
    waitForOpened();
    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connecting);

    connection->reset();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Inactive);

    connection->resume();
    QVERIFY(connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connecting);

    waitForOpened();
    waitForWritten(":irc.ser.ver 001 nick :Welcome to the Internet Relay Chat Network nick\r\n");
    QVERIFY(connection->isActive());
    QVERIFY(connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Connected);

    protocol->written.clear();
    connection->setNickName("communi");
    QVERIFY(protocol->written.contains("NICK"));
    QVERIFY(protocol->written.contains("communi"));

    protocol->written.clear();
    connection->quit();
    QVERIFY(protocol->written.contains("QUIT"));

    connection->close();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closed);
}

void tst_IrcConnection::testMessages()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    Irc::registerMetaTypes();

    QSignalSpy messageSpy(connection, SIGNAL(messageReceived(IrcMessage*)));
    QSignalSpy capabilityMessageSpy(connection, SIGNAL(capabilityMessageReceived(IrcCapabilityMessage*)));
    QSignalSpy errorMessageSpy(connection, SIGNAL(errorMessageReceived(IrcErrorMessage*)));
    QSignalSpy inviteMessageSpy(connection, SIGNAL(inviteMessageReceived(IrcInviteMessage*)));
    QSignalSpy joinMessageSpy(connection, SIGNAL(joinMessageReceived(IrcJoinMessage*)));
    QSignalSpy kickMessageSpy(connection, SIGNAL(kickMessageReceived(IrcKickMessage*)));
    QSignalSpy modeMessageSpy(connection, SIGNAL(modeMessageReceived(IrcModeMessage*)));
    QSignalSpy namesMessageSpy(connection, SIGNAL(namesMessageReceived(IrcNamesMessage*)));
    QSignalSpy nickMessageSpy(connection, SIGNAL(nickMessageReceived(IrcNickMessage*)));
    QSignalSpy noticeMessageSpy(connection, SIGNAL(noticeMessageReceived(IrcNoticeMessage*)));
    QSignalSpy numericMessageSpy(connection, SIGNAL(numericMessageReceived(IrcNumericMessage*)));
    QSignalSpy motdMessageSpy(connection, SIGNAL(motdMessageReceived(IrcMotdMessage*)));
    QSignalSpy partMessageSpy(connection, SIGNAL(partMessageReceived(IrcPartMessage*)));
    QSignalSpy pingMessageSpy(connection, SIGNAL(pingMessageReceived(IrcPingMessage*)));
    QSignalSpy pongMessageSpy(connection, SIGNAL(pongMessageReceived(IrcPongMessage*)));
    QSignalSpy privateMessageSpy(connection, SIGNAL(privateMessageReceived(IrcPrivateMessage*)));
    QSignalSpy quitMessageSpy(connection, SIGNAL(quitMessageReceived(IrcQuitMessage*)));
    QSignalSpy topicMessageSpy(connection, SIGNAL(topicMessageReceived(IrcTopicMessage*)));

    QVERIFY(messageSpy.isValid());
    QVERIFY(capabilityMessageSpy.isValid());
    QVERIFY(errorMessageSpy.isValid());
    QVERIFY(inviteMessageSpy.isValid());
    QVERIFY(joinMessageSpy.isValid());
    QVERIFY(kickMessageSpy.isValid());
    QVERIFY(modeMessageSpy.isValid());
    QVERIFY(namesMessageSpy.isValid());
    QVERIFY(nickMessageSpy.isValid());
    QVERIFY(noticeMessageSpy.isValid());
    QVERIFY(numericMessageSpy.isValid());
    QVERIFY(motdMessageSpy.isValid());
    QVERIFY(partMessageSpy.isValid());
    QVERIFY(pingMessageSpy.isValid());
    QVERIFY(pongMessageSpy.isValid());
    QVERIFY(privateMessageSpy.isValid());
    QVERIFY(quitMessageSpy.isValid());
    QVERIFY(topicMessageSpy.isValid());

    int messageCount = 0;
    int numericMessageCount = 0;

    waitForWritten(":moorcock.freenode.net CAP * LS :account-notify extended-join identify-msg multi-prefix sasl\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(capabilityMessageSpy.count(), 1);

    waitForWritten(":moorcock.freenode.net 001 communi :Welcome to the freenode Internet Relay Chat Network communi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 005 communi CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode KNOCK STATUSMSG=@+ CALLERID=g :are supported by this server\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 005 communi CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=100 FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: :are supported by this server\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 005 communi EXTBAN=$,arxz WHOX CLIENTVER=3.0 SAFELIST ELIST=CTU :are supported by this server\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 375 communi :- moorcock.freenode.net Message of the Day -\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 372 communi :- Welcome to moorcock.freenode.net in ...\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 376 communi :End of /MOTD command.\r\n");
    messageCount += 2; // RPL_ENDOFMOTD + IrcMotdMessage
    QCOMPARE(messageSpy.count(), messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);
    QCOMPARE(motdMessageSpy.count(), 1);

    waitForWritten(":communi!~communi@hidd.en JOIN #freenode\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(joinMessageSpy.count(), 1);

    waitForWritten(":moorcock.freenode.net 332 communi #freenode :Welcome to #freenode | Staff are voiced; some may also be on /stats p -- feel free to /msg us at any time | FAQ: http://freenode.net/faq.shtml | Unwelcome queries? Use /mode your_nick +R to block them. | Channel guidelines: http://freenode.net/poundfreenode.shtml | Blog: http://blog.freenode.net | Please don't comment on spam/trolls.\r\n");
    messageCount += 2; // RPL_TOPIC & IrcTopicMessage
    QCOMPARE(messageSpy.count(), messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);
    QCOMPARE(topicMessageSpy.count(), 1);

    waitForWritten(":moorcock.freenode.net 333 communi #freenode erry 1379357591\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 353 communi = #freenode :communi straterra absk007 pefn xlys Gromit TooCool Sambler gat0rs KarneAsada danis_963 Kiryx chrismeller deefloo black_male sxlnxdx bjork Kinny phobos_anomaly T13|sleeps JuxTApose Kolega2357 rorx techhelper1 hermatize Azimi iqualfragile fwilson skasturi mwallacesd mayday Guest76549 mcjohansen MangaKaDenza ARISTIDES ketas `- claptor ylluminate Cooky Brand3n cheater_1 Kirito digitaloktay Will| Iarfen abrotman smurfy Inaunt +mist Karol RougeR_\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 353 communi = #freenode :publickeating An_Ony_Moose michagogo Guest915` davidfg4 Ragnor s1lent_1 keee GingerGeek[Away] hibari derp S_T_A_N anonymuse asantoni road|runner LLckfan neoian2 aviancarrier nipples danieldaniel Pyrus Bry8Star shadowm_desktop furtardo rdymac TTSDA seaworthy Chiyo yscc Zombiebaron redpill f4cl3y Boohbah applebloom zorael kameloso^ Zetetic XAMPP wheels_up Cuppy-Cake mindlessjohnny Kymru mquin_ Rodja babilen kirin` David Affix jshyeung_ DarkAceZ karakedi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten(":moorcock.freenode.net 366 communi #freenode :End of /NAMES list.\r\n");
    messageCount += 2; // RPL_ENDOFNAMES & IrcNamesMessage
    QCOMPARE(messageSpy.count(), messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);
    QCOMPARE(namesMessageSpy.count(), 1);

    waitForWritten(":ChanServ!ChanServ@services. NOTICE communi :[#freenode] Welcome to #freenode. All network staff are voiced in here, but may not always be around - type /stats p to get a list of on call staff. Others may be hiding so do feel free to ping and /msg us at will! Also please read the channel guidelines at http://freenode.net/poundfreenode.shtml - thanks.\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(noticeMessageSpy.count(), 1);

    waitForWritten(":services. 328 communi #freenode :http://freenode.net/\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(numericMessageSpy.count(), ++numericMessageCount);

    waitForWritten("PING :moorcock.freenode.net\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(pingMessageSpy.count(), 1);

    waitForWritten("PONG :moorcock.freenode.net\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(pongMessageSpy.count(), 1);

    waitForWritten(":jpnurmi!jpnurmi@qt/jpnurmi INVITE Communi84194 :#communi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(inviteMessageSpy.count(), 1);

    waitForWritten(":Communi84194!ident@host NICK :communi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(nickMessageSpy.count(), 1);

    waitForWritten(":jpnurmi!jpnurmi@qt/jpnurmi MODE #communi +v communi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(modeMessageSpy.count(), 1);

    waitForWritten(":qtassistant!jpnurmi@qt/jpnurmi/bot/qtassistant PART #communi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(partMessageSpy.count(), 1);

    waitForWritten(":jpnurmi!jpnurmi@qt/jpnurmi PRIVMSG #communi :hello\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(privateMessageSpy.count(), 1);

    waitForWritten(":jpnurmi!jpnurmi@qt/jpnurmi QUIT :Client Quit\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(quitMessageSpy.count(), 1);

    waitForWritten(":jpnurmi!jpnurmi@qt/jpnurmi KICK #communi communi\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(kickMessageSpy.count(), 1);

    waitForWritten("ERROR :just testing...\r\n");
    QCOMPARE(messageSpy.count(), ++messageCount);
    QCOMPARE(errorMessageSpy.count(), 1);
}

class MsgFilter : public QObject, public IrcMessageFilter
{
    Q_OBJECT
    Q_INTERFACES(IrcMessageFilter)

public:
    MsgFilter() : count(0), type(IrcMessage::Unknown), flags(IrcMessage::None)
    {
    }

    bool messageFilter(IrcMessage* message)
    {
        ++count;
        type = message->type();
        flags = message->flags();
        value = message->property(property);
        return false;
    }

public:
    int count;
    QVariant value;
    QByteArray property;
    IrcMessage::Type type;
    IrcMessage::Flags flags;
};

void tst_IrcConnection::testMessageFlags()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    int count = 0;
    MsgFilter filter;
    connection->installMessageFilter(&filter);

    waitForWritten(":server CAP * LS :identify-msg\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Capability);
    QCOMPARE(filter.flags, IrcMessage::None);

    waitForWritten(":server CAP communi ACK :identify-msg\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Capability);
    QCOMPARE(filter.flags, IrcMessage::None);

    waitForWritten(":server 001 communi :Welcome...\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Numeric);
    QCOMPARE(filter.flags, IrcMessage::None);

    waitForWritten(":server 005 communi CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=fake KNOCK STATUSMSG=@+ CALLERID=g :are supported by this server\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Numeric);
    QCOMPARE(filter.flags, IrcMessage::None);

    waitForWritten(":server 005 communi CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=100 FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: :are supported by this server\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Numeric);
    QCOMPARE(filter.flags, IrcMessage::None);

    waitForWritten(":server 005 communi EXTBAN=$,arxz WHOX CLIENTVER=3.0 SAFELIST ELIST=CTU :are supported by this server\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Numeric);
    QCOMPARE(filter.flags, IrcMessage::None);

    filter.property = "message";
    waitForWritten(":communi!ident@host PRIVMSG #communi :hi all\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Private);
    QCOMPARE(filter.flags, IrcMessage::Own);
    QCOMPARE(filter.value.toString(), QString("hi all"));

    filter.property = "message";
    waitForWritten(":jpnurmi!ident@host PRIVMSG #communi :+hello there, communi\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Private);
    QCOMPARE(filter.flags, IrcMessage::Identified);
    QCOMPARE(filter.value.toString(), QString("hello there, communi"));

    filter.property = "message";
    waitForWritten(":Guest1234!ident@host PRIVMSG #communi :-hi communi\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Private);
    QCOMPARE(filter.flags, IrcMessage::Unidentified);
    QCOMPARE(filter.value.toString(), QString("hi communi"));

    filter.property = "message";
    waitForWritten(":communi!ident@host NOTICE #communi :hi all\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Notice);
    QCOMPARE(filter.flags, IrcMessage::Own);
    QCOMPARE(filter.value.toString(), QString("hi all"));

    filter.property = "message";
    waitForWritten(":jpnurmi!ident@host NOTICE #communi :+hello there, communi\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Notice);
    QCOMPARE(filter.flags, IrcMessage::Identified);
    QCOMPARE(filter.value.toString(), QString("hello there, communi"));

    filter.property = "message";
    waitForWritten(":Guest1234!ident@host NOTICE #communi :-hi communi\r\n");
    QCOMPARE(filter.count, ++count);
    QCOMPARE(filter.type, IrcMessage::Notice);
    QCOMPARE(filter.flags, IrcMessage::Unidentified);
    QCOMPARE(filter.value.toString(), QString("hi communi"));
}

void tst_IrcConnection::testSendCommand()
{
    IrcConnection conn;
    QVERIFY(!conn.sendCommand(0));
    QVERIFY(!conn.sendCommand(IrcCommand::createQuit()));

    if (!serverSocket)
        Q4SKIP("The address is not available");

    QVERIFY(connection->sendCommand(IrcCommand::createQuit()));
    QVERIFY(!connection->sendCommand(0));
}

void tst_IrcConnection::testSendData()
{
    IrcConnection conn;
    QVERIFY(!conn.sendData("QUIT"));

    if (!serverSocket)
        Q4SKIP("The address is not available");

    QVERIFY(connection->sendData("QUIT"));
}

class TestFilter : public QObject, public IrcMessageFilter, public IrcCommandFilter
{
    Q_OBJECT
    Q_INTERFACES(IrcMessageFilter IrcCommandFilter)

public:
    void clear()
    {
        commitSuicide = false;
        messageFiltered = 0;
        commandFiltered = 0;
        messageFilterEnabled = false;
        commandFilterEnabled = false;
    }

    bool messageFilter(IrcMessage*)
    {
        ++messageFiltered;
        if (commitSuicide)
            delete this;
        return messageFilterEnabled;
    }

    bool commandFilter(IrcCommand*)
    {
        ++commandFiltered;
        if (commitSuicide)
            delete this;
        return commandFilterEnabled;
    }

    bool commitSuicide;
    int messageFiltered;
    int commandFiltered;
    bool messageFilterEnabled;
    bool commandFilterEnabled;
};

void tst_IrcConnection::testMessageFilter()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    Irc::registerMetaTypes();

    QSignalSpy messageSpy(connection, SIGNAL(messageReceived(IrcMessage*)));
    QVERIFY(messageSpy.isValid());
    int messageCount = 0;

    QScopedPointer<TestFilter> filter1(new TestFilter);
    QScopedPointer<TestFilter> filter2(new TestFilter);
    QScopedPointer<TestFilter> filter3(new TestFilter);

    filter1->clear(); filter2->clear(); filter3->clear();

    connection->installMessageFilter(filter1.data());
    connection->installMessageFilter(filter2.data());
    connection->installMessageFilter(filter3.data());

    waitForWritten(":moorcock.freenode.net 001 communi :Welcome to the freenode Internet Relay Chat Network communi\r\n");
    QCOMPARE(filter1->messageFiltered, 1);
    QCOMPARE(filter2->messageFiltered, 1);
    QCOMPARE(filter3->messageFiltered, 1);
    QCOMPARE(messageSpy.count(), ++messageCount);

    filter1->clear(); filter2->clear(); filter3->clear();
    filter3->messageFilterEnabled = true;

    waitForWritten(":moorcock.freenode.net 005 communi CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode KNOCK STATUSMSG=@+ CALLERID=g :are supported by this server\r\n");
    QCOMPARE(filter1->messageFiltered, 0);
    QCOMPARE(filter2->messageFiltered, 0);
    QCOMPARE(filter3->messageFiltered, 1);
    QCOMPARE(messageSpy.count(), messageCount);

    filter1->clear(); filter2->clear(); filter3->clear();
    filter2->messageFilterEnabled = true;

    waitForWritten(":moorcock.freenode.net 005 communi CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=100 FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: :are supported by this server\r\n");
    QCOMPARE(filter1->messageFiltered, 0);
    QCOMPARE(filter2->messageFiltered, 1);
    QCOMPARE(filter3->messageFiltered, 1);
    QCOMPARE(messageSpy.count(), messageCount);

    filter1->clear(); filter2->clear(); filter3->clear();
    filter1->messageFilterEnabled = true;

    waitForWritten(":moorcock.freenode.net 005 communi EXTBAN=$,arxz WHOX CLIENTVER=3.0 SAFELIST ELIST=CTU :are supported by this server\r\n");
    QCOMPARE(filter1->messageFiltered, 1);
    QCOMPARE(filter2->messageFiltered, 1);
    QCOMPARE(filter3->messageFiltered, 1);
    QCOMPARE(messageSpy.count(), messageCount);

    filter1->clear(); filter2->clear(); filter3->clear();

    waitForWritten(":moorcock.freenode.net 375 communi :- moorcock.freenode.net Message of the Day -\r\n");
    QCOMPARE(filter1->messageFiltered, 1);
    QCOMPARE(filter2->messageFiltered, 1);
    QCOMPARE(filter3->messageFiltered, 1);
    QCOMPARE(messageSpy.count(), ++messageCount);

    // a deleted filter gets removed
    filter2.reset();
    filter1->clear(); filter3->clear();

    waitForWritten(":moorcock.freenode.net 372 communi :- Welcome to moorcock.freenode.net in ...\r\n");
    QCOMPARE(filter1->messageFiltered, 1);
    QCOMPARE(filter3->messageFiltered, 1);
    QCOMPARE(messageSpy.count(), ++messageCount);

    waitForWritten(":moorcock.freenode.net 376 communi :End of /MOTD command.\r\n");
    messageCount += 2; // RPL_ENDOFMOTD + IrcMotdMessage
    QCOMPARE(messageSpy.count(), messageCount);

    // double filters
    connection->installMessageFilter(filter1.data());
    connection->installMessageFilter(filter3.data());
    filter1->clear(); filter3->clear();

    waitForWritten(":communi!~communi@hidd.en JOIN #freenode\r\n");
    QCOMPARE(filter1->messageFiltered, 2);
    QCOMPARE(filter3->messageFiltered, 2);
    QCOMPARE(messageSpy.count(), ++messageCount);

    // remove & enable double filter
    filter1->clear(); filter3->clear();
    filter1->messageFilterEnabled = true;
    connection->removeMessageFilter(filter3.data());

    waitForWritten(":communi!~communi@hidd.en JOIN #communi\r\n");
    QCOMPARE(filter1->messageFiltered, 1);
    QCOMPARE(filter3->messageFiltered, 0);
    QCOMPARE(messageSpy.count(), messageCount);

    // remove & delete
    filter3.reset();
    filter1->clear();
    connection->removeMessageFilter(filter1.data());

    waitForWritten(":communi!~communi@hidd.en PART #communi\r\n");
    QCOMPARE(filter1->messageFiltered, 0);
    QCOMPARE(messageSpy.count(), ++messageCount);
}

void tst_IrcConnection::testCommandFilter()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    TestProtocol* protocol = new TestProtocol(connection);
    static_cast<FriendlyConnection*>(connection.data())->setProtocol(protocol);

    TestFilter* filter1 = new TestFilter; // suicidal
    QScopedPointer<TestFilter> filter2(new TestFilter);
    QScopedPointer<TestFilter> filter3(new TestFilter);

    filter1->clear(); filter2->clear(); filter3->clear();

    connection->installCommandFilter(filter1);
    connection->installCommandFilter(filter2.data());
    connection->installCommandFilter(filter3.data());

    connection->sendCommand(IrcCommand::createJoin("#freenode"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(!protocol->written.isEmpty());

    protocol->written.clear();
    filter1->clear(); filter2->clear(); filter3->clear();
    filter3->commandFilterEnabled = true;

    connection->sendCommand(IrcCommand::createJoin("#communi"));
    QCOMPARE(filter1->commandFiltered, 0);
    QCOMPARE(filter2->commandFiltered, 0);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(protocol->written.isEmpty());

    protocol->written.clear();
    filter1->clear(); filter2->clear(); filter3->clear();
    filter2->commandFilterEnabled = true;

    connection->sendCommand(IrcCommand::createJoin("#qt"));
    QCOMPARE(filter1->commandFiltered, 0);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(protocol->written.isEmpty());

    protocol->written.clear();
    filter1->clear(); filter2->clear(); filter3->clear();
    filter1->commandFilterEnabled = true;

    connection->sendCommand(IrcCommand::createPart("#freenode"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(protocol->written.isEmpty());

    protocol->written.clear();
    filter1->clear(); filter2->clear(); filter3->clear();

    connection->sendCommand(IrcCommand::createPart("#communi"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(!protocol->written.isEmpty());

    // a deleted filter gets removed
    filter2.reset();
    filter1->clear(); filter3->clear();
    protocol->written.clear();

    connection->sendCommand(IrcCommand::createPart("#qt"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(!protocol->written.isEmpty());

    // double filters
    connection->installCommandFilter(filter1);
    connection->installCommandFilter(filter3.data());
    filter1->clear(); filter3->clear();
    protocol->written.clear();

    connection->sendCommand(IrcCommand::createJoin("#freenode"));
    QCOMPARE(filter1->commandFiltered, 2);
    QCOMPARE(filter3->commandFiltered, 2);
    QVERIFY(!protocol->written.isEmpty());

    // remove & enable double filter
    filter1->clear(); filter3->clear();
    filter1->commandFilterEnabled = true;
    connection->removeCommandFilter(filter3.data());
    protocol->written.clear();

    connection->sendCommand(IrcCommand::createJoin("#communi"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 0);
    QVERIFY(protocol->written.isEmpty());

    // remove & delete
    filter3.reset();
    filter1->clear();
    connection->removeCommandFilter(filter1);
    protocol->written.clear();

    connection->sendCommand(IrcCommand::createJoin("#qt"));
    QCOMPARE(filter1->commandFiltered, 0);
    QVERIFY(!protocol->written.isEmpty());

    // commit a suicide
    filter1->commitSuicide = true;
    connection->installCommandFilter(filter1);
    protocol->written.clear();

    connection->sendCommand(IrcCommand::createPart("#qt"));
    QVERIFY(!protocol->written.isEmpty());
}

void tst_IrcConnection::testWarnings()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    QVERIFY(connection->isActive());

    QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setHost() has no effect until re-connect");
    connection->setHost("foo");

    QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setPort() has no effect until re-connect");
    connection->setPort(1234);

    QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setUserName() has no effect until re-connect");
    connection->setUserName("foo");

    QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setRealName() has no effect until re-connect");
    connection->setRealName("foo");

    QTest::ignoreMessage(QtWarningMsg, "IrcConnection::setPassword() has no effect until re-connect");
    connection->setPassword("foo");
}

class FakeQmlConnection : public IrcConnection
{
    Q_OBJECT
    friend class tst_IrcConnection;

public slots:
    QVariant createCtcpReply(const QVariant& request)
    {
        return QVariant::fromValue(IrcConnection::createCtcpReply(request.value<IrcPrivateMessage*>()));
    }
};

void tst_IrcConnection::testCtcp()
{
    FriendlyConnection* friendly = static_cast<FriendlyConnection*>(connection.data());

    // PING
    IrcMessage* msg = IrcMessage::fromData(":nick!user@host PRIVMSG communi :\1PING timestamp\1", connection);
    QScopedPointer<IrcPrivateMessage> pingRequest(qobject_cast<IrcPrivateMessage*>(msg));
    QVERIFY(pingRequest.data());

    QScopedPointer<IrcCommand> pingReply(friendly->createCtcpReply(pingRequest.data()));
    QVERIFY(pingReply.data());
    QCOMPARE(pingReply->type(), IrcCommand::CtcpReply);
    QCOMPARE(pingReply->toString(), QString("NOTICE nick :\1PING timestamp\1"));

    // TIME
    msg = IrcMessage::fromData(":nick!user@host PRIVMSG communi :\1TIME\1", connection);
    QScopedPointer<IrcPrivateMessage> timeRequest(qobject_cast<IrcPrivateMessage*>(msg));
    QVERIFY(timeRequest);

    QScopedPointer<IrcCommand> timeReply(friendly->createCtcpReply(timeRequest.data()));
    QVERIFY(timeReply.data());
    QCOMPARE(timeReply->type(), IrcCommand::CtcpReply);
    QCOMPARE(timeReply->toString(), QString("NOTICE nick :\1TIME %1\1").arg(QLocale().toString(QDateTime::currentDateTime(), QLocale::ShortFormat)));

    // VERSION
    msg = IrcMessage::fromData(":nick!user@host PRIVMSG communi :\1VERSION\1", connection);
    QScopedPointer<IrcPrivateMessage> versionRequest(qobject_cast<IrcPrivateMessage*>(msg));
    QVERIFY(versionRequest.data());

    QScopedPointer<IrcCommand> versionReply(friendly->createCtcpReply(versionRequest.data()));
    QVERIFY(versionReply.data());
    QCOMPARE(versionReply->type(), IrcCommand::CtcpReply);
    QVERIFY(versionReply->toString().startsWith("NOTICE nick :\1VERSION "));
    QVERIFY(versionReply->toString().contains(Irc::version()));
    QVERIFY(versionReply->toString().endsWith("\1"));

    // SOURCE
    msg = IrcMessage::fromData(":nick!user@host PRIVMSG communi :\1SOURCE\1", connection);
    QScopedPointer<IrcPrivateMessage> sourceRequest(qobject_cast<IrcPrivateMessage*>(msg));
    QVERIFY(sourceRequest.data());

    QScopedPointer<IrcCommand> sourceReply(friendly->createCtcpReply(sourceRequest.data()));
    QVERIFY(sourceReply.data());
    QCOMPARE(sourceReply->type(), IrcCommand::CtcpReply);
    QVERIFY(sourceReply->toString().startsWith("NOTICE nick :\1SOURCE "));
    QVERIFY(sourceReply->toString().contains("http://"));
    QVERIFY(sourceReply->toString().endsWith("\1"));

    // CLIENTINFO
    msg = IrcMessage::fromData(":nick!user@host PRIVMSG communi :\1CLIENTINFO\1", connection);
    QScopedPointer<IrcPrivateMessage> infoRequest(qobject_cast<IrcPrivateMessage*>(msg));
    QVERIFY(infoRequest.data());

    QScopedPointer<IrcCommand> infoReply(friendly->createCtcpReply(infoRequest.data()));
    QVERIFY(infoReply.data());
    QCOMPARE(infoReply->type(), IrcCommand::CtcpReply);
    QVERIFY(infoReply->toString().startsWith("NOTICE nick :\1CLIENTINFO "));
    QVERIFY(infoReply->toString().contains("PING"));
    QVERIFY(infoReply->toString().contains("TIME"));
    QVERIFY(infoReply->toString().contains("VERSION"));
    QVERIFY(infoReply->toString().contains("SOURCE"));
    QVERIFY(infoReply->toString().endsWith("\1"));

    // QML compatibility
    FakeQmlConnection qmlConnection;
    qmlConnection.setUserName("user");
    qmlConnection.setNickName("nick");
    qmlConnection.setRealName("real");
    qmlConnection.setPassword("secret");
    qmlConnection.setHost(server->serverAddress().toString());
    qmlConnection.setPort(server->serverPort());

    TestProtocol* qmlProtocol = new TestProtocol(&qmlConnection);
    qmlConnection.setProtocol(qmlProtocol);
    qmlConnection.open();

    if (!server->waitForNewConnection(200))
        QEXPECT_FAIL("", "The address is not available", Abort);
    QAbstractSocket* qmlServerSocket = server->nextPendingConnection();
    QVERIFY(qmlServerSocket);
    QAbstractSocket* qmlClientSocket = qmlConnection.socket();
    QVERIFY(qmlClientSocket);
    QVERIFY(qmlClientSocket->waitForConnected());

    qmlProtocol->written.clear();
    qmlServerSocket->write(":nick!user@host PRIVMSG communi :\1PING qml\1\r\n");
    QVERIFY(qmlServerSocket->waitForBytesWritten());
    QVERIFY(qmlClientSocket->waitForReadyRead());
    QCOMPARE(qmlProtocol->written, QByteArray("NOTICE nick :\1PING qml\1"));

    if (!serverSocket)
        Q4SKIP("The address is not available");

    TestProtocol* protocol = new TestProtocol(friendly);
    friendly->setProtocol(protocol);

    // PING
    protocol->written.clear();
    waitForWritten(":nick!user@host PRIVMSG communi :\1PING timestamp\1\r\n");
    QCOMPARE(protocol->written, QByteArray("NOTICE nick :\1PING timestamp\1"));

    // TIME
    protocol->written.clear();
    waitForWritten(":nick!user@host PRIVMSG communi :\1TIME\1\r\n");
    QVERIFY(protocol->written.startsWith("NOTICE nick :\1TIME "));
    QVERIFY(protocol->written.endsWith("\1"));

    // VERSION
    protocol->written.clear();
    waitForWritten(":nick!user@host PRIVMSG communi :\1VERSION\1\r\n");
    QVERIFY(protocol->written.startsWith("NOTICE nick :\1VERSION "));
    QVERIFY(protocol->written.contains(Irc::version().toUtf8()));
    QVERIFY(protocol->written.endsWith("\1"));

    // SOURCE
    protocol->written.clear();
    waitForWritten(":nick!user@host PRIVMSG communi :\1SOURCE\1\r\n");
    QVERIFY(protocol->written.startsWith("NOTICE nick :\1SOURCE "));
    QVERIFY(protocol->written.contains("http://"));
    QVERIFY(protocol->written.endsWith("\1"));

    // CLIENTINFO
    protocol->written.clear();
    waitForWritten(":nick!user@host PRIVMSG communi :\1CLIENTINFO\1\r\n");
    QVERIFY(protocol->written.startsWith("NOTICE nick :\1CLIENTINFO "));
    QVERIFY(protocol->written.contains("PING"));
    QVERIFY(protocol->written.contains("TIME"));
    QVERIFY(protocol->written.contains("VERSION"));
    QVERIFY(protocol->written.contains("SOURCE"));
    QVERIFY(protocol->written.endsWith("\1"));
}

QTEST_MAIN(tst_IrcConnection)

#include "tst_ircconnection.moc"
