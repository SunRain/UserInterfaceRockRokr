include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += plugin \
          link_pkgconfig

TARGET = $${PLUGINS_PREFIX}/ffmpeg_decoder

include(libPhoenixPlayer/Plugins/Decoder/ffmpeg/ffmpeg.pri)

PKGCONFIG += libavcodec libavformat libavutil

DEFINES += __STDC_CONSTANT_MACROS

include(plugins_common_cmd.pri)
