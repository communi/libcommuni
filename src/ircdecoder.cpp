/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
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

#include "ircdecoder_p.h"
#include "irccodecplugin.h"
#include <QCoreApplication>
#include <QPluginLoader>
#include <QStringList>
#include <QDebug>
#include <QDir>

Q_GLOBAL_STATIC(QList<IrcCodecPlugin*>, irc_codec_plugins)

IrcDecoder::IrcDecoder()
{
    d.fallback = QTextCodec::codecForName("UTF-8");
}

IrcDecoder::~IrcDecoder()
{
}

QByteArray IrcDecoder::encoding() const
{
    return d.fallback->name();
}

void IrcDecoder::setEncoding(const QByteArray& encoding)
{
    if (QTextCodec::availableCodecs().contains(encoding))
        d.fallback = QTextCodec::codecForName(encoding);
}

QString IrcDecoder::decode(const QByteArray& data) const
{
    static bool initialized = false;
    if (!initialized)
    {
        if (!const_cast<IrcDecoder*>(this)->loadPlugins())
            qWarning() << "IrcDecoder::decode(): no plugins available - expecting UTF-8";
        initialized = true;
    }

    QByteArray name = d.fallback->name();
    IrcCodecPlugin* plugin = irc_codec_plugins()->value(0);
    if (plugin)
        plugin->codecForData(data);
    QTextCodec* codec = QTextCodec::codecForName(name);
    if (!codec)
        codec = d.fallback;
    Q_ASSERT(codec);
    return codec->toUnicode(data);
}

bool IrcDecoder::loadPlugins()
{
    foreach (const QString& path, QCoreApplication::libraryPaths())
    {
        QDir dir(path);
        if (!dir.cd("communi"))
            continue;

        foreach (const QFileInfo& file, dir.entryInfoList(QDir::Files))
        {
            QPluginLoader loader(file.absoluteFilePath());
            IrcCodecPlugin* plugin = qobject_cast<IrcCodecPlugin*>(loader.instance());
            if (plugin)
                irc_codec_plugins()->append(plugin);
        }
    }
    return !irc_codec_plugins()->isEmpty();
}
