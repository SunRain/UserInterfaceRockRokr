#include "SRLeftBar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <DThemeManager>
#include "dimagebutton.h"
#include <dseparatorhorizontal.h>

#include "rockrokr_global.h"
#include "widget/ItemFragment.h"
#include "widget/RKListWidget.h"
#include "widget/ItemFragment.h"
#include "view/ViewUtility.h"

#include "SRLeftBarItem.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

SRLeftBar::SRLeftBar(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("SRLeftBar");
    DThemeManager::instance()->registerWidget(this);
    this->setFrameStyle(QFrame::NoFrame);

    m_sepEffect = new QGraphicsOpacityEffect(this);
    m_sepEffect->setOpacity(0);

    m_hSep = new DSeparatorHorizontal;
    m_hSep->setGraphicsEffect(m_sepEffect);

    m_sepAnimation = new QPropertyAnimation(this);
    m_sepAnimation->setTargetObject(m_sepEffect);
    m_sepAnimation->setPropertyName("opacity");
    m_sepAnimation->setDuration(1000);
    m_sepAnimation->setKeyValueAt(0.4, 1.0);
    m_sepAnimation->setKeyValueAt(0.8, 1.0);

    m_backBtn = new DImageButton;
    m_backBtn->setObjectName("BackBtn");
    m_backBtn->setFixedSize(LEFT_BAR_FONT_SIZE_SECTION, LEFT_BAR_FONT_SIZE_SECTION);
    m_backBtn->setNormalPic(":/light/image/ic_arrow_back_grey_24px.svg");
    m_backBtn->setHoverPic(":/light/image/ic_arrow_back_black_24px.svg");

    m_pluginView = new RKListWidget;
    m_pluginView->setObjectName("PluginView");
    m_pluginView->setSpacing(0);
    m_pluginView->setContentsMargins(0, 0, 0, 0);
    m_pluginView->setMouseTracking(true);
    m_pluginView->setSelectionMode(QListView::SingleSelection);
    m_pluginView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pluginView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pluginView->setFocusPolicy(Qt::NoFocus);

    connect(m_backBtn, &DImageButton::clicked, this, [&]() {
        Q_EMIT clickedBackBtn();
    });

    connect(m_pluginView, &QListWidget::itemClicked,
            this, [&](QListWidgetItem *item) {
        //TODO
        qDebug()<<" ----------------  m_pluginView clicked "<<item;

    });
    connect(m_pluginView, &QListWidget::itemEntered,
            this, [&](QListWidgetItem *item) {

        qDebug()<<" ----------------  m_resultView entered "<<item;;

//        m_pluginView->setCurrentItem(item);
    });

    connect(m_pluginView, &RKListWidget::scrollValueChanged,
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

    initUserInterface();
}

SRLeftBar::~SRLeftBar()
{

}

void SRLeftBar::setEnabledPlugins(const QList<PluginMetaData> &list)
{
    m_pluginView->clear();

    int idx = 0;
    foreach (const auto &it, list) {
        QListWidgetItem *item = new QListWidgetItem(m_pluginView);
        item->setSizeHint(QSize(this->width(), LEFT_BAR_ITEM_H));
        m_pluginView->addItem(item);

        ItemFragment *fm = new ItemFragment(ItemFragment::Horizontal, m_pluginView);
        fm->setFixedSize(this->width(), LEFT_BAR_ITEM_H);
        fm->layout()->setContentsMargins(LEFT_BAR_CONTENT_LR_MARGIN,
                                         0,
                                         LEFT_BAR_CONTENT_LR_MARGIN,
                                         0);
        fm->layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        SRLeftBarItem *sm = new SRLeftBarItem;
//        sm->setFixedHeight(LEFT_BAR_ITEM_H);
        sm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sm->setText(it.property.name);
        sm->setExtraData(idx);

        fm->layout()->addWidget(sm);

        m_pluginView->setItemWidget(item, fm);

        ++idx;
    }
}

QSize SRLeftBar::sizeHint() const
{
    return QSize(LEFT_BAR_W, LEFT_BAR_H);
}

void SRLeftBar::initUserInterface()
{
    QVBoxLayout *layout  = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->setSpacing(0);
    layout->addSpacing(LEFT_BAR_BT_TB_MARGIN);

    {
        QHBoxLayout *hb = new QHBoxLayout;
        hb->setContentsMargins(0, 0, 0, 0);
        hb->setSpacing(0);

        QLabel *title = new QLabel;
        title->setObjectName("RKTitle");
        title->setFixedHeight(LEFT_BAR_BIG_TITLE_FONT_SIZE);
        title->setAlignment(Qt::AlignVCenter);
        title->setWordWrap(true);
        title->setText(tr("Rock Rokr"));

        hb->addSpacing(LEFT_BAR_CONTENT_LR_MARGIN);
        hb->addWidget(m_backBtn, Qt::AlignVCenter);
        hb->addSpacing(LEFT_BAR_ITEM_CONTENT_MARGIN);
        hb->addWidget(title, Qt::AlignVCenter);

        layout->addSpacing(5);
        layout->addLayout(hb);
        layout->addSpacing(5);
    }
    layout->addWidget(m_hSep);
    layout->addSpacing(5);
    layout->addWidget(m_pluginView);
}




} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
