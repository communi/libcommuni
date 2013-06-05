/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QSplitter>
#include <QHash>

class IrcSession;
class IrcMessage;
class IrcChannel;
class IrcUserModel;
class IrcChannelModel;
class IrcCommandParser;

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QCompleter)
QT_FORWARD_DECLARE_CLASS(QModelIndex)
QT_FORWARD_DECLARE_CLASS(QTextDocument)

class IrcClient : public QSplitter
{
    Q_OBJECT

public:
    IrcClient(QWidget* parent = 0);
    ~IrcClient();

private slots:
    void onConnected();
    void onConnecting();
    void onDisconnected();

    void onTextEntered();

    void onChannelAdded(IrcChannel* channel);
    void onChannelRemoved(IrcChannel* channel);
    void onChannelActivated(const QModelIndex& index);

    void receiveServerMessage(IrcMessage* message);
    void receiveChannelMessage(IrcMessage* message);

private:
    void createUi();
    void createParser();
    void createSession();

    QLineEdit* lineEdit;
    QTextEdit* textEdit;
    QCompleter* completer;
    QListView* userList;
    QListView* channelList;

    IrcCommandParser* parser;

    QTextDocument* serverDocument;
    QHash<IrcChannel*, QTextDocument*> channelDocuments;

    IrcSession* session;
    IrcChannelModel* channelModel;
    QHash<IrcChannel*, IrcUserModel*> userModels;
};

#endif // IRCCLIENT_H
