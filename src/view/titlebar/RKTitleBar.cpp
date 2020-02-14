#include "RKTitleBar.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QApplication>

#include <DThemeManager>
#include <dwindowoptionbutton.h>
#include <DTitlebar>
#include <dimagebutton.h>
#include <dwindowclosebutton.h>
#include <dwindowminbutton.h>
#include <DSearchEdit>

#include "UserInterface/UserInterfaceMgr.h"
#include "UserInterface/IUserInterface.h"

#include "rockrokr_global.h"
#include "SearchEdit.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKTitleBar::RKTitleBar(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("RKTitleBar");

    DThemeManager::instance()->registerWidget(this);

    this->setFixedHeight(TITLE_BAR_H);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignVCenter);

    m_searchEdit = new SearchEdit;
    m_searchEdit->setFixedSize(TITLE_BAR_SEARCH_VIEW_W, TITLE_BAR_SEARCH_VIEW_H);
    m_searchEdit->setPlaceHolder(tr("Search"));

    QHBoxLayout *iconLy = new QHBoxLayout;
    iconLy->setContentsMargins(10, 0, 10, 0);
    iconLy->setAlignment(Qt::AlignVCenter);

    m_optionBtn = new DWindowOptionButton;
    connect(m_optionBtn, &DImageButton::clicked,
            this, [&](){
        qDebug()<<" option clicked ";
    });

    m_closeBtn = new DWindowCloseButton;
    connect(m_closeBtn, &DImageButton::clicked, this, [&] () {
        UserInterface::UserInterfaceMgr mgr;
        UserInterface::IUserInterface *us = mgr.usedInterface();
        if (us) {
            us->close();
        }
    });

    m_minimizeBtn = new DWindowMinButton;
    connect(m_minimizeBtn, &DImageButton::clicked,
            this, [&](){
        qDebug()<<" m_minimizeBtn clicked ";
    });
    iconLy->addWidget(m_optionBtn);
    iconLy->addWidget(m_minimizeBtn);
    iconLy->addWidget(m_closeBtn);

    layout->addWidget(m_searchEdit, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addLayout(iconLy);

}

RKTitleBar::~RKTitleBar()
{

}

void RKTitleBar::bindResultView(SearchResultView *view)
{
    m_searchEdit->bindResultView(view);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


