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

#ifndef IRC_H
#define IRC_H

#include "global.h"
#include <QtGlobal>
#include <QString>

namespace Irc
{
    LIBIRCCLIENT_QT_EXPORT uint libVersion();
    LIBIRCCLIENT_QT_EXPORT QString libVersionString();

    LIBIRCCLIENT_QT_EXPORT QString nickFromTarget(const QString& target);
    LIBIRCCLIENT_QT_EXPORT QString hostFromTarget(const QString& target);

    LIBIRCCLIENT_QT_EXPORT QString colorStripFromMirc(const QString& message);
    LIBIRCCLIENT_QT_EXPORT QString colorConvertFromMirc(const QString& message);
    LIBIRCCLIENT_QT_EXPORT QString colorConvertToMirc(const QString& message);
    LIBIRCCLIENT_QT_EXPORT QString colorConvertToHtml(const QString& message);
}

#endif // IRC_H
