#include "ViewUtility.h"

#include <QStyleFactory>
#include <QRadioButton>
#include <qcompilerdetection.h>
#include <QFont>
#include <QWidget>

#include <ddialog.h>

#include "LibPhoenixPlayerMain.h"
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
    if (Q_UNLIKELY(!s_mainWindow)) {
        UserInterfaceMgr mgr;
        UserInterfaceRockRokr *rk = qobject_cast<UserInterfaceRockRokr*>(mgr.usedInterface());
        if (rk) {
            s_mainWindow = rk->mainWindow();
        }
    }

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
//        phoenixPlayerLib->libraryMgr()->deleteObject(obj, btn->isChecked());
        MusicLibrary::MusicLibraryManager mgr;
        mgr.deleteObject(obj, btn->isChecked());
    }
}

void ViewUtility::showTrackInfoDialog(const AudioMetaObject &obj)
{
    if (Q_UNLIKELY(!s_mainWindow)) {
        UserInterfaceMgr mgr;
        UserInterfaceRockRokr *rk = qobject_cast<UserInterfaceRockRokr*>(mgr.usedInterface());
        if (rk) {
            s_mainWindow = rk->mainWindow();
        }
    }
    auto dlg = new TrackInfoDialog(obj, s_mainWindow);
//    dlg->setModal(false);
    dlg->exec();
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

