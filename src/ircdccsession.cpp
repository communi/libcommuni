/*
* Copyright (C) 2008-2009 J-P Nurmi jpnurmi@gmail.com
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

#include "ircdccsession.h"

/*!
    \class Irc::DccSession ircdccsession.h
    \brief The Irc::DccSession class provides a DCC session.

    TODO
 */

namespace Irc
{
    class DccSessionPrivate
    {
        Q_DECLARE_PUBLIC(DccSession)

    public:
        DccSessionPrivate();

        DccSession* q_ptr;
    };

    DccSessionPrivate::DccSessionPrivate() :
        q_ptr(0)
    {
    }

    /*!
        Constructs a new DCC session with \a parent.
     */
    DccSession::DccSession(QObject* parent) : QObject(parent), d_ptr(new DccSessionPrivate)
    {
        Q_D(DccSession);
        d->q_ptr = this;
    }

    /*!
        Destructs the DCC session.
     */
    DccSession::~DccSession()
    {
        Q_D(DccSession);
        delete d;
    }

    Session* DccSession::session() const
    {
        return 0;
    }

    bool DccSession::accept()
    {
        return false;
    }

    bool DccSession::decline()
    {
        return false;
    }

    bool DccSession::sendMessage(const QString& message)
    {
        return false;
    }

    bool DccSession::sendFile(const QString& fileName)
    {
        return false;
    }
}

//#include "moc_ircdccsession.cpp"
