include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += plugin

QT += network

TARGET = $${PLUGINS_PREFIX}/LyricsBaidu

include (libPhoenixPlayer/Plugins/MetadataLookup/Baidu/Baidu.pri)

include(plugins_common_cmd.pri)

