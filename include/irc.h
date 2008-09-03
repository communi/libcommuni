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

class LIBIRCCLIENT_QT_EXPORT Irc
{
public:
    static uint libVersion();
    static QString libVersionString();

    static QString nickFromTarget(const QString& target);
    static QString hostFromTarget(const QString& target);

    static QString colorStripFromMirc(const QString& message);
    static QString colorConvertFromMirc(const QString& message);
    static QString colorConvertToMirc(const QString& message);
};

#endif // IRC_H
