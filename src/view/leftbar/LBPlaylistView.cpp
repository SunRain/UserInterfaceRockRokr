#include "LBPlaylistView.h"

#include <QDebug>
#include <QFrame>
#include <QMenu>
#include <QResizeEvent>
#include <QStyleFactory>
#include <QTimer>


#include <DArrowButton>
#include <DThemeManager>
#include <dboxwidget.h>
#include <dimagebutton.h>
#include <ddialog.h>
#include <DToast>

#include "PlayerCore/PlayListMetaMgr.h"
#include "PlayerCore/PlayListObject.h"
#include "LibPhoenixPlayerMain.h"
#include "PPSettings.h"
#include "UserInterface/UserInterfaceMgr.h"

#include "rockrokr_global.h"
#include "UserInterfaceRockRokr.h"
#include "RKMainWindow.h"
#include "LBListItem.h"
#include "widget/RKListWidget.h"
#include "widget/RKBoxWidget.h"
#include "view/ViewUtility.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

LBPlaylistView::LBPlaylistView(QWidget *parent)
    : RKExpand(parent),
      m_headerLine(new QWidget),
      m_arrowBtn(new DArrowButton),
      m_ImgBtn(new DImageButton),
//      m_toast(Q_NULLPTR),
      m_listWidget(new RKVBoxWidget),
      m_plsMetaMgr(new PlayListMetaMgr(this))
{
    this->setObjectName("LBPlaylistView");
    DThemeManager::instance()->registerWidget(this);

    m_headerLine->setObjectName("HeaderLine");
    m_headerLine->setFixedHeight(LEFT_BAR_BIG_TITLE_FONT_SIZE);

    QHBoxLayout *headerLayout = new QHBoxLayout(m_headerLine);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignVCenter);
    headerLayout->setSpacing(0);
    {
        QLabel *l = new QLabel;
        l->setObjectName("HeaderTitle");
        l->setFixedHeight(LEFT_BAR_BIG_TITLE_FONT_SIZE);
        l->setText(tr("PLAYLIST"));
        headerLayout->addWidget(l);
        headerLayout->addStretch(9);

        m_ImgBtn->setHoverPic(":/light/image/playlist_add_black_18dp.png");
        m_ImgBtn->setNormalPic(":/light/image/playlist_add_grey600_18dp.png");
        m_ImgBtn->setFixedSize(LEFT_BAR_BIG_TITLE_FONT_SIZE, LEFT_BAR_BIG_TITLE_FONT_SIZE);
        if (this->expand()) {
            m_ImgBtn->setEnabled(true);
            m_ImgBtn->setVisible(true);
        } else {
            m_ImgBtn->setEnabled(false);
            m_ImgBtn->setVisible(false);
        }
        connect(m_ImgBtn, &DImageButton::clicked,
                this, [&](){
            PlayListMeta meta = m_plsMetaMgr->create();
            m_plsMetaMgr->tryAdd(meta);
        });
        headerLayout->addWidget(m_ImgBtn);
        headerLayout->addStretch(1);

        m_arrowBtn->setFixedSize(LEFT_BAR_BIG_TITLE_FONT_SIZE, LEFT_BAR_BIG_TITLE_FONT_SIZE);
        m_arrowBtn->setArrowDirection(DArrowButton::ArrowLeft);
        connect(m_arrowBtn, &DArrowButton::mouseRelease,
                [&]() {
//            qDebug()<<Q_FUNC_INFO<<" ------ DArrowButton::mouseRelease, expand "<<this->expand();
            if (this->expand()) {
                this->setExpand(false);
                m_arrowBtn->setArrowDirection(DArrowButton::ArrowLeft);
            } else {
                this->setExpand(true);
                m_arrowBtn->setArrowDirection(DArrowButton::ArrowDown);
            }
        });
        headerLayout->addWidget(m_arrowBtn);
    }
    m_headerLine->setLayout(headerLayout);
    this->setHeader(m_headerLine);
    this->setHeaderHeight(LEFT_BAR_BIG_TITLE_FONT_SIZE);
    connect(m_listWidget, &RKBoxWidget::sizeChanged,
            [&](QSize size) {
        this->updateExpandSize(size);
    });

    this->setContent(m_listWidget);

    foreach(const PlayListMeta &meta, m_plsMetaMgr->metaList()) {
        LBListItem *item = new LBListItem(LBListItem::ItemType::TypePlaylist, meta.getFileName());
        item->setExtraData(meta.toMap());
        connectLBListItemSignals(item);
        m_listWidget->layout()->addWidget(item);
    }

    connect(m_plsMetaMgr, &PlayListMetaMgr::metaAdded,
            this, [&](const PlayListMeta &meta) {
        LBListItem *item = new LBListItem(LBListItem::ItemType::TypePlaylist, meta.getFileName());
        item->setExtraData(meta.toMap());
        item->setEditMode(true);
        this->connectLBListItemSignals(item);
        m_listWidget->layout()->addWidget(item);
    });
    connect(m_plsMetaMgr, &PlayListMetaMgr::metaDeleted,
            this, [&](const PlayListMeta &meta) {
        qDebug()<<"PlayListMetaMgr::deleteMeta  "<<meta.getFileName();

        for (int i = 0; i < m_listWidget->layout()->count(); ++i) {
            QLayoutItem *ly = m_listWidget->layout()->itemAt(i);
            LBListItem *item = qobject_cast<LBListItem*>(ly->widget());
            if (!item) {
                continue;
            }
//             qDebug()<<Q_FUNC_INFO<<" loop for item "<<item->enumToStr("ItemType", int(item->itemType()));

            PlayListMeta m = PlayListMeta::fromMap(item->extraData().toMap());

//            qDebug()<<Q_FUNC_INFO<<" item name "<<m.getFileName();

            if (m == meta) {
                ly = m_listWidget->layout()->takeAt(i);

//                qDebug()<<Q_FUNC_INFO<<" --------- find item at "<<i<<" name is "<<m.getFileName();
                item->disconnect();
                m_listWidget->layout()->removeWidget(item);
                item->setParent(Q_NULLPTR);
                item->deleteLater();
                item = Q_NULLPTR;
                if (ly) {
                    delete  ly;
                    ly = Q_NULLPTR;
                }
                break;
            }
        }
    });
    connect(m_plsMetaMgr, &PlayListMetaMgr::metaDataChanged,
            this, [&](PlayListMetaMgr::UpdateMetaRet ret,
                        const PlayListMeta &old, const PlayListMeta &newMeta){
        LBListItem *item = findLBListItem(old.toMap());
        if (ret == PlayListMetaMgr::UpdateMetaRet::OK) {
            if (item) {
//                qDebug()<<Q_FUNC_INFO<<" find item "<<old.getFileName();
                item->setExtraData(newMeta.toMap());
            } else {
                qWarning()<<" can't find item for "<<old.getFileName()
                         <<" with new name "<<newMeta.getFileName();
            }
        } else {
            ViewUtility::showToast(tr("Rename name conflict!"));
            item->setEditMode(true);
        }
    });
}

