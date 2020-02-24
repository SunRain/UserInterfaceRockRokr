#include "RockRokrView.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleFactory>

#include <DThemeManager>

#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/PlayListMetaMgr.h"
#include "MusicLibrary/MusicLibraryManager.h"

#include "rockrokr_global.h"
#include "ViewUtility.h"
#include "titlebar/SearchResultView.h"
#include "leftbar/LeftBar.h"
#include "leftbar/LBListItem.h"
#include "view/titlebar/RKTitleBar.h"
#include "view/playbar/PlayBar.h"
#include "widget/RKStackWidget.h"

#include "rockrokr/AlbumCategoryView.h"
#include "rockrokr/ArtistCategoryView.h"
#include "rockrokr/GenresCategoryView.h"
#include "rockrokr/AllTrackView.h"
#include "rockrokr/FavoriteTrackView.h"
#include "rockrokr/CategoryModel.h"
#include "rockrokr/CategoryDetailView.h"
#include "rockrokr/PlayListDetailView.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RockRokrView::RockRokrView(QWidget *parent)
    : QFrame(parent)

{
    this->setObjectName("RockRokrView");
    DThemeManager::instance()->registerWidget(this);

    m_playerCore    = new PlayerCore(this);
    m_libMgr        = new MusicLibrary::MusicLibraryManager (this);
    m_plsMetaMgr    = new PlayListMetaMgr(this);

    m_ctgDetailView     = new CategoryDetailView;
    m_plsDetailView     = new PlayListDetailView;
    m_searchResultView  = new SearchResultView(this);

    m_leftbar   = new LeftBar;
    m_titlebar  = new RKTitleBar;
    m_titlebar->bindResultView(m_searchResultView);
    m_playbar   = new PlayBar;
    m_stack     = new RKStackedWidget;
    m_stack->setDuration(500);

    // order based on the order of enum in LBListItem(ItemType);
    m_artistCategory    = new ArtistCategoryView;
    m_albumCategory     = new AlbumCategoryView;
    m_genresCategory    = new GenresCategoryView;
    m_allTrackView      = new AllTrackView;
    m_favTrackView      = new FavoriteTrackView;

    m_stack->addWidget(m_artistCategory);
    m_stack->addWidget(m_albumCategory);
    m_stack->addWidget(m_genresCategory);
    m_stack->addWidget(m_allTrackView);
    m_stack->addWidget(m_favTrackView);
    m_stack->setCurrentWidget(m_artistCategory, RKStackedWidget::AnimationTypeNone);

    initUI();

    connect(m_leftbar, &LeftBar::itemClicked, this, [&](LBListItem *item){
        qDebug()<<"LeftBar::itemClicked item type "<<item->itemType()
               <<" "<<item->enumToStr("ItemType", item->itemType());
        if (item->itemType() == LBListItem::ItemType::TypePlaylist) {
            const PlayListMeta meta = PlayListMeta::fromMap(item->extraData().toMap());
            m_plsDetailView->showPlayList(meta);
            ViewUtility::showPlaylistDetailView();
            return;
        }
        //TODO addon && service item
        bool ok = false;
        const int nowIdx = item->extraData().toInt(&ok);
        qDebug()<<"LeftBar::itemClicked extraData "<<nowIdx<<" ok "<<ok;
        if (!m_preLeftBarItem) {
            m_preLeftBarItem = item;
            m_stack->setCurrentIndex(nowIdx, RKStackedWidget::AnimationTypeNone);
            return;
        }
        const int preIdx = m_preLeftBarItem->extraData().toInt();
        m_preLeftBarItem = item;
        if (nowIdx > preIdx) {
            m_stack->setCurrentIndex(item->extraData().toInt(), RKStackedWidget::BottomToTop);
        } else {
            m_stack->setCurrentIndex(item->extraData().toInt(), RKStackedWidget::TopToBottom);
        }
    });

#define GET_SHOW_PARA \
    const QString name = model->data(index, BaseCategoryModel::RoleCategoryName).toString(); \
    const QString sub = model->data(index, BaseCategoryModel::RoleCategorySubNameList).toStringList().join(", "); \
    const QUrl img = model->data(index, BaseCategoryModel::RoleImageUri).toUrl(); \
    const int num = model->data(index, BaseCategoryModel::RoleTrackNum).toInt();

#define PLAY_TRACK \
    const QString hash = model->data(index, BaseCategoryModel::RoleTrackHash).toString(); \
    if (hash.isEmpty()) { \
        qWarning()<<"Can't find hash for index "<<index; \
        return; \
    } \
    m_playerCore->playFromLibrary(hash);

    /** ArtistCategoryView ***/
    connect(m_artistCategory, &ArtistCategoryView::viewClicked,
            this, [&](BaseCategoryView::ViewType type, BaseCategoryModel *model, const QModelIndex &index) {
        if (type == BaseCategoryView::ViewTypeRecent) {
            PLAY_TRACK
        } else {
            GET_SHOW_PARA
            m_ctgDetailView->showArtistTracks(name, sub, img, num);
            ViewUtility::showCategoryDetailView();
        }
    });

    connect(m_artistCategory, &ArtistCategoryView::customContextMenuRequested,
            this, [&](BaseCategoryView::ViewType type, BaseCategoryModel *model,
                        const QModelIndex &index, const QPoint &pos) {
        if (type == BaseCategoryView::ViewTypeRecent) {
            showContextMenu(model, index, pos);
        }
    });

    /** AlbumCategoryView ***/
    connect(m_albumCategory, &AlbumCategoryView::viewClicked,
            this, [&](BaseCategoryView::ViewType type, BaseCategoryModel *model, const QModelIndex &index) {
        if (type == BaseCategoryView::ViewTypeRecent) {
            PLAY_TRACK
        } else {
            GET_SHOW_PARA
            m_ctgDetailView->showAlbumTracks(name, sub, img, num);
            ViewUtility::showCategoryDetailView();
        }
    });

    connect(m_albumCategory, &AlbumCategoryView::customContextMenuRequested,
            this, [&](BaseCategoryView::ViewType type, BaseCategoryModel *model,
                        const QModelIndex &index, const QPoint &pos) {
        if (type == BaseCategoryView::ViewTypeRecent) {
            showContextMenu(model, index, pos);
        }
    });

    /** GenresCategoryView ***/
    connect(m_genresCategory, &GenresCategoryView::viewClicked,
            this, [&](BaseCategoryView::ViewType type, BaseCategoryModel *model, const QModelIndex &index) {
        if (type == BaseCategoryView::ViewTypeRecent) {
            PLAY_TRACK
        } else {
            GET_SHOW_PARA
            m_ctgDetailView->showGenreTracks(name, sub, img, num);
            ViewUtility::showCategoryDetailView();
        }
    });

    connect(m_genresCategory, &GenresCategoryView::customContextMenuRequested,
            this, [&](BaseCategoryView::ViewType type, BaseCategoryModel *model,
                        const QModelIndex &index, const QPoint &pos) {
        if (type == BaseCategoryView::ViewTypeRecent) {
            showContextMenu(model, index, pos);
        }
    });
}

