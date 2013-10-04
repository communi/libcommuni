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
#include "ircmessage.h"
#include "ircnetwork.h"
#include "ircconnection.h"
#include "ircchannel.h"
#include "ircbuffer.h"
#include "ircuser.h"
#include <QtTest/QtTest>
#include <QRegExp>

class tst_DebugOutput : public QObject
{
    Q_OBJECT

private slots:
    void testIrc();
    void testIrcCommand();
    void testIrcConnection();
    void testIrcMessage();
    void testIrcNetwork();
    void testIrcModel();
};

void tst_DebugOutput::testIrc()
{
    QString str;
    QDebug dbg(&str);

    dbg << Irc::RPL_AWAY;
    QCOMPARE(str.trimmed(), QString::fromLatin1("RPL_AWAY"));
    str.clear();

    dbg << Irc::NameRole;
    QCOMPARE(str.trimmed(), QString::fromLatin1("NameRole"));
    str.clear();

    dbg << Irc::Brown;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Brown"));
    str.clear();

    dbg << Irc::SortByActivity;
    QCOMPARE(str.trimmed(), QString::fromLatin1("SortByActivity"));
    str.clear();
}

void tst_DebugOutput::testIrcCommand()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcCommand*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcCommand(0x0)"));
    str.clear();

    IrcCommand command;
    QTest::ignoreMessage(QtWarningMsg, "Reimplement IrcCommand::toString() for IrcCommand::Custom");
    dbg << &command;
    QVERIFY(QRegExp("IrcCommand\\(0x[0-9A-Fa-f]+, type=Custom\\) ").exactMatch(str));
    str.clear();

    command.setType(IrcCommand::Quit);
    dbg << &command;
    QVERIFY(QRegExp("IrcCommand\\(0x[0-9A-Fa-f]+, type=Quit, \"QUIT :\"\\) ").exactMatch(str));
    str.clear();

    command.setObjectName("foo");
    dbg << &command;
    QVERIFY(QRegExp("IrcCommand\\(0x[0-9A-Fa-f]+, name=foo, type=Quit, \"QUIT :\"\\) ").exactMatch(str));
    str.clear();

    dbg << IrcCommand::Join;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Join"));
    str.clear();
}

void tst_DebugOutput::testIrcConnection()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcConnection*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcConnection(0x0)"));
    str.clear();

    IrcConnection connection;
    dbg << &connection;
    QVERIFY(QRegExp("IrcConnection\\(0x[0-9A-Fa-f]+\\) ").exactMatch(str));
    str.clear();

    connection.setHost("irc.freenode.net");
    dbg << &connection;
    QVERIFY(QRegExp("IrcConnection\\(0x[0-9A-Fa-f]+, irc.freenode.net\\) ").exactMatch(str));
    str.clear();

    connection.setDisplayName("Freenode");
    dbg << &connection;
    QVERIFY(QRegExp("IrcConnection\\(0x[0-9A-Fa-f]+, Freenode\\) ").exactMatch(str));
    str.clear();

    dbg << IrcConnection::Connected;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Connected"));
    str.clear();
}

void tst_DebugOutput::testIrcMessage()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcMessage*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcMessage(0x0)"));
    str.clear();

    IrcMessage message(0);
    dbg << &message;
    QVERIFY(QRegExp("IrcMessage\\(0x[0-9A-Fa-f]+, flags=\\(None\\)\\) ").exactMatch(str));
    str.clear();

    dbg << IrcMessage::Join;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Join"));
    str.clear();

    dbg << IrcMessage::Unidentified;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Unidentified"));
    str.clear();

    dbg << (IrcMessage::Own | IrcMessage::Identified);
    QCOMPARE(str.trimmed(), QString::fromLatin1("(Own|Identified)"));
    str.clear();

    dbg << IrcModeMessage::Channel;
    QCOMPARE(str.trimmed(), QString::fromLatin1("Channel"));
    str.clear();
}

void tst_DebugOutput::testIrcNetwork()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcNetwork*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcNetwork(0x0)"));
    str.clear();

    IrcConnection connection;
    dbg << connection.network();
    QVERIFY(QRegExp("IrcNetwork\\(0x[0-9A-Fa-f]+\\) ").exactMatch(str));
    str.clear();

    dbg << IrcNetwork::MessageLength;
    QCOMPARE(str.trimmed(), QString::fromLatin1("MessageLength"));
    str.clear();

    dbg << IrcNetwork::AllTypes;
    QCOMPARE(str.trimmed(), QString::fromLatin1("AllTypes"));
    str.clear();

    dbg << (IrcNetwork::TypeA | IrcNetwork::TypeD);
    QCOMPARE(str.trimmed(), QString::fromLatin1("(TypeA|TypeD)"));
    str.clear();
}

void tst_DebugOutput::testIrcModel()
{
    QString str;
    QDebug dbg(&str);

    dbg << static_cast<IrcChannel*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcChannel(0x0)"));
    str.clear();

    dbg << static_cast<IrcBuffer*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcBuffer(0x0)"));
    str.clear();

    dbg << static_cast<IrcUser*>(0);
    QCOMPARE(str.trimmed(), QString::fromLatin1("IrcUser(0x0)"));
    str.clear();

    IrcChannel channel;
    channel.setPrefix("#");
    channel.setName("communi");
    dbg << &channel;
    QVERIFY(QRegExp("IrcChannel\\(0x[0-9A-Fa-f]+, title=#communi\\) ").exactMatch(str));
    str.clear();

    IrcBuffer buffer;
    buffer.setName("Communi");
    dbg << &buffer;
    QVERIFY(QRegExp("IrcBuffer\\(0x[0-9A-Fa-f]+, title=Communi\\) ").exactMatch(str));
    str.clear();

    IrcUser user;
    dbg << &user;
    QVERIFY(QRegExp("IrcUser\\(0x[0-9A-Fa-f]+\\) ").exactMatch(str));
}

QTEST_MAIN(tst_DebugOutput)

#include "tst_dbgout.moc"