LBPlaylistView::~LBPlaylistView()
{

}



void LBPlaylistView::resizeEvent(QResizeEvent *event)
{
    RKExpand::resizeEvent(event);
//    DBaseExpand::resizeEvent(event);
//    qDebug()<<Q_FUNC_INFO<<"------ "<<event->size()
//           <<"headerline sizeHint "<<m_headerLine->sizeHint()
    //          <<" size "<<m_headerLine->size();
}

void LBPlaylistView::setExpand(bool value)
{
    if (value) {
        m_ImgBtn->setEnabled(true);
        m_ImgBtn->setVisible(true);
    } else {
        m_ImgBtn->setEnabled(false);
        m_ImgBtn->setVisible(false);
    }
    RKExpand::setExpand(value);
}

void LBPlaylistView::showPlaylistRightClickMenu(LBListItem *item, const QPoint &pos)
{
    QMenu menu;
    menu.setStyle(QStyleFactory::create("dlight"));
    menu.addAction(tr("play"), [&](){
        qDebug()<<" play clicked";
    });
    menu.addAction(tr("rename"), [&](){
        qDebug()<<" rename clicked";
        item->setEditMode(true);
    });
    menu.addAction(tr("delete"), [&, item](){
        this->showDeleteDlg(item);
    });
    menu.exec(item->mapToGlobal(pos));
}

void LBPlaylistView::showDeleteDlg(LBListItem *item)
{
    QString message = QString(tr("Are you sure you want to delete this playlist?"));

    DDialog warnDlg(this);
    warnDlg.setIcon(QIcon(":/light/image/delete_grey600_48dp.png"),
                    QSize(LEFT_BAR_ITEM_ICON_PART_W, LEFT_BAR_ITEM_ICON_PART_H));
    warnDlg.setTextFormat(Qt::AutoText);
    warnDlg.setTitle(message);
    warnDlg.addButton(tr("Cancel"), false, Dtk::Widget::DDialog::ButtonNormal);
    warnDlg.addButton(tr("Delete"), true, Dtk::Widget::DDialog::ButtonWarning);

    if (0 == warnDlg.exec()) {
        return;
    }
    m_plsMetaMgr->deleteMeta(PlayListMeta::fromMap(item->extraData().toMap()));
}

void LBPlaylistView::connectLBListItemSignals(LBListItem *item)
{
    if (!item) {
        return;
    }
    connect(item, &LBListItem::leftBtnClicked,
            this, [&, item](LBListItem *i){
        qDebug()<<"LBListItem::clicked item clicked";
        UserInterface::UserInterfaceMgr mgr;
        UserInterfaceRockRokr *rk = qobject_cast<UserInterfaceRockRokr*>(mgr.usedInterface());
        RKMainWindow *w = rk->mainWindow();
        w->showOverlay();

        //TODO left click to show playlist view
    });
    connect(item, &LBListItem::rename,
            this, [&, item](const QString &text){
        qDebug()<<"LBListItem::renam  rename to "<<text;

        PlayListMeta meta = PlayListMeta::fromMap(item->extraData().toMap());
        if (meta.getFileName() != text) {
            meta.setFileName(text);
            m_plsMetaMgr->updateMeta(PlayListMeta::fromMap(item->extraData().toMap()), meta);
        }
    });
    connect(item, &LBListItem::rightBtnClicked,
            this, [&] (LBListItem *item,  const QPoint &pos) {
        item->keepHover(true);
        this->showPlaylistRightClickMenu(item, pos);
        item->keepHover(false);
    });
}

LBListItem *LBPlaylistView::findLBListItem(const QVariant &value)
{
    for (int i = 0; i < m_listWidget->layout()->count(); ++i) {
        QLayoutItem *ly = m_listWidget->layout()->itemAt(i);
        LBListItem *item = qobject_cast<LBListItem*>(ly->widget());
        if (!item) {
            continue;
        }
        if (value == item->extraData()) {
            return item;
        }
    }
    return Q_NULLPTR;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

//#include "LBPlaylistView.moc"
