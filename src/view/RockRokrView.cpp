#include "RockRokrView.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <DThemeManager>

#include "rockrokr_global.h"
#include "leftbar/LeftBar.h"
#include "view/titlebar/RKTitleBar.h"
#include "view/playbar/PlayBar.h"

RockRokrView::RockRokrView(QWidget *parent)
    : QFrame(parent)

{
//    DThemeManager::instance()->registerWidget(this);
    this->setStyleSheet(QString("QWidget{background-color:%1;}").arg(MAIN_VIEW_BG_COLOR));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_leftbar = new LeftBar;
    m_leftbar->setFixedWidth(LEFT_BAR_W);
    m_leftbar->setFixedHeight(LEFT_BAR_H);

    QVBoxLayout *rPartMainLy = new QVBoxLayout;
    rPartMainLy->setContentsMargins(0, LEFT_BAR_BT_TB_MARGIN, 0, 0);
    rPartMainLy->setSpacing(0);

    QVBoxLayout *rPartContentLy = new QVBoxLayout;
    rPartContentLy->setContentsMargins(RIGHT_PART_L_MARGIN, 0, 0, 0);
    rPartContentLy->setSpacing(0);

    m_titlebar = new RKTitleBar;
    m_playbar = new PlayBar;
    m_playbar->setFixedSize(PLAY_BAR_W, PLAY_BAR_H);

    rPartContentLy->addWidget(m_titlebar, 0, Qt::AlignTop);
    rPartMainLy->addLayout(rPartContentLy);
    rPartMainLy->addWidget(m_playbar, 0, Qt::AlignBottom);
    layout->addWidget(m_leftbar);
    layout->addLayout(rPartMainLy);
}

RockRokrView::~RockRokrView()
{

}
