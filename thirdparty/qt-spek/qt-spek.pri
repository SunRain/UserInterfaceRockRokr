
CONFIG += link_pkgconfig
          
QMAKE_CXXFLAGS += -std=c++11

PKGCONFIG += libavcodec libavformat libavutil

HEADERS += \
    $$PWD/qt-spek/spek-audio.h \
    $$PWD/qt-spek/spek-fft.h \
    $$PWD/qt-spek/spek-palette.h \
    $$PWD/qt-spek/spek-pipeline.h \
    $$PWD/qt-spek/spek-ruler.h \
    $$PWD/qt-spek/spek-spectrogram.h

SOURCES += \
    $$PWD/qt-spek/spek-audio.cc \
    $$PWD/qt-spek/spek-fft.cc \
    $$PWD/qt-spek/spek-palette.cc \
    $$PWD/qt-spek/spek-pipeline.cc \
    $$PWD/qt-spek/spek-ruler.cc \
    $$PWD/qt-spek/spek-spectrogram.cc

