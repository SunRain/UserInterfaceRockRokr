#include "CategoryDetailView.h"

#include <QVBoxLayout>

#include <DThemeManager>
#include <DHiDPIHelper>

#include "MusicLibrary/MusicLibraryManager.h"

#include "rockrokr_global.h"
#include "widget/RKTableHeaderItem.h"
#include "TrackListModel.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


class CategoryDetailViewDataProvider : public BaseTrackViewDataProvider
{
public:
    explicit CategoryDetailViewDataProvider()
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
    virtual ~CategoryDetailViewDataProvider() override
    {

    }

    // BaseTrackViewDataProvider interface
public:
    RKTableHeaderItem *headerItem() const Q_DECL_OVERRIDE
    {
        return m_header;
    }
    TrackListModel *dataModel() const Q_DECL_OVERRIDE
    {
        return m_dataModel;
    }

private:
    RKTableHeaderItem           *m_header  = Q_NULLPTR;
    TrackListModel              *m_dataModel = Q_NULLPTR;
};

/*************************************************************************
 *
 *************************************************************************/

class CategoryDetailTrackView : public BaseTrackView
{
    Q_OBJECT
public:
    explicit CategoryDetailTrackView(QWidget *parent = Q_NULLPTR)
        : BaseTrackView(new CategoryDetailViewDataProvider, parent)
    {

    }
    virtual ~CategoryDetailTrackView() override
    {

    }

    void showArtistTracks(const QString &artistName)
    {
        getModel()->showArtistTracks(artistName);
    }
    void showAlbumTracks(const QString  &albumName)
    {
        getModel()->showAlbumTracks(albumName);
    }
    void showGenreTracks(const QString &genreName)
    {
        getModel()->showGenreTracks(genreName);
    }

    // BaseTrackView interface
protected:
    void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE
    {

    }
    void onClicked(const QModelIndex &index) Q_DECL_OVERRIDE
    {
    }
};

/*************************************************************************
 *
 *************************************************************************/

CategoryDetailView::CategoryDetailView(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("CategoryDetailView");
    DThemeManager::instance()->registerWidget(this);

    m_trackView = new CategoryDetailTrackView;

    initUserInterface();
}

CategoryDetailView::~CategoryDetailView()
{

}

void CategoryDetailView::showArtistTracks(const QString &artistName)
{
    m_trackView->showArtistTracks(artistName);
}

void CategoryDetailView::showAlbumTracks(const QString &albumName)
{
    m_trackView->showAlbumTracks(albumName);
}

void CategoryDetailView::showGenreTracks(const QString &genreName)
{
    m_trackView->showGenreTracks(genreName);
}

void CategoryDetailView::resizeEvent(QResizeEvent *event)
{
//    m_trackView->setFixedSize(event->size().width()-20, event->size().height()-20);
    QFrame::resizeEvent(event);
}

void CategoryDetailView::initUserInterface()
{
    QVBoxLayout *ly = new QVBoxLayout;
    ly->setContentsMargins(5, 5, 5, 5);
    ly->setSpacing(0);
    ly->addWidget(m_trackView, Qt::AlignCenter);
    this->setLayout(ly);
}











} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#include "CategoryDetailView.moc"
