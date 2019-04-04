include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += plugin

QT += sql

TARGET = $${PLUGINS_PREFIX}/SQLite3DAO

include(libPhoenixPlayer/Plugins/MusicLibrary/SQLite3/SQLite3.pri)

include(plugins_common_cmd.pri)

