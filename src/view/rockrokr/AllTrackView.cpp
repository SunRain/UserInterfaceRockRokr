#include "AllTrackView.h"

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
#include "UserInterface/UserInterfaceMgr.h"

#include "rockrokr_global.h"
#include "UserInterfaceRockRokr.h"
#include "widget/RKTableHeaderItem.h"
#include "widget/RKListView.h"
#include "TrackListModel.h"
#include "TrackListViewDelegate.h"
//#include "RockRokrApp.h"
#include "RKMainWindow.h"
#include "view/ViewUtility.h"



DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

AllTrackView::AllTrackView(QWidget *parent)
    : BaseTrackView (new AllTrackViewDataProvider, parent)
{
    m_uiMgr = new UserInterfaceMgr(this);
    m_ui = qobject_cast<UserInterfaceRockRokr*>(m_uiMgr->usedInterface());
    m_playerCore = new PlayerCore(this);
    m_libMgr = new MusicLibrary::MusicLibraryManager(this);
    m_plsMetaMgr = new PlayListMetaMgr(this);
}

AllTrackView::~AllTrackView()
{
    if (m_uiMgr) {
        m_uiMgr->deleteLater();
        m_uiMgr = Q_NULLPTR;
    }
    m_ui = Q_NULLPTR;
    if (m_playerCore) {
        m_playerCore->deleteLater();
        m_playerCore = Q_NULLPTR;
    }
    if (m_libMgr) {
        m_libMgr->deleteLater();
        m_libMgr = Q_NULLPTR;
    }
    if (m_plsMetaMgr) {
        m_plsMetaMgr->deleteLater();
        m_plsMetaMgr = Q_NULLPTR;
    }
}

void AllTrackView::showContextMenu(const QPoint &pos)
{
    const QModelIndex idx = indexAtPos(pos);
    if (!idx.isValid()) {
        qWarning()<<"Invalid QModelIndex!!";
        return;
    }
    const QString hash = getModel()->data(idx, TrackListModel::RoleHash).toString();
    const AudioMetaObject obj = m_libMgr->trackFromHash(hash);
    if (obj.isHashEmpty()) {
        m_ui->mainWindow()->showToast(tr("Empty audio meta object !!"));
        return;
    }
    QMenu menu;
    menu.setStyle(QStyleFactory::create("dlight"));

    ViewUtility::menuAddToQueue(&menu, obj, m_playerCore);
    ViewUtility::menuAddToPlaylist(&menu, obj, m_plsMetaMgr);
    menu.addSeparator();
    ViewUtility::menuRemoveObject(&menu, obj);
    ViewUtility::menuShowInFileMgr(&menu, obj);
    menu.addSeparator();
    ViewUtility::menuTrackInfo(&menu, obj);

    menu.exec(this->mapToGlobal(pos));
}

void AllTrackView::onClicked(const QModelIndex &index)
{
//    qDebug()<<Q_FUNC_INFO<<" index "<<index;
    const QString hash = index.data(TrackListModel::ModelRoles::RoleHash).toString();
//    QString title = index.data(TrackListModel::ModelRoles::RoleSongTitle).toString();

//    qDebug()<<Q_FUNC_INFO<<" "<<hash<<" "<<title;
    m_playerCore->playFromLibrary(hash);
}

AllTrackViewDataProvider::AllTrackViewDataProvider()
{
    m_header = new RKTableHeaderItem;
    m_header->setFixedHeight(_to_px(30));
    m_header->setEnableClick(true);
    m_header->setEnableArrowIcon(true);
    m_header->setCoverColumnWidth(_to_px(50));
    m_header->setCoverColumnIcon(DHiDPIHelper::loadNxPixmap(":/light/image/ic_format_list_numbered.svg"),
                                 DHiDPIHelper::loadNxPixmap(":/light/image/ic_format_list_numbered_hover.svg"));
    m_header->setCoverColumnToolTip(QObject::tr("Reset to default order"));
    m_header->setDisplayAllColumns();

    m_dataModel = new TrackListModel;
    m_dataModel->showAllTracks();
}

AllTrackViewDataProvider::~AllTrackViewDataProvider()
{

}

RKTableHeaderItem *AllTrackViewDataProvider::headerItem() const
{
    return m_header;
}

TrackListModel *AllTrackViewDataProvider::dataModel() const
{
    return m_dataModel;
}

void AllTrackViewDataProvider::resetDataModelToDefalutState()
{
    m_dataModel->showAllTracks();
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
