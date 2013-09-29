/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircbuffermodel.h"
#include "ircconnection.h"
#include "ircchannel.h"
#include "ircbuffer.h"
#include <QtTest/QtTest>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#if QT_VERSION >= 0x050000
#define Q4SKIP(description) QSKIP(description)
#else
#define Q4SKIP(description) QSKIP(description, SkipAll)
#endif

class tst_IrcBufferModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testDefaults();
    void testAddRemove();
    void testPrototypes();
    void testChanges();
};

Q_DECLARE_METATYPE(QModelIndex)
void tst_IrcBufferModel::initTestCase()
{
    qRegisterMetaType<QModelIndex>();
    qRegisterMetaType<IrcBuffer*>("IrcBuffer*");
    qRegisterMetaType<IrcChannel*>("IrcChannel*");
    qRegisterMetaType<IrcMessage*>("IrcMessage*");
    qRegisterMetaType<IrcNetwork*>("IrcNetwork*");
    qRegisterMetaType<IrcConnection*>("IrcConnection*");
    qRegisterMetaType<QList<IrcBuffer*> >("QList<IrcBuffer*>");
}

void tst_IrcBufferModel::testDefaults()
{
    IrcBufferModel model;
    QCOMPARE(model.count(), 0);
    QVERIFY(!model.dynamicSort());
    QVERIFY(model.channels().isEmpty());
    QCOMPARE(model.displayRole(), Irc::TitleRole);
    QVERIFY(model.buffers().isEmpty());
    QVERIFY(!model.connection());
    QVERIFY(!model.network());
    QVERIFY(model.bufferPrototype());
    QVERIFY(model.channelPrototype());
}

