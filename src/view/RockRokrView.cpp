#include "RockRokrView.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <DThemeManager>

#include "rockrokr_global.h"
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

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RockRokrView::RockRokrView(QWidget *parent)
    : QFrame(parent)

{
//    DThemeManager::instance()->registerWidget(this);
    this->setStyleSheet(QString("QWidget{background-color:%1;}").arg(MAIN_VIEW_BG_COLOR));

    m_leftbar = new LeftBar;
    m_titlebar = new RKTitleBar;
    m_playbar = new PlayBar;
    m_stack = new RKStackedWidget;
    m_stack->setDuration(500);

    // order based on the order of enum in LBListItem(ItemType);
    m_artistCategory = new ArtistCategoryView;
    m_albumCategory = new AlbumCategoryView;
    m_genresCategory = new GenresCategoryView;
    m_allTrackView = new AllTrackView;
    m_favTrackView = new FavoriteTrackView;

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
            //TODO playlist

            return;
        }
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
}

RockRokrView::~RockRokrView()
{

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

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

