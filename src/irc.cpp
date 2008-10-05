/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#include "irc.h"
#include <libircclient.h>
#include <QRegExp>

/*!
    \class Irc irc.h
    \ingroup LibIrcClient-Qt
    \brief Irc provides IRC related utility functions.

 */

static const size_t BUFLEN = 8096;

/*!
    Returns the libircclient version number in form
    0xMMNNPP (MM = major, NN = minor, PP = patch).
 */
uint Irc::libVersion()
{
    uint high = 0;
    uint low = 0;
    irc_get_version(&high, &low);
    return (high << 16) + (low << 8);
}

/*!
    Returns the libircclient version number as string
    in form M.N.P (M = major, N = minor, P = patch).
 */
QString Irc::libVersionString()
{
    uint number = libVersion();
    QString text("%1.%2.%3");
    text = text.arg((number & 0xFF0000) >> 16)
               .arg((number & 0x00FF00) >> 8)
               .arg((number & 0x0000FF));
    return text;
}

/*!
    Parses and returns the nick part from target.
 */
QString Irc::nickFromTarget(const QString& target)
{
    char buffer[BUFLEN-1];
    irc_target_get_nick(target.toUtf8(), buffer, BUFLEN);
    return QString::fromUtf8(buffer);
}

/*!
    Parses and returns the host part from target.
 */
QString Irc::hostFromTarget(const QString& target)
{
    char buffer[BUFLEN-1];
    irc_target_get_host(target.toUtf8(), buffer, BUFLEN);
    return QString::fromUtf8(buffer);
}

static QString irc_colorStringHelper(char* message)
{
    QString value = QString::fromUtf8(message);
    free(message);
    return value;
}

/*!
    \sa irc_color_strip_from_mirc
*/
QString Irc::colorStripFromMirc(const QString& message)
{
    char* tmp = irc_color_strip_from_mirc(message.toUtf8());
    return irc_colorStringHelper(tmp);
}

/*!
    \sa irc_color_convert_from_mirc
*/
QString Irc::colorConvertFromMirc(const QString& message)
{
    char* tmp = irc_color_convert_from_mirc(message.toUtf8());
    return irc_colorStringHelper(tmp);
}

/*!
    \sa irc_color_convert_to_mirc
*/
QString Irc::colorConvertToMirc(const QString& message)
{
    char* tmp = irc_color_convert_to_mirc(message.toUtf8());
    return irc_colorStringHelper(tmp);
}

/*!
    \sa irc_color_convert_to_html
*/
QString Irc::colorConvertToHtml(const QString& message)
{
    QString tmp = colorConvertToMirc(message);
    tmp.replace(QRegExp("\\[([^\\]]+)\\]([^\\[]*)\\(/[^\\]]+)\\]"), "<\\1>\\2<\\3>");
    return tmp;
}
