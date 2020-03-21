#ifndef FAVORITETRACKVIEW_H
#define FAVORITETRACKVIEW_H

#include "BaseTrackView.h"

namespace PhoenixPlayer {
    class PlayerCore;
    class PlayListMetaMgr;

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
    explicit FavoriteTrackViewDataProvider();
    virtual ~FavoriteTrackViewDataProvider() override;

    // BaseTrackViewDataProvider interface
public:
    RKTableHeaderItem *headerItem() const Q_DECL_OVERRIDE;
    QAbstractListModel *dataModel() const Q_DECL_OVERRIDE;
    QStyledItemDelegate *delegate() const Q_DECL_OVERRIDE;
    void resetDataModelToDefalutState() Q_DECL_OVERRIDE;
private:
    RKTableHeaderItem           *m_header  = Q_NULLPTR;
    TrackListModel              *m_dataModel = Q_NULLPTR;
    TrackListViewDelegate       *m_delegate = Q_NULLPTR;
};


class FavoriteTrackView : public BaseTrackView
{
public:
    FavoriteTrackView(QWidget *parent = Q_NULLPTR);
    virtual ~FavoriteTrackView() override;

    // BaseTrackView interface
protected:
    void onClicked(const QModelIndex &index) Q_DECL_OVERRIDE;
    void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE;

private:
    PlayerCore                          *m_playerCore   = Q_NULLPTR;
    PlayListMetaMgr                     *m_plsMetaMgr   = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libraryMgr   = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // FAVORITETRACKVIEW_H
