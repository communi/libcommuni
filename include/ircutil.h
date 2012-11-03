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

#ifndef IRCUTIL_H
#define IRCUTIL_H

#include <IrcGlobal>
#include <QString>

class COMMUNI_EXPORT IrcUtil
{
public:
    static QString messageToHtml(const QString& message);

    enum ColorCode {
        White = 0,
        Black = 1,
        Blue = 2, Navy = 2,
        Green = 3,
        Red = 4,
        Brown = 5, Maroon = 5,
        Purple = 6,
        Orange = 7, Olive = 7,
        Yellow = 8,
        LightGreen = 9, Lime = 9,
        DarkCyan = 10, Teal = 10,
        LightCyan = 11, Cyan = 11, Aqua = 11,
        LightBlue = 12, RoyalBlue = 12,
        Pink = 13, Fuchsia = 13,
        Grey = 14, Gray = 14, DarkGrey = 14, DarkGray = 14,
        LightGrey = 15, LightGray = 15, Silver = 15
    };

    static QString colorCodeToName(uint code, const QString& defaultColor = QLatin1String("black"));
    static void setColorName(uint code, const QString& color);
};

#endif // IRCUTIL_H
