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
#include <QtCore/qmetatype.h>
#include <QtCore/qscopedpointer.h>

IRC_BEGIN_NAMESPACE

class IrcPalette;
class IrcTextFormatPrivate;

class IRC_UTIL_EXPORT IrcTextFormat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(IrcPalette* palette READ palette CONSTANT)
    Q_PROPERTY(QString urlPattern READ urlPattern WRITE setUrlPattern)

public:
    explicit IrcTextFormat(QObject* parent = 0);
    virtual ~IrcTextFormat();

    IrcPalette* palette() const;

    QString urlPattern() const;
    void setUrlPattern(const QString& pattern);

    Q_INVOKABLE QString toHtml(const QString& text) const;
    Q_INVOKABLE QString toPlainText(const QString& text) const;

private:
    QScopedPointer<IrcTextFormatPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcTextFormat)
    Q_DISABLE_COPY(IrcTextFormat)
};

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcTextFormat*))

#endif // IRCTEXTFORMAT_H
