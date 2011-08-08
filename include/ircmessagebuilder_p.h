/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#ifndef IRCMESSAGEBUILDER_P_H
#define IRCMESSAGEBUILDER_P_H

#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

class IrcMessage;
class IrcNumericMessage;

class IrcMessageBuilder : public QObject
{
    Q_OBJECT

public:
    IrcMessageBuilder(QObject* parent = 0);

    bool handleMessage(IrcNumericMessage* message);

signals:
    void messageBuilt(IrcMessage* message);

private:
    QStringList motdLines;
};

#endif // IRCMESSAGEBUILDER_P_H
