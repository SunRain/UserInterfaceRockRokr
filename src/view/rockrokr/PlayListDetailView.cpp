#include "PlayListDetailView.h"

#include <QVBoxLayout>
#include <QStyle>
#include <QStyleFactory>
#include <QFont>
#include <QFontMetrics>
#include <QPalette>
#include <QMenu>

#include <DThemeManager>
#include <DHiDPIHelper>
#include <dimagebutton.h>

#include "MusicLibrary/MusicLibraryManager.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/PlayListMetaMgr.h"

#include "rockrokr_global.h"
#include "widget/RKImage.h"
#include "widget/RKTableHeaderItem.h"
#include "widget/RKLineEdit.h"
#include "view/ViewUtility.h"
#include "TrackListModel.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class PlayListDetailViewDataProvider : public BaseTrackViewDataProvider
{
public:
    explicit PlayListDetailViewDataProvider()
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
    virtual ~PlayListDetailViewDataProvider() override
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

class PlayListDetailTrackView : public BaseTrackView
{
    Q_OBJECT
public:
    explicit PlayListDetailTrackView(QWidget *parent = Q_NULLPTR)
        : BaseTrackView(new PlayListDetailViewDataProvider, parent)
    {
        m_playerCore = new PlayerCore(this);
        m_libMgr = new MusicLibrary::MusicLibraryManager(this);
        m_plsMetaMgr = new PlayListMetaMgr(this);
    }

    virtual ~PlayListDetailTrackView() override
    {
        if (m_playerCore) {
            m_playerCore->deleteLater();
            m_playerCore = Q_NULLPTR;
        }
        if (m_libMgr) {
            m_libMgr->deleteLater();
            m_libMgr = Q_NULLPTR;
        }
        if (m_plsMetaMgr) {
            m_plsMetaMgr->deleteLater();
            m_plsMetaMgr = Q_NULLPTR;
        }
    }

    void showPlaylist(const PlayListMeta &meta)
    {
        getModel()->showPlaylist(meta);
    }

    // BaseTrackView interface
protected:
    void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE
    {
        const QModelIndex idx = indexAtPos(pos);
        if (!idx.isValid()) {
            qWarning()<<"Invalid QModelIndex!!";
            return;
        }
        const QString hash = getModel()->data(idx, TrackListModel::RoleHash).toString();
        const AudioMetaObject obj = m_libMgr->trackFromHash(hash);
        if (obj.isHashEmpty()) {
             ViewUtility::showToast(tr("Empty audio meta object !!"));
            return;
        }
        QMenu menu;
        menu.setStyle(QStyleFactory::create("dlight"));

        ViewUtility::menuAddToQueue(&menu, obj, m_playerCore);
        ViewUtility::menuAddToPlaylist(&menu, obj, m_plsMetaMgr);
        menu.addSeparator();
        ViewUtility::menuRemoveObject(&menu, obj);
        ViewUtility::menuShowInFileMgr(&menu, obj);
        menu.addSeparator();
        ViewUtility::menuTrackInfo(&menu, obj);

        menu.exec(this->mapToGlobal(pos));
    }
    void onClicked(const QModelIndex &index) Q_DECL_OVERRIDE
    {
        const QString hash = index.data(TrackListModel::ModelRoles::RoleHash).toString();
        m_playerCore->playFromLibrary(hash);
    }

private:
    PlayerCore                          *m_playerCore   = Q_NULLPTR;
    PlayListMetaMgr                     *m_plsMetaMgr   = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libMgr       = Q_NULLPTR;
};



/*************************************************************************
 *
 *************************************************************************/


PlayListDetailView::PlayListDetailView(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("PlayListDetailView");
    DThemeManager::instance()->registerWidget(this);

    m_trackView = new PlayListDetailTrackView;
    m_trackView->setObjectName("TrackView");

    m_imgView = new RKImage;

    m_title = new QLabel;
    m_title->setObjectName("Title");

    m_time = new QLabel;
    m_time->setObjectName("Time");

    m_taglb = new QLabel;
    m_taglb->setObjectName("Tag");
    m_taglb->setText(tr("Tag:"));

    m_antlb = new QLabel;
    m_antlb->setObjectName("Annotation");
    m_antlb->setText(tr("Annotation:"));

    m_tagedt = new RKLineEdit;
    m_tagedt->setReadOnly(true);
    m_tagedt->setObjectName("TagEditor");

    m_tagBtn = new DImageButton;
    m_tagBtn->setCheckable(true);
    m_tagBtn->setChecked(false);
    m_tagBtn->setFixedSize(_to_px(18), _to_px(18));
    m_tagBtn->setNormalPic(":/light/image/ic_pls_detail_view_mode_edit_normal.svg");
    m_tagBtn->setPressPic(":/light/image/ic_pls_detail_view_mode_edit_normal.svg");
    m_tagBtn->setHoverPic(":/light/image/ic_pls_detail_view_mode_edit_normal.svg");
    m_tagBtn->setCheckedPic(":/light/image/ic_pls_detail_view_mode_check_normal.svg");

    m_antedt = new RKLineEdit;
    m_antedt->setObjectName("AnnotationEditor");
    m_antedt->setReadOnly(true);

    m_antBtn = new DImageButton;
    m_antBtn->setCheckable(true);
    m_antBtn->setChecked(false);
    m_antBtn->setFixedSize(_to_px(18), _to_px(18));
    m_antBtn->setNormalPic(":/light/image/ic_pls_detail_view_mode_edit_normal.svg");
    m_antBtn->setPressPic(":/light/image/ic_pls_detail_view_mode_edit_normal.svg");
    m_antBtn->setHoverPic(":/light/image/ic_pls_detail_view_mode_edit_normal.svg");
    m_antBtn->setCheckedPic(":/light/image/ic_pls_detail_view_mode_check_normal.svg");

    connect(m_tagBtn, &DImageButton::clicked,
            this, [&]() {
        m_tagedt->setReadOnly(!m_tagBtn->isChecked());
    });

    connect(m_antBtn, &DImageButton::clicked,
            this, [&]() {
        m_antedt->setReadOnly(!m_antBtn->isChecked());
    });

    connect(m_tagedt, &RKLineEdit::editingFinished,
            this, [&]() {
        PlayListMeta old = m_meta;
        m_meta.setTag(m_tagedt->text());
        PlayListMetaMgr mgr;
        mgr.updateMeta(old, m_meta, true);
        mgr.saveToDatabase();
    });

    connect(m_antedt, &RKLineEdit::editingFinished,
            this, [&]() {
        PlayListMeta old = m_meta;
        m_meta.setAnnotation(m_antedt->text());
        PlayListMetaMgr mgr;
        mgr.updateMeta(old, m_meta, true);
        mgr.saveToDatabase();
    });

    initUserInterface();
}

PlayListDetailView::~PlayListDetailView()
{

}

void PlayListDetailView::showPlayList(const PlayListMeta &meta)
{
    m_meta = meta;
    const int w = this->width() - m_imgView->width()
                  - 5*2
                  - 10;

#define SET_FONT(label, height, fontSize, text, width) \
    label->setFixedHeight(height); \
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); \
    QFont f(label->font()); \
    f.setPixelSize(fontSize); \
    label->setFont(f); \
    QFontMetrics fm(f); \
    label->setText(fm.elidedText(text, Qt::ElideRight, width));

    {
        SET_FONT(m_title, _to_px(24), _to_font_px(14), meta.getFileName(), w)
    }
    {
        const QString st = PlayListMetaMgr::formatTimeStamp(meta, QString("yyyy-MM-dd"));
        SET_FONT(m_time, _to_px(18), _to_font_px(12), tr("TimeStamp:")+" "+st, w);
    }
    {
        SET_FONT(m_tagedt, _to_px(18), _to_font_px(12), meta.getTag(), w-20);
    }
    {
         SET_FONT(m_antedt, _to_px(18), _to_font_px(12), meta.getAnnotation(), w-20);
    }

    m_trackView->showPlaylist(meta);
}

