#include "RKOverlayWidget.h"

#include <DThemeManager>

#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

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

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKOverlayWidget::RKOverlayWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("RKOverlayWidget");
    DThemeManager::instance()->registerWidget(this);

    this->setAttribute(Qt::WA_TranslucentBackground);

    m_backgroundWidget = new QWidget;
//    m_backgroundWidget->setObjectName("BackgroundWidget");
    m_backgroundWidget->setStyleSheet("QWidget{background-color: rgb(255, 255, 255);border-radius:4px}");

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);
    effect->setColor(Qt::gray);
    effect->setBlurRadius(15);
    m_backgroundWidget->setGraphicsEffect(effect);

    QVBoxLayout *ly = new QVBoxLayout(this);
    ly->addWidget(m_backgroundWidget, 0, Qt::AlignmentFlag::AlignCenter);

    this->setLayout(ly);

    m_stack = new QStackedLayout(m_backgroundWidget);
    m_stack->setSpacing(0);
    m_stack->setContentsMargins(0, 0, 0, 0);

    connect(m_stack, &QStackedLayout::currentChanged,
            this, &RKOverlayWidget::currentChanged);

    connect(m_stack, &QStackedLayout::widgetRemoved,
            this, &RKOverlayWidget::widgetRemoved);
}

RKOverlayWidget::~RKOverlayWidget()
{

}

void RKOverlayWidget::setBackgroundPixmap(const QPixmap &pixmap)
{
    m_bgPixmap = pixmap;
}

int RKOverlayWidget::addWidget(QWidget *widget)
{
    return m_stack->addWidget(widget);
}

int RKOverlayWidget::currentIndex() const
{
    return m_stack->currentIndex();
}

QWidget *RKOverlayWidget::currentWidget() const
{
    return m_stack->currentWidget();
}

QWidget *RKOverlayWidget::widget(int index) const
{
    return m_stack->widget(index);
}

void RKOverlayWidget::removeWidget(QWidget *widget)
{
    m_stack->removeWidget(widget);
}

//void RKOverlayWidget::addContent(QWidget *content, Qt::AlignmentFlag flag)
//{
//    if (m_content) {
//        m_content->close();
//        m_content->deleteLater();
//    }
//    m_content = content;
//    if (m_content) {
//        m_backgroundWidget->setFixedSize(m_content->size().width() + 10,
//                                         m_content->size().height() +10);
//    }
//    m_contentLayout->addWidget(m_content, 0, flag);
//}

void RKOverlayWidget::mousePressEvent(QMouseEvent *event)
{
    const QPoint pt = event->pos();
    const QRect gm = m_backgroundWidget->geometry();
    if (pt.x() < gm.x()
            || pt.x() > (gm.x() + gm.width())
            || pt.y() < gm.y()
            || pt.y() > (gm.y() + gm.height())) {
        emit mousePressOutsideContent();
    } else {
        QWidget::mousePressEvent(event);
    }
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
        qt_blurImage(&painter, src, 10, true, false);
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::HighQualityAntialiasing |
                           QPainter::SmoothPixmapTransform);
    painter.drawPixmap(0, 0, pm);

    // draw background mask
    painter.setPen(QColor(255, 255, 255, 50));
    painter.setBrush(QColor(255, 255, 255, 50));
    painter.drawRect(0, 0, pm.width(), pm.height());

    // draw content rect
    painter.setPen(/*QPen(QColor(0, 0, 0),  0)*/Qt::gray); //border
    painter.setBrush(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = m_backgroundWidget->rect();
    QPoint point = m_backgroundWidget->geometry().topLeft();

    painter.drawRoundedRect(point.x()-2 , point.y()-2, rect.width()+4, rect.height()+4, 4, 4);

    QWidget::paintEvent(event);
}

void RKOverlayWidget::setCurrentIndex(int index)
{
    QWidget *content = m_stack->widget(index);
    if (!content) {
        qDebug()<<"Can't find widget at index";
        return;
    }
    m_backgroundWidget->setFixedSize(m_content->size().width() + 10,
                                     m_content->size().height() +10);
    m_stack->setCurrentIndex(index);
}

void RKOverlayWidget::setCurrentWidget(QWidget *widget)
{
    if (!widget) {
        return;
    }
    if (m_stack->indexOf(widget) < 0) {
        qDebug()<<"Can't find current widget";
        return;
    }
    m_backgroundWidget->setFixedSize(widget->size().width() + 10,
                                     widget->size().height() +10);
    m_stack->setCurrentWidget(widget);
}




} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


