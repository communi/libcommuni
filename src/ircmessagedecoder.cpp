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

#include "ircmessagedecoder_p.h"
#include "irccodecplugin.h"
#include <QCoreApplication>
#include <QPluginLoader>
#include <QStringList>
#include <QDebug>
#include <QMap>
#include <QDir>

extern "C" {
    int IsUTF8Text(const char* utf8, int len);
}

typedef QMap<QByteArray, IrcCodecPlugin*> IrcCodecPluginMap;
Q_GLOBAL_STATIC(IrcCodecPluginMap, irc_codec_plugins)

static QByteArray irc_plugin_key = qgetenv("COMMUNI_CODEC_PLUGIN");

COMMUNI_EXPORT void irc_set_codec_plugin(const QByteArray& key)
{
    irc_plugin_key = key;
}

IrcMessageDecoder::IrcMessageDecoder()
{
    d.fallback = QTextCodec::codecForName("ISO-8859-15");
}

IrcMessageDecoder::~IrcMessageDecoder()
{
}

QByteArray IrcMessageDecoder::encoding() const
{
    return d.fallback->name();
}

void IrcMessageDecoder::setEncoding(const QByteArray& encoding)
{
    if (QTextCodec::availableCodecs().contains(encoding))
        d.fallback = QTextCodec::codecForName(encoding);
}

QString IrcMessageDecoder::decode(const QByteArray& data) const
{
    // TODO: not thread safe
    static QByteArray pluginKey;
    static bool initialized = false;
    if (!initialized) {
        pluginKey = const_cast<IrcMessageDecoder*>(this)->initialize();
        initialized = true;
    }

    QTextCodec* codec = 0;
    if (IsUTF8Text(data, data.length())) {
        codec = QTextCodec::codecForName("UTF-8");
    } else {
        QByteArray name = d.fallback->name();
        IrcCodecPlugin* plugin = irc_codec_plugins()->value(pluginKey);
        if (plugin)
            name = plugin->codecForData(data);
        codec = QTextCodec::codecForName(name);
    }

    if (!codec)
        codec = d.fallback;
    Q_ASSERT(codec);
    return codec->toUnicode(data);
}

QByteArray IrcMessageDecoder::initialize()
{
    bool loaded = loadPlugins();
    QByteArray pluginKey = irc_plugin_key;
    if (!pluginKey.isEmpty() && !irc_codec_plugins()->contains(pluginKey)) {
        qWarning() << "IrcMessageDecoder:" << pluginKey << "plugin not loaded";
        if (loaded)
            qWarning() << "IrcMessageDecoder: available plugins:" << irc_codec_plugins()->keys();
    }
    if (!loaded)
        qWarning() << "IrcMessageDecoder: no plugins available";

    if (pluginKey.isEmpty() && !irc_codec_plugins()->isEmpty())
        pluginKey = irc_codec_plugins()->keys().last();
    return pluginKey;
}

#if defined(Q_OS_WIN)
#define COMMUNI_PATH_SEPARATOR ';'
#else
#define COMMUNI_PATH_SEPARATOR ':'
#endif

static QStringList pluginPaths()
{
    QStringList paths = QCoreApplication::libraryPaths();
    const QByteArray env = qgetenv("COMMUNI_PLUGIN_PATH");
    if (!env.isEmpty()) {
        foreach(const QString & path, QFile::decodeName(env).split(COMMUNI_PATH_SEPARATOR, QString::SkipEmptyParts)) {
            QString canonicalPath = QDir(path).canonicalPath();
            if (!canonicalPath.isEmpty() && !paths.contains(canonicalPath))
                paths += canonicalPath;
        }
    }
    static bool dbg = qgetenv("COMMUNI_DEBUG").toInt();
    if (dbg) qDebug() << "IrcMessageDecoder: dynamic plugin paths:" << paths;
    return paths;
}

bool IrcMessageDecoder::loadPlugins()
{
    static bool dbg = qgetenv("COMMUNI_DEBUG").toInt();
    foreach(QObject* instance, QPluginLoader::staticInstances()) {
        IrcCodecPlugin* plugin = qobject_cast<IrcCodecPlugin*>(instance);
        if (plugin) {
            irc_codec_plugins()->insert(plugin->key(), plugin);
            if (dbg) qDebug() << "IrcMessageDecoder: loaded static plugin:" << plugin->key();
        }
    }

    foreach(const QString & path, pluginPaths()) {
        QDir dir(path);
        if (!dir.cd("communi"))
            continue;

        foreach(const QFileInfo & file, dir.entryInfoList(QDir::Files)) {
            QPluginLoader loader(file.absoluteFilePath());
            IrcCodecPlugin* plugin = qobject_cast<IrcCodecPlugin*>(loader.instance());
            if (plugin) {
                irc_codec_plugins()->insert(plugin->key(), plugin);
                if (dbg) qDebug() << "IrcMessageDecoder: loaded dynamic plugin:" << file.fileName();
            }
        }
    }
    return !irc_codec_plugins()->isEmpty();
}
