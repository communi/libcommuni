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

#ifndef IRCTEXTFORMAT_H
#define IRCTEXTFORMAT_H

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qscopedpointer.h>

IRC_BEGIN_NAMESPACE

class IrcTextFormatPrivate;

class IRC_UTIL_EXPORT IrcTextFormat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString urlPattern READ urlPattern WRITE setUrlPattern)

public:
    explicit IrcTextFormat(QObject* parent = 0);
    virtual ~IrcTextFormat();

    QString urlPattern() const;
    void setUrlPattern(const QString& pattern);

    Q_INVOKABLE QString colorName(int color, const QString& fallback = QLatin1String("black")) const;
    Q_INVOKABLE void setColorName(int color, const QString& name);

    Q_INVOKABLE QString toHtml(const QString& text) const;
    Q_INVOKABLE QString toPlainText(const QString& text) const;

private:
    QScopedPointer<IrcTextFormatPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcTextFormat)
    Q_DISABLE_COPY(IrcTextFormat)
};

IRC_END_NAMESPACE

#endif // IRCTEXTFORMAT_H
