#ifndef RKUTILITY_H
#define RKUTILITY_H

#include <QString>
#include <QImage>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKUtility
{
public:
    explicit RKUtility();
    ~RKUtility();

    static QString coverCacheDir();

    static QString httpCookieFile();

    static QImage svgToImage(const QString &svg, const QSize &targetSize);

    static QPixmap svgToPixmap(const QString &svg, const QSize &targetSize);

    static QImage defaultCoverImage(const QSize &size, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);

    static QPixmap defaultCoverPixmap(const QSize &size);

};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKUTILITY_H
