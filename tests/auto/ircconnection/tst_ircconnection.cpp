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

    void testStatus();
    void testConnection();
    void testMessages();
    void testMessageFlags();

    void testSendCommand();
    void testSendData();

    void testMessageFilter();
    void testCommandFilter();
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
    clientSocket->waitForDisconnected();
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
    clientSocket->waitForDisconnected();
    QVERIFY(!connection->isConnected());
    QVERIFY(!connection->isActive());

    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Error);
    QCOMPARE(statusSpy.at(statusCount++).at(0).value<IrcConnection::Status>(), IrcConnection::Waiting);
    QCOMPARE(statusSpy.count(), statusCount);

    connection->close();
    QVERIFY(!connection->isActive());
    QVERIFY(!connection->isConnected());
    QCOMPARE(connection->status(), IrcConnection::Closed);
    QCOMPARE(statusSpy.count(), ++statusCount);
    QCOMPARE(statusSpy.last().at(0).value<IrcConnection::Status>(), IrcConnection::Closed);
}

void tst_IrcConnection::testConnection()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    Irc::registerMetaTypes();

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
        messageFiltered = 0;
        commandFiltered = 0;
        messageFilterEnabled = false;
        commandFilterEnabled = false;
    }

    bool messageFilter(IrcMessage*)
    {
        ++messageFiltered;
        return messageFilterEnabled;
    }

    bool commandFilter(IrcCommand*)
    {
        ++commandFiltered;
        return commandFilterEnabled;
    }

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

class TestProtocol : public IrcProtocol
{
public:
    TestProtocol(IrcConnection* connection) : IrcProtocol(connection), written(false)
    {
    }

    virtual bool write(const QByteArray& data)
    {
        written = true;
        return IrcProtocol::write(data);
    }

    bool written;
};

class FriendlyConnection : public IrcConnection
{
    friend class tst_IrcConnection;
};

void tst_IrcConnection::testCommandFilter()
{
    if (!serverSocket)
        Q4SKIP("The address is not available");

    TestProtocol* protocol = new TestProtocol(connection);
    static_cast<FriendlyConnection*>(connection.data())->setProtocol(protocol);

    QScopedPointer<TestFilter> filter1(new TestFilter);
    QScopedPointer<TestFilter> filter2(new TestFilter);
    QScopedPointer<TestFilter> filter3(new TestFilter);

    filter1->clear(); filter2->clear(); filter3->clear();

    connection->installCommandFilter(filter1.data());
    connection->installCommandFilter(filter2.data());
    connection->installCommandFilter(filter3.data());

    connection->sendCommand(IrcCommand::createJoin("#freenode"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(protocol->written);

    protocol->written = false;
    filter1->clear(); filter2->clear(); filter3->clear();
    filter3->commandFilterEnabled = true;

    connection->sendCommand(IrcCommand::createJoin("#communi"));
    QCOMPARE(filter1->commandFiltered, 0);
    QCOMPARE(filter2->commandFiltered, 0);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(!protocol->written);

    protocol->written = false;
    filter1->clear(); filter2->clear(); filter3->clear();
    filter2->commandFilterEnabled = true;

    connection->sendCommand(IrcCommand::createJoin("#qt"));
    QCOMPARE(filter1->commandFiltered, 0);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(!protocol->written);

    protocol->written = false;
    filter1->clear(); filter2->clear(); filter3->clear();
    filter1->commandFilterEnabled = true;

    connection->sendCommand(IrcCommand::createPart("#freenode"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(!protocol->written);

    protocol->written = false;
    filter1->clear(); filter2->clear(); filter3->clear();

    connection->sendCommand(IrcCommand::createPart("#communi"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter2->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(protocol->written);

    // a deleted filter gets removed
    filter2.reset();
    filter1->clear(); filter3->clear();
    protocol->written = false;

    connection->sendCommand(IrcCommand::createPart("#qt"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 1);
    QVERIFY(protocol->written);

    // double filters
    connection->installCommandFilter(filter1.data());
    connection->installCommandFilter(filter3.data());
    filter1->clear(); filter3->clear();
    protocol->written = false;

    connection->sendCommand(IrcCommand::createJoin("#freenode"));
    QCOMPARE(filter1->commandFiltered, 2);
    QCOMPARE(filter3->commandFiltered, 2);
    QVERIFY(protocol->written);

    // remove & enable double filter
    filter1->clear(); filter3->clear();
    filter1->commandFilterEnabled = true;
    connection->removeCommandFilter(filter3.data());
    protocol->written = false;

    connection->sendCommand(IrcCommand::createJoin("#communi"));
    QCOMPARE(filter1->commandFiltered, 1);
    QCOMPARE(filter3->commandFiltered, 0);
    QVERIFY(!protocol->written);

    // remove & delete
    filter3.reset();
    filter1->clear();
    connection->removeCommandFilter(filter1.data());
    protocol->written = false;

    connection->sendCommand(IrcCommand::createJoin("#qt"));
    QCOMPARE(filter1->commandFiltered, 0);
    QVERIFY(protocol->written);
}

QTEST_MAIN(tst_IrcConnection)

#include "tst_ircconnection.moc"
