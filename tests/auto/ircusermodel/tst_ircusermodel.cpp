/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircusermodel.h"
#include "ircconnection.h"
#include "ircbuffermodel.h"
#include "ircchannel.h"
#include "ircuser.h"
#include "irc.h"

#include "tst_freenode.h"
#include "tst_ircnet.h"
#include "tst_euirc.h"

#include <QtTest/QtTest>
#include <QtCore/QPointer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

static bool caseInsensitiveLessThan(const QString& s1, const QString& s2)
{
    return s1.compare(s2, Qt::CaseInsensitive) < 0;
}

static bool caseInsensitiveGreaterThan(const QString& s1, const QString& s2)
{
    return s1.compare(s2, Qt::CaseInsensitive) > 0;
}

class tst_IrcUserModel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testDefaults();
    void testSorting_data();
    void testSorting();
    void testActivity_freenode();
    void testActivity_ircnet();
    void testActivity_euirc();

private:
    void waitForWritten(const QByteArray& data = QByteArray());

    QPointer<QTcpServer> server;
    QPointer<QTcpSocket> serverSocket;
    QPointer<IrcConnection> connection;
    QPointer<QAbstractSocket> clientSocket;
};

void tst_IrcUserModel::init()
{
    server = new QTcpServer(this);
    QVERIFY(server->listen());

    connection = new IrcConnection(this);
    connection->setUserName("user");
    connection->setNickName("nick");
    connection->setRealName("real");
    connection->setHost(server->serverAddress().toString());
    connection->setPort(server->serverPort());

    connection->open();
    if (!server->waitForNewConnection(200))
        QEXPECT_FAIL("", "The address is not available", Abort);
    serverSocket = server->nextPendingConnection();
    QVERIFY(serverSocket);

    clientSocket = connection->socket();
    QVERIFY(clientSocket);
    QVERIFY(clientSocket->waitForConnected());
}

void tst_IrcUserModel::cleanup()
{
    delete server;
    delete connection;
}

void tst_IrcUserModel::waitForWritten(const QByteArray& data)
{
    if (!data.isNull())
        serverSocket->write(data);
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(clientSocket->waitForReadyRead());
}

void tst_IrcUserModel::testDefaults()
{
    IrcUserModel model;
    QCOMPARE(model.count(), 0);
    QVERIFY(model.names().isEmpty());
    QVERIFY(model.users().isEmpty());
    QCOMPARE(model.displayRole(), Irc::TitleRole);
    QVERIFY(!model.channel());
    QVERIFY(!model.dynamicSort());
}

void tst_IrcUserModel::testSorting_data()
{
    QTest::addColumn<QString>("welcomeData");
    QTest::addColumn<QString>("joinData");
    QTest::addColumn<QStringList>("names");
    QTest::addColumn<QStringList>("admins");
    QTest::addColumn<QStringList>("ops");
    QTest::addColumn<QStringList>("halfops");
    QTest::addColumn<QStringList>("voices");

    QTest::newRow("freenode")
        << QString::fromUtf8(freenode_welcome)
        << QString::fromUtf8(freenode_join)
        << QString::fromUtf8(freenode_names).split(" ")
        << QString::fromUtf8(freenode_admins).split(" ")
        << QString::fromUtf8(freenode_ops).split(" ")
        << QString::fromUtf8(freenode_halfops).split(" ")
        << QString::fromUtf8(freenode_voices).split(" ");

    QTest::newRow("ircnet")
        << QString::fromUtf8(ircnet_welcome)
        << QString::fromUtf8(ircnet_join)
        << QString::fromUtf8(ircnet_names).split(" ")
        << QString::fromUtf8(ircnet_admins).split(" ")
        << QString::fromUtf8(ircnet_ops).split(" ")
        << QString::fromUtf8(ircnet_halfops).split(" ")
        << QString::fromUtf8(ircnet_voices).split(" ");

    QTest::newRow("euirc")
        << QString::fromUtf8(euirc_welcome)
        << QString::fromUtf8(euirc_join)
        << QString::fromUtf8(euirc_names).split(" ")
        << QString::fromUtf8(euirc_admins).split(" ")
        << QString::fromUtf8(euirc_ops).split(" ")
        << QString::fromUtf8(euirc_halfops).split(" ")
        << QString::fromUtf8(euirc_voices).split(" ");
}

