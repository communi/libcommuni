/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircclient.h"
#include "ircmessageformatter.h"

#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QTextEdit>
#include <QTime>

#include <Irc>
#include <IrcSession>
#include <IrcCommand>
#include <IrcMessage>
#include <IrcChannel>
#include <IrcUserModel>
#include <IrcChannelModel>

static const char* CHANNEL = "#communi";
static const char* SERVER = "irc.freenode.net";

IrcClient::IrcClient(QWidget* parent) : QSplitter(parent)
{
    createUi();
    createSession();

    session->open();
}

IrcClient::~IrcClient()
{
    if (session->isActive()) {
        session->sendCommand(IrcCommand::createQuit(session->realName()));
        session->socket()->waitForDisconnected(1000);
    }
}

void IrcClient::onConnected()
{
    textEdit->append(IrcMessageFormatter::formatMessage("Connected, joining the chat room..."));
    session->sendCommand(IrcCommand::createJoin(CHANNEL));
}

void IrcClient::onConnecting()
{
    textEdit->append(IrcMessageFormatter::formatMessage("Connecting the chat server..."));
}

void IrcClient::onDisconnected()
{
    textEdit->append(IrcMessageFormatter::formatMessage("Disconnected from the chat server."));
}

void IrcClient::onTextEntered()
{
    IrcCommand* command = IrcCommand::createMessage(CHANNEL, lineEdit->text());

    // echo own messages
    IrcMessage* msg = IrcMessage::fromCommand(session->nickName(), command, session);
    receiveMessage(msg);
    delete msg;

    lineEdit->clear();
    session->sendCommand(command);
}

void IrcClient::onChannelAdded(IrcChannel* channel)
{
    IrcUserModel* model = channel->model();
    model->setDisplayRole(Irc::NameRole);

    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(model);
    proxy->setSourceModel(model);
    proxy->setDynamicSortFilter(true);
    proxy->sort(0, Qt::AscendingOrder);

    listView->setModel(proxy);
}

void IrcClient::receiveMessage(IrcMessage* message)
{
    QString html = IrcMessageFormatter::formatMessage(message);
    if (!html.isEmpty())
        textEdit->append(html);
}

void IrcClient::createUi()
{
    QWidget* container = new QWidget(this);

    textEdit = new QTextEdit(container);
    textEdit->setReadOnly(true);
    textEdit->append(IrcMessageFormatter::formatMessage(tr("Welcome to the Communi %1 example client.").arg(COMMUNI_VERSION_STR)));
    textEdit->append(IrcMessageFormatter::formatMessage(tr("PS. This example connects %1 and joins %2.").arg(SERVER, CHANNEL)));

    lineEdit = new QLineEdit(container);
    textEdit->setFocusProxy(lineEdit);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(onTextEntered()));

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(textEdit);
    layout->addWidget(lineEdit);

    listView = new QListView(this);

    addWidget(container);
    addWidget(listView);

    setStretchFactor(0, 4);
    setStretchFactor(1, 1);
}

void IrcClient::createSession()
{
    session = new IrcSession(this);
    connect(session, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(session, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(session, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(session, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(receiveMessage(IrcMessage*)));

    IrcChannelModel* model = new IrcChannelModel(session);
    connect(model, SIGNAL(channelAdded(IrcChannel*)), this, SLOT(onChannelAdded(IrcChannel*)));

    qsrand(QTime::currentTime().msec());

    session->setHost(SERVER);
    session->setUserName("communi");
    session->setNickName(tr("Communi%1").arg(qrand() % 99999));
    session->setRealName(tr("Communi %1 example client").arg(COMMUNI_VERSION_STR));
}
