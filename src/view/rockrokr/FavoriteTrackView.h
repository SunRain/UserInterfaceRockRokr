#ifndef FAVORITETRACKVIEW_H
#define FAVORITETRACKVIEW_H

#include "BaseTrackView.h"

namespace PhoenixPlayer {

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

            class RKTableHeaderItem;
            class TrackListModel;
            class RKListView;
            class TrackListViewDelegate;

class FavoriteTrackViewDataProvider : public BaseTrackViewDataProvider
{
public:
    FavoriteTrackViewDataProvider();
    virtual ~FavoriteTrackViewDataProvider() override;

    // BaseTrackViewDataProvider interface
public:
    RKTableHeaderItem *headerItem() const Q_DECL_OVERRIDE;
    TrackListModel *dataModel() const Q_DECL_OVERRIDE;
    void resetDataModelToDefalutState() Q_DECL_OVERRIDE;

private:
    RKTableHeaderItem           *m_header  = Q_NULLPTR;
    TrackListModel              *m_dataModel = Q_NULLPTR;
};


class FavoriteTrackView : public BaseTrackView
{
public:
    FavoriteTrackView(QWidget *parent = Q_NULLPTR);
    ~FavoriteTrackView() override;

    // BaseTrackView interface
protected:
    void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE;

private:
    MusicLibrary::MusicLibraryManager       *m_libraryMgr   = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // FAVORITETRACKVIEW_H
