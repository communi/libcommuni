#include <QtTest/QtTest>
#include <QtNetwork/QtNetwork>
#include <ircsession.h>

class tst_IrcSession : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void ircsession_data();
    void ircsession();

    void addAutoJoinChannel_data();
    void addAutoJoinChannel();
    void autoJoinChannels_data();
    void autoJoinChannels();
    void autoReconnectDelay_data();
    void autoReconnectDelay();
    void cmdCtcpAction_data();
    void cmdCtcpAction();
    void cmdCtcpReply_data();
    void cmdCtcpReply();
    void cmdCtcpRequest_data();
    void cmdCtcpRequest();
    void cmdInvite_data();
    void cmdInvite();
    void cmdJoin_data();
    void cmdJoin();
    void cmdKick_data();
    void cmdKick();
    void cmdList_data();
    void cmdList();
    void cmdMessage_data();
    void cmdMessage();
    void cmdMode_data();
    void cmdMode();
    void cmdNames_data();
    void cmdNames();
    void cmdNotice_data();
    void cmdNotice();
    void cmdPart_data();
    void cmdPart();
    void cmdQuit_data();
    void cmdQuit();
    void cmdTopic_data();
    void cmdTopic();
    void cmdWhois_data();
    void cmdWhois();
    void connectSlotsByName_data();
    void connectSlotsByName();
    void connectToServer_data();
    void connectToServer();
    void disconnectFromServer_data();
    void disconnectFromServer();
    void encoding_data();
    void encoding();
    void host_data();
    void host();
    void ident_data();
    void ident();
    void isConnected_data();
    void isConnected();
    void nick_data();
    void nick();
    void options_data();
    void options();
    void password_data();
    void password();
    void port_data();
    void port();
    void realName_data();
    void realName();
    void removeAutoJoinChannel_data();
    void removeAutoJoinChannel();
    void sendRaw_data();
    void sendRaw();
    void Session_data();
    void Session();
    void setAutoJoinChannels_data();
    void setAutoJoinChannels();
    void setAutoReconnectDelay_data();
    void setAutoReconnectDelay();
    void setEncoding_data();
    void setEncoding();
    void setIdent_data();
    void setIdent();
    void setNick_data();
    void setNick();
    void setOptions_data();
    void setOptions();
    void setPassword_data();
    void setPassword();
    void setRealName_data();
    void setRealName();
    void setSocket_data();
    void setSocket();
    void socket_data();
    void socket();
    void connected_data();
    void connected();
    void disconnected_data();
    void disconnected();
    void msgCtcpActionReceived_data();
    void msgCtcpActionReceived();
    void msgCtcpReplyReceived_data();
    void msgCtcpReplyReceived();
    void msgCtcpRequestReceived_data();
    void msgCtcpRequestReceived();
    void msgInvited_data();
    void msgInvited();
    void msgJoined_data();
    void msgJoined();
    void msgKicked_data();
    void msgKicked();
    void msgMessageReceived_data();
    void msgMessageReceived();
    void msgModeChanged_data();
    void msgModeChanged();
    void msgNickChanged_data();
    void msgNickChanged();
    void msgNoticeReceived_data();
    void msgNoticeReceived();
    void msgNumericMessageReceived_data();
    void msgNumericMessageReceived();
    void msgParted_data();
    void msgParted();
    void msgQuit_data();
    void msgQuit();
    void msgTopicChanged_data();
    void msgTopicChanged();
    void msgUnknownMessageReceived_data();
    void msgUnknownMessageReceived();
};

// Subclass that exposes the protected functions.
class SubIrcSession : public Irc::Session
{
public:
    void call_connected()
        { return SubIrcSession::connected(); }

    void call_disconnected()
        { return SubIrcSession::disconnected(); }

    void call_msgCtcpActionReceived(QString const& origin, QString const& receiver, QString const& action)
        { return SubIrcSession::msgCtcpActionReceived(origin, receiver, action); }

    void call_msgCtcpReplyReceived(QString const& origin, QString const& reply)
        { return SubIrcSession::msgCtcpReplyReceived(origin, reply); }

