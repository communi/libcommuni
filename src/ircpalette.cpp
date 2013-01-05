/*
* Copyright (C) 2008-2013 J-P Nurmi <jpnurmi@gmail.com>
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

    \sa <a href="http://www.mirc.com/colors.html">mIRC Colors</a>
 */

/*!
    \enum IrcPalette::Color
    This enum describes the built-in colors in the palette.

    The default values are
    <a href="http://www.w3.org/TR/SVG/types.html#ColorKeywords">SVG color keyword names</a>
    provided by the World Wide Web Consortium
 */

/*!
    \var IrcPalette::White
    \brief The default value is "white"
 */

/*!
    \var IrcPalette::Black
    \brief The default value is "black"
 */

/*!
    \var IrcPalette::Blue
    \brief The default value is "navy"
 */

/*!
    \var IrcPalette::Green
    \brief The default value is "green"
 */

/*!
    \var IrcPalette::Red
    \brief The default value is "red"
 */

/*!
    \var IrcPalette::Brown
    \brief The default value is "maroon"
 */

/*!
    \var IrcPalette::Purple
    \brief The default value is "purple"
 */

/*!
    \var IrcPalette::Orange
    \brief The default value is "olive"
 */

/*!
    \var IrcPalette::Yellow
    \brief The default value is "yellow"
 */

/*!
    \var IrcPalette::LightGreen
    \brief The default value is "lime"
 */

/*!
    \var IrcPalette::Cyan
    \brief The default value is "teal"
 */

/*!
    \var IrcPalette::LightCyan
    \brief The default value is "aqua"
 */

/*!
    \var IrcPalette::LightBlue
    \brief The default value is "royalblue"
 */

/*!
    \var IrcPalette::Pink
    \brief The default value is "fuchsia"
 */

/*!
    \var IrcPalette::Gray
    \brief The default value is "gray"
 */

/*!
    \var IrcPalette::LightGray
    \brief The default value is "lightgray"
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
    Converts a \a color to a color name. If the \a color
    is unknown, the function returns the \a fallback color name.

    \sa setColorName()
*/
QString IrcPalette::colorName(uint color, const QString& fallback) const
{
    return d->colors.value(color, fallback);
}

/*!
    Assigns a \a name for \a color.

    \sa colorName()
*/
void IrcPalette::setColorName(uint color, const QString& name)
{
    d->colors.insert(color, name);
}
