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
* $Id: irc.h 2 2008-08-31 18:33:45Z jpnurmi $
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>

#if BUILD_LIBIRCCLIENT_QT
#    define LIBIRCCLIENT_QT_EXPORT Q_DECL_EXPORT
#else
#    define LIBIRCCLIENT_QT_EXPORT Q_DECL_IMPORT
#endif // BUILD_LIBIRCCLIENT_QT

#endif // GLOBAL_H