void tst_IrcUserModel::testSorting()
{
    if (!serverSocket)
        QSKIP("The address is not available");

    QFETCH(QString, welcomeData);
    QFETCH(QString, joinData);
    QFETCH(QStringList, names);
    QFETCH(QStringList, admins);
    QFETCH(QStringList, ops);
    QFETCH(QStringList, halfops);
    QFETCH(QStringList, voices);

    IrcBufferModel bufferModel;
    bufferModel.setConnection(connection);

    waitForWritten(welcomeData.toUtf8());
    QCOMPARE(bufferModel.count(), 0);

    waitForWritten(joinData.toUtf8());

    QCOMPARE(bufferModel.count(), 1);
    IrcChannel* channel = bufferModel.get(0)->toChannel();
    QVERIFY(channel);

    IrcUserModel staticModel(channel);
    staticModel.setDynamicSort(false);
    QCOMPARE(staticModel.count(), names.count());
    for (int i = 0; i < staticModel.count(); ++i) {
        IrcUser* user = staticModel.get(i);
        QCOMPARE(user->name(), names.at(i));
        if (admins.contains(user->name())) {
            QCOMPARE(user->mode(), QString("a"));
            QCOMPARE(user->prefix(), QString("!"));
        } else if (ops.contains(user->name())) {
            QCOMPARE(user->mode(), QString("o"));
            QCOMPARE(user->prefix(), QString("@"));
        } else if (halfops.contains(user->name())) {
            QCOMPARE(user->mode(), QString("h"));
            QCOMPARE(user->prefix(), QString("%"));
        } else if (voices.contains(user->name())) {
            QCOMPARE(user->mode(), QString("v"));
            QCOMPARE(user->prefix(), QString("+"));
        }
    }

    QStringList sorted = names;
    qSort(sorted);
    QCOMPARE(staticModel.names(), sorted);

    // STATIC - BY NAME - ASCENDING
    staticModel.setSortMethod(Irc::SortByName);
    staticModel.sort(0, Qt::AscendingOrder);

    QStringList nasc = names;
    qSort(nasc.begin(), nasc.end(), caseInsensitiveLessThan);

    for (int i = 0; i < staticModel.count(); ++i)
        QCOMPARE(staticModel.get(i)->name(), nasc.at(i));

    // STATIC - BY NAME - DESCENDING
    staticModel.setSortMethod(Irc::SortByName);
    staticModel.sort(0, Qt::DescendingOrder);

    QStringList ndesc = names;
    qSort(ndesc.begin(), ndesc.end(), caseInsensitiveGreaterThan);

    for (int i = 0; i < staticModel.count(); ++i)
        QCOMPARE(staticModel.get(i)->name(), ndesc.at(i));

    // STATIC - BY TITLE - ASCENDING
    staticModel.setSortMethod(Irc::SortByTitle);
    staticModel.sort(0, Qt::AscendingOrder);

    QStringList aasc = admins;
    qSort(aasc.begin(), aasc.end(), caseInsensitiveLessThan);

    QStringList oasc = ops;
    qSort(oasc.begin(), oasc.end(), caseInsensitiveLessThan);

    QStringList hasc = halfops;
    qSort(hasc.begin(), hasc.end(), caseInsensitiveLessThan);

    QStringList vasc = voices;
    qSort(vasc.begin(), vasc.end(), caseInsensitiveLessThan);

    QStringList tasc = aasc + oasc + hasc + vasc + nasc;
    // remove duplicates
    foreach (const QString& voice, voices)
        tasc.removeAt(tasc.lastIndexOf(voice));
    foreach (const QString& halfop, halfops)
        tasc.removeAt(tasc.lastIndexOf(halfop));
    foreach (const QString& op, ops)
        tasc.removeAt(tasc.lastIndexOf(op));
    foreach (const QString& admin, admins)
        tasc.removeAt(tasc.lastIndexOf(admin));

    for (int i = 0; i < staticModel.count(); ++i)
        QCOMPARE(staticModel.get(i)->name(), tasc.at(i));

    // STATIC - BY TITLE - DESCENDING
    staticModel.setSortMethod(Irc::SortByTitle);
    staticModel.sort(0, Qt::DescendingOrder);

    QStringList adesc = admins;
    qSort(adesc.begin(), adesc.end(), caseInsensitiveGreaterThan);

    QStringList odesc = ops;
    qSort(odesc.begin(), odesc.end(), caseInsensitiveGreaterThan);

    QStringList hdesc = halfops;
    qSort(hdesc.begin(), hdesc.end(), caseInsensitiveGreaterThan);

    QStringList vdesc = voices;
    qSort(vdesc.begin(), vdesc.end(), caseInsensitiveGreaterThan);

    QStringList tdesc = ndesc + vdesc + hdesc + odesc + adesc;
    // remove duplicates
    foreach (const QString& voice, voices)
        tdesc.removeAt(tdesc.indexOf(voice));
    foreach (const QString& halfop, halfops)
        tdesc.removeAt(tdesc.indexOf(halfop));
    foreach (const QString& op, ops)
        tdesc.removeAt(tdesc.indexOf(op));
    foreach (const QString& admin, admins)
        tdesc.removeAt(tdesc.indexOf(admin));

    for (int i = 0; i < staticModel.count(); ++i)
        QCOMPARE(staticModel.get(i)->name(), tdesc.at(i));

    // DYNAMIC - BY NAME, TITLE & ACTIVITY - ASCENDING
    IrcUserModel dynamicModel(channel);
    dynamicModel.setDynamicSort(true);

    dynamicModel.setSortMethod(Irc::SortByName);
    for (int i = 0; i < dynamicModel.count(); ++i)
        QCOMPARE(dynamicModel.get(i)->name(), nasc.at(i));

    dynamicModel.setSortMethod(Irc::SortByTitle);
    for (int i = 0; i < dynamicModel.count(); ++i)
        QCOMPARE(dynamicModel.get(i)->name(), tasc.at(i));

    dynamicModel.setSortMethod(Irc::SortByActivity);
    for (int i = 0; i < dynamicModel.count(); ++i)
        QCOMPARE(dynamicModel.get(i)->name(), names.at(i));

    // DYNAMIC - BY NAME, TITLE & ACTIVITY - DESCENDING
    dynamicModel.sort(0, Qt::DescendingOrder);

    dynamicModel.setSortMethod(Irc::SortByName);
    for (int i = 0; i < dynamicModel.count(); ++i)
        QCOMPARE(dynamicModel.get(i)->name(), ndesc.at(i));

    dynamicModel.setSortMethod(Irc::SortByTitle);
    for (int i = 0; i < dynamicModel.count(); ++i)
        QCOMPARE(dynamicModel.get(i)->name(), tdesc.at(i));

    dynamicModel.setSortMethod(Irc::SortByActivity);
    for (int i = 0; i < dynamicModel.count(); ++i)
        QCOMPARE(dynamicModel.get(i)->name(), names.at(names.count() - 1 - i));
}

