#include "CategoryDetailView.h"

#include <QVBoxLayout>

#include <DThemeManager>
#include <DHiDPIHelper>

#include "rockrokr_global.h"

#include "widget/RKTableHeaderItem.h"

#include "TrackListModel.h"


DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


CategoryDetailViewDataProvider::CategoryDetailViewDataProvider()
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
}

CategoryDetailViewDataProvider::~CategoryDetailViewDataProvider()
{

}

RKTableHeaderItem *CategoryDetailViewDataProvider::headerItem() const
{
    return m_header;
}

TrackListModel *CategoryDetailViewDataProvider::dataModel() const
{
    return m_dataModel;
}

void CategoryDetailViewDataProvider::resetDataModelToDefalutState() const
{
    //TODO implement later
}

CategoryDetailView::CategoryDetailView(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("BaseCategoryDetailView");
    DThemeManager::instance()->registerWidget(this);

    m_trackView = new CategoryDetailTrackView;

    initUserInterface();
}

CategoryDetailView::~CategoryDetailView()
{

}

void CategoryDetailView::initUserInterface()
{
    QVBoxLayout *ly = new QVBoxLayout;
    ly->setContentsMargins(0, 0, 0, 0);
    ly->setSpacing(0);
    ly->addWidget(m_trackView);
    this->setLayout(ly);
}

CategoryDetailTrackView::CategoryDetailTrackView(QWidget *parent)
    : BaseTrackView(new CategoryDetailViewDataProvider, parent)
{

}

CategoryDetailTrackView::~CategoryDetailTrackView()
{

}

void CategoryDetailTrackView::onClicked(const QModelIndex &index)
{
}

void CategoryDetailTrackView::showContextMenu(const QPoint &pos)
{

}



} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

