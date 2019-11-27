#include "LeftBar.h"

#include <QList>
#include <QDebug>
#include <QFont>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPainter>
#include <QPalette>
#include <QStyle>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>

#include <dlabel.h>
#include <DTitlebar>
#include <DThemeManager>
#include <dimagebutton.h>
#include <dseparatorhorizontal.h>

#include "rockrokr_global.h"
#include "MainListView.h"
#include "ListModel.h"
#include "ListViewItemDelegate.h"
#include "LBListItem.h"
#include "LBListWidget.h"
#include "LBPlaylistView.h"
#include "LBPlaylistView.h"

#include "widget/ItemFragment.h"


DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

LeftBar::LeftBar(QWidget *parent)
    : QFrame(parent),
      m_listWidget(new LBListWidget),
      m_plsView(new LBPlaylistView),
      m_sepEffect(new QGraphicsOpacityEffect(this)),
      m_sepAnimation(new QPropertyAnimation(this)),
      m_isSepHidden(true)
{
    this->setObjectName("LeftBar");

    DThemeManager::instance()->registerWidget(this);

    this->setFrameStyle(QFrame::NoFrame);

    m_plsView->setExpandedSeparatorVisible(false);

    m_sepEffect->setOpacity(0);
    m_sepAnimation->setTargetObject(m_sepEffect);
    m_sepAnimation->setPropertyName("opacity");
    m_sepAnimation->setDuration(1000);
    m_sepAnimation->setKeyValueAt(0.4, 1.0);
    m_sepAnimation->setKeyValueAt(0.8, 1.0);

//    connect(m_listWidget, &QListWidget::itemClicked,
//            this, [&](QListWidgetItem *item) {
//        LBListItem *lbi = qobject_cast<LBListItem*>(m_listWidget->itemWidget(item));
//        if (!lbi) {
//            qWarning()<<Q_FUNC_INFO<<"empty item";
//            return;
//        }
//        qDebug()<<Q_FUNC_INFO<<"item clicked "<<lbi->enumToStr("ItemType", lbi->itemType());
//        emit itemClicked(lbi);
//    });

    connect(m_listWidget, &QListWidget::currentItemChanged,
            this, [&](QListWidgetItem *current, QListWidgetItem *previous) {
        qDebug()<<"---- currentItemChanged ---";
        LBListItem *lbi = qobject_cast<LBListItem*>(m_listWidget->itemWidget(current));
        if (!lbi) {
            qWarning()<<"empty item";
        } else {
            lbi->setHoverState(true);
        }
        lbi = qobject_cast<LBListItem*>(m_listWidget->itemWidget(previous));
        if (!lbi) {
            qWarning()<<"empty item";
        } else {
            lbi->setHoverState(false);
        }
    });

    connect(m_listWidget, &QListWidget::entered,
            this, [&](const QModelIndex &index) {
        qDebug()<<"entered "<<index;

    });

    connect(m_listWidget, &LBListWidget::scrollValueChanged,
            this, [&](int value) {
        if (value == 0) {
            if (m_sepAnimation->state() == QAbstractAnimation::State::Running) {
                m_sepAnimation->stop();
            }
            m_sepAnimation->setStartValue(1.0);
            m_sepAnimation->setEndValue(0.0);
            m_sepAnimation->start();
        } else {
            if (m_sepAnimation->state() == QAbstractAnimation::State::Running) {
                return;
            } else {
                m_sepAnimation->setStartValue(0.0);
                m_sepAnimation->setEndValue(1.0);
                m_sepAnimation->start();
            }
        }
    });

    initUI();
}

LeftBar::~LeftBar()
{

}

//void LeftBar::paintEvent(QPaintEvent *event)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    QWidget::paintEvent(event);
//}

