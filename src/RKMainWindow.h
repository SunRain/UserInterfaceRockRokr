#ifndef RKMAINWINDOW_H
#define RKMAINWINDOW_H

#include <DMainWindow>

DWIDGET_BEGIN_NAMESPACE
class DToast;
DWIDGET_END_NAMESPACE

class QStackedWidget;
class QStackedLayout;
class QTimer;

namespace PhoenixPlayer {

    class PPSettings;

    namespace DataProvider {
        class TrackSearchProvider;
    } // namespace DataProvider

    namespace MusicLibrary {
        class MusicLibraryManager;
        class LocalMusicScanner;
    }

    namespace UserInterface {

        namespace RockRokr {

            class LeftBar;
            class ImportPage;
            class LoadingWidget;
            class RockRokrPage;
            class RKStackedWidget;
            class RKOverlayWidget;
            class CategoryDetailView;
            class SearchPage;

class RKMainWindow : public DTK_WIDGET_NAMESPACE::DMainWindow
{
    Q_OBJECT

    friend class UserInterfaceRockRokr;
protected:
    RKMainWindow(QWidget *parent = Q_NULLPTR);
    ~RKMainWindow();

    Q_SLOT void show();

public:
    void showImportView();

    void showLoadingView();

    void showRockRokrView();

    void showSearchPage(DataProvider::TrackSearchProvider *provider);

    void showPlayingView();

    void setDefaultTitlebarHidden(bool hidden);

    void showFileImportDlg();

    void showTip(QWidget *baseWidget, const QString &text, int delayShowMilliseconds = 0);

    void showTip(const QPoint &globalCenterPos, const QString &text, int delayShowMilliseconds = 0);

    void hideTip(int delayMilliseconds = 0);

    void showToast(const QString &text);

    void showCategoryDetailView();

    void showPlaylistDetailView();

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget                             *m_mainWidget       = Q_NULLPTR;
    QTimer                              *m_tipTimer         = Q_NULLPTR;

    PPSettings                          *m_ppSettings       = Q_NULLPTR;
//    MusicLibrary::MusicLibraryManager   *m_libraryMgr       = Q_NULLPTR;
    MusicLibrary::LocalMusicScanner     *m_localMSC         = Q_NULLPTR;

    RKStackedWidget                     *m_stack            = Q_NULLPTR;
    LeftBar                             *m_leftBar          = Q_NULLPTR;
    ImportPage                          *m_importView       = Q_NULLPTR;
    LoadingWidget                       *m_loadingWidget    = Q_NULLPTR;
    RockRokrPage                        *m_rkView           = Q_NULLPTR;
    RKOverlayWidget                     *m_overlayWidget    = Q_NULLPTR;
    SearchPage                          *m_searchPage       = Q_NULLPTR;

    DTK_WIDGET_NAMESPACE::DToast        *m_tip              = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DToast        *m_toast            = Q_NULLPTR;
    const int                           m_defaultTitlebarH  = 0;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKMAINWINDOW_H
