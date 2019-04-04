include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += plugin

QT += network

TARGET = $${PLUGINS_PREFIX}/LastFmMetaDataLookup

include (libPhoenixPlayer/Plugins/MetadataLookup/LastFM/LastFM.pri)

include(plugins_common_cmd.pri)
