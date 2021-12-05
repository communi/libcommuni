/*
 * Copyright (C) 2008-2020 The Communi Project
 *
 * This test is free, and not covered by the BSD license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircnetwork.h"
#include "irccommand.h"
#include "ircconnection.h"
#include <QtTest/QtTest>
#include "tst_ircclientserver.h"
#include "tst_ircdata.h"
#ifdef Q_OS_LINUX
#include "ircnetwork_p.h"
#endif // Q_OS_LINUX

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
namespace Qt {
    const QString::SplitBehavior SkipEmptyParts = QString::SkipEmptyParts;
}
#endif

class tst_IrcNetwork : public tst_IrcClientServer
{
    Q_OBJECT

private slots:
    void testDefaults();

    void testInfo_data();
    void testInfo();

    void testCapabilities_data();
    void testCapabilities();

    void testCapNotify();

    void testDebug();
};

void tst_IrcNetwork::testDefaults()
{
    IrcConnection connection;
    IrcNetwork* network = connection.network();
    QVERIFY(!network->isInitialized());
    QVERIFY(network->name().isNull());
    QCOMPARE(network->modes(), QStringList() << "o" << "v");
    QCOMPARE(network->prefixes(), QStringList() << "@" << "+");
    QCOMPARE(network->channelTypes(), QStringList() << "#");
    QVERIFY(network->availableCapabilities().isEmpty());
    QVERIFY(network->requestedCapabilities().isEmpty());
    QVERIFY(network->activeCapabilities().isEmpty());
}

void tst_IrcNetwork::testInfo_data()
{
    QTest::addColumn<QByteArray>("welcome");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("modes");
    QTest::addColumn<QString>("prefixes");
    QTest::addColumn<QString>("channelTypes");

    QTest::newRow("libera") << tst_IrcData::welcome("libera") << "libera" << "ov" << "@+" << "#";
    QTest::newRow("ircnet") << tst_IrcData::welcome("ircnet") << "IRCNet" << "ov" << "@+" << "#&!+";
    QTest::newRow("euirc") << tst_IrcData::welcome("euirc") << "euIRCnet" << "qaohv" << "*!@%+" << "#&+";
}

void tst_IrcNetwork::testInfo()
{
    QFETCH(QByteArray, welcome);
    QFETCH(QString, name);
    QFETCH(QString, modes);
    QFETCH(QString, prefixes);
    QFETCH(QString, channelTypes);

    IrcNetwork* network = connection->network();

    QSignalSpy initSpy(network, SIGNAL(initialized()));
    QSignalSpy nameSpy(network, SIGNAL(nameChanged(QString)));
    QSignalSpy modesSpy(network, SIGNAL(modesChanged(QStringList)));
    QSignalSpy prefixesSpy(network, SIGNAL(prefixesChanged(QStringList)));
    QSignalSpy channelTypesSpy(network, SIGNAL(channelTypesChanged(QStringList)));

    QVERIFY(initSpy.isValid());
    QVERIFY(nameSpy.isValid());
    QVERIFY(modesSpy.isValid());
    QVERIFY(prefixesSpy.isValid());
    QVERIFY(channelTypesSpy.isValid());

    connection->open();
    QVERIFY(waitForOpened());
    QVERIFY(waitForWritten(welcome));

    QVERIFY(network->isInitialized());

    QCOMPARE(network->name(), name);
    QCOMPARE(network->modes(), modes.split("", Qt::SkipEmptyParts));
    QCOMPARE(network->prefixes(), prefixes.split("", Qt::SkipEmptyParts));
    QCOMPARE(network->channelTypes(), channelTypes.split("", Qt::SkipEmptyParts));

    QCOMPARE(network->prefixes().count(), network->modes().count());
    for (int i = 0; i < network->prefixes().count(); ++i) {
        QString prefix = network->prefixes().at(i);
        QString mode = network->modes().at(i);
        QCOMPARE(network->prefixToMode(prefix), mode);
        QCOMPARE(network->modeToPrefix(mode), prefix);
    }

    QVERIFY(!network->channelTypes().isEmpty());
    QVERIFY(!network->isChannel("foo"));
    QVERIFY(network->isChannel(network->channelTypes().at(0) + "foo"));

    QVERIFY(!network->channelModes(IrcNetwork::TypeA).isEmpty());
    QVERIFY(!network->channelModes(IrcNetwork::TypeB).isEmpty());
    QVERIFY(!network->channelModes(IrcNetwork::TypeC).isEmpty());
    QVERIFY(!network->channelModes(IrcNetwork::TypeD).isEmpty());

    if (welcome.contains("NICKLEN="))
        QVERIFY(network->numericLimit(IrcNetwork::NickLength) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::NickLength), -1);
    if (welcome.contains("CHANNELLEN="))
        QVERIFY(network->numericLimit(IrcNetwork::ChannelLength) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::ChannelLength), -1);
    if (welcome.contains("TOPICLEN="))
        QVERIFY(network->numericLimit(IrcNetwork::TopicLength) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::TopicLength), -1);
    if (welcome.contains("KICKLEN="))
        QVERIFY(network->numericLimit(IrcNetwork::KickReasonLength) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::KickReasonLength), -1);
    if (welcome.contains("AWAYLEN="))
        QVERIFY(network->numericLimit(IrcNetwork::AwayReasonLength) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::AwayReasonLength), -1);
    if (welcome.contains("MODES="))
        QVERIFY(network->numericLimit(IrcNetwork::ModeCount) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::ModeCount), -1);
    if (welcome.contains("MONITOR="))
        QVERIFY(network->numericLimit(IrcNetwork::MonitorCount) != -1);
    else
        QCOMPARE(network->numericLimit(IrcNetwork::MonitorCount), -1);

    QCOMPARE(network->numericLimit(IrcNetwork::MessageLength), 512); // hard-coded :/

    if (welcome.contains("MAXLIST=")) {
        bool limited = false;
        foreach (const QString& mode, network->modes())
        if (network->modeLimit(mode) != -1)
            limited = true;
        QVERIFY(limited);
    }

    if (welcome.contains("CHANLIMIT=")) {
        bool limited = false;
        foreach (const QString& type, network->channelTypes())
            if (network->channelLimit(type) != -1)
                limited = true;
        QVERIFY(limited);
    }

    if (welcome.contains("TARGMAX=")) {
        bool limited = false;
        IrcCommand command;
        for (int i = IrcCommand::Admin; i <= IrcCommand::Whowas; ++i) {
            if (i != IrcCommand::Custom) {
                command.setType(static_cast<IrcCommand::Type>(i));
                QString cmd = command.toString().split(QStringLiteral(" "), Qt::SkipEmptyParts).value(0);
                if (network->targetLimit(cmd) != -1)
                    limited = true;
            }
        }
        QVERIFY(limited);
    }

    QCOMPARE(initSpy.count(), 1);
    QCOMPARE(nameSpy.count(), 1);
    QCOMPARE(nameSpy.first().first().toString(), name);

    bool defaultModes = network->modes() == IrcConnection().network()->modes();
    QCOMPARE(modesSpy.count(), defaultModes ? 0 : 1);
    if (!defaultModes)
        QCOMPARE(modesSpy.first().first().toStringList(), modes.split("", Qt::SkipEmptyParts));

    bool defaultPrefixes = network->prefixes() == IrcConnection().network()->prefixes();
    QCOMPARE(prefixesSpy.count(), defaultPrefixes ? 0 : 1);
    if (!defaultPrefixes)
        QCOMPARE(prefixesSpy.first().first().toStringList(), prefixes.split("", Qt::SkipEmptyParts));

    bool defaultTypes = network->channelTypes() == IrcConnection().network()->channelTypes();
    QCOMPARE(channelTypesSpy.count(), defaultTypes ? 0 : 1);
    if (!defaultTypes)
        QCOMPARE(channelTypesSpy.first().first().toStringList(), channelTypes.split("", Qt::SkipEmptyParts));
}

void tst_IrcNetwork::testCapabilities_data()
{
    QTest::addColumn<QString>("initialCaps");
    QTest::addColumn<QString>("requestedCaps");
    QTest::addColumn<QString>("ackedCaps");
    QTest::addColumn<QString>("nakedCaps");
    QTest::addColumn<QString>("listedCaps");
    QTest::addColumn<QString>("availableCaps");
    QTest::addColumn<QString>("activeCaps");

    QTest::newRow("empty")     << QString()
                               << QString()
                               << QString()
                               << QString()
                               << QString()
                               << QString()
                               << QString();

    QTest::newRow("sasl")      << QStringLiteral("multi-prefix sasl identify-msg") // initial
                               << QStringLiteral("sasl") // requested
                               << QStringLiteral("sasl") // acked
                               << QString() // naked
                               << QStringLiteral("sasl identify-msg multi-prefix") // listed
                               << QStringLiteral("sasl identify-msg multi-prefix") // available
                               << QStringLiteral("sasl"); // active

    QTest::newRow("sasl mech") << QStringLiteral("multi-prefix sasl=PLAIN,EXTERNAL identify-msg") // initial
                               << QStringLiteral("sasl") // requested
                               << QStringLiteral("sasl") // acked
                               << QString() // naked
                               << QStringLiteral("sasl=PLAIN,EXTERNAL identify-msg multi-prefix") // listed
                               << QStringLiteral("sasl=PLAIN,EXTERNAL identify-msg multi-prefix") // available
                               << QStringLiteral("sasl"); // active

    QTest::newRow("saslnmech") << QStringLiteral("multi-prefix sasl=EXTERNAL identify-msg") // initial
                               << QStringLiteral("") // requested
                               << QStringLiteral("") // acked
                               << QString() // naked
                               << QStringLiteral("sasl=EXTERNAL identify-msg multi-prefix") // listed
                               << QStringLiteral("sasl=EXTERNAL identify-msg multi-prefix") // available
                               << QStringLiteral(""); // active

    QTest::newRow("unk")       << QStringLiteral("multi-prefix sasl identify-msg") // initial
                               << QStringLiteral("unk") // requested
                               << QString() // acked
                               << QStringLiteral("nak") // naked
                               << QStringLiteral("multi-prefix sasl identify-msg") // listed
                               << QStringLiteral("multi-prefix sasl identify-msg") // available
                               << QString(); // active

    QTest::newRow("nak all")   << QStringLiteral("multi-prefix sasl identify-msg") // initial
                               << QStringLiteral("sasl identify-msg multi-prefix") // requested
                               << QString() // acked
                               << QStringLiteral("sasl identify-msg multi-prefix") // naked
                               << QStringLiteral("sasl identify-msg multi-prefix") // listed
                               << QStringLiteral("sasl identify-msg multi-prefix") // available
                               << QString(); // active

    QTest::newRow("sticky")    << QStringLiteral("=sticky") // initial
                               << QStringLiteral("sticky") // requested
                               << QStringLiteral("=sticky") // acked
                               << QString() // naked
                               << QString() // listed
                               << QStringLiteral("sticky") // available
                               << QStringLiteral("sticky"); // active

    QTest::newRow("ackmod")    << QStringLiteral("~ackmod") // initial
                               << QStringLiteral("ackmod") // requested
                               << QStringLiteral("~ackmod") // acked
                               << QString() // naked
                               << QString() // listed
                               << QStringLiteral("ackmod") // available
                               << QStringLiteral("ackmod"); // active

    QTest::newRow("acksticky") << QStringLiteral("~=acksticky") // initial
                               << QStringLiteral("acksticky") // requested
                               << QStringLiteral("=~acksticky") // acked
                               << QString() // naked
                               << QString() // listed
                               << QStringLiteral("acksticky") // available
                               << QStringLiteral("acksticky"); // active
}

static QStringList sorted(QStringList lst)
{
    std::sort(lst.begin(), lst.end());
    return lst;
}

static bool equalCaps(const QString& left, const QString& right)
{
    return sorted(left.split(QStringLiteral(" "), Qt::SkipEmptyParts)) == sorted(right.split(QStringLiteral(" "), Qt::SkipEmptyParts));
}

void tst_IrcNetwork::testCapabilities()
{
    QFETCH(QString, initialCaps);
    QFETCH(QString, requestedCaps);
    QFETCH(QString, ackedCaps);
    QFETCH(QString, nakedCaps);
    QFETCH(QString, listedCaps);
    QFETCH(QString, availableCaps);
    QFETCH(QString, activeCaps);

    IrcNetwork* network = connection->network();

    QSignalSpy availableSpy(network, SIGNAL(availableCapabilitiesChanged(QStringList)));
    QSignalSpy requestedSpy(network, SIGNAL(requestedCapabilitiesChanged(QStringList)));
    QSignalSpy activeSpy(network, SIGNAL(activeCapabilitiesChanged(QStringList)));
    QSignalSpy requestingSpy(network, SIGNAL(requestingCapabilities()));

    QVERIFY(availableSpy.isValid());
    QVERIFY(requestedSpy.isValid());
    QVERIFY(activeSpy.isValid());
    QVERIFY(requestingSpy.isValid());

    int availableCount = 0;
    int requestedCount = 0;
    int activeCount = 0;
    int requestingCount = 0;

    if (!requestedCaps.isEmpty()) {
        network->setRequestedCapabilities(requestedCaps.split(QStringLiteral(" "), Qt::SkipEmptyParts));
        ++requestedCount;
    }
    QCOMPARE(requestedSpy.count(), requestedCount);

    foreach (const QString& cap, availableCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(!network->hasCapability(cap));
    foreach (const QString& cap, activeCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(!network->isCapable(cap));

    connection->open();
    QVERIFY(waitForOpened());

    if (!initialCaps.isEmpty()) {
        // typical ircd: "*", znc: "unknown-nick"
        QVERIFY(waitForWritten(":irc.ser.ver CAP unknown-nick LS :" + initialCaps.toUtf8()));
        ++availableCount;
        ++requestingCount;
    }
    QCOMPARE(availableSpy.count(), availableCount);
    QCOMPARE(requestingSpy.count(), requestingCount);

    foreach (const QString& cap, availableCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(network->hasCapability(cap));
    foreach (const QString& cap, activeCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(!network->isCapable(cap));

    if (!ackedCaps.isEmpty()) {
        QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi ACK :" + ackedCaps.toUtf8()));
        ++activeCount;
    }
    QCOMPARE(activeSpy.count(), activeCount);

    foreach (const QString& cap, availableCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(network->hasCapability(cap));
    foreach (const QString& cap, activeCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(network->isCapable(cap));

    if (!nakedCaps.isEmpty())
        QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi NAK :" + nakedCaps.toUtf8()));

    QVERIFY(waitForWritten(tst_IrcData::welcome()));

    if (!listedCaps.isEmpty()) {
        QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi LS :" + listedCaps.toUtf8()));
        if (!equalCaps(listedCaps, initialCaps))
            ++availableCount;
    }
    QCOMPARE(availableSpy.count(), availableCount);

    QVERIFY(equalCaps(network->availableCapabilities().join(" "), availableCaps));
    QVERIFY(equalCaps(network->activeCapabilities().join(" "), activeCaps));
    QVERIFY(equalCaps(network->requestedCapabilities().join(" "), requestedCaps));

    QCOMPARE(requestedSpy.count(), requestedCount);
    QCOMPARE(requestingSpy.count(), requestingCount);
    QCOMPARE(availableSpy.count(), availableCount);
    QCOMPARE(activeSpy.count(), activeCount);

    foreach (const QString& cap, availableCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(network->hasCapability(cap));
    foreach (const QString& cap, activeCaps.split(" ", Qt::SkipEmptyParts))
        QVERIFY(network->isCapable(cap));

    // -> CLEAR
    QString clearCaps;
    foreach (const QString& cap, activeCaps.split(" ", Qt::SkipEmptyParts))
        clearCaps += QStringLiteral("-") + cap;
    QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi ACK :" + clearCaps.toUtf8()));

    if (!activeCaps.isEmpty())
        ++activeCount;
    QCOMPARE(activeSpy.count(), activeCount);
}

void tst_IrcNetwork::testCapNotify()
{
    connection->open();
    QVERIFY(waitForOpened());

    IrcNetwork* network = connection->network();
    network->setRequestedCapabilities(QStringList() << QStringLiteral("cap-notify") << QStringLiteral("away-notify"));

    QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi LS :cap-notify"));

    QSignalSpy availableSpy(network, SIGNAL(availableCapabilitiesChanged(QStringList)));
    QSignalSpy activeSpy(network, SIGNAL(activeCapabilitiesChanged(QStringList)));

    QVERIFY(availableSpy.isValid());
    QVERIFY(activeSpy.isValid());

    QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi NEW :away-notify foo-bar"));
    QCOMPARE(availableSpy.count(), 1);
    QVERIFY(network->hasCapability("away-notify"));
    QVERIFY(network->hasCapability("foo-bar"));
    QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi ACK :away-notify"));
    QCOMPARE(activeSpy.count(), 1);
    QVERIFY(network->isCapable("away-notify"));

    QVERIFY(waitForWritten(":irc.ser.ver CAP jpnurmi DEL :away-notify"));
    QCOMPARE(availableSpy.count(), 2);
    QCOMPARE(activeSpy.count(), 2);
    QVERIFY(!network->isCapable("away-notify"));
    QVERIFY(!network->hasCapability("away-notify"));
    QVERIFY(network->hasCapability("foo-bar"));
}

void tst_IrcNetwork::testDebug()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcNetwork*>(nullptr);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcNetwork(0x0)"));
    str.clear();

    IrcConnection connection;
    dbg << connection.network();
    QVERIFY(QRegularExpression("IrcNetwork\\(0x[0-9A-Fa-f]+\\) ").match(str).hasMatch());
    str.clear();

    connection.network()->setObjectName(QStringLiteral("obj"));
    dbg << connection.network();
    QVERIFY(QRegularExpression("IrcNetwork\\(0x[0-9A-Fa-f]+, name=obj\\) ").match(str).hasMatch());
    str.clear();

#ifdef Q_OS_LINUX
    // others have problems with symbols (win) or private headers (osx frameworks)
    IrcNetworkPrivate::get(connection.network())->name = QLatin1String("net");
    dbg << connection.network();
    QVERIFY(QRegularExpression("IrcNetwork\\(0x[0-9A-Fa-f]+, name=obj, network=net\\) ").match(str).hasMatch());
    str.clear();
#endif // Q_OS_LINUX

    dbg << IrcNetwork::MessageLength;
    QCOMPARE(str.trimmed(), QString::fromLatin1("MessageLength"));
    str.clear();

    dbg << IrcNetwork::AllTypes;
    QCOMPARE(str.trimmed(), QString::fromLatin1("AllTypes"));
    str.clear();

    dbg << (IrcNetwork::TypeA | IrcNetwork::TypeD);
    QCOMPARE(str.trimmed(), QString::fromLatin1("(TypeA|TypeD)"));
    str.clear();

    dbg << (IrcNetwork::TypeB | IrcNetwork::TypeC);
    QCOMPARE(str.trimmed(), QString::fromLatin1("(TypeB|TypeC)"));
    str.clear();

    dbg << (IrcNetwork::TypeA | IrcNetwork::TypeB | IrcNetwork::TypeC | IrcNetwork::TypeD);
    QCOMPARE(str.trimmed(), QString::fromLatin1("(AllTypes)"));
    str.clear();
}

QTEST_MAIN(tst_IrcNetwork)

#include "tst_ircnetwork.moc"
