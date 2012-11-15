/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
* Copyright (C) 2010-2011 SmokeX <smokexjc@gmail.com>
* Copyright (C) 2012 Mark Johnson <marknotgeorge@googlemail.com>
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

#include "ircpalette.h"
#include <QHash>

/*!
    \file ircpalette.h
    \brief #include &lt;IrcPalette&gt;
 */

/*!
    \class IrcPalette ircpalette.h <IrcPalette>
    \ingroup utility
    \brief The IrcPalette class provides IRC color names

    \sa http://www.mirc.com/colors.html
 */

class IrcPalettePrivate : public QSharedData
{
public:
    QHash<uint, QString> colors;
};

static QHash<uint, QString>& irc_default_colors()
{
    static QHash<uint, QString> x;
    if (x.isEmpty()) {
        x.insert(IrcPalette::White, QLatin1String("white"));
        x.insert(IrcPalette::Black, QLatin1String("black"));
        x.insert(IrcPalette::Blue, QLatin1String("navy"));
        x.insert(IrcPalette::Green, QLatin1String("green"));
        x.insert(IrcPalette::Red, QLatin1String("red"));
        x.insert(IrcPalette::Brown, QLatin1String("maroon"));
        x.insert(IrcPalette::Purple, QLatin1String("purple"));
        x.insert(IrcPalette::Orange, QLatin1String("olive"));
        x.insert(IrcPalette::Yellow, QLatin1String("yellow"));
        x.insert(IrcPalette::LightGreen, QLatin1String("lime"));
        x.insert(IrcPalette::Cyan, QLatin1String("teal"));
        x.insert(IrcPalette::LightCyan, QLatin1String("aqua"));
        x.insert(IrcPalette::LightBlue, QLatin1String("royalblue"));
        x.insert(IrcPalette::Pink, QLatin1String("fuchsia"));
        x.insert(IrcPalette::Gray, QLatin1String("gray"));
        x.insert(IrcPalette::LightGray, QLatin1String("lightgray"));
    }
    return x;
}

/*!
    Constructs a new palette.
 */
IrcPalette::IrcPalette() : d(new IrcPalettePrivate)
{
    d->colors = irc_default_colors();
}

/*!
    Constructs a copy of \a other palette.
 */
IrcPalette::IrcPalette(const IrcPalette& other) : d(other.d)
{
}

/*!
    Assigns an \a other palette to this.
 */
IrcPalette& IrcPalette::operator=(const IrcPalette& other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*!
    Destructs the palette.
 */
IrcPalette::~IrcPalette()
{
}

/*!
    Converts a color \a code to a color name. If the color \a code
    is unknown, the function returns \a defaultColor.

    \sa setColorName()
*/
QString IrcPalette::colorName(uint code, const QString& defaultColor) const
{
    return d->colors.value(code, defaultColor);
}

/*!
    Assigns a \a color name for \a code.

    \sa colorName()
*/
void IrcPalette::setColorName(uint code, const QString& color)
{
    d->colors.insert(code, color);
}
