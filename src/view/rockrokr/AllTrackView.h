#ifndef ALLTRACKVIEW_H
#define ALLTRACKVIEW_H

#include "BaseTrackView.h"

namespace PhoenixPlayer {

    class PlayerCore;

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        class UserInterfaceMgr;

        namespace RockRokr {

class RKTableHeaderItem;
class TrackListModel;
class RKListView;
class TrackListViewDelegate;
class UserInterfaceRockRokr;

class AllTrackViewDataProvider : public BaseTrackViewDataProvider
{
public:
    AllTrackViewDataProvider();
    virtual ~AllTrackViewDataProvider() override;

    // BaseTrackViewDataProvider interface
public:
    RKTableHeaderItem *headerItem() const Q_DECL_OVERRIDE;
    TrackListModel *dataModel() const Q_DECL_OVERRIDE;
    void resetDataModelToDefalutState() const Q_DECL_OVERRIDE;

private:
    RKTableHeaderItem           *m_header  = Q_NULLPTR;
    TrackListModel              *m_dataModel = Q_NULLPTR;
};

class AllTrackView : public BaseTrackView
{
    Q_OBJECT
public:
    AllTrackView(QWidget *parent = Q_NULLPTR);
    virtual ~AllTrackView() override;

    // BaseTrackView interface
protected:
    void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE;
    void onClicked(const QModelIndex &index) Q_DECL_OVERRIDE;

private:
    UserInterfaceMgr                    *m_uiMgr        = Q_NULLPTR;
    UserInterfaceRockRokr               *m_ui           = Q_NULLPTR;
    PlayerCore                          *m_playerCore   = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libMgr       = Q_NULLPTR;

};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // ALLTRACKVIEW_H
