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
#include <IrcUser>
#include <IrcBuffer>
#include <IrcSession>
#include <IrcCommand>
#include <IrcMessage>
#include <IrcUserModel>
#include <IrcBufferModel>
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

void IrcClient::onTextEdited()
{
    // clear the possible error indication
    lineEdit->setStyleSheet(QString());
}

void IrcClient::onTextEntered()
{
    QString input = lineEdit->text();
    IrcCommand* command = parser->parse(input);
    if (command) {
        session->sendCommand(command);

        // echo own messages (servers do not send our own messages back)
        if (command->type() == IrcCommand::Message || command->type() == IrcCommand::CtcpAction) {
            IrcMessage* msg = IrcMessage::fromCommand(session->nickName(), command, session);
            receiveBufferMessage(msg);
            delete msg;
        }

        lineEdit->clear();
    } else if (input.length() > 1) {
        QString error;
        QString command = lineEdit->text().mid(1).split(" ", QString::SkipEmptyParts).value(0).toUpper();
        if (parser->commands().contains(command))
            error = tr("[ERROR] Syntax: %1").arg(parser->syntax(command).replace("<", "&lt;").replace(">", "&gt;"));
        else
            error = tr("[ERROR] Unknown command: %1").arg(command);
        textEdit->append(IrcMessageFormatter::formatMessage(error));
        lineEdit->setStyleSheet("background: salmon");
    }
}

void IrcClient::onBufferAdded(IrcBuffer* buffer)
{
    // joined a buffer - start listening to buffer specific messages
    connect(buffer, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(receiveBufferMessage(IrcMessage*)));

    // create a document for storing the buffer specific messages
    QTextDocument* document = new QTextDocument(buffer);
    bufferDocuments.insert(buffer, document);

    // create a model for buffer users
    IrcUserModel* userModel = new IrcUserModel(buffer);
    userModel->setDisplayRole(Irc::NameRole);
    userModels.insert(buffer, userModel);

    // activate the new buffer
    int idx = bufferModel->buffers().indexOf(buffer);
    if (idx != -1)
        bufferList->setCurrentIndex(bufferModel->index(idx));
}

void IrcClient::onBufferRemoved(IrcBuffer* buffer)
{
    // the buffer specific models and documents are no longer needed
    delete userModels.take(buffer);
    delete bufferDocuments.take(buffer);
}

void IrcClient::onBufferActivated(const QModelIndex& index)
{
    IrcBuffer* buffer = index.data(Irc::BufferRole).value<IrcBuffer*>();

    // user list and nick completion for the current buffer
    IrcUserModel* userModel = userModels.value(buffer);
    QSortFilterProxyModel* proxy = qobject_cast<QSortFilterProxyModel*>(userList->model());
    if (proxy)
        proxy->setSourceModel(userModel);
    completer->setModel(userModel);

    // document for the current buffer
    QTextDocument* document = bufferDocuments.value(buffer);
    if (document)
        textEdit->setDocument(document);
    else
        textEdit->setDocument(serverDocument);

    // keep the command parser aware of the context
    if (buffer)
        parser->setCurrentTarget(buffer->title());
}

void IrcClient::onUserActivated(const QModelIndex& index)
{
    IrcUser* user = index.data(Irc::UserRole).value<IrcUser*>();

    if (user) {
        IrcBuffer* buffer = bufferModel->addBuffer(user->name());

        // activate the new query
        int idx = bufferModel->buffers().indexOf(buffer);
        if (idx != -1)
            bufferList->setCurrentIndex(bufferModel->index(idx));
    }
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

void IrcClient::receiveBufferMessage(IrcMessage* message)
{
    IrcBuffer* buffer = qobject_cast<IrcBuffer*>(sender());
    if (!buffer)
        buffer = bufferList->currentIndex().data(Irc::BufferRole).value<IrcBuffer*>();

    QTextDocument* document = bufferDocuments.value(buffer);
    if (document) {
        QString html = IrcMessageFormatter::formatMessage(message);
        if (!html.isEmpty())
            appendHtml(document, html);
    }
}

void IrcClient::createUi()
{
    setWindowTitle(tr("Communi %1 example client").arg(COMMUNI_VERSION_STR));

    // keep track of buffers
    bufferModel = new IrcBufferModel(session);
    bufferList = new QListView(this);
    bufferList->setFocusPolicy(Qt::NoFocus);
    bufferList->setModel(bufferModel);
    connect(bufferModel, SIGNAL(bufferAdded(IrcBuffer*)), this, SLOT(onBufferAdded(IrcBuffer*)));
    connect(bufferModel, SIGNAL(bufferRemoved(IrcBuffer*)), this, SLOT(onBufferRemoved(IrcBuffer*)));

    // keep the command parser aware of the context
    connect(bufferModel, SIGNAL(channelsChanged(QStringList)), parser, SLOT(setChannels(QStringList)));

    // keep track of the current buffer, see also onBufferActivated()
    connect(bufferList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onBufferActivated(QModelIndex)));

    // IrcBufferModel::messageIgnored() is emitted for non-buffer specific messages
    connect(bufferModel, SIGNAL(messageIgnored(IrcMessage*)), this, SLOT(receiveServerMessage(IrcMessage*)));

    // create a document for non-buffer specific messages
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
    connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited()));

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

    // open a private query when double clicking a user
    connect(userList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onUserActivated(QModelIndex)));

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

    addWidget(bufferList);
    addWidget(container);

    setStretchFactor(0, 1);
    setStretchFactor(1, 3);

    setHandleWidth(1);
}

void IrcClient::createParser()
{
    // create a command parser and teach it some commands. notice also
    // that we must keep the command parser aware of the context in
    // createUi() and onBufferActivated()

    parser = new IrcCommandParser(this);
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
