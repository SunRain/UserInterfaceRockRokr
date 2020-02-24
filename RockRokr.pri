
QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

HEADERS += \
    $$PWD/src/RKMainWindow.h \
    $$PWD/src/RKUtility.h \
    $$PWD/src/UserInterfaceRockRokr.h \
    $$PWD/src/rockrokr_global.h \
    $$PWD/src/view/ImportView.h \
    $$PWD/src/view/MainWindowBGView.h \
    $$PWD/src/view/RockRokrView.h \
    $$PWD/src/view/titlebar/SearchResultItem.h \
    $$PWD/src/view/titlebar/SearchResultView.h \
    $$PWD/src/view/TrackInfoDialog.h \
    $$PWD/src/view/ViewUtility.h \
    $$PWD/src/view/leftbar/LBListItem.h \
    $$PWD/src/view/leftbar/LBListWidget.h \
    $$PWD/src/view/leftbar/LBPlaylistView.h \
    $$PWD/src/view/leftbar/LeftBar.h \
    $$PWD/src/view/leftbar/ListModel.h \
    $$PWD/src/view/leftbar/ListViewItemDelegate.h \
    $$PWD/src/view/leftbar/MainListView.h \
    $$PWD/src/view/playbar/PlayBar.h \
    $$PWD/src/view/playbar/VolumeWidget.h \
    $$PWD/src/view/playbar/WaveformSlider.h \
    $$PWD/src/view/rockrokr/AlbumCategoryView.h \
    $$PWD/src/view/rockrokr/AllTrackView.h \
    $$PWD/src/view/rockrokr/ArtistCategoryView.h \
    $$PWD/src/view/rockrokr/BaseCategoryView.h \
    $$PWD/src/view/rockrokr/BaseTrackView.h \
    $$PWD/src/view/rockrokr/CategoryAllViewDelegate.h \
    $$PWD/src/view/rockrokr/CategoryDetailView.h \
    $$PWD/src/view/rockrokr/CategoryModel.h \
    $$PWD/src/view/rockrokr/CategoryModelImageProvider.h \
    $$PWD/src/view/rockrokr/CategoryRecentViewDelegate.h \
    $$PWD/src/view/rockrokr/CategoryRecommendedViewDelegate.h \
    $$PWD/src/view/rockrokr/CategoryViewDataProvider.h \
    $$PWD/src/view/rockrokr/FavoriteTrackView.h \
    $$PWD/src/view/rockrokr/GenresCategoryView.h \
    $$PWD/src/view/rockrokr/PlayListDetailView.h \
    $$PWD/src/view/rockrokr/TrackListModel.h \
    $$PWD/src/view/rockrokr/TrackListViewDelegate.h \
    $$PWD/src/view/titlebar/RKTitleBar.h \
    $$PWD/src/view/titlebar/SearchEdit.h \
    $$PWD/src/widget/ItemFragment.h \
    $$PWD/src/widget/LoadingWidget.h \
    $$PWD/src/widget/RKBoxWidget.h \
    $$PWD/src/widget/RKExpand.h \
    $$PWD/src/widget/RKIconButton.h \
    $$PWD/src/widget/RKImage.h \
    $$PWD/src/widget/RKImageProxy.h \
    $$PWD/src/widget/RKLineEdit.h \
    $$PWD/src/widget/RKListView.h \
    $$PWD/src/widget/RKListWidget.h \
    $$PWD/src/widget/RKMarqueeLabel.h \
    $$PWD/src/widget/RKOverlayWidget.h \
    $$PWD/src/widget/RKSearchEdit.h \
    $$PWD/src/widget/RKSlider.h \
    $$PWD/src/widget/RKStackWidget.h \
    $$PWD/src/widget/RKTableHeaderItem.h \
    $$PWD/src/widget/filter.h \
    $$PWD/src/widget/qtmaterialiconbutton_p.h \
    $$PWD/src/widget/qtmaterialoverlaywidget.h \
    $$PWD/src/widget/qtmaterialripple.h \
    $$PWD/src/widget/qtmaterialrippleoverlay.h