RockRokrView::~RockRokrView()
{
    if (m_playerCore) {
        m_playerCore->deleteLater();
        m_playerCore = Q_NULLPTR;
    }
}

void RockRokrView::initUI()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    {
        m_leftbar->setFixedWidth(LEFT_BAR_W);
        layout->addWidget(m_leftbar);
    }
    {
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->setContentsMargins(0, LEFT_BAR_BT_TB_MARGIN, 0, 0);
        vbox->setSpacing(0);
        {
            QVBoxLayout *vv = new QVBoxLayout;
            vv->setContentsMargins(RIGHT_PART_L_MARGIN, 0, 0, 0);
            vv->setSpacing(0);

            vv->addWidget(m_titlebar, 0, Qt::AlignTop);
            vv->addSpacing(32);
            vv->addWidget(m_stack);
            vbox->addLayout(vv);
        }
        m_playbar->setFixedHeight(PLAY_BAR_H);
        vbox->addWidget(m_playbar, 0, Qt::AlignBottom);
        layout->addLayout(vbox);
    }
}

void RockRokrView::showContextMenu(BaseCategoryModel *model, const QModelIndex &index, const QPoint &pos)
{
    const QString hash = model->data(index, BaseCategoryModel::RoleTrackHash).toString();
    const AudioMetaObject obj = m_libMgr->trackFromHash(hash);
    if (obj.isHashEmpty()) {
         ViewUtility::showToast(tr("Empty audio meta object !!"));
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

    menu.exec(/*this->mapToGlobal(pos)*/pos);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

