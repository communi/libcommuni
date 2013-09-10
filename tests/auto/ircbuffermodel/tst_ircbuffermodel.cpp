/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircbuffermodel.h"
#include "ircchannel.h"
#include "ircbuffer.h"
#include <QtTest/QtTest>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class tst_IrcBufferModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testDefaults();
    void testAddRemove();
    void testPrototypes();
};

void tst_IrcBufferModel::initTestCase()
{
    qRegisterMetaType<IrcBuffer*>("IrcBuffer*");
    qRegisterMetaType<IrcChannel*>("IrcChannel*");
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
    QCOMPARE(model.buffer("first"), first);
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
    QCOMPARE(model.buffer("second"), second);
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
    QVERIFY(!model.buffer("second"));
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
    QVERIFY(!model.buffer("first"));
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

QTEST_MAIN(tst_IrcBufferModel)

#include "tst_ircbuffermodel.moc"
