/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircnetwork.h"
#include "ircconnection.h"
#include <QtTest/QtTest>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class tst_IrcNetwork : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testDefaults();

    void testInfo_data();
    void testInfo();

    void testCapabilities_data();
    void testCapabilities();

private:
    QTcpServer server;
};

void tst_IrcNetwork::initTestCase()
{
    QVERIFY(server.listen());
}

void tst_IrcNetwork::cleanupTestCase()
{
    server.close();
}

void tst_IrcNetwork::testDefaults()
{
    IrcConnection connection;
    IrcNetwork* network = connection.network();
    QVERIFY(network->name().isNull());
    QVERIFY(network->modes().isEmpty());
    QVERIFY(network->prefixes().isEmpty());
    QVERIFY(network->channelTypes().isEmpty());
    QVERIFY(network->availableCapabilities().isEmpty());
    QVERIFY(network->requestedCapabilities().isEmpty());
    QVERIFY(network->activeCapabilities().isEmpty());
}

void tst_IrcNetwork::testInfo_data()
{
    QTest::addColumn<QStringList>("lines");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("modes");
    QTest::addColumn<QString>("prefixes");
    QTest::addColumn<QString>("channelTypes");

    QStringList freenode;
    freenode += ":adams.freenode.net 001 jpnurmi :Welcome to the freenode Internet Relay Chat Network jpnurmi";
    freenode += ":adams.freenode.net 005 jpnurmi CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode KNOCK STATUSMSG=@+ CALLERID=g :are supported by this server";
    freenode += ":adams.freenode.net 005 jpnurmi CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=100 FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: :are supported by this server";
    freenode += ":adams.freenode.net 005 jpnurmi EXTBAN=$,arxz WHOX CLIENTVER=3.0 SAFELIST ELIST=CTU :are supported by this server";

    QStringList ircnet;
    ircnet += ":irc.cc.tut.fi 001 jpnurmi :Welcome to the Internet Relay Network jpnurmi!jpnurmi@xxx.xxx.fi";
    ircnet += ":irc.cc.tut.fi 005 jpnurmi RFC2812 PREFIX=(ov)@+ CHANTYPES=#&!+ MODES=3 CHANLIMIT=#&!+:21 NICKLEN=15 TOPICLEN=255 KICKLEN=255 MAXLIST=beIR:64 CHANNELLEN=50 IDCHAN=!:5 CHANMODES=beIR,k,l,imnpstaqr :are supported by this server";
    ircnet += ":irc.cc.tut.fi 005 jpnurmi PENALTY FNC EXCEPTS=e INVEX=I CASEMAPPING=ascii NETWORK=IRCnet :are supported by this server";

    QStringList euirc;
    euirc += ":irc.inn.at.euirc.net 001 jipsu :Welcome to the euIRCnet IRC Network jipsu!~communi@xxx.xxx.no";
    euirc += ":irc.inn.at.euirc.net 005 jipsu NETWORK=euIRCnet WATCH=128 SAFELIST PREFIX=(qaohv)*!@%+ CHANMODES=bewI,k,flBL,cimnprstxACHKNOQRiTSVWXY CHANTYPES=#&+ KICKLEN=307 KNOCK MAP MAXLIST=bewI:100 MODES=6 NICKLEN=30 SILENCE=5 TOPICLEN=307 AWAYLEN=307 WALLCHOPS CHANNELLEN=32 MAXCHANNELS=30 MAXTARGETS=20 INVEX=I EXCEPT=e :are available on this server";
    euirc += ":irc.inn.at.euirc.net 005 jipsu STARTTLS :are available on this server";

    QTest::newRow("freenode") << freenode << "freenode" << "ov" << "@+" << "#";
    QTest::newRow("ircnet") << ircnet << "IRCnet" << "ov" << "@+" << "#&!+";
    QTest::newRow("euirc") << euirc << "euIRCnet" << "qaohv" << "*!@%+" << "#&+";
}