void LeftBar::initUI()
{
    const int contentW = LEFT_BAR_W - LEFT_BAR_CONTENT_LR_MARGIN*2;
    QVBoxLayout *layout  = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
    layout->setSpacing(0);
    layout->addSpacing(LEFT_BAR_BT_TB_MARGIN);
    {
        QLabel *title = new QLabel;
        title->setObjectName("RKTitle");
        title->setFixedHeight(LEFT_BAR_BIG_TITLE_FONT_SIZE);
//        title->setFixedWidth(contentW);
        title->setFixedWidth(LEFT_BAR_W);
        title->setAlignment(Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignVCenter);
        title->setWordWrap(true);
        title->setText(tr("Rock Rokr"));
        layout->addWidget(title);

        qDebug()<<" title "<<title;
    }
    {
        m_hSep = new DSeparatorHorizontal;
        m_hSep->setGraphicsEffect(m_sepEffect);
        layout->addWidget(m_hSep);
    }
    addSpacingListWidgetItem(LEFT_BAR_CONTENT_BASE_MARGIN);
    {
        QListWidgetItem *item = createListWidgetItem(LEFT_BAR_FONT_SIZE_SECTION);
        m_listWidget->addItem(item);

        ItemFragment *fm = createItemFragment(LEFT_BAR_FONT_SIZE_SECTION);

        QLabel *ts = new QLabel;
        ts->setObjectName("HomeTitle");
        ts->setFixedHeight(LEFT_BAR_FONT_SIZE_SECTION);
        ts->setText(tr("MUSIC"));
        ts->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignVCenter);
        fm->layout()->addWidget(ts);
        m_listWidget->setItemWidget(item, fm);
    }
    addSpacingListWidgetItem(LEFT_BAR_CONTENT_BASE_MARGIN);
    {
        QList<int> list;
        list << LBListItem::ItemType::TypeArtist
             << LBListItem::ItemType::TypeAlbums
             << LBListItem::ItemType::TypeGenres
             << LBListItem::ItemType::TypeSongs
             << LBListItem::ItemType::TypeFavorites;
        QStringList names;
        names << tr("TypeArtist")
              << tr("TypeAlbums")
              << tr("TypeGenres")
              << tr("TypeSongs")
              << tr("TypeFavorites");
        for (int i=0; i<list.size(); ++i) {
            QListWidgetItem *item = createListWidgetItem(LEFT_BAR_ITEM_H);
            m_listWidget->addItem(item);

            ItemFragment *fm = createItemFragment(LEFT_BAR_ITEM_H);

            LBListItem *lbi = new LBListItem((LBListItem::ItemType)list.at(i), names.at(i));
            lbi->setExtraData(i);
            lbi->setFixedWidth(LEFT_BAR_ITEM_W);
            lbi->setFixedHeight(LEFT_BAR_ITEM_H);
            connect(lbi, &LBListItem::leftBtnClicked,
                    this, [&](LBListItem *item){
                qDebug()<<"item clicked "<<item->enumToStr("ItemType", item->itemType());
                emit this->itemClicked(item);
            });
            fm->layout()->addWidget(lbi);
            m_listWidget->setItemWidget(item, fm);
        }
    }
    addSpacingListWidgetItem(LEFT_BAR_CONTENT_BASE_MARGIN);
    {
        const int headerItemH = m_plsView->getHeader()->height() +
                                m_plsView->getSeparator()->height();

        QListWidgetItem *item = createListWidgetItem(headerItemH);
        m_listWidget->addItem(item);

        ItemFragment *fm = createItemFragment(headerItemH);
        m_plsView->setFixedWidth(LEFT_BAR_ITEM_W);
        connect(m_plsView, &RKExpand::expandExpectedSizeChanged,
                this, [&, item, fm, headerItemH](QSize expandSize) {
//            qDebug()<<Q_FUNC_INFO<<" --- &RKExpand::expandExpectedSizeChanged  expandSize"<<expandSize
//                   <<" headerItemH "<<headerItemH;

            if (m_plsView->expand()) {
                const int h = headerItemH + expandSize.height() + m_plsView->getExpandedSeparato()->height();
//                qDebug()<<Q_FUNC_INFO<<" --- &RKExpand::expandExpectedSizeChanged all expanded view height "<<h;
                item->setSizeHint(QSize(LEFT_BAR_ITEM_W, h));
                fm->setFixedHeight(h);
            } else {
                item->setSizeHint(QSize(LEFT_BAR_ITEM_W, headerItemH));
                fm->setFixedHeight(headerItemH);
            }
            m_listWidget->updateHeight();
        });

        connect(m_plsView, &RKExpand::expandChange, this, [&, item, fm, headerItemH](bool expanded) {

        });

        fm->layout()->addWidget(m_plsView);
        m_listWidget->setItemWidget(item, fm);

    }
    layout->addWidget(m_listWidget);
    layout->addSpacing(4);

    {
        ItemFragment *fm = createItemFragment(LEFT_BAR_FONT_SIZE_SECTION);
        DSeparatorHorizontal *sep = new DSeparatorHorizontal;
        sep->setFixedWidth(LEFT_BAR_ITEM_W);
        fm->layout()->addWidget(sep);
        layout->addWidget(fm);
    }
    layout->addSpacing(4);
    {
        ItemFragment *fm = createItemFragment(LEFT_BAR_ITEM_H);
        LBListItem *item = new LBListItem(LBListItem::TypeAddon, tr("TypeAddon"));
        item->setFixedHeight(LEFT_BAR_ITEM_H);
        item->setFixedWidth(LEFT_BAR_ITEM_W);
        connect(item, &LBListItem::leftBtnClicked, this, [&](LBListItem *item) {

        });
        fm->layout()->addWidget(item);
        layout->addWidget(fm);
    }
    layout->addSpacing(4);
    {
        ItemFragment *fm = createItemFragment(LEFT_BAR_ITEM_H);
        LBListItem *item = new LBListItem(LBListItem::TypeService, tr("TypeService"));
        item->setFixedHeight(LEFT_BAR_ITEM_H);
        item->setFixedWidth(LEFT_BAR_ITEM_W);
        connect(item, &LBListItem::leftBtnClicked, this, [&](LBListItem *item) {

        });
        fm->layout()->addWidget(item);
        layout->addWidget(fm);
    }
    layout->addSpacing(LEFT_BAR_CONTENT_BASE_MARGIN);
    this->setLayout(layout);
}

void LeftBar::addSpacingListWidgetItem(int height)
{
    QListWidgetItem *item = new QListWidgetItem(m_listWidget);
    item->setSizeHint(QSize(LEFT_BAR_ITEM_W, height));
    m_listWidget->addItem(item);
}

inline QListWidgetItem *LeftBar::createListWidgetItem(int height)
{
    QListWidgetItem *item = new QListWidgetItem(m_listWidget);
    item->setSizeHint(QSize(LEFT_BAR_ITEM_W, height));
    return item;
}

ItemFragment *LeftBar::createItemFragment(int height)
{
    ItemFragment *fm = new ItemFragment(ItemFragment::LayoutOrientation::Horizontal, m_listWidget);
    fm->resize(LEFT_BAR_ITEM_W, height);
    fm->layout()->setContentsMargins(LEFT_BAR_CONTENT_LR_MARGIN, 0, LEFT_BAR_CONTENT_LR_MARGIN, 0);
    fm->layout()->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignVCenter);
    return  fm;
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

