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
#include <QTextDocument>
#include <QTextCursor>
#include <QVBoxLayout>
#include <QCompleter>
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
#include <IrcCommandParser>

static const char* CHANNEL = "#communi";
static const char* SERVER = "irc.freenode.net";

IrcClient::IrcClient(QWidget* parent) : QSplitter(parent)
{
    createSession();
    createParser();
    createUi();

    session->open();
}

IrcClient::~IrcClient()
{
    if (session->isActive()) {
        session->sendCommand(IrcCommand::createQuit(session->realName()));
        // let the server process the quit message and close the connection
        session->socket()->waitForDisconnected(1000);
    }
}

void IrcClient::onConnected()
{
    textEdit->append(IrcMessageFormatter::formatMessage("! Connected to %1.").arg(SERVER));
    textEdit->append(IrcMessageFormatter::formatMessage("! Joining %1...").arg(CHANNEL));
    session->sendCommand(IrcCommand::createJoin(CHANNEL));
}

void IrcClient::onConnecting()
{
    textEdit->append(IrcMessageFormatter::formatMessage("! Connecting %1...").arg(SERVER));
}

void IrcClient::onDisconnected()
{
    textEdit->append(IrcMessageFormatter::formatMessage("! Disconnected from %1.").arg(SERVER));
}

void IrcClient::onTextEntered()
{
    IrcCommand* command = parser->parse(lineEdit->text());
    if (command) {
        session->sendCommand(command);

        // echo own messages (servers do not send our own messages back)
        if (command->type() == IrcCommand::Message || command->type() == IrcCommand::CtcpAction) {
            IrcMessage* msg = IrcMessage::fromCommand(session->nickName(), command, session);
            receiveChannelMessage(msg);
            delete msg;
        }

        lineEdit->clear();
    }
}

void IrcClient::onChannelAdded(IrcChannel* channel)
{
    // joined a channel - start listening to channel specific messages
    connect(channel, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(receiveChannelMessage(IrcMessage*)));

    // create a document for storing the channel specific messages
    QTextDocument* document = new QTextDocument(channel);
    channelDocuments.insert(channel, document);

    // create a model for channel users
    IrcUserModel* userModel = new IrcUserModel(channel);
    userModel->setDisplayRole(Irc::NameRole);
    userModels.insert(channel, userModel);

    // keep the command parser aware of the context
    parser->setChannels(channelModel->titles());

    // activate the new channel
    int idx = channelModel->channels().indexOf(channel);
    if (idx != -1)
        channelList->setCurrentIndex(channelModel->index(idx));
}

void IrcClient::onChannelRemoved(IrcChannel* channel)
{
    // left a channel - the channel specific models and documents are no longer needed
    delete userModels.take(channel);
    delete channelDocuments.take(channel);

    // keep the command parser aware of the context
    parser->setChannels(channelModel->titles());
}

void IrcClient::onChannelActivated(const QModelIndex& index)
{
    IrcChannel* channel = index.data(Irc::ChannelRole).value<IrcChannel*>();

    // user list and nick completion for the current channel
    IrcUserModel* userModel = userModels.value(channel);
    QSortFilterProxyModel* proxy = qobject_cast<QSortFilterProxyModel*>(userList->model());
    if (proxy)
        proxy->setSourceModel(userModel);
    completer->setModel(userModel);

    // document for the current channel
    QTextDocument* document = channelDocuments.value(channel);
    if (document)
        textEdit->setDocument(document);
    else
        textEdit->setDocument(serverDocument);

    // keep the command parser aware of the context
    if (channel)
        parser->setCurrentTarget(channel->title());
}

static void appendHtml(QTextDocument* document, const QString& html)
{
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::End);
    if (!document->isEmpty())
        cursor.insertBlock();
    cursor.insertHtml(html);
    cursor.endEditBlock();
}

void IrcClient::receiveServerMessage(IrcMessage* message)
{
    QString html = IrcMessageFormatter::formatMessage(message);
    if (!html.isEmpty())
        appendHtml(serverDocument, html);
}

