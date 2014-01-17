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
    Q_ENUMS(SpanFormat)

public:
    explicit IrcTextFormat(QObject* parent = 0);
    virtual ~IrcTextFormat();

    IrcPalette* palette() const;

    QString urlPattern() const;
    void setUrlPattern(const QString& pattern);

    enum SpanFormat { SpanStyle, SpanClass };

    SpanFormat spanFormat() const;
    void setSpanFormat(SpanFormat format);

    Q_INVOKABLE QString toHtml(const QString& text) const;
    Q_INVOKABLE QString toPlainText(const QString& text) const;

private:
    QScopedPointer<IrcTextFormatPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcTextFormat)
    Q_DISABLE_COPY(IrcTextFormat)
};

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcTextFormat*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcTextFormat::SpanFormat))

#endif // IRCTEXTFORMAT_H
