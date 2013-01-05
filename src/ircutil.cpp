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

#include "ircutil.h"
#include "ircpalette.h"
#include "irctextformat.h"

/*!
    \file ircutil.h
    \brief #include &lt;IrcUtil&gt;
 */

/*!
    \class IrcUtil ircutil.h <IrcUtil>
    \ingroup utility
    \brief The IrcUtil class provides miscellaneous utility functions.
    \deprecated Use IrcTextFormat and IrcPalette instead.
 */

/*!
    \deprecated Use IrcTextFormat::messageToHtml() instead.
*/
QString IrcUtil::messageToHtml(const QString& message)
{
    return IrcTextFormat().messageToHtml(message);
}

/*!
    \deprecated Use IrcPalette::colorName() instead.
*/
QString IrcUtil::colorCodeToName(int code, const QString& defaultColor)
{
    return IrcPalette().colorName(code, defaultColor);
}
