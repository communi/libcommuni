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

class IrcSession;
class IrcMessage;
class IrcChannel;

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QTextEdit)

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
    void receiveMessage(IrcMessage* message);

private:
    void createUi();
    void createSession();

    QLineEdit* lineEdit;
    QListView* listView;
    QTextEdit* textEdit;

    IrcSession* session;
};

#endif // IRCCLIENT_H
