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

#ifndef IRCPALETTE_H
#define IRCPALETTE_H

#include <IrcGlobal>
#include <QtCore/qstring.h>
#include <QtCore/qshareddata.h>

class IrcPalettePrivate;

class COMMUNI_EXPORT IrcPalette
{
public:
    IrcPalette();
    IrcPalette(const IrcPalette& other);
    IrcPalette& operator=(const IrcPalette& other);
    ~IrcPalette();

    enum ColorCode {
        White = 0,
        Black = 1,
        Blue = 2,
        Green = 3,
        Red = 4,
        Brown = 5,
        Purple = 6,
        Orange = 7,
        Yellow = 8,
        LightGreen = 9,
        Cyan = 10,
        LightCyan = 11,
        LightBlue = 12,
        Pink = 13,
        Gray = 14,
        LightGray = 15
    };

    QString colorName(uint code, const QString& defaultColor = QLatin1String("black")) const;
    void setColorName(uint code, const QString& color);

private:
    QSharedDataPointer<IrcPalettePrivate> d;
};

#endif // IRCPALETTE_H
