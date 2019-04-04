
INCLUDEPATH += \
        $$PWD

isEmpty (LIB_DIR){
    LIB_DIR = /opt/PhoenixPlayer
}

QMAKE_LIBDIR += \
    $${LIB_PHOENIX_DIR} \
    $${LIB_DIR}/lib

QMAKE_RPATHDIR += \
    $${LIB_PHOENIX_DIR} \
    $${LIB_DIR}/lib

LIBS += -lPhoenixPlayer

json.files = $${OTHER_FILES}
target.path = $$LIB_DIR/plugins
json.path = $$LIB_DIR/plugins

INSTALLS += target json

win32 {
    COPY = copy /y
    MKDIR = mkdir
} else {
    COPY = cp
    MKDIR = mkdir -p
}

!equals($${_PRO_FILE_PWD_}, $${OUT_PWD}) {
    for(f, OTHER_FILES){
        #TODO need windows basename cmd
        unix:base_name = $$basename(f)
        dist = $${PLUGINS_PREFIX}
        dist_file = $${PLUGINS_PREFIX}/$${base_name}
        !exists($$dist):system($$MKDIR $$dist)
        !exists($$dist_file):system($$COPY $$f $$dist_file)
    }
}
