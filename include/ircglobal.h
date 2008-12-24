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

#ifndef IRC_GLOBAL_H
#define IRC_GLOBAL_H

#include <QtGlobal>

#if BUILD_IRC
#    define IRC_EXPORT Q_DECL_EXPORT
#else
#    define IRC_EXPORT Q_DECL_IMPORT
#endif // BUILD_IRC

/*!
    \relates Irc

    Blaa
 */
#define IRC_VERSION 0x000200

/*!
    \relates Irc

    Blee
 */
#define IRC_VERSION_STR "0.2.0"

#endif // IRC_GLOBAL_H
