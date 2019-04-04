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

#include "rockrokr_global.h"
#include "widget/RKSearchEdit.h"

DWIDGET_USE_NAMESPACE

RKTitleBar::RKTitleBar(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("RKTitleBar");

    DThemeManager::instance()->registerWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignVCenter);

    m_searchEdit = new RKSearchEdit;
    m_searchEdit->setFixedSize(TITLE_BAR_SEARCH_VIEW_W, TITLE_BAR_SEARCH_VIEW_H);
    m_searchEdit->setPlaceHolder(tr("Search"));

    QHBoxLayout *iconLy = new QHBoxLayout;
    iconLy->setContentsMargins(10, 0, 10, 0);
    iconLy->setAlignment(Qt::AlignVCenter);

    m_optionBtn = new DWindowOptionButton;
    connect(m_optionBtn, &DImageButton::clicked,
            this, [&](){
        qDebug()<<Q_FUNC_INFO<<" option clicked ";
    });

    m_closeBtn = new DWindowCloseButton;
    connect(m_closeBtn, &DImageButton::clicked, this, &QApplication::quit);

    m_minimizeBtn = new DWindowMinButton;
    connect(m_minimizeBtn, &DImageButton::clicked,
            this, [&](){
        qDebug()<<Q_FUNC_INFO<<" m_minimizeBtn clicked ";
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




