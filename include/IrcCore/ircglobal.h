/*
* Copyright (C) 2008-2013 The Communi Project
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

#ifndef IRCGLOBAL_H
#define IRCGLOBAL_H

#include <QtCore/qglobal.h>

/*!
    \file ircglobal.h
    \brief \#include &lt;IrcGlobal&gt;
 */

#if defined(IRC_SHARED)
#
#  if defined(BUILD_IRC_CORE)
#    define IRC_CORE_EXPORT Q_DECL_EXPORT
#  else
#    define IRC_CORE_EXPORT Q_DECL_IMPORT
#  endif
#
#  if defined(BUILD_IRC_MODEL)
#    define IRC_MODEL_EXPORT Q_DECL_EXPORT
#  else
#    define IRC_MODEL_EXPORT Q_DECL_IMPORT
#  endif
#
#  if defined(BUILD_IRC_UTIL)
#    define IRC_UTIL_EXPORT Q_DECL_EXPORT
#  else
#    define IRC_UTIL_EXPORT Q_DECL_IMPORT
#  endif
#
#elif defined(IRC_STATIC) || defined(BUILD_IRC_CORE) || defined(BUILD_IRC_MODEL) || defined(BUILD_IRC_UTIL)
#
#    define IRC_CORE_EXPORT
#    define IRC_MODEL_EXPORT
#    define IRC_UTIL_EXPORT
#
#else
#  error Installation problem: either IRC_SHARED or IRC_STATIC must be defined!
#endif

/*!
    \def IRC_VERSION

    This macro expands a numeric value of the form 0xMMNNPP (MM = major, NN = minor, PP = patch) that specifies Communi's version number.
    For example, if you compile your application against Communi 1.2.3, the IRC_VERSION macro will expand to 0x010203.

    You can use IRC_VERSION to use the latest Communi features where available. For example:
    \code
 #if IRC_VERSION >= 0x000300
     // SSL support since version 0.3.0
     connection->setSocket(new QSslSocket(connection));
 #endif
    \endcode

    \sa Irc::version()
 */
#define IRC_VERSION 0x030000

/*!
    \def IRC_VERSION_STR

    This macro expands to a string that specifies Communi's version number (for example, "1.2.3").
    This is the version against which the application is compiled.

    \sa Irc::version()
 */
#define IRC_VERSION_STR "3.0.0"

#ifdef IRC_NAMESPACE

# define IRC_PREPEND_NAMESPACE(name) ::IRC_NAMESPACE::name
# define IRC_USE_NAMESPACE using namespace ::IRC_NAMESPACE;
# define IRC_BEGIN_NAMESPACE namespace IRC_NAMESPACE {
# define IRC_END_NAMESPACE }
# define IRC_FORWARD_DECLARE_CLASS(name) \
    IRC_BEGIN_NAMESPACE class name; IRC_END_NAMESPACE \
    using IRC_PREPEND_NAMESPACE(name);

#else

# define IRC_PREPEND_NAMESPACE(name) ::name
# define IRC_USE_NAMESPACE
# define IRC_BEGIN_NAMESPACE
# define IRC_END_NAMESPACE
# define IRC_FORWARD_DECLARE_CLASS(name) class name;

#endif // IRC_NAMESPACE

#endif // IRCGLOBAL_H
