/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
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
#include <IrcPalette>
#include <QtCore/qstring.h>
#include <QtCore/qregexp.h>
#include <QtCore/qshareddata.h>

class IrcTextFormatPrivate;

class COMMUNI_EXPORT IrcTextFormat
{
public:
    IrcTextFormat();
    IrcTextFormat(const IrcTextFormat& other);
    IrcTextFormat& operator=(const IrcTextFormat& other);
    ~IrcTextFormat();

    IrcPalette palette() const;
    void setPalette(const IrcPalette& palette);

    QRegExp urlRegExp() const;
    void setUrlRegExp(const QRegExp& rx);

    QString messageToHtml(const QString& message) const;

private:
    mutable QSharedDataPointer<IrcTextFormatPrivate> d;
};

#endif // IRCTEXTFORMAT_H
