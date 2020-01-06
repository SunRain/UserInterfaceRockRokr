#include "FavoriteTrackView.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QStyleFactory>

#include <DThemeManager>
#include <DHiDPIHelper>
#include <DDesktopServices>

#include "AudioMetaObject.h"
#include "LibPhoenixPlayerMain.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/PlayListObject.h"
#include "PlayerCore/PlayListMeta.h"
#include "PlayerCore/PlayListMetaMgr.h"
#include "PlayerCore/MusicQueue.h"
#include "MusicLibrary/MusicLibraryManager.h"

#include "rockrokr_global.h"
#include "UserInterfaceRockRokr.h"
#include "widget/RKTableHeaderItem.h"
#include "widget/RKListView.h"
#include "TrackListModel.h"
#include "TrackListViewDelegate.h"
#include "RKMainWindow.h"
#include "view/ViewUtility.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

FavoriteTrackViewDataProvider::FavoriteTrackViewDataProvider()
{
    m_header = new RKTableHeaderItem;
    m_header->setFixedHeight(_to_px(30));
    m_header->setEnableClick(true);
    m_header->setEnableArrowIcon(true);
    m_header->setCoverColumnWidth(_to_px(50));
    m_header->setCoverColumnIcon(DHiDPIHelper::loadNxPixmap(":/light/image/ic_format_list_numbered.svg"),
                                 DHiDPIHelper::loadNxPixmap(":/light/image/ic_format_list_numbered_hover.svg"));
    m_header->setCoverColumnToolTip(QObject::tr("Rest to default order"));
    m_header->setDisplayAllColumns();

    m_dataModel = new TrackListModel;
    m_dataModel->showFavorites();
}

FavoriteTrackViewDataProvider::~FavoriteTrackViewDataProvider()
{

}

RKTableHeaderItem *FavoriteTrackViewDataProvider::headerItem() const
{
    return m_header;
}

TrackListModel *FavoriteTrackViewDataProvider::dataModel() const
{
    return m_dataModel;
}

void FavoriteTrackViewDataProvider::resetDataModelToDefalutState()
{
    m_dataModel->showFavorites();
}

FavoriteTrackView::FavoriteTrackView(QWidget *parent)
    : BaseTrackView (new FavoriteTrackViewDataProvider, parent)
{
    m_libraryMgr = new MusicLibrary::MusicLibraryManager(this);
}

FavoriteTrackView::~FavoriteTrackView()
{
    if (m_libraryMgr) {
        m_libraryMgr->deleteLater();
        m_libraryMgr = Q_NULLPTR;
    }
}

void FavoriteTrackView::showContextMenu(const QPoint &pos)
{
    const QModelIndex idx = indexAtPos(pos);
    if (!idx.isValid()) {
        qWarning()<<"Invalid QModelIndex!!";
        return;
    }
    const QString hash = getModel()->data(idx, TrackListModel::RoleHash).toString();
    const AudioMetaObject obj = m_libraryMgr->trackFromHash(hash);
    if (obj.isHashEmpty()) {
//        this->userInterfaceRockRokr()->mainWindow()->showToast(tr("Empty audio meta object !!"));
        ViewUtility::showToast(tr("Empty audio meta object !!"));
        return;
    }
    QMenu menu;
    menu.setStyle(QStyleFactory::create("dlight"));

    menu.addAction(tr("UnLike"), [&, obj](){
        m_libraryMgr->setLike(obj, false);
        getModel()->showFavorites();
    });

    menuAddToQueue(&menu, obj);
    menuAddToPlaylist(&menu, obj);
    menu.addSeparator();
    menuRemoveObject(&menu, obj);
    menuShowInFileMgr(&menu, obj);
    menu.addSeparator();
    menuTrackInfo(&menu, obj);

    menu.exec(this->mapToGlobal(pos));
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