    void call_msgCtcpRequestReceived(QString const& origin, QString const& request)
        { return SubIrcSession::msgCtcpRequestReceived(origin, request); }

    void call_msgInvited(QString const& origin, QString const& receiver, QString const& channel)
        { return SubIrcSession::msgInvited(origin, receiver, channel); }

    void call_msgJoined(QString const& origin, QString const& channel)
        { return SubIrcSession::msgJoined(origin, channel); }

    void call_msgKicked(QString const& origin, QString const& channel, QString const& nick, QString const& message)
        { return SubIrcSession::msgKicked(origin, channel, nick, message); }

    void call_msgMessageReceived(QString const& origin, QString const& receiver, QString const& message)
        { return SubIrcSession::msgMessageReceived(origin, receiver, message); }

    void call_msgModeChanged(QString const& origin, QString const& receiver, QString const& mode, QString const& args)
        { return SubIrcSession::msgModeChanged(origin, receiver, mode, args); }

    void call_msgNickChanged(QString const& origin, QString const& nick)
        { return SubIrcSession::msgNickChanged(origin, nick); }

    void call_msgNoticeReceived(QString const& origin, QString const& receiver, QString const& notice)
        { return SubIrcSession::msgNoticeReceived(origin, receiver, notice); }

    void call_msgNumericMessageReceived(QString const& origin, uint code, QStringList const& params)
        { return SubIrcSession::msgNumericMessageReceived(origin, code, params); }

    void call_msgParted(QString const& origin, QString const& channel, QString const& message)
        { return SubIrcSession::msgParted(origin, channel, message); }

    void call_msgQuit(QString const& origin, QString const& message)
        { return SubIrcSession::msgQuit(origin, message); }

    void call_msgTopicChanged(QString const& origin, QString const& channel, QString const& topic)
        { return SubIrcSession::msgTopicChanged(origin, channel, topic); }

    void call_msgUnknownMessageReceived(QString const& origin, QStringList const& params)
        { return SubIrcSession::msgUnknownMessageReceived(origin, params); }
};

// This will be called before the first test function is executed.
// It is only called once.
void tst_IrcSession::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_IrcSession::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_IrcSession::init()
{
}

// This will be called after every test function.
void tst_IrcSession::cleanup()
{
}

void tst_IrcSession::ircsession_data()
{
}

