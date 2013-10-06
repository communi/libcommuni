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
#include "tst_ircclientserver.h"

class tst_IrcBufferModel : public tst_IrcClientServer
{
    Q_OBJECT

public:
    tst_IrcBufferModel();

private slots:
    void testDefaults();
    void testAddRemove();
    void testClear();
    void testPrototypes();
    void testChanges();
    void testRoles();
    void testAIM();
};

Q_DECLARE_METATYPE(QModelIndex)
tst_IrcBufferModel::tst_IrcBufferModel()
{
    Irc::registerMetaTypes();
    qRegisterMetaType<QModelIndex>();
    qRegisterMetaType<IrcBuffer*>("IrcBuffer*");
    qRegisterMetaType<IrcChannel*>("IrcChannel*");
    qRegisterMetaType<IrcConnection*>("IrcConnection*");
    qRegisterMetaType<QList<IrcBuffer*> >("QList<IrcBuffer*>");
    qRegisterMetaType<QList<IrcChannel*> >("QList<IrcChannel*>");
}

void tst_IrcBufferModel::testDefaults()
{
    IrcBufferModel model;
    QCOMPARE(model.count(), 0);
    QVERIFY(!model.dynamicSort());
    QCOMPARE(model.sortMethod(), Irc::SortByTitle);
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

    QModelIndex firstIdx = model.index(first);
    QVERIFY(firstIdx.isValid());
    QCOMPARE(firstIdx.data(Irc::NameRole).toString(), QString("first"));
    QCOMPARE(firstIdx.data(Irc::TitleRole).toString(), QString("first"));
    QCOMPARE(firstIdx.data(Irc::PrefixRole).toString(), QString());
    QCOMPARE(firstIdx.data(Irc::BufferRole).value<IrcBuffer*>(), first);
    QVERIFY(!firstIdx.data(Irc::ChannelRole).value<IrcChannel*>());

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

    QModelIndex secondIdx = model.index(second);
    QVERIFY(secondIdx.isValid());
    QCOMPARE(secondIdx.data(Irc::NameRole).toString(), QString("second"));
    QCOMPARE(secondIdx.data(Irc::TitleRole).toString(), QString("second"));
    QCOMPARE(secondIdx.data(Irc::PrefixRole).toString(), QString());
    QCOMPARE(secondIdx.data(Irc::BufferRole).value<IrcBuffer*>(), second);
    QVERIFY(!secondIdx.data(Irc::ChannelRole).value<IrcChannel*>());

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

void tst_IrcBufferModel::testClear()
{
    IrcBufferModel model(connection);
    QPointer<IrcBuffer> a = model.add("#a");
    QPointer<IrcBuffer> b = model.add("#b");
    QPointer<IrcBuffer> c = model.add("c");

    QSignalSpy countSpy(&model, SIGNAL(countChanged(int)));
    QSignalSpy buffersSpy(&model, SIGNAL(buffersChanged(QList<IrcBuffer*>)));
    QSignalSpy channelsSpy(&model, SIGNAL(channelsChanged(QStringList)));
    QSignalSpy modelAboutToBeResetSpy(&model, SIGNAL(modelAboutToBeReset()));
    QSignalSpy modelResetSpy(&model, SIGNAL(modelReset()));

    QVERIFY(countSpy.isValid());
    QVERIFY(buffersSpy.isValid());
    QVERIFY(channelsSpy.isValid());
    QVERIFY(modelAboutToBeResetSpy.isValid());
    QVERIFY(modelResetSpy.isValid());

    model.clear();

    QCOMPARE(model.count(), 0);
    QCOMPARE(model.buffers(), QList<IrcBuffer*>());
    QCOMPARE(model.channels(), QStringList());

    QVERIFY(!model.get(0));
    QVERIFY(!model.find("#a"));
    QVERIFY(!model.find("#b"));
    QVERIFY(!model.find("c"));
    QVERIFY(!model.contains("#a"));
    QVERIFY(!model.contains("#b"));
    QVERIFY(!model.contains("c"));

    QVERIFY(!a);
    QVERIFY(!b);
    QVERIFY(!c);

    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(countSpy.last().at(0).toInt(), 0);

    QCOMPARE(buffersSpy.count(), 1);
    QCOMPARE(buffersSpy.last().at(0).value<QList<IrcBuffer*> >(), QList<IrcBuffer*>());

    QCOMPARE(channelsSpy.count(), 1);
    QCOMPARE(channelsSpy.last().at(0).toStringList(), QStringList());

    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);
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

void tst_IrcBufferModel::testChanges()
{
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

    int countChangedCount = 0;
    int aboutToBeAddedCount = 0, addedCount = 0;
    int aboutToBeRemovedCount = 0, removedCount = 0;
    int buffersChangedCount = 0;
    int channelsChangedCount = 0;

    QSignalSpy connectionChangedSpy(&bufferModel, SIGNAL(connectionChanged(IrcConnection*)));
    QSignalSpy networkChangedSpy(&bufferModel, SIGNAL(networkChanged(IrcNetwork*)));
    QSignalSpy messageIgnoredSpy(&bufferModel, SIGNAL(messageIgnored(IrcMessage*)));

    QVERIFY(connectionChangedSpy.isValid());
    QVERIFY(networkChangedSpy.isValid());
    QVERIFY(messageIgnoredSpy.isValid());

    int connectionChangedCount = 0;
    int networkChangedCount = 0;
    int messageIgnoredCount = 0;

    // relevant QAbstractItemModel signals
    QSignalSpy dataChangedSpy(&bufferModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)));
    QSignalSpy layoutAboutToBeChangedSpy(&bufferModel, SIGNAL(layoutAboutToBeChanged()));
    QSignalSpy layoutChangedSpy(&bufferModel, SIGNAL(layoutChanged()));
    QSignalSpy rowsAboutToBeInsertedSpy(&bufferModel, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy rowsInsertedSpy(&bufferModel, SIGNAL(rowsInserted(QModelIndex,int,int)));
    QSignalSpy rowsAboutToBeRemovedSpy(&bufferModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)));
    QSignalSpy rowsRemovedSpy(&bufferModel, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    QVERIFY(dataChangedSpy.isValid());
    QVERIFY(layoutAboutToBeChangedSpy.isValid());
    QVERIFY(layoutChangedSpy.isValid());
    QVERIFY(rowsAboutToBeInsertedSpy.isValid());
    QVERIFY(rowsInsertedSpy.isValid());
    QVERIFY(rowsAboutToBeRemovedSpy.isValid());
    QVERIFY(rowsRemovedSpy.isValid());

    int dataChangedCount = 0;
    int layoutAboutToBeChangedCount = 0, layoutChangedCount = 0;
    int rowsAboutToBeInsertedCount = 0, rowsInsertedCount = 0;
    int rowsAboutToBeRemovedCount = 0, rowsRemovedCount = 0;

    bufferModel.setConnection(connection);
    QCOMPARE(connectionChangedSpy.count(), ++connectionChangedCount);
    QCOMPARE(networkChangedSpy.count(), ++networkChangedCount);

    connection->open();
    QVERIFY(waitForOpened());

    QVERIFY(waitForWritten(":moorcock.freenode.net 001 communi :Welcome to the freenode Internet Relay Chat Network communi"));
    QVERIFY(waitForWritten(":moorcock.freenode.net 005 communi CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode KNOCK STATUSMSG=@+ CALLERID=g :are supported by this server"));
    QVERIFY(waitForWritten(":moorcock.freenode.net 005 communi CASEMAPPING=rfc1459 CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 ETRACE CPRIVMSG CNOTICE DEAF=D MONITOR=100 FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: :are supported by this server"));
    QVERIFY(waitForWritten(":moorcock.freenode.net 005 communi EXTBAN=$,arxz WHOX CLIENTVER=3.0 SAFELIST ELIST=CTU :are supported by this server"));
    QVERIFY(waitForWritten(":moorcock.freenode.net 375 communi :- moorcock.freenode.net Message of the Day -"));
    QVERIFY(waitForWritten(":moorcock.freenode.net 372 communi :- Welcome to moorcock.freenode.net in ..."));
    QVERIFY(waitForWritten(":moorcock.freenode.net 376 communi :End of /MOTD command."));

    messageIgnoredCount = 7 + 1; // 7 lines + a combined motd msg

    QCOMPARE(bufferModel.count(), 0);
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QVERIFY(waitForWritten(":communi!communi@hidd.en JOIN :#communi"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QCOMPARE(bufferModel.count(), 1);

    QPointer<IrcChannel> communi = bufferModel.get(0)->toChannel();
    QVERIFY(communi);
    QCOMPARE(communi->title(), QString("#communi"));
    QCOMPARE(communi->name(), QString("communi"));
    QCOMPARE(communi->prefix(), QString("#"));

    int previousIndex = -1;

    QList<IrcBuffer*> buffers = QList<IrcBuffer*>() << communi;
    QStringList channels = QStringList() << "#communi";

    int nextIndex = buffers.indexOf(communi);

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(countChangedSpy.count(), ++countChangedCount);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), buffers.count());

    QCOMPARE(aboutToBeAddedSpy.count(), ++aboutToBeAddedCount);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), communi.data());

    QCOMPARE(addedSpy.count(), ++addedCount);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), communi.data());

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), ++channelsChangedCount);
    QCOMPARE(channelsChangedSpy.last().at(0).toStringList(), channels);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);

    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), ++rowsAboutToBeInsertedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(2).toInt(), nextIndex);

    QCOMPARE(rowsInsertedSpy.count(), ++rowsInsertedCount);
    QCOMPARE(rowsInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsInsertedSpy.last().at(2).toInt(), nextIndex);

    QVERIFY(waitForWritten(":ChanServ!ChanServ@services. NOTICE communi :fake..."));
    QCOMPARE(messageIgnoredSpy.count(), ++messageIgnoredCount);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(countChangedSpy.count(), countChangedCount);
    QCOMPARE(aboutToBeAddedSpy.count(), aboutToBeAddedCount);
    QCOMPARE(addedSpy.count(), addedCount);
    QCOMPARE(buffersChangedSpy.count(), buffersChangedCount);
    QCOMPARE(channelsChangedSpy.count(), channelsChangedCount);
    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);
    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.count(), rowsAboutToBeInsertedCount);
    QCOMPARE(rowsInsertedSpy.count(), rowsInsertedCount);

    QVERIFY(waitForWritten(":ChanServ!ChanServ@services. PRIVMSG communi :fake..."));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QPointer<IrcBuffer> ChanServ = bufferModel.get(1);
    QVERIFY(ChanServ);
    QCOMPARE(ChanServ->title(), QString("ChanServ"));
    QCOMPARE(ChanServ->name(), QString("ChanServ"));
    QCOMPARE(ChanServ->prefix(), QString());

    previousIndex = -1;

    buffers = QList<IrcBuffer*>() << communi << ChanServ;
    channels = QStringList() << "#communi";

    nextIndex = buffers.indexOf(ChanServ);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), ++countChangedCount);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), buffers.count());

    QCOMPARE(aboutToBeAddedSpy.count(), ++aboutToBeAddedCount);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), ChanServ.data());

    QCOMPARE(addedSpy.count(), ++addedCount);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), ChanServ.data());

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), channelsChangedCount);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);

    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), ++rowsAboutToBeInsertedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(2).toInt(), nextIndex);

    QCOMPARE(rowsInsertedSpy.count(), ++rowsInsertedCount);
    QCOMPARE(rowsInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsInsertedSpy.last().at(2).toInt(), nextIndex);

    QVERIFY(waitForWritten(":ChanServ!ChanServ@services. NOTICE communi :fake..."));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    QCOMPARE(bufferModel.count(), buffers.count());
    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), countChangedCount);
    QCOMPARE(aboutToBeAddedSpy.count(), aboutToBeAddedCount);
    QCOMPARE(addedSpy.count(), addedCount);
    QCOMPARE(buffersChangedSpy.count(), buffersChangedCount);
    QCOMPARE(channelsChangedSpy.count(), channelsChangedCount);
    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);
    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.count(), rowsAboutToBeInsertedCount);
    QCOMPARE(rowsInsertedSpy.count(), rowsInsertedCount);

    QVERIFY(waitForWritten(":communi!communi@hidd.en JOIN :#freenode"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QPointer<IrcChannel> freenode = bufferModel.get(2)->toChannel();
    QVERIFY(freenode);
    QCOMPARE(freenode->title(), QString("#freenode"));
    QCOMPARE(freenode->name(), QString("freenode"));
    QCOMPARE(freenode->prefix(), QString("#"));

    previousIndex = -1;

    buffers = QList<IrcBuffer*>() << communi << ChanServ << freenode;
    channels = QStringList() << "#communi" << "#freenode";

    nextIndex = buffers.indexOf(freenode);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), ++countChangedCount);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), buffers.count());

    QCOMPARE(aboutToBeAddedSpy.count(), ++aboutToBeAddedCount);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), freenode.data());

    QCOMPARE(addedSpy.count(), ++addedCount);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), freenode.data());

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), ++channelsChangedCount);
    QCOMPARE(channelsChangedSpy.last().at(0).toStringList(), channels);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);

    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), ++rowsAboutToBeInsertedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(2).toInt(), nextIndex);

    QCOMPARE(rowsInsertedSpy.count(), ++rowsInsertedCount);
    QCOMPARE(rowsInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsInsertedSpy.last().at(2).toInt(), nextIndex);

    bufferModel.setDynamicSort(true);
    QCOMPARE(layoutAboutToBeChangedSpy.count(), ++layoutAboutToBeChangedCount);
    QCOMPARE(layoutChangedSpy.count(), ++layoutChangedCount);

    previousIndex = -1;

    // Irc::SortByTitle
    buffers = QList<IrcBuffer*>() << communi << freenode << ChanServ;
    channels = QStringList() << "#communi" << "#freenode";

    nextIndex = -1;

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    bufferModel.setSortMethod(Irc::SortByName);
    QCOMPARE(layoutAboutToBeChangedSpy.count(), ++layoutAboutToBeChangedCount);
    QCOMPARE(layoutChangedSpy.count(), ++layoutChangedCount);

    bufferModel.sort(0, Qt::DescendingOrder);
    QCOMPARE(layoutAboutToBeChangedSpy.count(), ++layoutAboutToBeChangedCount);
    QCOMPARE(layoutChangedSpy.count(), ++layoutChangedCount);

    previousIndex = -1;

    // Irc::SortByName, Qt::DescendingOrder
    buffers = QList<IrcBuffer*>() << freenode << communi << ChanServ;
    channels = QStringList() << "#communi" << "#freenode";

    nextIndex = -1;

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QVERIFY(waitForWritten(":qtassistant!qtassistant@hidd.en PRIVMSG communi :hola"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QPointer<IrcBuffer> qtassistant = bufferModel.get(0);
    QVERIFY(qtassistant);
    QCOMPARE(qtassistant->title(), QString("qtassistant"));
    QCOMPARE(qtassistant->name(), QString("qtassistant"));
    QCOMPARE(qtassistant->prefix(), QString());

    previousIndex = -1;

    // Irc::SortByName, Qt::DescendingOrder
    buffers = QList<IrcBuffer*>() << qtassistant << freenode << communi << ChanServ;
    channels = QStringList() << "#communi" << "#freenode";

    nextIndex = buffers.indexOf(qtassistant);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), ++countChangedCount);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), buffers.count());

    QCOMPARE(aboutToBeAddedSpy.count(), ++aboutToBeAddedCount);
    QCOMPARE(aboutToBeAddedSpy.last().at(0).value<IrcBuffer*>(), qtassistant.data());

    QCOMPARE(addedSpy.count(), ++addedCount);
    QCOMPARE(addedSpy.last().at(0).value<IrcBuffer*>(), qtassistant.data());

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), channelsChangedCount);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);

    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), ++rowsAboutToBeInsertedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(2).toInt(), nextIndex);

    QCOMPARE(rowsInsertedSpy.count(), ++rowsInsertedCount);
    QCOMPARE(rowsInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsInsertedSpy.last().at(2).toInt(), nextIndex);

    QSignalSpy titleChangedSpy(qtassistant, SIGNAL(titleChanged(QString)));
    QSignalSpy nameChangedSpy(qtassistant, SIGNAL(nameChanged(QString)));
    QSignalSpy prefixChangedSpy(qtassistant, SIGNAL(prefixChanged(QString)));

    QVERIFY(titleChangedSpy.isValid());
    QVERIFY(nameChangedSpy.isValid());
    QVERIFY(prefixChangedSpy.isValid());

    int titleChangedCount = 0;
    int nameChangedCount = 0;
    int prefixChangedCount = 0;

    QVERIFY(waitForWritten(":qtassistant!qtassistant@hidd.en NICK assistant :hola"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QCOMPARE(qtassistant->title(), QString("assistant"));
    QCOMPARE(qtassistant->name(), QString("assistant"));
    QCOMPARE(qtassistant->prefix(), QString());

    QCOMPARE(titleChangedSpy.count(), ++titleChangedCount);
    QCOMPARE(nameChangedSpy.count(), ++nameChangedCount);
    QCOMPARE(prefixChangedSpy.count(), prefixChangedCount);

    previousIndex = buffers.indexOf(qtassistant);

    // Irc::SortByName, Qt::DescendingOrder
    buffers = QList<IrcBuffer*>() << freenode << communi << ChanServ << qtassistant;
    channels = QStringList() << "#communi" << "#freenode";

    nextIndex = buffers.indexOf(qtassistant);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), countChangedCount);

    QCOMPARE(aboutToBeAddedSpy.count(), aboutToBeAddedCount);

    QCOMPARE(addedSpy.count(), addedCount);

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), channelsChangedCount);

    QCOMPARE(dataChangedSpy.count(), ++dataChangedCount);
    QModelIndex topLeft = dataChangedSpy.last().at(0).value<QModelIndex>();
    QModelIndex bottomRight = dataChangedSpy.last().at(0).value<QModelIndex>();
    QVERIFY(!topLeft.parent().isValid());
    QVERIFY(topLeft.isValid());
    QVERIFY(bottomRight.isValid());
    QVERIFY(topLeft == bottomRight);
    QCOMPARE(topLeft.row(), previousIndex);
    QCOMPARE(topLeft.column(), 0);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), ++rowsAboutToBeRemovedCount);
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(1).toInt(), previousIndex);
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(2).toInt(), previousIndex);

    QCOMPARE(rowsRemovedSpy.count(), ++rowsRemovedCount);
    QCOMPARE(rowsRemovedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsRemovedSpy.last().at(1).toInt(), previousIndex);
    QCOMPARE(rowsRemovedSpy.last().at(2).toInt(), previousIndex);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), ++rowsAboutToBeInsertedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsAboutToBeInsertedSpy.last().at(2).toInt(), nextIndex);

    QCOMPARE(rowsInsertedSpy.count(), ++rowsInsertedCount);
    QCOMPARE(rowsInsertedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsInsertedSpy.last().at(1).toInt(), nextIndex);
    QCOMPARE(rowsInsertedSpy.last().at(2).toInt(), nextIndex);

    QVERIFY(waitForWritten(":communi!communi@hidd.en PART #communi"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QVERIFY(communi); // deleteLater()'d

    QCOMPARE(aboutToBeRemovedSpy.count(), ++aboutToBeRemovedCount);
    QCOMPARE(aboutToBeRemovedSpy.last().at(0).value<IrcBuffer*>(), communi.data());

    QCOMPARE(removedSpy.count(), ++removedCount);
    QCOMPARE(removedSpy.last().at(0).value<IrcBuffer*>(), communi.data());

    previousIndex = buffers.indexOf(communi);

    QCoreApplication::sendPostedEvents(communi, QEvent::DeferredDelete);
    QVERIFY(!communi);

    // Irc::SortByName, Qt::DescendingOrder
    buffers = QList<IrcBuffer*>() << freenode << ChanServ << qtassistant;
    channels = QStringList() << "#freenode";

    nextIndex = buffers.indexOf(communi);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), ++countChangedCount);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), buffers.count());

    QCOMPARE(aboutToBeAddedSpy.count(), aboutToBeAddedCount);

    QCOMPARE(addedSpy.count(), addedCount);

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), ++channelsChangedCount);
    QCOMPARE(channelsChangedSpy.last().at(0).toStringList(), channels);

    QCOMPARE(dataChangedSpy.count(), dataChangedCount);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), ++rowsAboutToBeRemovedCount);
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(1).toInt(), previousIndex);
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(2).toInt(), previousIndex);

    QCOMPARE(rowsRemovedSpy.count(), ++rowsRemovedCount);
    QCOMPARE(rowsRemovedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsRemovedSpy.last().at(1).toInt(), previousIndex);
    QCOMPARE(rowsRemovedSpy.last().at(2).toInt(), previousIndex);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), rowsAboutToBeInsertedCount);

    QCOMPARE(rowsInsertedSpy.count(), rowsInsertedCount);

    QVERIFY(waitForWritten(":jpnurmi!jpnurmi@qt/jpnurmi KICK #freenode communi"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    QVERIFY(freenode); // deleteLater()'d

    QCOMPARE(aboutToBeRemovedSpy.count(), ++aboutToBeRemovedCount);
    QCOMPARE(aboutToBeRemovedSpy.last().at(0).value<IrcBuffer*>(), freenode.data());

    QCOMPARE(removedSpy.count(), ++removedCount);
    QCOMPARE(removedSpy.last().at(0).value<IrcBuffer*>(), freenode.data());

    previousIndex = buffers.indexOf(freenode);

    QCoreApplication::sendPostedEvents(freenode, QEvent::DeferredDelete);
    QVERIFY(!freenode);

    // Irc::SortByName, Qt::DescendingOrder
    buffers = QList<IrcBuffer*>() << ChanServ << qtassistant;
    channels = QStringList();

    nextIndex = buffers.indexOf(freenode);

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
    }

    QCOMPARE(countChangedSpy.count(), ++countChangedCount);
    QCOMPARE(countChangedSpy.last().at(0).toInt(), buffers.count());

    QCOMPARE(aboutToBeAddedSpy.count(), aboutToBeAddedCount);

    QCOMPARE(addedSpy.count(), addedCount);

    QCOMPARE(buffersChangedSpy.count(), ++buffersChangedCount);
    QCOMPARE(buffersChangedSpy.last().at(0).value<QList<IrcBuffer*> >(), buffers);

    QCOMPARE(channelsChangedSpy.count(), ++channelsChangedCount);
    QCOMPARE(channelsChangedSpy.last().at(0).toStringList(), channels);

    QCOMPARE(dataChangedSpy.count(), dataChangedCount);

    QCOMPARE(rowsAboutToBeRemovedSpy.count(), ++rowsAboutToBeRemovedCount);
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(1).toInt(), previousIndex);
    QCOMPARE(rowsAboutToBeRemovedSpy.last().at(2).toInt(), previousIndex);

    QCOMPARE(rowsRemovedSpy.count(), ++rowsRemovedCount);
    QCOMPARE(rowsRemovedSpy.last().at(0).value<QModelIndex>(), QModelIndex());
    QCOMPARE(rowsRemovedSpy.last().at(1).toInt(), previousIndex);
    QCOMPARE(rowsRemovedSpy.last().at(2).toInt(), previousIndex);

    QCOMPARE(rowsAboutToBeInsertedSpy.count(), rowsAboutToBeInsertedCount);

    QCOMPARE(rowsInsertedSpy.count(), rowsInsertedCount);

    QVERIFY(waitForWritten(":communi!communi@hidd.en QUIT :bye"));
    QCOMPARE(messageIgnoredSpy.count(), messageIgnoredCount);

    serverSocket->close();
    QVERIFY(clientSocket->waitForDisconnected(100));
    QVERIFY(!connection->isConnected());
    QVERIFY(!connection->isActive());

    QCOMPARE(bufferModel.count(), buffers.count());
    QCOMPARE(bufferModel.buffers(), buffers);
    QCOMPARE(bufferModel.channels(), channels);

    for (int i = 0; i < bufferModel.count(); ++i) {
        QCOMPARE(bufferModel.get(i), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::BufferRole).value<IrcBuffer*>(), buffers.at(i));
        QCOMPARE(bufferModel.index(i).data(Irc::ChannelRole).value<IrcChannel*>(), buffers.at(i)->toChannel());
        QVERIFY(!buffers.at(i)->isActive());
    }

    QCOMPARE(countChangedSpy.count(), countChangedCount);
    QCOMPARE(aboutToBeAddedSpy.count(), aboutToBeAddedCount);
    QCOMPARE(addedSpy.count(), addedCount);
    QCOMPARE(buffersChangedSpy.count(), buffersChangedCount);
    QCOMPARE(channelsChangedSpy.count(), channelsChangedCount);
    QCOMPARE(dataChangedSpy.count(), dataChangedCount);
    QCOMPARE(rowsAboutToBeRemovedSpy.count(), rowsAboutToBeRemovedCount);
    QCOMPARE(rowsRemovedSpy.count(), rowsRemovedCount);
    QCOMPARE(rowsAboutToBeInsertedSpy.count(), rowsAboutToBeInsertedCount);
    QCOMPARE(rowsInsertedSpy.count(), rowsInsertedCount);
}

