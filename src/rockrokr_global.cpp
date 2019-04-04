#include "rockrokr_global.h"

#include <QPixmap>
#include <QSize>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QImageReader>
#include <QApplication>


QPixmap svgToPixmap(const QString &svg, const QSize &targetSize)
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
