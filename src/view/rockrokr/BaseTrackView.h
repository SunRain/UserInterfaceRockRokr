#ifndef BASETRACKVIEW_H
#define BASETRACKVIEW_H

#include <QFrame>
#include "AudioMetaObject.h"

#include "TrackListModel.h"

class QMenu;

namespace PhoenixPlayer {

    class PlayerCore;
    class PlayListMetaMgr;

    namespace UserInterface {

        class UserInterfaceMgr;;

        namespace RockRokr {

            class RKTableHeaderItem;
            class TrackListModel;
            class RKListView;
            class TrackListViewDelegate;
            class UserInterfaceRockRokr;

class BaseTrackViewDataProvider
{
public:
    BaseTrackViewDataProvider() {}
    virtual ~BaseTrackViewDataProvider() {};

    virtual RKTableHeaderItem *headerItem() const = 0;
    virtual TrackListModel *dataModel() const = 0;
    virtual void resetDataModelToDefalutState()
    {
        dataModel()->resetToDefalutState();
    }
};

class BaseTrackView : public QFrame
{
    Q_OBJECT
public:
    explicit BaseTrackView(BaseTrackViewDataProvider *pr, QWidget *parent = nullptr);
    virtual ~BaseTrackView() override;

    /*!
     * \brief setInternalListViewSize
     * Set listview size or use default size
     * \param size
     */
//    void setInternalListViewSize(const QSize &size);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    void initUserInterface();

    virtual void showContextMenu(const QPoint &pos) {}

    virtual void onClicked(const QModelIndex &index) {}

    RKTableHeaderItem *getHeaderItem() const;
    TrackListModel *getModel() const;

    inline BaseTrackViewDataProvider *dataProvider() const
    {
        return m_provider;
    }

    void menuAddToQueue(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);
    void menuAddToPlaylist(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);
    void menuShowInFileMgr(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);
    void menuRemoveObject(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);
    void menuTrackInfo(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);

    QModelIndex indexAtPos(const QPoint &pos) const;

//    inline UserInterfaceRockRokr *userInterfaceRockRokr() const
//    {
//        return m_ui;
//    }

private:
    PlayerCore                  *m_playerCore = Q_NULLPTR;
    PlayListMetaMgr             *m_plsMetaMgr = Q_NULLPTR;

    RKListView                  *m_listView = Q_NULLPTR;
    TrackListViewDelegate       *m_delegate = Q_NULLPTR;
    BaseTrackViewDataProvider   *m_provider = Q_NULLPTR;
//    UserInterfaceMgr            *m_uiMgr    = Q_NULLPTR;
//    UserInterfaceRockRokr       *m_ui       = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // BASETRACKVIEW_H