void tst_IrcBufferModel::testRoles()
{
    IrcBufferModel model;
    QHash<int, QByteArray> roles = model.roleNames();
    QCOMPARE(roles.take(Qt::DisplayRole), QByteArray("display"));
    QCOMPARE(roles.take(Irc::BufferRole), QByteArray("buffer"));
    QCOMPARE(roles.take(Irc::ChannelRole), QByteArray("channel"));
    QCOMPARE(roles.take(Irc::NameRole), QByteArray("name"));
    QCOMPARE(roles.take(Irc::PrefixRole), QByteArray("prefix"));
    QCOMPARE(roles.take(Irc::TitleRole), QByteArray("title"));
    QVERIFY(roles.isEmpty());
}

void tst_IrcBufferModel::testAIM()
{
    IrcBufferModel bufferModel(connection);
    IrcBuffer* a = bufferModel.add("#a");
    IrcBuffer* b = bufferModel.add("#b");
    IrcBuffer* c = bufferModel.add("c");
    IrcBuffer* o = 0;

    QAbstractItemModel* aim = &bufferModel;
    QModelIndex ai = aim->index(0, 0);
    QModelIndex bi = aim->index(1, 0);
    QModelIndex ci = aim->index(2, 0);
    QModelIndex oi = aim->index(100, 100);

    QVERIFY(ai.isValid());
    QVERIFY(bi.isValid());
    QVERIFY(ci.isValid());
    QVERIFY(!oi.isValid());

    QCOMPARE(aim->rowCount(QModelIndex()), 3);
    QCOMPARE(aim->rowCount(ai), 0);

    QCOMPARE(aim->columnCount(QModelIndex()), 1);
    QCOMPARE(aim->columnCount(ai), 0);

    QCOMPARE(bufferModel.index(a), ai);
    QCOMPARE(bufferModel.index(b), bi);
    QCOMPARE(bufferModel.index(c), ci);
    QVERIFY(!bufferModel.index(o).isValid());

    QCOMPARE(bufferModel.buffer(ai), a);
    QCOMPARE(bufferModel.buffer(bi), b);
    QCOMPARE(bufferModel.buffer(ci), c);
    QVERIFY(!bufferModel.buffer(oi));

    bufferModel.setDisplayRole(Irc::TitleRole);
    QCOMPARE(aim->data(ai, Qt::DisplayRole).toString(), a->title());
    QCOMPARE(aim->data(bi, Qt::DisplayRole).toString(), b->title());
    QCOMPARE(aim->data(ci, Qt::DisplayRole).toString(), c->title());
    QVERIFY(aim->data(oi, Qt::DisplayRole).toString().isEmpty());

    bufferModel.setDisplayRole(Irc::BufferRole);
    QCOMPARE(aim->data(ai, Qt::DisplayRole).value<IrcBuffer*>(), a);
    QCOMPARE(aim->data(bi, Qt::DisplayRole).value<IrcBuffer*>(), b);
    QCOMPARE(aim->data(ci, Qt::DisplayRole).value<IrcBuffer*>(), c);
    QVERIFY(!aim->data(oi, Qt::DisplayRole).value<IrcBuffer*>());

    QCOMPARE(aim->data(ai, Irc::BufferRole).value<IrcBuffer*>(), a);
    QCOMPARE(aim->data(bi, Irc::BufferRole).value<IrcBuffer*>(), b);
    QCOMPARE(aim->data(ci, Irc::BufferRole).value<IrcBuffer*>(), c);
    QVERIFY(!aim->data(oi, Irc::BufferRole).value<IrcBuffer*>());

    QCOMPARE(aim->data(ai, Irc::ChannelRole).value<IrcChannel*>(), a->toChannel());
    QCOMPARE(aim->data(bi, Irc::ChannelRole).value<IrcChannel*>(), b->toChannel());
    QCOMPARE(aim->data(ci, Irc::ChannelRole).value<IrcChannel*>(), c->toChannel());
    QVERIFY(!aim->data(oi, Irc::ChannelRole).value<IrcChannel*>());

    QCOMPARE(aim->data(ai, Irc::TitleRole).toString(), a->title());
    QCOMPARE(aim->data(bi, Irc::TitleRole).toString(), b->title());
    QCOMPARE(aim->data(ci, Irc::TitleRole).toString(), c->title());
    QVERIFY(aim->data(oi, Irc::TitleRole).toString().isEmpty());

    QCOMPARE(aim->data(ai, Irc::NameRole).toString(), a->name());
    QCOMPARE(aim->data(bi, Irc::NameRole).toString(), b->name());
    QCOMPARE(aim->data(ci, Irc::NameRole).toString(), c->name());
    QVERIFY(aim->data(oi, Irc::NameRole).toString().isEmpty());

    QVERIFY(aim->data(ai, Irc::PrefixRole).toString().isEmpty());
    QVERIFY(aim->data(bi, Irc::PrefixRole).toString().isEmpty());
    QVERIFY(aim->data(ci, Irc::PrefixRole).toString().isEmpty());
    QVERIFY(aim->data(oi, Irc::PrefixRole).toString().isEmpty());
}

QTEST_MAIN(tst_IrcBufferModel)

#include "tst_ircbuffermodel.moc"
