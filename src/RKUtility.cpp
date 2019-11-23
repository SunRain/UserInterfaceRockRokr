#include "RKUtility.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QPixmap>
#include <QImageReader>
#include <QApplication>
#include <QFont>
#include <QWidget>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

//const static char *DEFAULT_COVER_RES = ":/light/image/default_cover.svg";
const static char *DEFAULT_COVER_RES = ":/light/image/default_cover.jpg";

QString RKUtility::coverCacheDir()
{
    QString p = QString("%1/cover").arg(QStandardPaths::CacheLocation);
    QDir dir(p);
    if (!dir.exists()) {
        dir.mkpath(p);
    }
    return p;
}

QString RKUtility::httpCookieFile()
{
    return QString("%1/cookie.txt").arg(QStandardPaths::AppDataLocation);
}

QImage RKUtility::svgToImage(const QString &svg, const QSize &targetSize)
{
    if (!QFile::exists(svg) || !targetSize.isValid()) {
        return QImage();
    }
    QImageReader reader;
    reader.setFileName(svg);
    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(targetSize * ratio);
        QImage img = reader.read();
        if (img.isNull()) {
            qDebug()<<QString("Read svg file [%1], error code [%2], string [%3]")
                      .arg(svg).arg(reader.error()).arg(reader.errorString());
        }
        return img;
    }
    return QImage();
}

QPixmap RKUtility::svgToPixmap(const QString &svg, const QSize &targetSize)
{
    if (!QFile::exists(svg) || !targetSize.isValid()) {
        return QPixmap();
    }
    QImageReader reader;
    reader.setFileName(svg);

    QPixmap pixmap;
    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(targetSize * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    } else {
        pixmap.load(svg);
    }
    return pixmap;
}

static QImage   s_defaultCover;
QImage RKUtility::defaultCoverImage(const QSize &size, Qt::AspectRatioMode aspectRatioMode)
{
    if (Q_LIKELY(!s_defaultCover.isNull())) {
        const qreal ratio = qApp->devicePixelRatio();
        return s_defaultCover.scaled(size * ratio, aspectRatioMode);
    }
    s_defaultCover = RKUtility::svgToImage(DEFAULT_COVER_RES, size);
    return s_defaultCover;
}

QPixmap RKUtility::defaultCoverPixmap(const QSize &size)
{
    return QPixmap::fromImage(defaultCoverImage(size));
}

void RKUtility::setWidgetFontSize(QWidget *target, int pixelSize)
{
    if (!target) {
        return;
    }
    QFont f(target->font());
    f.setPixelSize(pixelSize);
    target->setFont(f);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
