#include "RKOverlayWidget.h"

#include <QPainter>
#include <QImage>
#include <QDebug>


QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE


//  extern QImage srcImg;//source image
//   QPixmap pxDst( srcImg.size() );//blurred destination
//   pxDst.fill( Qt::transparent );
//   {
//     QPainter painter( &pxDst );
//     qt_blurImage( &painter, srcImg, 2, true, false );//blur radius: 2px
//   }

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKOverlayWidget::RKOverlayWidget(QWidget *parent)
    : QWidget(parent)
{

}

RKOverlayWidget::~RKOverlayWidget()
{

}

void RKOverlayWidget::setBackgroundPixmap(const QPixmap &pixmap)
{
    m_bgPixmap = pixmap;
}

void RKOverlayWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void RKOverlayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

void RKOverlayWidget::paintEvent(QPaintEvent *event)
{
    QImage src = m_bgPixmap.toImage();

    QPixmap pm(m_bgPixmap.size());
    pm.fill(Qt::transparent);

    {
        QPainter painter(&pm);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::HighQualityAntialiasing |
                               QPainter::SmoothPixmapTransform);
        qt_blurImage(&painter, src, 15, true, false);
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::HighQualityAntialiasing |
                           QPainter::SmoothPixmapTransform);
    painter.drawPixmap(0, 0, pm);


}




} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


