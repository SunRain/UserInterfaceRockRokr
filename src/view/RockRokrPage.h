#ifndef ROCKROKRPAGE_H
#define ROCKROKRPAGE_H

#include <QFrame>

namespace PhoenixPlayer {
    class PlayerCore;
    class PlayListMetaMgr;

    namespace DataProvider {
        class TrackSearchProvider;
        class MatchObject;
    }

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

namespace UserInterface {
namespace RockRokr {

class LBListItem;
class LeftBar;
class RockrokrTitleBar;
class PlayBar;
class RKStackedWidget;
class AlbumCategoryView;
class ArtistCategoryView;
class GenresCategoryView;
class AllTrackView;
class FavoriteTrackView;
class CategoryDetailView;
class PlayListDetailView;
class BaseCategoryModel;
class SearchResultPopup;
class RockRokrPage : public QFrame
{
    Q_OBJECT

    friend class RKMainWindow;
public:
    explicit RockRokrPage(QWidget *parent = nullptr);
    virtual ~RockRokrPage() override;

//    void bindTrackSearchProvider(DataProvider::TrackSearchProvider *provider);

    void bindSearchResultPopup(SearchResultPopup *pop);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    void initUserInterface();

    inline CategoryDetailView *categoryDetailView()
    {
        return m_ctgDetailView;
    }

    inline PlayListDetailView *playListDetailView()
    {
        return m_plsDetailView;
    }

    void showContextMenu(BaseCategoryModel *model, const QModelIndex &index, const QPoint &pos);
private:
    PlayerCore          *m_playerCore       = Q_NULLPTR;
    LBListItem          *m_preLeftBarItem   = Q_NULLPTR;
    LeftBar             *m_leftbar          = Q_NULLPTR;
    RockrokrTitleBar    *m_titlebar         = Q_NULLPTR;
    PlayBar             *m_playbar          = Q_NULLPTR;
    RKStackedWidget     *m_stack            = Q_NULLPTR;
    AlbumCategoryView   *m_albumCategory    = Q_NULLPTR;
    ArtistCategoryView  *m_artistCategory   = Q_NULLPTR;
    GenresCategoryView  *m_genresCategory   = Q_NULLPTR;
    AllTrackView        *m_allTrackView     = Q_NULLPTR;
    FavoriteTrackView   *m_favTrackView     = Q_NULLPTR;
    CategoryDetailView  *m_ctgDetailView    = Q_NULLPTR;
    PlayListDetailView  *m_plsDetailView    = Q_NULLPTR;
//    SearchResultPopup   *m_searchResultPopup = Q_NULLPTR;
    PlayListMetaMgr                     *m_plsMetaMgr   = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libMgr       = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // ROCKROKRPAGE_H
