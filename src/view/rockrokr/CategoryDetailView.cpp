#include "CategoryDetailView.h"

#include <QVBoxLayout>
#include <QStyle>
#include <QFont>
#include <QFontMetrics>
#include <QPalette>

#include <DThemeManager>
#include <DHiDPIHelper>

#include "MusicLibrary/MusicLibraryManager.h"

#include "rockrokr_global.h"
#include "widget/RKImage.h"
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

//    friend class CategoryDetailView;
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
    m_trackView->setObjectName("TrackView");

    initUserInterface();
}

CategoryDetailView::~CategoryDetailView()
{

}

void CategoryDetailView::showArtistTracks(const QString &artistName, const QString &subName, const QUrl &preferImg, int totalNum)
{
    setTitle(artistName.isEmpty() ? tr("unknown") : artistName);
    setText(subName.isEmpty() ? tr("unknown") : subName);
    m_imageView->setUri(preferImg);
    m_numLabel->setText(QString::number(totalNum) + tr("tracks"));

    m_trackView->showArtistTracks(artistName);
}

void CategoryDetailView::showAlbumTracks(const QString &albumName, const QString &subName, const QUrl &preferImg, int totalNum)
{
    setTitle(albumName.isEmpty() ? tr("unknown") : albumName);
    setText(subName.isEmpty() ? tr("unknown") : subName);
    m_imageView->setUri(preferImg);
    m_numLabel->setText(QString::number(totalNum) + tr("tracks"));

    m_trackView->showAlbumTracks(albumName);
}

void CategoryDetailView::showGenreTracks(const QString &genreName, const QString &subName, const QUrl &preferImg, int totalNum)
{
    setTitle(genreName.isEmpty() ? tr("unknown") : genreName);
    setText(subName.isEmpty() ? tr("unknown") : subName);
    m_imageView->setUri(preferImg);
    m_numLabel->setText(QString::number(totalNum) + tr("tracks"));

    m_trackView->showGenreTracks(genreName);
}

void CategoryDetailView::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
}

void CategoryDetailView::initUserInterface()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);
    {
        QHBoxLayout *hb = new QHBoxLayout;
        hb->setContentsMargins(0, 0, 0, 0);
        hb->setSpacing(0);

        if (!m_imageView) {
            m_imageView = new RKImage;
        }
        m_imageView->setFixedSize(_to_px(64), _to_px(64));
        hb->addWidget(m_imageView);

        QVBoxLayout *vb = new QVBoxLayout;
        vb->setContentsMargins(0, 0, 0, 0);
        vb->setSpacing(10);

        if (!m_titleLabel) {
            m_titleLabel = new QLabel;
            m_titleLabel->setObjectName("TitleLabel");
        }
        vb->addWidget(m_titleLabel);

        if (!m_textLabel) {
            m_textLabel = new QLabel;
            m_textLabel->setObjectName("TextLabel");
        }
        vb->addWidget(m_textLabel);

        if (!m_numLabel) {
            m_numLabel = new QLabel;
            m_numLabel->setObjectName("NumLabel");
            QFont f(m_numLabel->font());
            f.setPixelSize(_to_font_px(12));
            m_numLabel->setFont(f);
        }
        vb->addWidget(m_numLabel);

        hb->addSpacing(10);
        hb->addLayout(vb);

        layout->addLayout(hb);
    }
    layout->addSpacing(10);
    layout->addWidget(m_trackView, Qt::AlignCenter);
    this->setLayout(layout);
}

void CategoryDetailView::setTitle(const QString &text)
{
    if (!m_titleLabel) {
        return;
    }
    const int w = this->width() - m_imageView->width()
                  - 5*2
                  - 10;
    m_titleLabel->setFixedHeight(_to_px(16));
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QFont f(m_titleLabel->font());
    f.setPixelSize(_to_font_px(14));
    m_titleLabel->setFont(f);

    QFontMetrics fm(f);
    m_titleLabel->setText(fm.elidedText(text, Qt::ElideRight, w));
}

void CategoryDetailView::setText(const QString &text)
{
    if (!m_textLabel) {
        return;
    }
    const int w = this->width() - m_imageView->width()
                  - 5*2
                  - 10;
    m_textLabel->setFixedHeight(_to_px(14));
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QFont f(m_textLabel->font());
    f.setPixelSize(_to_font_px(12));
    m_textLabel->setFont(f);

    QFontMetrics fm(f);
    m_textLabel->setText(fm.elidedText(text, Qt::ElideRight, w));
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#include "CategoryDetailView.moc"