SOURCES += \
    $$PWD/src/RKMainWindow.cpp \
    $$PWD/src/RKUtility.cpp \
    $$PWD/src/UserInterfaceRockRokr.cpp \
    $$PWD/src/rockrokr_global.cpp \
    $$PWD/src/view/ImportView.cpp \
    $$PWD/src/view/MainWindowBGView.cpp \
    $$PWD/src/view/RockRokrView.cpp \
    $$PWD/src/view/titlebar/SearchResultItem.cpp \
    $$PWD/src/view/titlebar/SearchResultView.cpp \
    $$PWD/src/view/TrackInfoDialog.cpp \
    $$PWD/src/view/ViewUtility.cpp \
    $$PWD/src/view/leftbar/LBListItem.cpp \
    $$PWD/src/view/leftbar/LBListWidget.cpp \
    $$PWD/src/view/leftbar/LBPlaylistView.cpp \
    $$PWD/src/view/leftbar/LeftBar.cpp \
    $$PWD/src/view/leftbar/ListModel.cpp \
    $$PWD/src/view/leftbar/ListViewItemDelegate.cpp \
    $$PWD/src/view/leftbar/MainListView.cpp \
    $$PWD/src/view/playbar/PlayBar.cpp \
    $$PWD/src/view/playbar/VolumeWidget.cpp \
    $$PWD/src/view/playbar/WaveformSlider.cpp \
    $$PWD/src/view/rockrokr/AlbumCategoryView.cpp \
    $$PWD/src/view/rockrokr/AllTrackView.cpp \
    $$PWD/src/view/rockrokr/ArtistCategoryView.cpp \
    $$PWD/src/view/rockrokr/BaseCategoryView.cpp \
    $$PWD/src/view/rockrokr/BaseTrackView.cpp \
    $$PWD/src/view/rockrokr/CategoryAllViewDelegate.cpp \
    $$PWD/src/view/rockrokr/CategoryDetailView.cpp \
    $$PWD/src/view/rockrokr/CategoryModel.cpp \
    $$PWD/src/view/rockrokr/CategoryModelImageProvider.cpp \
    $$PWD/src/view/rockrokr/CategoryRecentViewDelegate.cpp \
    $$PWD/src/view/rockrokr/CategoryRecommendedViewDelegate.cpp \
    $$PWD/src/view/rockrokr/CategoryViewDataProvider.cpp \
    $$PWD/src/view/rockrokr/FavoriteTrackView.cpp \
    $$PWD/src/view/rockrokr/GenresCategoryView.cpp \
    $$PWD/src/view/rockrokr/PlayListDetailView.cpp \
    $$PWD/src/view/rockrokr/TrackListModel.cpp \
    $$PWD/src/view/rockrokr/TrackListViewDelegate.cpp \
    $$PWD/src/view/titlebar/RKTitleBar.cpp \
    $$PWD/src/view/titlebar/SearchEdit.cpp \
    $$PWD/src/widget/ItemFragment.cpp \
    $$PWD/src/widget/LoadingWidget.cpp \
    $$PWD/src/widget/RKBoxWidget.cpp \
    $$PWD/src/widget/RKExpand.cpp \
    $$PWD/src/widget/RKIconButton.cpp \
    $$PWD/src/widget/RKImage.cpp \
    $$PWD/src/widget/RKImageProxy.cpp \
    $$PWD/src/widget/RKLineEdit.cpp \
    $$PWD/src/widget/RKListView.cpp \
    $$PWD/src/widget/RKListWidget.cpp \
    $$PWD/src/widget/RKMarqueeLabel.cpp \
    $$PWD/src/widget/RKOverlayWidget.cpp \
    $$PWD/src/widget/RKSearchEdit.cpp \
    $$PWD/src/widget/RKSlider.cpp \
    $$PWD/src/widget/RKTableHeaderItem.cpp \
    $$PWD/src/widget/filter.cpp \
    $$PWD/src/widget/qtmaterialoverlaywidget.cpp \
    $$PWD/src/widget/qtmaterialripple.cpp \
    $$PWD/src/widget/qtmaterialrippleoverlay.cpp \
    $$PWD/src/widget/RKStackWidget.cpp \

RESOURCES += \
    $$PWD/src/resources/theme/theme.qrc

OTHER_FILES += \
    $$PWD/src/RockRokr.json

