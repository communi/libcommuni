/*
* Copyright (C) 2008-2014 The Communi Project
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
#include <QtCore/qmetatype.h>
#include <QtCore/qstringlist.h>

IRC_BEGIN_NAMESPACE

class IrcCommandParserPrivate;

class IRC_UTIL_EXPORT IrcCommandParser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList commands READ commands NOTIFY commandsChanged)
    Q_PROPERTY(QStringList triggers READ triggers WRITE setTriggers NOTIFY triggersChanged)
    Q_PROPERTY(QStringList channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_PROPERTY(QString target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool tolerant READ isTolerant WRITE setTolerant NOTIFY tolerancyChanged)
    Q_FLAGS(Details)

public:
    explicit IrcCommandParser(QObject* parent = 0);
    virtual ~IrcCommandParser();

    QStringList commands() const;

    enum Detail {
        Full = 0x0,
        NoTarget = 0x1,
        NoPrefix = 0x2,
        NoEllipsis = 0x4,
        NoParentheses = 0x8,
        NoBrackets = 0x10,
        NoAngles = 0x20,
        Visual = NoTarget | NoPrefix | NoEllipsis
    };
    Q_DECLARE_FLAGS(Details, Detail)

    Q_INVOKABLE QString syntax(const QString& command, Details details = Visual) const;

    Q_INVOKABLE void addCommand(IrcCommand::Type type, const QString& syntax);
    Q_INVOKABLE void removeCommand(IrcCommand::Type type, const QString& syntax = QString());

    QStringList triggers() const;

    QString target() const;
    QStringList channels() const;

    bool isTolerant() const;
    void setTolerant(bool tolerant);

    Q_INVOKABLE IrcCommand* parse(const QString& input) const;

public Q_SLOTS:
    void clear();
    void reset();

    void setTriggers(const QStringList& triggers);
    void setChannels(const QStringList& channels);
    void setTarget(const QString& target);

Q_SIGNALS:
    void commandsChanged(const QStringList& commands);
    void triggersChanged(const QStringList& triggers);
    void channelsChanged(const QStringList& channels);
    void targetChanged(const QString& target);
    void tolerancyChanged(bool tolerant);

private:
    QScopedPointer<IrcCommandParserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcCommandParser)
    Q_DISABLE_COPY(IrcCommandParser)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IrcCommandParser::Details)

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcCommandParser*))

#endif // IRCCOMMANDPARSER_H
