#-------------------------------------------------
#
# Project created by QtCreator 2019-04-03T15:37:54
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $${OUT_PWD}/../target/RockRokr
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS Q_OS_LINUX
#DTK_TITLE_DRAG_WINDOW

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS = -fpermissive
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += dtkwidget
PKGCONFIG += xcb xcb-util x11 xext

#include("../thirdparty/qsyncable/qsyncable/qsyncable.pri")
include("../thirdparty/libPhoenixPlayer/libPhoenixPlayer/libPhoenixPlayer/Core/CoreHeaders.pri")
include("../thirdparty/libPhoenixPlayer/libPhoenixPlayer/ThirdParty/QCurl/src/qcurl.pri")
include("../thirdparty/qt-spek/qt-spek.pri")

SOURCES += \
    RKUtility.cpp \
    RockRokrApp.cpp \
    main.cpp \
    MainWindow.cpp \
    rockrokr_global.cpp \
    view/ImportView.cpp \
    view/MainWindowBGView.cpp \
    view/RockRokrView.cpp \
    view/leftbar/LBListItem.cpp \
    view/leftbar/LBListWidget.cpp \
    view/leftbar/LBPlaylistView.cpp \
    view/leftbar/LeftBar.cpp \
    view/leftbar/ListModel.cpp \
    view/leftbar/ListViewItemDelegate.cpp \
    view/leftbar/MainListView.cpp \
    view/playbar/PlayBar.cpp \
    view/playbar/VolumeWidget.cpp \
    view/playbar/WaveformSlider.cpp \
    view/titlebar/RKTitleBar.cpp \
    widget/ItemFragment.cpp \
    widget/LoadingWidget.cpp \
    widget/RKBoxWidget.cpp \
    widget/RKExpand.cpp \
    widget/RKImage.cpp \
    widget/RKListWidget.cpp \
    widget/RKOverlayWidget.cpp \
    widget/RKSearchEdit.cpp \
    widget/RKSlider.cpp \
    widget/RKStackWidget.cpp \
    widget/filter.cpp \
    widget/qtmaterialiconbutton.cpp \
    widget/qtmaterialoverlaywidget.cpp \
    widget/qtmaterialripple.cpp \
    widget/qtmaterialrippleoverlay.cpp

HEADERS += \
    MainWindow.h \
    RKUtility.h \
    RockRokrApp.h \
    rockrokr_global.h \
    view/ImportView.h \
    view/MainWindowBGView.h \
    view/RockRokrView.h \
    view/leftbar/LBListItem.h \
    view/leftbar/LBListWidget.h \
    view/leftbar/LBPlaylistView.h \
    view/leftbar/LeftBar.h \
    view/leftbar/ListModel.h \
    view/leftbar/ListViewItemDelegate.h \
    view/leftbar/MainListView.h \
    view/playbar/PlayBar.h \
    view/playbar/VolumeWidget.h \
    view/playbar/WaveformSlider.h \
    view/titlebar/RKTitleBar.h \
    widget/ItemFragment.h \
    widget/LoadingWidget.h \
    widget/RKBoxWidget.h \
    widget/RKExpand.h \
    widget/RKImage.h \
    widget/RKListWidget.h \
    widget/RKOverlayWidget.h \
    widget/RKSearchEdit.h \
    widget/RKSlider.h \
    widget/RKStackWidget.h \
    widget/filter.h \
    widget/qtmaterialiconbutton.h \
    widget/qtmaterialiconbutton_p.h \
    widget/qtmaterialoverlaywidget.h \
    widget/qtmaterialripple.h \
    widget/qtmaterialrippleoverlay.h

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

isEmpty (LIB_DIR){
    LIB_DIR = /opt/PhoenixPlayer
}

QMAKE_LIBDIR += \
    $${OUT_PWD}/../target/lib \
    $${LIB_DIR}/lib \

QMAKE_RPATHDIR += \
    $${OUT_PWD}/../target/lib \
    $${LIB_DIR}/lib \

LIBS += -lPhoenixPlayer

target.path = $$LIB_DIR
INSTALLS += target

RESOURCES += \
    resources/theme/theme.qrc