void IrcClient::receiveChannelMessage(IrcMessage* message)
{
    IrcChannel* channel = qobject_cast<IrcChannel*>(sender());
    if (!channel)
        channel = channelList->currentIndex().data(Irc::ChannelRole).value<IrcChannel*>();

    QTextDocument* document = channelDocuments.value(channel);
    if (document) {
        QString html = IrcMessageFormatter::formatMessage(message);
        if (!html.isEmpty())
            appendHtml(document, html);
    }
}

void IrcClient::createUi()
{
    setWindowTitle(tr("Communi %1 example client").arg(COMMUNI_VERSION_STR));

    // keep track of channels
    channelModel = new IrcChannelModel(session);
    channelList = new QListView(this);
    channelList->setFocusPolicy(Qt::NoFocus);
    channelList->setModel(channelModel);
    connect(channelModel, SIGNAL(channelAdded(IrcChannel*)), this, SLOT(onChannelAdded(IrcChannel*)));
    connect(channelModel, SIGNAL(channelRemoved(IrcChannel*)), this, SLOT(onChannelRemoved(IrcChannel*)));

    // keep track of the current channel, see also onChannelActivated()
    connect(channelList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onChannelActivated(QModelIndex)));

    // IrcChannelModel::messageIgnored() is emitted for non-channel specific messages
    connect(channelModel, SIGNAL(messageIgnored(IrcMessage*)), this, SLOT(receiveServerMessage(IrcMessage*)));

    // create a document for non-channel specific messages
    serverDocument = new QTextDocument(this);

    // a read-only text editor for showing the messages
    textEdit = new QTextEdit(this);
    textEdit->setDocument(serverDocument);
    textEdit->setReadOnly(true);
    textEdit->append(IrcMessageFormatter::formatMessage(tr("! Welcome to the Communi %1 example client.").arg(COMMUNI_VERSION_STR)));
    textEdit->append(IrcMessageFormatter::formatMessage(tr("! This example connects %1 and joins %2.").arg(SERVER, CHANNEL)));
    textEdit->append(IrcMessageFormatter::formatMessage(tr("! PS. Available commands: JOIN, ME, NICK, PART")));

    // a line editor for entering commands
    lineEdit = new QLineEdit(this);
    lineEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    textEdit->setFocusProxy(lineEdit);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(onTextEntered()));

    // nick name completion
    completer = new QCompleter(lineEdit);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionRole(Irc::NameRole);
    lineEdit->setCompleter(completer);

    // user list in alphabetical order
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
    proxy->setDynamicSortFilter(true);
    proxy->sort(0, Qt::AscendingOrder);

    userList = new QListView(this);
    userList->setFocusPolicy(Qt::NoFocus);
    userList->setModel(proxy);

    // the rest is just setting up the UI layout...
    QSplitter* splitter = new QSplitter(this);
    splitter->setHandleWidth(1);
    splitter->addWidget(textEdit);
    splitter->addWidget(userList);
    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 1);

    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(splitter);
    layout->addWidget(lineEdit);

    addWidget(channelList);
    addWidget(container);

    setStretchFactor(0, 1);
    setStretchFactor(1, 3);

    setHandleWidth(1);
}

void IrcClient::createParser()
{
    // create a command parser and teach it some commands. notice also
    // that we must keep the command parser aware of the context in:
    // onChannelAdded(), onChannelRemoved() and onChannelActivated()

    parser = new IrcCommandParser(this);
    parser->setPrefix("/");
    parser->addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser->addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser->addCommand(IrcCommand::Nick, "NICK <nick>");
    parser->addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
}

void IrcClient::createSession()
{
    session = new IrcSession(this);
    connect(session, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(session, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(session, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    qsrand(QTime::currentTime().msec());

    session->setHost(SERVER);
    session->setUserName("communi");
    session->setNickName(tr("Communi%1").arg(qrand() % 99999));
    session->setRealName(tr("Communi %1 example client").arg(COMMUNI_VERSION_STR));
}
