#include "BaseTrackView.h"

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
#include "RKMainWindow.h"
#include "view/ViewUtility.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

BaseTrackView::BaseTrackView(BaseTrackViewDataProvider *pr, QWidget *parent)
    : QFrame (parent),
    m_provider(pr)
{
    this->setObjectName("BaseTrackView");
    DThemeManager::instance()->registerWidget(this);

//    m_playerCore = new PlayerCore(this);
//    m_plsMetaMgr = new PlayListMetaMgr(this);
//    m_uiMgr = new UserInterfaceMgr(this);
//    m_ui = qobject_cast<UserInterfaceRockRokr*>(m_uiMgr->usedInterface());

    m_delegate = new TrackListViewDelegate;

    m_listView = new RKListView;
    m_listView->setDragEnabled(false);
    m_listView->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    m_listView->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    m_listView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    m_listView->setSelectionBehavior(QListView::SelectRows);
    //    m_listView->setFixedWidth(MAIN_VIEW_V_LISTVIEW_W);
    m_listView->setSpacing(0);
    m_listView->setContentsMargins(0, 0, 0, 0);
    m_listView->setScrollBarOrientation(Qt::Vertical);
    m_listView->setDefaultItemSize(QSize(MAIN_VIEW_V_LISTVIEW_W, MAIN_VIEW_LISTITEM_H));
    m_listView->setTableHeaders(m_provider->headerItem()->displayedColumnsWidth());
    m_listView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_listView->setModel(m_provider->dataModel());
    m_listView->setItemDelegate(m_delegate);

    initUserInterface();

    connect(m_provider->headerItem(), &RKTableHeaderItem::clicked,
            this, [&](RKTableHeaderItem::HeaderColumn c, RKTableHeaderItem::ArrowIconDirection direc){
                qDebug()<<"KTableHeaderItem::clicked "<<c<<" direc "<<direc;
                Qt::SortOrder order = Qt::SortOrder::AscendingOrder;
                if (direc == RKTableHeaderItem::ArrowIconDirection::DirectionDown) {
                    order = Qt::SortOrder::DescendingOrder;
                }
                if (c == RKTableHeaderItem::HeaderTitle) {
                    m_provider->dataModel()->sort(TrackListModel::ModelRoles::RoleSongTitle, order);
                } else if (c == RKTableHeaderItem::HeaderAlbum) {
                    m_provider->dataModel()->sort(TrackListModel::ModelRoles::RoleAlbumName, order);
                } else if (c == RKTableHeaderItem::HeaderArtist) {
                    m_provider->dataModel()->sort(TrackListModel::ModelRoles::RoleArtistName, order);
                } else if (c == RKTableHeaderItem::HeaderCover) {
                    m_provider->resetDataModelToDefalutState();
                }
            });

    connect(m_provider->headerItem(), &RKTableHeaderItem::columnsWidthChanged,
            this, [&](const QMap<RKTableHeaderItem::HeaderColumn, int> &map) {
                m_listView->setTableHeaders(map);
            });
    connect(m_listView, &RKListView::entered,
            this, [&](const QModelIndex &idx) {
                //                qDebug()<<Q_FUNC_INFO<<" RKListView::entered entered "<<idx;
//                m_provider->dataModel()->setCurIdx(idx);
//        m_listView->update();
            });

    connect(m_listView, &RKListView::customContextMenuRequested,
            this, &BaseTrackView::showContextMenu);

    connect(m_listView, &RKListView::clicked,
            this, &BaseTrackView::onClicked);

}

BaseTrackView::~BaseTrackView()
{
//    if (m_playerCore) {
//        m_playerCore->deleteLater();
//        m_playerCore = Q_NULLPTR;
//    }
//    if (m_plsMetaMgr) {
//        m_plsMetaMgr->deleteLater();
//        m_plsMetaMgr = Q_NULLPTR;
//    }
//    if (m_uiMgr) {
//        m_uiMgr->deleteLater();
//        m_uiMgr = Q_NULLPTR;
//    }

    if (m_provider) {
        delete m_provider;
        m_provider = Q_NULLPTR;
    }
}

void BaseTrackView::resizeEvent(QResizeEvent *event)
{
    m_provider->headerItem()->setFixedWidth(event->size().width());

    QWidget::resizeEvent(event);
//    m_provider->headerItem()->setFixedWidth(event->size().width());
}

void BaseTrackView::initUserInterface()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_provider->headerItem());
    layout->addWidget(m_listView);
    this->setLayout(layout);
}

//void BaseTrackView::setInternalListViewSize(const QSize &size)
//{
//    m_listView->setFixedSize(size);
//}

RKTableHeaderItem *BaseTrackView::getHeaderItem() const
{
    return m_provider->headerItem();
}

TrackListModel *BaseTrackView::getModel() const
{
    return m_provider->dataModel();
}

//void BaseTrackView::menuAddToQueue(QMenu *menu, const AudioMetaObject &obj)
//{
//    menu->addAction(tr("Add to queue"), [&, obj](){
//        m_playerCore->playQueue()->addTrack(obj);
//    });
//}

//void BaseTrackView::menuAddToPlaylist(QMenu *menu, const AudioMetaObject &obj)
//{
//    QMenu *subMenu = new QMenu(menu);
//    subMenu->setStyle(QStyleFactory::create("dlight"));
//    subMenu->addAction(QIcon(":/light/image/playlist_add.svg"), tr("New playlist"), [&](){
//        PlayListMeta meta = m_plsMetaMgr->create();
//        m_plsMetaMgr->tryAdd(meta);
//    });

//    subMenu->addSeparator();

//    auto metalist = m_plsMetaMgr->metaList();
//    QFont font(subMenu->font());
//    QFontMetrics fm(font);
//    foreach(const PlayListMeta &meta, metalist) {
//        auto text = fm.elidedText(QString(meta.getFileName().replace("&", "&&")),
//                                  Qt::ElideMiddle, _to_px(160));
//        subMenu->addAction(text, [&, meta, obj](){
//            PlayListObject po(meta);
//            if (!po.open()) {
//                ViewUtility::showToast(QString(tr("Can't open to add playlist %1!")).arg(meta.getFileName()));
//                return;
//            }
//            po.addTrack(obj);
//            if (!po.save()) {
//                ViewUtility::showToast(QString(tr("save to %1 error!")).arg(meta.getFileName()));
//            }
//        });
//    }
//    menu->addAction(tr("Add to playlist"))->setMenu(subMenu);
//}

//void BaseTrackView::menuShowInFileMgr(QMenu *menu, const AudioMetaObject &obj)
//{
//    menu->addAction(tr("Display in file manager"), [&, obj](){
//        QString file = QString("%1/%2").arg(obj.path()).arg(obj.name());
//        QUrl uri = QUrl::fromLocalFile(file);
//        DDesktopServices::showFileItem(uri);
//    });
//}

//void BaseTrackView::menuRemoveObject(QMenu *menu, const AudioMetaObject &obj)
//{
//    menu->addAction(tr("Remove"), [&, obj]() {
//       ViewUtility::showTrackRemoveDialog(obj);
//    });
//}

//void BaseTrackView::menuTrackInfo(QMenu *menu, const AudioMetaObject &obj)
//{
//    menu->addAction(tr("Track info"), [&, obj](){
//        ViewUtility::showTrackInfoDialog(obj);
//    });
//}

QModelIndex BaseTrackView::indexAtPos(const QPoint &pos) const
{
    return m_listView->indexAt(pos);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
