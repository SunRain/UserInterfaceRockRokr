include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += \
        plugin \
        thread \
        warn_on \
        link_pkgconfig

TARGET = $${PLUGINS_PREFIX}/pulseaudio

include(libPhoenixPlayer/Plugins/OutPut/pulseaudio/pulseaudio.pri)

PKGCONFIG += libpulse-simple

include(plugins_common_cmd.pri)
