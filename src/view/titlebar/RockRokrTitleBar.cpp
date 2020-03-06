#include "RockRokrTitleBar.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QApplication>

#include <DThemeManager>

#include "UserInterface/UserInterfaceMgr.h"
#include "UserInterface/IUserInterface.h"

#include "rockrokr_global.h"
#include "SearchEdit.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RockrokrTitleBar::RockrokrTitleBar(QWidget *parent)
    : RKTitleBar(parent)
{
    this->setObjectName("RockrokrTitleBar");
    DThemeManager::instance()->registerWidget(this);

    this->setDisplayedButtons(WindowOptionButton | WindowMinButton | WindowCloseButton);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignVCenter);

    m_searchEdit = new SearchEdit;
    m_searchEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_searchEdit->setPlaceHolder(tr("Search"));

    this->getContentWidget()->setLayout(layout);
    layout->addWidget(m_searchEdit, Qt::AlignVCenter);
}

RockrokrTitleBar::~RockrokrTitleBar()
{

}

void RockrokrTitleBar::bindResultView(SearchResultView *view)
{
    m_searchEdit->bindResultView(view);
}

QSize RockrokrTitleBar::sizeHint() const
{
    return QSize(TITLE_BAR_W, TITLE_BAR_H);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