void tst_IrcBufferModel::testAddRemove()
{
    IrcBufferModel model;

    QSignalSpy countSpy(&model, SIGNAL(countChanged(int)));
    QSignalSpy addedSpy(&model, SIGNAL(added(IrcBuffer*)));
    QSignalSpy removedSpy(&model, SIGNAL(removed(IrcBuffer*)));
    QSignalSpy aboutToBeAddedSpy(&model, SIGNAL(aboutToBeAdded(IrcBuffer*)));
    QSignalSpy aboutToBeRemovedSpy(&model, SIGNAL(aboutToBeRemoved(IrcBuffer*)));
    QSignalSpy buffersSpy(&model, SIGNAL(buffersChanged(QList<IrcBuffer*>)));
    QSignalSpy channelsSpy(&model, SIGNAL(channelsChanged(QStringList)));
    QVERIFY(countSpy.isValid());
    QVERIFY(addedSpy.isValid());
    QVERIFY(removedSpy.isValid());
    QVERIFY(aboutToBeAddedSpy.isValid());
    QVERIFY(aboutToBeRemovedSpy.isValid());
    QVERIFY(buffersSpy.isValid());
    QVERIFY(channelsSpy.isValid());

    // IrcBuffer* IrcBufferModel::add(const QString& title)
    IrcBuffer* first = model.add("first");
    QCOMPARE(model.count(), 1);
    QCOMPARE(model.get(0), first);
    QCOMPARE(model.find("first"), first);
    QCOMPARE(model.buffers(), QList<IrcBuffer*>() << first);
    QVERIFY(model.contains("first"));
    QCOMPARE(model.indexOf(first), 0);
    QVERIFY(model.channels().isEmpty());

    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(countSpy.last().last().toInt(), 1);
    QCOMPARE(aboutToBeAddedSpy.count(), 1);
    QCOMPARE(aboutToBeAddedSpy.last().last().value<IrcBuffer*>(), first);
    QCOMPARE(addedSpy.count(), 1);
    QCOMPARE(addedSpy.last().last().value<IrcBuffer*>(), first);
    QCOMPARE(buffersSpy.count(), 1);
    QCOMPARE(buffersSpy.last().last().value<QList<IrcBuffer*> >(), QList<IrcBuffer*>() << first);
    QCOMPARE(channelsSpy.count(), 0);

    // void IrcBufferModel::add(IrcBuffer* buffer)
    IrcBuffer* second = new IrcBuffer(&model);
    second->setName("second");
    model.add(second);
    QCOMPARE(model.count(), 2);
    QCOMPARE(model.get(1), second);
    QCOMPARE(model.find("second"), second);
    QCOMPARE(model.buffers(), QList<IrcBuffer*>() << first << second);
    QVERIFY(model.contains("second"));
    QCOMPARE(model.indexOf(second), 1);
    QVERIFY(model.channels().isEmpty());

    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(countSpy.last().last().toInt(), 2);
    QCOMPARE(aboutToBeAddedSpy.count(), 2);
    QCOMPARE(aboutToBeAddedSpy.last().last().value<IrcBuffer*>(), second);
    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(addedSpy.last().last().value<IrcBuffer*>(), second);
    QCOMPARE(buffersSpy.count(), 2);
    QCOMPARE(buffersSpy.last().last().value<QList<IrcBuffer*> >(), QList<IrcBuffer*>() << first << second);
    QCOMPARE(channelsSpy.count(), 0);

    // void IrcBufferModel::remove(IrcBuffer* buffer)
    model.remove(second);
    QCOMPARE(model.count(), 1);
    QVERIFY(!model.find("second"));
    QCOMPARE(model.buffers(), QList<IrcBuffer*>() << first);
    QVERIFY(!model.contains("second"));
    QVERIFY(model.channels().isEmpty());

    QCOMPARE(countSpy.count(), 3);
    QCOMPARE(countSpy.last().last().toInt(), 1);
    QCOMPARE(aboutToBeRemovedSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(buffersSpy.count(), 3);
    QCOMPARE(buffersSpy.last().last().value<QList<IrcBuffer*> >(), QList<IrcBuffer*>() << first);
    QCOMPARE(channelsSpy.count(), 0);

    // void IrcBufferModel::remove(const QString& title)
    model.remove("first");
    QCOMPARE(model.count(), 0);
    QVERIFY(!model.find("first"));
    QVERIFY(model.buffers().isEmpty());
    QVERIFY(!model.contains("first"));
    QVERIFY(model.channels().isEmpty());

    QCOMPARE(countSpy.count(), 4);
    QCOMPARE(countSpy.last().last().toInt(), 0);
    QCOMPARE(aboutToBeRemovedSpy.count(), 2);
    QCOMPARE(removedSpy.count(), 2);
    QCOMPARE(buffersSpy.count(), 4);
    QCOMPARE(buffersSpy.last().last().value<QList<IrcBuffer*> >(), QList<IrcBuffer*>());
    QCOMPARE(channelsSpy.count(), 0);
}

void tst_IrcBufferModel::testPrototypes()
{
    IrcBufferModel model;

    QSignalSpy bufferProtoSpy(&model, SIGNAL(bufferPrototypeChanged(IrcBuffer*)));
    QSignalSpy channelProtoSpy(&model, SIGNAL(channelPrototypeChanged(IrcChannel*)));
    QVERIFY(bufferProtoSpy.isValid());
    QVERIFY(channelProtoSpy.isValid());

    model.setBufferPrototype(0);
    QVERIFY(model.bufferPrototype());
    QCOMPARE(bufferProtoSpy.count(), 1);

    model.setChannelPrototype(0);
    QVERIFY(model.channelPrototype());
    QCOMPARE(channelProtoSpy.count(), 1);

    IrcBuffer* bufferProto = new IrcBuffer(&model);
    model.setBufferPrototype(bufferProto);
    QCOMPARE(model.bufferPrototype(), bufferProto);
    QCOMPARE(bufferProtoSpy.count(), 2);

    IrcChannel* channelProto = new IrcChannel(&model);
    model.setChannelPrototype(channelProto);
    QCOMPARE(model.channelPrototype(), channelProto);
    QCOMPARE(channelProtoSpy.count(), 2);
}

static void waitForWritten(QAbstractSocket* serverSocket, QAbstractSocket* clientSocket, const QByteArray& data = QByteArray())
{
    if (!data.isNull())
        serverSocket->write(data);
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(clientSocket->waitForReadyRead());
}

void tst_IrcBufferModel::testChanges()
{
    QTcpServer server;
    QVERIFY(server.listen());

    IrcConnection connection;
    connection.setUserName("communi");
    connection.setNickName("communi");
    connection.setRealName("communi");
    connection.setHost(server.serverAddress().toString());
    connection.setPort(server.serverPort());

    IrcBufferModel bufferModel;

    // IrcBufferModel signals
    QSignalSpy countChangedSpy(&bufferModel, SIGNAL(countChanged(int)));
    QSignalSpy addedSpy(&bufferModel, SIGNAL(added(IrcBuffer*)));
    QSignalSpy removedSpy(&bufferModel, SIGNAL(removed(IrcBuffer*)));
    QSignalSpy aboutToBeAddedSpy(&bufferModel, SIGNAL(aboutToBeAdded(IrcBuffer*)));
    QSignalSpy aboutToBeRemovedSpy(&bufferModel, SIGNAL(aboutToBeRemoved(IrcBuffer*)));
    QSignalSpy buffersChangedSpy(&bufferModel, SIGNAL(buffersChanged(QList<IrcBuffer*>)));
    QSignalSpy channelsChangedSpy(&bufferModel, SIGNAL(channelsChanged(QStringList)));

    QVERIFY(countChangedSpy.isValid());
    QVERIFY(addedSpy.isValid());
    QVERIFY(removedSpy.isValid());
    QVERIFY(aboutToBeAddedSpy.isValid());
    QVERIFY(aboutToBeRemovedSpy.isValid());
    QVERIFY(buffersChangedSpy.isValid());
    QVERIFY(channelsChangedSpy.isValid());

    QSignalSpy connectionChangedSpy(&bufferModel, SIGNAL(connectionChanged(IrcConnection*)));
    QSignalSpy networkChangedSpy(&bufferModel, SIGNAL(networkChanged(IrcNetwork*)));
    QSignalSpy messageIgnoredSpy(&bufferModel, SIGNAL(messageIgnored(IrcMessage*)));

    QVERIFY(connectionChangedSpy.isValid());
    QVERIFY(networkChangedSpy.isValid());
    QVERIFY(messageIgnoredSpy.isValid());

    // relevant QAbstractItemModel signals
    QSignalSpy dataChangedSpy(&bufferModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)));
    QSignalSpy modelAboutToBeResetSpy(&bufferModel, SIGNAL(modelAboutToBeReset()));
    QSignalSpy modelResetSpy(&bufferModel, SIGNAL(modelReset()));
    QSignalSpy layoutAboutToBeChangedSpy(&bufferModel, SIGNAL(layoutAboutToBeChanged()));
    QSignalSpy layoutChangedSpy(&bufferModel, SIGNAL(layoutChanged()));
    QSignalSpy rowsAboutToBeInsertedSpy(&bufferModel, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy rowsInsertedSpy(&bufferModel, SIGNAL(rowsInserted(QModelIndex,int,int)));
    QSignalSpy rowsAboutToBeRemovedSpy(&bufferModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)));
    QSignalSpy rowsRemovedSpy(&bufferModel, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    QVERIFY(dataChangedSpy.isValid());
    QVERIFY(modelAboutToBeResetSpy.isValid());
    QVERIFY(modelResetSpy.isValid());
    QVERIFY(layoutAboutToBeChangedSpy.isValid());
    QVERIFY(layoutChangedSpy.isValid());
    QVERIFY(rowsAboutToBeInsertedSpy.isValid());
    QVERIFY(rowsInsertedSpy.isValid());
    QVERIFY(rowsAboutToBeRemovedSpy.isValid());
    QVERIFY(rowsRemovedSpy.isValid());

    bufferModel.setConnection(&connection);
    QCOMPARE(connectionChangedSpy.count(), 1);
    QCOMPARE(networkChangedSpy.count(), 1);

    connection.open();
    if (!server.waitForNewConnection(200))
        Q4SKIP("The address is not available");
    QTcpSocket* ss = server.nextPendingConnection();
    QVERIFY(ss);

    QAbstractSocket* cs = connection.socket();
    QVERIFY(cs);
    QVERIFY(cs->waitForConnected());

    waitForWritten(ss, cs, ":moorcock.freenode.net 001 communi :Welcome to the freenode Internet Relay Chat Network communi\r\n");
    waitForWritten(ss, cs, ":moorcock.freenode.net 005 communi CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode KNOCK STATUSMSG=@+ CALLERID=g :are supported by this server\r\n");
    waitForWritten(ss, cs, ":moorcock.freenode.net 005 communi CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=100 FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: :are supported by this server\r\n");
    waitForWritten(ss, cs, ":moorcock.freenode.net 005 communi EXTBAN=$,arxz WHOX CLIENTVER=3.0 SAFELIST ELIST=CTU :are supported by this server\r\n");
    waitForWritten(ss, cs, ":moorcock.freenode.net 375 communi :- moorcock.freenode.net Message of the Day -\r\n");
    waitForWritten(ss, cs, ":moorcock.freenode.net 372 communi :- Welcome to moorcock.freenode.net in ...\r\n");
    waitForWritten(ss, cs, ":moorcock.freenode.net 376 communi :End of /MOTD command.\r\n");

    int ignored = 7 + 1; // 7 lines + a combined motd msg

    QCOMPARE(bufferModel.count(), 0);
    QCOMPARE(messageIgnoredSpy.count(), ignored);

    waitForWritten(ss, cs, ":communi!communi@hidd.en JOIN :#communi\r\n");
    QCOMPARE(messageIgnoredSpy.count(), ignored);

    QCOMPARE(bufferModel.count(), 1);

    QPointer<IrcChannel> communi = bufferModel.get(0)->toChannel();
    QVERIFY(communi);
    QCOMPARE(communi->title(), QString("#communi"));
    QCOMPARE(communi->name(), QString("communi"));
    QCOMPARE(communi->prefix(), QString("#"));

    QList<IrcBuffer*> buffers = QList<IrcBuffer*>() << communi;
    QStringList channels = QStringList() << "#communi";

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i)
        QCOMPARE(bufferModel.get(i), buffers.at(i));
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(countChangedSpy.count(), 1);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), 1);

    QCOMPARE(aboutToBeAddedSpy.count(), 1);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), communi.data());

    QCOMPARE(addedSpy.count(), 1);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), communi.data());

    QCOMPARE(buffersChangedSpy.count(), 1);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), 1);
    QCOMPARE(channelsChangedSpy.last().at(0).toStringList(), channels);

    waitForWritten(ss, cs, ":ChanServ!ChanServ@services. PRIVMSG communi :fake...\r\n");
    QCOMPARE(messageIgnoredSpy.count(), ignored);

    QCOMPARE(bufferModel.count(), 2);

    QPointer<IrcBuffer> ChanServ = bufferModel.get(1);
    QVERIFY(ChanServ);
    QCOMPARE(ChanServ->title(), QString("ChanServ"));
    QCOMPARE(ChanServ->name(), QString("ChanServ"));
    QCOMPARE(ChanServ->prefix(), QString());

    buffers = QList<IrcBuffer*>() << communi << ChanServ;
    channels = QStringList() << "#communi";

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i)
        QCOMPARE(bufferModel.get(i), buffers.at(i));
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(countChangedSpy.count(), 2);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), 2);

    QCOMPARE(aboutToBeAddedSpy.count(), 2);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), ChanServ.data());

    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), ChanServ.data());

    QCOMPARE(buffersChangedSpy.count(), 2);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), 1);

    waitForWritten(ss, cs, ":communi!communi@hidd.en JOIN :#freenode\r\n");
    QCOMPARE(messageIgnoredSpy.count(), ignored);

    QCOMPARE(bufferModel.count(), 3);

    QPointer<IrcChannel> freenode = bufferModel.get(2)->toChannel();
    QVERIFY(freenode);
    QCOMPARE(freenode->title(), QString("#freenode"));
    QCOMPARE(freenode->name(), QString("freenode"));
    QCOMPARE(freenode->prefix(), QString("#"));

    buffers = QList<IrcBuffer*>() << communi << ChanServ << freenode;
    channels = QStringList() << "#communi" << "#freenode";

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i)
        QCOMPARE(bufferModel.get(i), buffers.at(i));
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(countChangedSpy.count(), 3);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), 3);

    QCOMPARE(aboutToBeAddedSpy.count(), 3);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), freenode.data());

    QCOMPARE(addedSpy.count(), 3);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), freenode.data());

    QCOMPARE(buffersChangedSpy.count(), 3);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), 2);
    QCOMPARE(channelsChangedSpy.last().at(0).toStringList(), channels);

    bufferModel.setDynamicSort(true);
    QCOMPARE(layoutAboutToBeChangedSpy.count(), 1);
    QCOMPARE(layoutChangedSpy.count(), 1);

    // Irc::SortByTitle
    buffers = QList<IrcBuffer*>() << communi << freenode << ChanServ;
    channels = QStringList() << "#communi" << "#freenode";

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i)
        QCOMPARE(bufferModel.get(i), buffers.at(i));
    QCOMPARE(bufferModel.channels(), channels);

    bufferModel.setSortMethod(Irc::SortByName);
    QCOMPARE(layoutAboutToBeChangedSpy.count(), 2);
    QCOMPARE(layoutChangedSpy.count(), 2);

    // Irc::SortByName
    buffers = QList<IrcBuffer*>() << ChanServ << communi << freenode;
    channels = QStringList() << "#communi" << "#freenode";

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i)
        QCOMPARE(bufferModel.get(i), buffers.at(i));
    QCOMPARE(bufferModel.channels(), channels);
}

QTEST_MAIN(tst_IrcBufferModel)

#include "tst_ircbuffermodel.moc"