void tst_IrcUserModel::testActivity_freenode()
{
    if (!serverSocket)
        QSKIP("The address is not available");

    IrcBufferModel bufferModel;
    bufferModel.setConnection(connection);

    waitForWritten(freenode_welcome);
    QCOMPARE(bufferModel.count(), 0);

    waitForWritten(freenode_join);

    QCOMPARE(bufferModel.count(), 1);
    IrcChannel* channel = bufferModel.get(0)->toChannel();
    QVERIFY(channel);

    QStringList names = QString::fromUtf8(freenode_names).split(" ");

    IrcUserModel activityModel(channel);
    activityModel.setDynamicSort(true);
    activityModel.setSortMethod(Irc::SortByActivity);

    int count = names.count();

    waitForWritten(":smurfy!~smurfy@hidd.en PART #freenode\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("smurfy"));

    waitForWritten(":ToApolytoXaos!~ToApolyto@hidd.en QUIT :Quit: Leaving\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("ToApolytoXaos"));

    waitForWritten(":agsrv!~guest@hidd.en JOIN #freenode\r\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("agsrv")), 0);

    waitForWritten(":Hello71!~Hello71@hidd.en PRIVMSG #freenode :straterra: there are many users on it\r\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("Hello71")), 0);

    waitForWritten(":straterra!straterra@hidd.en PRIVMSG #freenode :what?\r\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("straterra")), 0);
    QCOMPARE(activityModel.indexOf(activityModel.user("Hello71")), 1);

    waitForWritten(":JuxTApose!~indigital@hidd.en NICK :JuxTApose_afk\r\n");
    QCOMPARE(activityModel.count(), count);
    QVERIFY(!activityModel.contains("JuxTApose"));
    QCOMPARE(activityModel.indexOf(activityModel.user("JuxTApose_afk")), 0);
    QCOMPARE(activityModel.indexOf(activityModel.user("straterra")), 1);
    QCOMPARE(activityModel.indexOf(activityModel.user("Hello71")), 2);
}

void tst_IrcUserModel::testActivity_ircnet()
{
    if (!serverSocket)
        QSKIP("The address is not available");

    IrcBufferModel bufferModel;
    bufferModel.setConnection(connection);

    waitForWritten(ircnet_welcome);
    QCOMPARE(bufferModel.count(), 0);

    waitForWritten(ircnet_join);

    QCOMPARE(bufferModel.count(), 1);
    IrcChannel* channel = bufferModel.get(0)->toChannel();
    QVERIFY(channel);

    QStringList names = QString::fromUtf8(ircnet_names).split(" ");

    IrcUserModel activityModel(channel);
    activityModel.setDynamicSort(true);
    activityModel.setSortMethod(Irc::SortByActivity);

    int count = names.count();

    waitForWritten(":_box!~box@hidd.en QUIT :Broken pipe\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("_box"));

    waitForWritten(":ip!~v6@hidd.en QUIT :Connection reset by peer\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("ip"));

    waitForWritten(":[m]!m@hidd.en MODE #uptimed +b *!*x@does.matter.not*\r\n");
    QCOMPARE(activityModel.count(), count);

    waitForWritten(":[m]!m@hidd.en KICK #uptimed \\x00 :lame exit sorry :P\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("\\x00"));

    waitForWritten(":_box!~box@hidd.en JOIN :#uptimed\r\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("_box")), 0);

    waitForWritten(":Voicer!mrozu@hidd.en MODE #uptimed +v _box\r\n");
    QCOMPARE(activityModel.count(), count);

    waitForWritten(":t0r-!t0r@hidd.en PRIVMSG #uptimed :there is no sense for _box and ip to join the contest\r\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("t0r-")), 0);

    waitForWritten(":ip!~v6@hidd.en JOIN :#uptimed\r\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("ip")), 0);

    waitForWritten(":Voicer!mrozu@hidd.en MODE #uptimed +v ip\r\n");
    QCOMPARE(activityModel.count(), count);

    waitForWritten(":[m]!m@hidd.en MODE #uptimed +b *!*v6@*.does.matter.not\r\n");
    QCOMPARE(activityModel.count(), count);

    waitForWritten(":[m]!m@hidd.en KICK #uptimed ip :no reason\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("ip"));

    waitForWritten(":t0r-!t0r@hidd.en PRIVMSG #uptimed :they are going down every second\r\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("t0r-")), 0);

    waitForWritten(":t0r-!t0r@hidd.en PRIVMSG #uptimed :yeah\r\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("t0r-")), 0);

    waitForWritten(":[m]!m@hidd.en MODE #uptimed +b *!*box@*.does.not.matter\r\n");
    QCOMPARE(activityModel.count(), count);

    waitForWritten(":[m]!m@hidd.en KICK #uptimed _box :no reason\r\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("_box"));
}

void tst_IrcUserModel::testActivity_euirc()
{
    if (!serverSocket)
        QSKIP("The address is not available");

    IrcBufferModel bufferModel;
    bufferModel.setConnection(connection);

    waitForWritten(euirc_welcome);
    QCOMPARE(bufferModel.count(), 0);

    waitForWritten(euirc_join);

    QCOMPARE(bufferModel.count(), 1);
    IrcChannel* channel = bufferModel.get(0)->toChannel();
    QVERIFY(channel);

    QStringList names = QString::fromUtf8(euirc_names).split(" ");

    IrcUserModel activityModel(channel);
    activityModel.setDynamicSort(true);
    activityModel.setSortMethod(Irc::SortByActivity);

    int count = names.count();

    waitForWritten(":Marko10_000!~marko@hidd.en JOIN :#euirc\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("Marko10_000")), 0);

    waitForWritten(":Marko10_000!~marko@hidd.en NICK :Guest775\n");
    QCOMPARE(activityModel.count(), count);
    QVERIFY(!activityModel.contains("Marko10_000"));
    QCOMPARE(activityModel.indexOf(activityModel.user("Guest775")), 0);

    waitForWritten(":Guest775!~marko@hidd.en QUIT :Quit: Verlassend\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("Guest775"));

    waitForWritten(":Marko10_000!~marko@hidd.en JOIN :#euirc\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("Marko10_000")), 0);

    waitForWritten(":Guest774!absk007@hidd.en QUIT :Quit: Good Bye. I Quit...\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("Guest774"));

    waitForWritten(":absk007!absk007@hidd.en JOIN :#euirc\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("absk007")), 0);

    waitForWritten(":charly6!~Miranda@hidd.en QUIT :Client exited\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("charly6"));

    waitForWritten(":absk007!absk007@hidd.en NICK :Guest776\n");
    QCOMPARE(activityModel.count(), count);
    QVERIFY(!activityModel.contains("absk007"));
    QCOMPARE(activityModel.indexOf(activityModel.user("Guest776")), 0);

    waitForWritten(":Tina-chan_onAir!~kvirc@hidd.en NICK :Tina-chan\n");
    QCOMPARE(activityModel.count(), count);
    QVERIFY(!activityModel.contains("Tina-chan_onAir"));
    QCOMPARE(activityModel.indexOf(activityModel.user("Tina-chan")), 0);

    waitForWritten(":Guest776!absk007@hidd.en NICK :absk007\n");
    QCOMPARE(activityModel.count(), count);
    QVERIFY(!activityModel.contains("Guest776"));
    QCOMPARE(activityModel.indexOf(activityModel.user("absk007")), 0);

    waitForWritten(":aleksandr!~aleksandr@hidd.en PRIVMSG #euirc :absk007, last warning. fix your client/script\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("aleksandr")), 0);

    waitForWritten(":charly6!~Miranda@hidd.en JOIN :#euirc\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("charly6")), 0);

    waitForWritten(":absk007!absk007@hidd.en PRIVMSG #euirc :aleksandr, what did i do this time?\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("absk007")), 0);

    waitForWritten(":aleksandr!~aleksandr@hidd.en PRIVMSG #euirc :if you need help, join #opers\n");
    QCOMPARE(activityModel.count(), count);
    QCOMPARE(activityModel.indexOf(activityModel.user("aleksandr")), 0);

    waitForWritten(":charly6!~Miranda@hidd.en QUIT :Client exited\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("charly6"));

    waitForWritten(":icefly!~icefly@hidd.en PART #euirc :Once you know what it is you want to be true, instinct is a very useful device for enabling you to know that it is\n");
    QCOMPARE(activityModel.count(), --count);
    QVERIFY(!activityModel.contains("icefly"));

    waitForWritten(":icefly!~icefly@hidd.en JOIN :#euirc\n");
    QCOMPARE(activityModel.count(), ++count);
    QCOMPARE(activityModel.indexOf(activityModel.user("icefly")), 0);
}

QTEST_MAIN(tst_IrcUserModel)

#include "tst_ircusermodel.moc"