void PlayListDetailView::initUserInterface()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);
    {
        QHBoxLayout *hb = new QHBoxLayout;
        hb->setContentsMargins(0, 0, 0, 0);
        hb->setSpacing(0);

        m_imgView->setFixedSize(_to_px(96), _to_px(96));
        hb->addWidget(m_imgView);

        QVBoxLayout *vb = new QVBoxLayout;
        vb->setContentsMargins(0, 0, 0, 0);
        vb->setSpacing(10);

        vb->addWidget(m_title);
        vb->addWidget(m_time);

        {
            QHBoxLayout *l = new QHBoxLayout;
            l->setContentsMargins(0, 0, 0, 0);
            l->setSpacing(0);

            l->addWidget(m_taglb);
            l->addSpacing(5);
            l->addWidget(m_tagedt);
            l->addSpacing(5);
            l->addWidget(m_tagBtn);
            vb->addLayout(l);
        }
        {
            QHBoxLayout *l = new QHBoxLayout;
            l->setContentsMargins(0, 0, 0, 0);
            l->setSpacing(0);

            l->addWidget(m_antlb);
            l->addSpacing(5);
            l->addWidget(m_antedt);
            l->addSpacing(5);
            l->addWidget(m_antBtn);
            vb->addLayout(l);
        }
        hb->addSpacing(10);
        hb->addLayout(vb);

        layout->addLayout(hb);
    }
    layout->addSpacing(10);
    layout->addWidget(m_trackView, Qt::AlignCenter);
    this->setLayout(layout);
}





} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#include "PlayListDetailView.moc"

