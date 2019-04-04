include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += plugin

TARGET = $${PLUGINS_PREFIX}/PhoenixBackend

include(libPhoenixPlayer/Plugins/Backend/PhoenixBackend/PhoenixBackend.pri)

QT += multimedia


include(plugins_common_cmd.pri)

#!equals($${_PRO_FILE_PWD_}, $${OUT_PWD}) {
#    for(f, OTHER_FILES){
#        #TODO need windows basename cmd
#        unix:base_name = $$basename(f)
#        dist = $${PLUGINS_PREFIX}
#        dist_file = $${PLUGINS_PREFIX}/$${base_name}
#        !exists($$dist):system($$MKDIR $$dist)
#        !exists($$dist_file):system($$COPY $$f $$dist_file)
#    }
#}
