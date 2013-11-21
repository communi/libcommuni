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

#ifndef IRCCOMPLETER_H
#define IRCCOMPLETER_H

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qmetatype.h>

IRC_BEGIN_NAMESPACE

class IrcBuffer;
class IrcCommandParser;
class IrcCompleterPrivate;

class IRC_UTIL_EXPORT IrcCompleter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(IrcBuffer* buffer READ buffer WRITE setBuffer NOTIFY bufferChanged)
    Q_PROPERTY(IrcCommandParser* parser READ parser WRITE setParser NOTIFY parserChanged)

public:
    explicit IrcCompleter(QObject* parent = 0);
    virtual ~IrcCompleter();

    IrcBuffer* buffer() const;
    IrcCommandParser* parser() const;

public Q_SLOTS:
    void setBuffer(IrcBuffer* buffer);
    void setParser(IrcCommandParser* parser);

    void complete(const QString& text, int cursor);

Q_SIGNALS:
    void bufferChanged(IrcBuffer* buffer);
    void parserChanged(IrcCommandParser* parser);

    void completed(const QString& text, int cursor);

private:
    QScopedPointer<IrcCompleterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcCompleter)
    Q_DISABLE_COPY(IrcCompleter)
};

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcCompleter*))

#endif // IRCCOMPLETER_H
