#include "ViewUtility.h"

#include <qcompilerdetection.h>
#include <QDebug>
#include <QStyleFactory>
#include <QRadioButton>
#include <QFont>
#include <QWidget>
#include <QMenu>

#include <ddialog.h>
#include <DDesktopServices>

#include "LibPhoenixPlayerMain.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/MusicQueue.h"
#include "PlayerCore/PlayListMeta.h"
#include "PlayerCore/PlayListMetaMgr.h"
#include "MusicLibrary/MusicLibraryManager.h"
#include "UserInterface/UserInterfaceMgr.h"

#include "rockrokr_global.h"
#include "UserInterfaceRockRokr.h"
#include "RKMainWindow.h"
#include "TrackInfoDialog.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

static RKMainWindow *s_mainWindow = Q_NULLPTR;

inline void chkMainWindow()
{
    if (Q_UNLIKELY(!s_mainWindow)) {
        UserInterfaceMgr mgr;
        UserInterfaceRockRokr *rk = qobject_cast<UserInterfaceRockRokr*>(mgr.usedInterface());
        if (rk) {
            s_mainWindow = rk->mainWindow();
        } else {
            qFatal("No main window point found!!");
        }
    }
}

QImage ViewUtility::cropRect(const QImage &image, QSize sz)
{
    QImage newImage = image.scaled(sz, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    auto center = QRectF(newImage.rect()).center();
    auto topLeft = QPointF(center.x() - sz.width() / 2.0,
                           center.y() - sz.height() / 2.0);
    if (topLeft.x() < 0) {
        topLeft.setX(0);
    }
    if (topLeft.y() < 0) {
        topLeft.setY(0);
    }
    QRect crop(topLeft.toPoint(), sz);
    return newImage.copy(crop);
}

void ViewUtility::showTrackRemoveDialog(const PhoenixPlayer::AudioMetaObject &obj)
{
    chkMainWindow();

    DDialog dlg(s_mainWindow);
    dlg.setStyle(QStyleFactory::create("dlight"));
    dlg.setTextFormat(Qt::RichText);
    dlg.addButton(QObject::tr("Cancel"), true, DDialog::ButtonWarning);
    dlg.addButton(QObject::tr("Delete"), false, DDialog::ButtonNormal);

    auto cover = QImage(QString(":/light/image/del_notify.svg"));
    QUrl uri = obj.queryImgUri();
    if (uri.isValid()) {
        QString f;
        if (uri.isRelative()) {
            f = uri.toString();
        } else {
            f = uri.toLocalFile();
        }
        auto img = QImage(f);
        if (!img.isNull()) {
            cover = img;
        }
    }
    auto icon = QPixmap::fromImage(ViewUtility::cropRect(cover, QSize(_to_px(64), _to_px((64)))));
    dlg.setIcon(icon, QSize(_to_px(64), _to_px(64)));

    QString name = obj.trackMeta().title();
    if (name.isEmpty()) {
        name = obj.name();
    }
    const QString file = QString("%1/%2").arg(obj.path()).arg(obj.name());
    QString text = QString(QObject::tr("Are you sure you want to delete %1 from database ?")).arg(name);
    dlg.setTitle(text);
    dlg.setMessage(QString(QObject::tr(("File path : %1"))).arg(file));

    auto btn = new QRadioButton(&dlg);
    btn->setText(QObject::tr("Delete from local disk (Can't undo)"));
    dlg.addContent(btn, Qt::AlignLeft);

    if (dlg.exec() == QDialog::Accepted) {
        MusicLibrary::MusicLibraryManager mgr;
        mgr.deleteObject(obj, btn->isChecked());
    }
}

void ViewUtility::showTrackInfoDialog(const AudioMetaObject &obj)
{
    chkMainWindow();

    auto dlg = new TrackInfoDialog(obj, s_mainWindow);
    dlg->exec();
}

void ViewUtility::showToast(const QString &msg)
{
    chkMainWindow();

    s_mainWindow->showToast(msg);
}

void ViewUtility::showTip(QWidget *baseWidget, const QString &text, int delayShowMilliseconds)
{
    chkMainWindow();

    s_mainWindow->showTip(baseWidget, text, delayShowMilliseconds);
}

void ViewUtility::showCategoryDetailView()
{
    chkMainWindow();

    s_mainWindow->showCategoryDetailView();
}

void ViewUtility::showPlaylistDetailView()
{
    chkMainWindow();
    s_mainWindow->showPlaylistDetailView();
}

void ViewUtility::menuAddToQueue(QMenu *menu, const AudioMetaObject &obj, PlayerCore *core)
{
    menu->addAction(QObject::tr("Add to queue"), [core, obj]() {
        core->playQueue()->addTrack(obj);
    });
}

void ViewUtility::menuAddToPlaylist(QMenu *menu, const AudioMetaObject &obj, PlayListMetaMgr *mgr)
{
    QMenu *subMenu = new QMenu(menu);
    subMenu->setStyle(QStyleFactory::create("dlight"));
    subMenu->addAction(QIcon(":/light/image/playlist_add.svg"), QObject::tr("New playlist"), [mgr](){
        PlayListMeta meta = mgr->create();
        mgr->tryAdd(meta);
    });

    subMenu->addSeparator();

    auto metalist = mgr->metaList();
    QFont font(subMenu->font());
    QFontMetrics fm(font);
    foreach(const PlayListMeta &meta, metalist) {
        auto text = fm.elidedText(QString(meta.getFileName().replace("&", "&&")),
                                  Qt::ElideMiddle, _to_px(160));
        subMenu->addAction(text, [meta, obj](){
            PlayListObject po(meta);
            if (!po.open()) {
                ViewUtility::showToast(QString(QObject::tr("Can't open to add playlist %1!")).arg(meta.getFileName()));
                return;
            }
            po.addTrack(obj);
            if (!po.save()) {
                ViewUtility::showToast(QString(QObject::tr("save to %1 error!")).arg(meta.getFileName()));
            }
        });
    }
    menu->addAction(QObject::tr("Add to playlist"))->setMenu(subMenu);
}

void ViewUtility::menuShowInFileMgr(QMenu *menu, const AudioMetaObject &obj)
{
    menu->addAction(QObject::tr("Display in file manager"), [obj](){
        QString file = QString("%1/%2").arg(obj.path()).arg(obj.name());
        QUrl uri = QUrl::fromLocalFile(file);
        DDesktopServices::showFileItem(uri);
    });
}

void ViewUtility::menuRemoveObject(QMenu *menu, const AudioMetaObject &obj)
{
    menu->addAction(QObject::tr("Remove"), [obj]() {
       ViewUtility::showTrackRemoveDialog(obj);
    });
}

void ViewUtility::menuTrackInfo(QMenu *menu, const AudioMetaObject &obj)
{
    menu->addAction(QObject::tr("Track info"), [obj](){
        ViewUtility::showTrackInfoDialog(obj);
    });
}

void ViewUtility::showSearchPage()
{
    chkMainWindow();
    s_mainWindow->showSearchPage();
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