void tst_IrcNetwork::testInfo()
{
    QFETCH(QStringList, lines);
    QFETCH(QString, name);
    QFETCH(QString, modes);
    QFETCH(QString, prefixes);
    QFETCH(QString, channelTypes);

    IrcConnection connection;
    connection.setUserName("user");
    connection.setNickName("nick");
    connection.setRealName("real");
    connection.setHost(server.serverAddress().toString());
    connection.setPort(server.serverPort());

    IrcNetwork* network = connection.network();

    QSignalSpy nameSpy(network, SIGNAL(nameChanged(QString)));
    QSignalSpy modesSpy(network, SIGNAL(modesChanged(QStringList)));
    QSignalSpy prefixesSpy(network, SIGNAL(prefixesChanged(QStringList)));
    QSignalSpy channelTypesSpy(network, SIGNAL(channelTypesChanged(QStringList)));

    QVERIFY(nameSpy.isValid());
    QVERIFY(modesSpy.isValid());
    QVERIFY(prefixesSpy.isValid());
    QVERIFY(channelTypesSpy.isValid());

    connection.open();
    if (!server.waitForNewConnection(200))
        QSKIP("The address is not available");
    QTcpSocket* serverSocket = server.nextPendingConnection();
    QVERIFY(serverSocket);

    QVERIFY(connection.socket()->waitForConnected());

    foreach (const QString& line, lines)
        QVERIFY(serverSocket->write(line.toUtf8() + "\r\n"));
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(connection.socket()->waitForReadyRead());

    QCOMPARE(network->name(), name);
    QCOMPARE(network->modes(), modes.split("", QString::SkipEmptyParts));
    QCOMPARE(network->prefixes(), prefixes.split("", QString::SkipEmptyParts));
    QCOMPARE(network->channelTypes(), channelTypes.split("", QString::SkipEmptyParts));

    QCOMPARE(nameSpy.count(), 1);
    QCOMPARE(modesSpy.count(), 1);
    QCOMPARE(prefixesSpy.count(), 1);
    QCOMPARE(channelTypesSpy.count(), 1);

    QCOMPARE(nameSpy.first().first().toString(), name);
    QCOMPARE(modesSpy.first().first().toStringList(), modes.split("", QString::SkipEmptyParts));
    QCOMPARE(prefixesSpy.first().first().toStringList(), prefixes.split("", QString::SkipEmptyParts));
    QCOMPARE(channelTypesSpy.first().first().toStringList(), channelTypes.split("", QString::SkipEmptyParts));
}

void tst_IrcNetwork::testCapabilities_data()
{
    QTest::addColumn<QStringList>("availableCaps");
    QTest::addColumn<QStringList>("requestedCaps");
    QTest::addColumn<QStringList>("activeCaps");

    QTest::newRow("empty") << QStringList() << QStringList() << QStringList();
    QTest::newRow("none")  << QString("multi-prefix sasl identify-msg").split(" ", QString::SkipEmptyParts)
                           << QString("sasl").split("", QString::SkipEmptyParts)
                           << QStringList();
    QTest::newRow("sasl")  << QString("multi-prefix sasl identify-msg").split(" ", QString::SkipEmptyParts)
                           << QStringList("sasl")
                           << QStringList("sasl");
    QTest::newRow("unk")   << QString("multi-prefix sasl identify-msg").split(" ", QString::SkipEmptyParts)
                           << QStringList("unk")
                           << QStringList();
}

void tst_IrcNetwork::testCapabilities()
{
    QFETCH(QStringList, availableCaps);
    QFETCH(QStringList, requestedCaps);
    QFETCH(QStringList, activeCaps);

    IrcConnection connection;
    connection.setUserName("user");
    connection.setNickName("nick");
    connection.setRealName("real");
    connection.setHost(server.serverAddress().toString());
    connection.setPort(server.serverPort());

    IrcNetwork* network = connection.network();
    network->setRequestedCapabilities(requestedCaps);

    connection.open();
    if (!server.waitForNewConnection(200))
        QSKIP("The address is not available");
    QTcpSocket* serverSocket = server.nextPendingConnection();
    QVERIFY(serverSocket);
    QAbstractSocket* clientSocket = connection.socket();
    QVERIFY(clientSocket);

    QVERIFY(clientSocket->waitForConnected());
    QVERIFY(clientSocket->waitForBytesWritten());
    QVERIFY(serverSocket->waitForReadyRead());

    // "CAP LS\r\n”
    // "NICK nick\r\n"
    // "USER user hostname servername :real\r\n"
    QVERIFY(!serverSocket->readAll().isEmpty());

    QVERIFY(serverSocket->write(":irc.ser.ver CAP * LS :" + availableCaps.join(" ").toUtf8() + "\r\n"));
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(clientSocket->waitForReadyRead());

    if (!requestedCaps.isEmpty()) {
        // -> "CAP REQ :<requestedCaps>\r\n"
        // -> "CAP END\r\n"
        QVERIFY(clientSocket->waitForBytesWritten());
        QVERIFY(serverSocket->waitForReadyRead());
    }

    QVERIFY(serverSocket->write(":irc.ser.ver CAP jpnurmi ACK :" + activeCaps.join(" ").toUtf8() + "\r\n"));
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(clientSocket->waitForReadyRead());

    QCOMPARE(network->availableCapabilities().toSet(), availableCaps.toSet());
    QCOMPARE(network->activeCapabilities().toSet(), activeCaps.toSet());
    QCOMPARE(network->requestedCapabilities().toSet(), requestedCaps.toSet());
}

QTEST_MAIN(tst_IrcNetwork)

#include "tst_ircnetwork.moc"