void tst_IrcSession::ircsession()
{
    SubIrcSession session;
#if 0
    session.addAutoJoinChannel(QString());
    QCOMPARE(session.autoJoinChannels(), QStringList);
    QCOMPARE(session.autoReconnectDelay(), -1);
    QCOMPARE(session.cmdCtcpAction(QString(), QString()), false);
    QCOMPARE(session.cmdCtcpReply(QString(), QString()), false);
    QCOMPARE(session.cmdCtcpRequest(QString(), QString()), false);
    QCOMPARE(session.cmdInvite(QString(), QString()), false);
    QCOMPARE(session.cmdJoin(QString(), QString()), false);
    QCOMPARE(session.cmdKick(QString(), QString(), QString()), false);
    QCOMPARE(session.cmdList(QString()), false);
    QCOMPARE(session.cmdMessage(QString(), QString()), false);
    QCOMPARE(session.cmdMode(QString(), QString()), false);
    QCOMPARE(session.cmdNames(QString()), false);
    QCOMPARE(session.cmdNotice(QString(), QString()), false);
    QCOMPARE(session.cmdPart(QString(), QString()), false);
    QCOMPARE(session.cmdQuit(QString()), false);
    QCOMPARE(session.cmdTopic(QString(), QString()), false);
    QCOMPARE(session.cmdWhois(QString()), false);
    session.connectSlotsByName(QObject const*);
    session.connectToServer(QHostAddress(), quint16);
    session.connectToServer(QString(), quint16);
    session.disconnectFromServer();
    QCOMPARE(session.encoding(), QByteArray);
    QCOMPARE(session.host(), QString());
    QCOMPARE(session.ident(), QString());
    QCOMPARE(session.isConnected(), false);
    QCOMPARE(session.nick(), QString());
    QCOMPARE(session.options(), IrcSession::Options);
    QCOMPARE(session.password(), QString());
    QCOMPARE(session.port(), quint16);
    QCOMPARE(session.realName(), QString());
    session.removeAutoJoinChannel(QString());
    QCOMPARE(session.sendRaw(QString()), false);
    QCOMPARE(session.Session(QObject*), IrcSession);
    session.setAutoJoinChannels(QStringList());
    session.setAutoReconnectDelay(-1);
    session.setEncoding(QByteArray());
    session.setIdent(QString());
    session.setNick(QString());
    session.setOptions(IrcSession::Options);
    session.setPassword(QString());
    session.setRealName(QString());
    session.setSocket(QAbstractSocket*);
    QCOMPARE(session.socket(), QAbstractSocket*);
    QCOMPARE(session.~Session(), IrcSession);
    session.call_connected();
    session.call_disconnected();
    session.call_msgCtcpActionReceived(QString(), QString(), QString());
    session.call_msgCtcpReplyReceived(QString(), QString());
    session.call_msgCtcpRequestReceived(QString(), QString());
    session.call_msgInvited(QString(), QString(), QString());
    session.call_msgJoined(QString(), QString());
    session.call_msgKicked(QString(), QString(), QString(), QString());
    session.call_msgMessageReceived(QString(), QString(), QString());
    session.call_msgModeChanged(QString(), QString(), QString(), QString());
    session.call_msgNickChanged(QString(), QString());
    session.call_msgNoticeReceived(QString(), QString(), QString());
    session.call_msgNumericMessageReceived(QString(), uint, QStringList());
    session.call_msgParted(QString(), QString(), QString());
    session.call_msgQuit(QString(), QString());
    session.call_msgTopicChanged(QString(), QString(), QString());
    session.call_msgUnknownMessageReceived(QString(), QStringList());
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::addAutoJoinChannel_data()
{
    QTest::addColumn<QString>("channel");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public void addAutoJoinChannel(QString const& channel)
void tst_IrcSession::addAutoJoinChannel()
{
#if 0
    QFETCH(QString, channel);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.addAutoJoinChannel(channel);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

Q_DECLARE_METATYPE(QStringList)
void tst_IrcSession::autoJoinChannels_data()
{
#if 0
    QTest::addColumn<QStringList>("autoJoinChannels");
    QTest::newRow("null") << QStringList();
#endif
}

// public QStringList autoJoinChannels() const
void tst_IrcSession::autoJoinChannels()
{
#if 0
    QFETCH(QStringList, autoJoinChannels);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.autoJoinChannels(), autoJoinChannels);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::autoReconnectDelay_data()
{
    QTest::addColumn<int>("autoReconnectDelay");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// public int autoReconnectDelay() const
void tst_IrcSession::autoReconnectDelay()
{
#if 0
    QFETCH(int, autoReconnectDelay);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.autoReconnectDelay(), autoReconnectDelay);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdCtcpAction_data()
{
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("action");
    QTest::addColumn<bool>("cmdCtcpAction");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdCtcpAction(QString const& receiver, QString const& action)
void tst_IrcSession::cmdCtcpAction()
{
#if 0
    QFETCH(QString, receiver);
    QFETCH(QString, action);
    QFETCH(bool, cmdCtcpAction);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdCtcpAction(receiver, action), cmdCtcpAction);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdCtcpReply_data()
{
    QTest::addColumn<QString>("nick");
    QTest::addColumn<QString>("reply");
    QTest::addColumn<bool>("cmdCtcpReply");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdCtcpReply(QString const& nick, QString const& reply)
void tst_IrcSession::cmdCtcpReply()
{
#if 0
    QFETCH(QString, nick);
    QFETCH(QString, reply);
    QFETCH(bool, cmdCtcpReply);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdCtcpReply(nick, reply), cmdCtcpReply);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdCtcpRequest_data()
{
    QTest::addColumn<QString>("nick");
    QTest::addColumn<QString>("request");
    QTest::addColumn<bool>("cmdCtcpRequest");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdCtcpRequest(QString const& nick, QString const& request)
void tst_IrcSession::cmdCtcpRequest()
{
#if 0
    QFETCH(QString, nick);
    QFETCH(QString, request);
    QFETCH(bool, cmdCtcpRequest);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdCtcpRequest(nick, request), cmdCtcpRequest);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdInvite_data()
{
    QTest::addColumn<QString>("nick");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<bool>("cmdInvite");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdInvite(QString const& nick, QString const& channel)
void tst_IrcSession::cmdInvite()
{
#if 0
    QFETCH(QString, nick);
    QFETCH(QString, channel);
    QFETCH(bool, cmdInvite);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdInvite(nick, channel), cmdInvite);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdJoin_data()
{
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("key");
    QTest::addColumn<bool>("cmdJoin");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdJoin(QString const& channel, QString const& key = QString())
void tst_IrcSession::cmdJoin()
{
#if 0
    QFETCH(QString, channel);
    QFETCH(QString, key);
    QFETCH(bool, cmdJoin);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdJoin(channel, key), cmdJoin);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdKick_data()
{
    QTest::addColumn<QString>("nick");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("reason");
    QTest::addColumn<bool>("cmdKick");
    QTest::newRow("null") << QString() << QString() << QString() << false;
}

// public bool cmdKick(QString const& nick, QString const& channel, QString const& reason = QString())
void tst_IrcSession::cmdKick()
{
#if 0
    QFETCH(QString, nick);
    QFETCH(QString, channel);
    QFETCH(QString, reason);
    QFETCH(bool, cmdKick);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdKick(nick, channel, reason), cmdKick);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdList_data()
{
    QTest::addColumn<QString>("channel");
    QTest::addColumn<bool>("cmdList");
    QTest::newRow("null") << QString() << false;
}

// public bool cmdList(QString const& channel = QString())
void tst_IrcSession::cmdList()
{
#if 0
    QFETCH(QString, channel);
    QFETCH(bool, cmdList);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdList(channel), cmdList);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdMessage_data()
{
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("message");
    QTest::addColumn<bool>("cmdMessage");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdMessage(QString const& receiver, QString const& message)
void tst_IrcSession::cmdMessage()
{
#if 0
    QFETCH(QString, receiver);
    QFETCH(QString, message);
    QFETCH(bool, cmdMessage);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdMessage(receiver, message), cmdMessage);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdMode_data()
{
    QTest::addColumn<QString>("target");
    QTest::addColumn<QString>("mode");
    QTest::addColumn<bool>("cmdMode");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdMode(QString const& target, QString const& mode = QString())
void tst_IrcSession::cmdMode()
{
#if 0
    QFETCH(QString, target);
    QFETCH(QString, mode);
    QFETCH(bool, cmdMode);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdMode(target, mode), cmdMode);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdNames_data()
{
    QTest::addColumn<QString>("channel");
    QTest::addColumn<bool>("cmdNames");
    QTest::newRow("null") << QString() << false;
}

// public bool cmdNames(QString const& channel)
void tst_IrcSession::cmdNames()
{
#if 0
    QFETCH(QString, channel);
    QFETCH(bool, cmdNames);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdNames(channel), cmdNames);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdNotice_data()
{
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("notice");
    QTest::addColumn<bool>("cmdNotice");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdNotice(QString const& receiver, QString const& notice)
void tst_IrcSession::cmdNotice()
{
#if 0
    QFETCH(QString, receiver);
    QFETCH(QString, notice);
    QFETCH(bool, cmdNotice);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdNotice(receiver, notice), cmdNotice);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdPart_data()
{
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("reason");
    QTest::addColumn<bool>("cmdPart");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdPart(QString const& channel, QString const& reason = QString())
void tst_IrcSession::cmdPart()
{
#if 0
    QFETCH(QString, channel);
    QFETCH(QString, reason);
    QFETCH(bool, cmdPart);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdPart(channel, reason), cmdPart);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdQuit_data()
{
    QTest::addColumn<QString>("reason");
    QTest::addColumn<bool>("cmdQuit");
    QTest::newRow("null") << QString() << false;
}

// public bool cmdQuit(QString const& reason = QString())
void tst_IrcSession::cmdQuit()
{
#if 0
    QFETCH(QString, reason);
    QFETCH(bool, cmdQuit);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdQuit(reason), cmdQuit);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdTopic_data()
{
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("topic");
    QTest::addColumn<bool>("cmdTopic");
    QTest::newRow("null") << QString() << QString() << false;
}

// public bool cmdTopic(QString const& channel, QString const& topic = QString())
void tst_IrcSession::cmdTopic()
{
#if 0
    QFETCH(QString, channel);
    QFETCH(QString, topic);
    QFETCH(bool, cmdTopic);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdTopic(channel, topic), cmdTopic);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::cmdWhois_data()
{
    QTest::addColumn<QString>("nick");
    QTest::addColumn<bool>("cmdWhois");
    QTest::newRow("null") << QString() << false;
}

// public bool cmdWhois(QString const& nick)
void tst_IrcSession::cmdWhois()
{
#if 0
    QFETCH(QString, nick);
    QFETCH(bool, cmdWhois);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.cmdWhois(nick), cmdWhois);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::connectSlotsByName_data()
{
    QTest::addColumn<int>("receiverCount");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// public void connectSlotsByName(QObject const* receiver)
void tst_IrcSession::connectSlotsByName()
{
#if 0
    QFETCH(int, receiverCount);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.connectSlotsByName(receiver);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

Q_DECLARE_METATYPE(QHostAddress)
Q_DECLARE_METATYPE(quint16)
void tst_IrcSession::connectToServer_data()
{
#if 0
    QTest::addColumn<QHostAddress>("address");
    QTest::addColumn<quint16>("port");
    QTest::newRow("null") << QHostAddress() << quint16();
#endif
}

// public void connectToServer(QHostAddress const& address, quint16 port = 6667)
void tst_IrcSession::connectToServer()
{
#if 0
    QFETCH(QHostAddress, address);
    QFETCH(quint16, port);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.connectToServer(address, port);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::disconnectFromServer_data()
{
    QTest::addColumn<int>("foo");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// public void disconnectFromServer()
void tst_IrcSession::disconnectFromServer()
{
#if 0
    QFETCH(int, foo);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.disconnectFromServer();

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

Q_DECLARE_METATYPE(QByteArray)
void tst_IrcSession::encoding_data()
{
#if 0
    QTest::addColumn<QByteArray>("encoding");
    QTest::newRow("null") << QByteArray();
#endif
}

// public QByteArray encoding() const
void tst_IrcSession::encoding()
{
#if 0
    QFETCH(QByteArray, encoding);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.encoding(), encoding);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::host_data()
{
    QTest::addColumn<QString>("host");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public QString host() const
void tst_IrcSession::host()
{
#if 0
    QFETCH(QString, host);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.host(), host);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::ident_data()
{
    QTest::addColumn<QString>("ident");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public QString ident() const
void tst_IrcSession::ident()
{
#if 0
    QFETCH(QString, ident);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.ident(), ident);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::isConnected_data()
{
    QTest::addColumn<bool>("isConnected");
    QTest::newRow("true") << true;
    QTest::newRow("false") << false;
}

// public bool isConnected() const
void tst_IrcSession::isConnected()
{
#if 0
    QFETCH(bool, isConnected);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.isConnected(), isConnected);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::nick_data()
{
    QTest::addColumn<QString>("nick");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public QString nick() const
void tst_IrcSession::nick()
{
#if 0
    QFETCH(QString, nick);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.nick(), nick);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

Q_DECLARE_METATYPE(Irc::Session::Options)
void tst_IrcSession::options_data()
{
#if 0
    QTest::addColumn<IrcSession::Options>("options");
    QTest::newRow("null") << IrcSession::Options();
#endif
}

// public IrcSession::Options options() const
void tst_IrcSession::options()
{
#if 0
    QFETCH(IrcSession::Options, options);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.options(), options);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::password_data()
{
    QTest::addColumn<QString>("password");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public QString password() const
void tst_IrcSession::password()
{
#if 0
    QFETCH(QString, password);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.password(), password);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::port_data()
{
#if 0
    QTest::addColumn<quint16>("port");
    QTest::newRow("null") << quint16();
#endif
}

// public quint16 port() const
void tst_IrcSession::port()
{
#if 0
    QFETCH(quint16, port);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.port(), port);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::realName_data()
{
    QTest::addColumn<QString>("realName");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public QString realName() const
void tst_IrcSession::realName()
{
#if 0
    QFETCH(QString, realName);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.realName(), realName);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::removeAutoJoinChannel_data()
{
    QTest::addColumn<QString>("channel");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public void removeAutoJoinChannel(QString const& channel)
void tst_IrcSession::removeAutoJoinChannel()
{
#if 0
    QFETCH(QString, channel);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.removeAutoJoinChannel(channel);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::sendRaw_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<bool>("sendRaw");
    QTest::newRow("null") << QString() << false;
}

// public bool sendRaw(QString const& message)
void tst_IrcSession::sendRaw()
{
#if 0
    QFETCH(QString, message);
    QFETCH(bool, sendRaw);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.sendRaw(message), sendRaw);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::Session_data()
{
#if 0
    QTest::addColumn<int>("parentCount");
    QTest::addColumn<IrcSession>("Session");
    QTest::newRow("null") << 0 << IrcSession();
#endif
}

// public IrcSession Session(QObject* parent = 0)
void tst_IrcSession::Session()
{
#if 0
    QFETCH(int, parentCount);
    QFETCH(IrcSession, Session);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.Session(parent), Session);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setAutoJoinChannels_data()
{
#if 0
    QTest::addColumn<QStringList>("channels");
    QTest::newRow("null") << QStringList();
#endif
}

// public void setAutoJoinChannels(QStringList const& channels)
void tst_IrcSession::setAutoJoinChannels()
{
#if 0
    QFETCH(QStringList, channels);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setAutoJoinChannels(channels);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setAutoReconnectDelay_data()
{
    QTest::addColumn<int>("seconds");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// public void setAutoReconnectDelay(int seconds)
void tst_IrcSession::setAutoReconnectDelay()
{
#if 0
    QFETCH(int, seconds);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setAutoReconnectDelay(seconds);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setEncoding_data()
{
#if 0
    QTest::addColumn<QByteArray>("encoding");
    QTest::newRow("null") << QByteArray();
#endif
}

// public void setEncoding(QByteArray const& encoding)
void tst_IrcSession::setEncoding()
{
#if 0
    QFETCH(QByteArray, encoding);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setEncoding(encoding);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setIdent_data()
{
    QTest::addColumn<QString>("ident");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public void setIdent(QString const& ident)
void tst_IrcSession::setIdent()
{
#if 0
    QFETCH(QString, ident);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setIdent(ident);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setNick_data()
{
    QTest::addColumn<QString>("nick");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public void setNick(QString const& nick)
void tst_IrcSession::setNick()
{
#if 0
    QFETCH(QString, nick);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setNick(nick);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setOptions_data()
{
#if 0
    QTest::addColumn<IrcSession::Options>("options");
    QTest::newRow("null") << IrcSession::Options();
#endif
}

// public void setOptions(IrcSession::Options options)
void tst_IrcSession::setOptions()
{
#if 0
    QFETCH(IrcSession::Options, options);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setOptions(options);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setPassword_data()
{
    QTest::addColumn<QString>("password");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public void setPassword(QString const& password)
void tst_IrcSession::setPassword()
{
#if 0
    QFETCH(QString, password);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setPassword(password);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setRealName_data()
{
    QTest::addColumn<QString>("realName");
    QTest::newRow("null") << QString();
    QTest::newRow("foo") << QString("foo");
}

// public void setRealName(QString const& realName)
void tst_IrcSession::setRealName()
{
#if 0
    QFETCH(QString, realName);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setRealName(realName);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::setSocket_data()
{
    QTest::addColumn<int>("socketCount");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// public void setSocket(QAbstractSocket* socket)
void tst_IrcSession::setSocket()
{
#if 0
    QFETCH(int, socketCount);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.setSocket(socket);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

Q_DECLARE_METATYPE(QAbstractSocket*)
void tst_IrcSession::socket_data()
{
#if 0
    QTest::addColumn<QAbstractSocket*>("socket");
    QTest::newRow("null") << QAbstractSocket*();
#endif
}

// public QAbstractSocket* socket() const
void tst_IrcSession::socket()
{
#if 0
    QFETCH(QAbstractSocket*, socket);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    QCOMPARE(session.socket(), socket);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::connected_data()
{
    QTest::addColumn<int>("foo");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// protected void connected()
void tst_IrcSession::connected()
{
#if 0
    QFETCH(int, foo);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_connected();

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::disconnected_data()
{
    QTest::addColumn<int>("foo");
    QTest::newRow("0") << 0;
    QTest::newRow("-1") << -1;
}

// protected void disconnected()
void tst_IrcSession::disconnected()
{
#if 0
    QFETCH(int, foo);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_disconnected();

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgCtcpActionReceived_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("action");
    QTest::newRow("null") << QString() << QString() << QString();
}

// protected void msgCtcpActionReceived(QString const& origin, QString const& receiver, QString const& action)
void tst_IrcSession::msgCtcpActionReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, receiver);
    QFETCH(QString, action);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgCtcpActionReceived(origin, receiver, action);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgCtcpReplyReceived_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("reply");
    QTest::newRow("null") << QString() << QString();
}

// protected void msgCtcpReplyReceived(QString const& origin, QString const& reply)
void tst_IrcSession::msgCtcpReplyReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, reply);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgCtcpReplyReceived(origin, reply);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgCtcpRequestReceived_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("request");
    QTest::newRow("null") << QString() << QString();
}

// protected void msgCtcpRequestReceived(QString const& origin, QString const& request)
void tst_IrcSession::msgCtcpRequestReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, request);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgCtcpRequestReceived(origin, request);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgInvited_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("channel");
    QTest::newRow("null") << QString() << QString() << QString();
}

// protected void msgInvited(QString const& origin, QString const& receiver, QString const& channel)
void tst_IrcSession::msgInvited()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, receiver);
    QFETCH(QString, channel);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgInvited(origin, receiver, channel);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgJoined_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("channel");
    QTest::newRow("null") << QString() << QString();
}

// protected void msgJoined(QString const& origin, QString const& channel)
void tst_IrcSession::msgJoined()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, channel);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgJoined(origin, channel);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgKicked_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("nick");
    QTest::addColumn<QString>("message");
    QTest::newRow("null") << QString() << QString() << QString() << QString();
}

// protected void msgKicked(QString const& origin, QString const& channel, QString const& nick, QString const& message)
void tst_IrcSession::msgKicked()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, channel);
    QFETCH(QString, nick);
    QFETCH(QString, message);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgKicked(origin, channel, nick, message);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgMessageReceived_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("message");
    QTest::newRow("null") << QString() << QString() << QString();
}

// protected void msgMessageReceived(QString const& origin, QString const& receiver, QString const& message)
void tst_IrcSession::msgMessageReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, receiver);
    QFETCH(QString, message);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgMessageReceived(origin, receiver, message);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgModeChanged_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("mode");
    QTest::addColumn<QString>("args");
    QTest::newRow("null") << QString() << QString() << QString() << QString();
}

// protected void msgModeChanged(QString const& origin, QString const& receiver, QString const& mode, QString const& args)
void tst_IrcSession::msgModeChanged()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, receiver);
    QFETCH(QString, mode);
    QFETCH(QString, args);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgModeChanged(origin, receiver, mode, args);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgNickChanged_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("nick");
    QTest::newRow("null") << QString() << QString();
}

// protected void msgNickChanged(QString const& origin, QString const& nick)
void tst_IrcSession::msgNickChanged()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, nick);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgNickChanged(origin, nick);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgNoticeReceived_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("receiver");
    QTest::addColumn<QString>("notice");
    QTest::newRow("null") << QString() << QString() << QString();
}

// protected void msgNoticeReceived(QString const& origin, QString const& receiver, QString const& notice)
void tst_IrcSession::msgNoticeReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, receiver);
    QFETCH(QString, notice);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgNoticeReceived(origin, receiver, notice);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgNumericMessageReceived_data()
{
#if 0
    QTest::addColumn<QString>("origin");
    QTest::addColumn<uint>("code");
    QTest::addColumn<QStringList>("params");
    QTest::newRow("null") << QString() << uint() << QStringList();
#endif
}

// protected void msgNumericMessageReceived(QString const& origin, uint code, QStringList const& params)
void tst_IrcSession::msgNumericMessageReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(uint, code);
    QFETCH(QStringList, params);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgNumericMessageReceived(origin, code, params);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgParted_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("message");
    QTest::newRow("null") << QString() << QString() << QString();
}

// protected void msgParted(QString const& origin, QString const& channel, QString const& message)
void tst_IrcSession::msgParted()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, channel);
    QFETCH(QString, message);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgParted(origin, channel, message);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgQuit_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("message");
    QTest::newRow("null") << QString() << QString();
}

// protected void msgQuit(QString const& origin, QString const& message)
void tst_IrcSession::msgQuit()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, message);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgQuit(origin, message);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgTopicChanged_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("channel");
    QTest::addColumn<QString>("topic");
    QTest::newRow("null") << QString() << QString() << QString();
}

// protected void msgTopicChanged(QString const& origin, QString const& channel, QString const& topic)
void tst_IrcSession::msgTopicChanged()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QString, channel);
    QFETCH(QString, topic);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgTopicChanged(origin, channel, topic);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

void tst_IrcSession::msgUnknownMessageReceived_data()
{
#if 0
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QStringList>("params");
    QTest::newRow("null") << QString() << QStringList();
#endif
}

// protected void msgUnknownMessageReceived(QString const& origin, QStringList const& params)
void tst_IrcSession::msgUnknownMessageReceived()
{
#if 0
    QFETCH(QString, origin);
    QFETCH(QStringList, params);

    SubIrcSession session;

    QSignalSpy spy0(&session, SIGNAL(connected()));
    QSignalSpy spy1(&session, SIGNAL(disconnected()));
    QSignalSpy spy2(&session, SIGNAL(msgCtcpActionReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy3(&session, SIGNAL(msgCtcpReplyReceived(QString const&, QString const&)));
    QSignalSpy spy4(&session, SIGNAL(msgCtcpRequestReceived(QString const&, QString const&)));
    QSignalSpy spy5(&session, SIGNAL(msgInvited(QString const&, QString const&, QString const&)));
    QSignalSpy spy6(&session, SIGNAL(msgJoined(QString const&, QString const&)));
    QSignalSpy spy7(&session, SIGNAL(msgKicked(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy8(&session, SIGNAL(msgMessageReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy9(&session, SIGNAL(msgModeChanged(QString const&, QString const&, QString const&, QString const&)));
    QSignalSpy spy10(&session, SIGNAL(msgNickChanged(QString const&, QString const&)));
    QSignalSpy spy11(&session, SIGNAL(msgNoticeReceived(QString const&, QString const&, QString const&)));
    QSignalSpy spy12(&session, SIGNAL(msgNumericMessageReceived(QString const&, uint, QStringList const&)));
    QSignalSpy spy13(&session, SIGNAL(msgParted(QString const&, QString const&, QString const&)));
    QSignalSpy spy14(&session, SIGNAL(msgQuit(QString const&, QString const&)));
    QSignalSpy spy15(&session, SIGNAL(msgTopicChanged(QString const&, QString const&, QString const&)));
    QSignalSpy spy16(&session, SIGNAL(msgUnknownMessageReceived(QString const&, QStringList const&)));

    session.call_msgUnknownMessageReceived(origin, params);

    QCOMPARE(spy0.count(), 0);
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    QCOMPARE(spy4.count(), 0);
    QCOMPARE(spy5.count(), 0);
    QCOMPARE(spy6.count(), 0);
    QCOMPARE(spy7.count(), 0);
    QCOMPARE(spy8.count(), 0);
    QCOMPARE(spy9.count(), 0);
    QCOMPARE(spy10.count(), 0);
    QCOMPARE(spy11.count(), 0);
    QCOMPARE(spy12.count(), 0);
    QCOMPARE(spy13.count(), 0);
    QCOMPARE(spy14.count(), 0);
    QCOMPARE(spy15.count(), 0);
    QCOMPARE(spy16.count(), 0);
#endif
    QSKIP("Test is not implemented.", SkipAll);
}

QTEST_MAIN(tst_IrcSession)
#include "tst_ircsession.moc"
