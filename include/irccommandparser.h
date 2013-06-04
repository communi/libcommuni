/*
* Copyright (C) 2008-2013 The Communi Project
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*/

#ifndef IRCCOMMANDPARSER_H
#define IRCCOMMANDPARSER_H

#include <IrcGlobal>
#include <IrcCommand>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class IrcCommandParserPrivate;

class COMMUNI_EXPORT IrcCommandParser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList commands READ commands NOTIFY commandsChanged)
    Q_PROPERTY(QStringList channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_PROPERTY(QString currentTarget READ currentTarget WRITE setCurrentTarget NOTIFY currentTargetChanged)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)

public:
    explicit IrcCommandParser(QObject* parent = 0);
    virtual ~IrcCommandParser();

    QStringList commands() const;

    Q_INVOKABLE void addCommand(IrcCommand::Type type, const QString& syntax);
    Q_INVOKABLE void removeCommand(IrcCommand::Type type, const QString& syntax = QString());

    QStringList channels() const;
    void setChannels(const QStringList& channels);

    QString currentTarget() const;
    void setCurrentTarget(const QString& target);

    QString prefix() const;
    void setPrefix(const QString& prefix);

    Q_INVOKABLE IrcCommand* parse(const QString& input) const;

Q_SIGNALS:
    void commandsChanged(const QStringList& commands);
    void channelsChanged(const QStringList& channels);
    void currentTargetChanged(const QString& target);
    void prefixChanged(const QString& prefix);

private:
    QScopedPointer<IrcCommandParserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcCommandParser)
    Q_DISABLE_COPY(IrcCommandParser)
};

#endif // IRCCOMMANDPARSER_H
