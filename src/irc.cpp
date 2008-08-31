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

static const size_t BUFLEN = 8096;

uint Irc::libVersion()
{
    uint high = 0;
    uint low = 0;
    irc_get_version(&high, &low);
    return (high << 16) + (low << 8);
}

QString Irc::libVersionString()
{
    uint number = libVersion();
    QString text("%1.%2.%3");
    text = text.arg((number & 0xFF0000) >> 16)
               .arg((number & 0x00FF00) >> 8)
               .arg((number & 0x0000FF));
    return text;
}

QString Irc::nickFromTarget(const QString& target)
{
    char buffer[BUFLEN-1];
    irc_target_get_nick(target.toUtf8(), buffer, BUFLEN);
    return QString::fromUtf8(buffer);
}

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

QString Irc::colorStripFromMirc(const QString& message)
{
    char* tmp = irc_color_strip_from_mirc(message.toUtf8());
    return irc_colorStringHelper(tmp);
}

QString Irc::colorConvertFromMirc(const QString& message)
{
    char* tmp = irc_color_convert_from_mirc(message.toUtf8());
    return irc_colorStringHelper(tmp);
}

QString Irc::colorConvertToMirc(const QString& message)
{
    char* tmp = irc_color_convert_to_mirc(message.toUtf8());
    return irc_colorStringHelper(tmp);
}
