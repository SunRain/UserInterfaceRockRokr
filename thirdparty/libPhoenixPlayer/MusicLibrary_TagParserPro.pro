include (plugins_prefix.pri)

TEMPLATE = lib
CONFIG += plugin

TARGET = $${PLUGINS_PREFIX}/TagParserPro

QT += gui

include (libPhoenixPlayer/Plugins/MusicLibrary/TagParserPro/TagParserPro.pri)
include(plugins_common_cmd.pri)

include (libPhoenixPlayer/ThirdParty/Taglib/Taglib.pri)
LIBS += -ltag_mod

TAGLIB_BUILD_DIR = $${OUT_PWD}/../../Taglib/build
TAGLIB_TARGET_DIR = $${OUT_PWD}/../../Taglib/target
BUILD_TAGLIB = cd $${TAGLIB_BUILD_DIR} \
    && cmake $${PWD}/libPhoenixPlayer/ThirdParty/Taglib/src -DCMAKE_INSTALL_PREFIX=$${TAGLIB_TARGET_DIR} -DCMAKE_RELEASE_TYPE=Release \
    && make \
    && make install
system ($$MKDIR $$TAGLIB_BUILD_DIR)
system ($$MKDIR $$TAGLIB_TARGET_DIR)
system ($$BUILD_TAGLIB)

TAGLIB_FILE = \
    $${TAGLIB_TARGET_DIR}/lib/*.so \
    $${TAGLIB_TARGET_DIR}/lib/*.so.*

taglib.files = $${TAGLIB_FILE}
taglib.path = $${LIB_DIR}/lib
INSTALLS += taglib

COPY_LIB = cd $${TAGLIB_TARGET_DIR}/lib \
    && for i in `ls`;do if [ -f $i ];then $$COPY $i $${LIB_PHOENIX_DIR};fi;done

system ($$COPY_LIB)
