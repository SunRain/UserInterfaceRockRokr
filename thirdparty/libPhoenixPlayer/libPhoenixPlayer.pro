TEMPLATE = lib
TARGET = $${OUT_PWD}/../../target/lib/PhoenixPlayer
QT += network core qml

CONFIG += c++11
#CONFIG += WITH_QML_LIB

include (libPhoenixPlayer.pri)

VERSION = $$LIB_VERSION

include (libPhoenixPlayer/libPhoenixPlayer/Core/core.pri)
#include (libPhoenixPlayer/libPhoenixPlayer/QtQuick/qml.pri)

unix {
    isEmpty(LIB_DIR) {
        LIB_DIR = /opt/PhoenixPlayer/lib
    }
    target.path = $$LIB_DIR
    INSTALLS += target
}

#message($${OUT_PWD}/../../target/lib/PhoenixPlayer)
