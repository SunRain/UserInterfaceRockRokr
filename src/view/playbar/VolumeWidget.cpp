#include "VolumeWidget.h"

#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QWheelEvent>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

#include <DThemeManager>


#include "widget/RKSlider.h"

DWIDGET_USE_NAMESPACE

VolumeWidget::VolumeWidget(QWidget *parent)
    : QFrame(parent),
      m_timer(new QTimer(this)),
      m_mouseIn(false),
      m_radius(4),
      m_background(QColor(255, 255, 255, 255)),
      m_borderColor(QColor(255, 255, 255, 255/10))

{
    this->setObjectName("VolumeWidget");
    DThemeManager::instance()->registerWidget(this);

    this->setFrameStyle(QFrame::NoFrame);
    this->setFixedSize(150, 40);

//    this->setStyleSheet(QString("QWidget{background-color:transparent;}"));

    m_timer->setInterval(1500);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout,
            [&](){
        if (!m_mouseIn) {
            this->hide();
        }
    });


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_slider = new RKSlider(Qt::Orientation::Horizontal);
    m_slider->setMaximum(100);
    m_slider->setMinimum(0);
    m_slider->setSingleStep(10);
    m_slider->setValue(50);
//    m_slider->setFixedHeight(40);

    layout->addStretch();
    layout->addWidget(m_slider);
    layout->addStretch();

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(10.0);
    effect->setColor(QColor(0, 0, 0,  160));
    effect->setOffset(0, 0);
    this->setGraphicsEffect(effect);

//    connect(m_slider, &RKSlider::valueChanged,
//            this, [&](int value){
//        qDebug()<<Q_FUNC_INFO<<" RKSlider::valueChanged "<<value;
//    });

//    connect(m_slider, &RKSlider::sliderMoved,
//            this, [&](int pos){
//        qDebug()<<Q_FUNC_INFO<<" RKSlider::sliderMoved "<<pos;
//    });

//    connect(m_slider, &RKSlider::sliderReleasedAt,
//            this, [&](int pos){
//        qDebug()<<Q_FUNC_INFO<<" RKSlider::sliderReleasedAt "<<pos;
//    });

}

VolumeWidget::~VolumeWidget()
{

}

int VolumeWidget::radius() const
{
    return m_radius;
}

QBrush VolumeWidget::background() const
{
    return m_background;
}

QColor VolumeWidget::borderColor() const
{
    return m_borderColor;
}

RKSlider *VolumeWidget::slider() const
{
    return m_slider;
}

void VolumeWidget::wheelEvent(QWheelEvent *event)
{
    QFrame::wheelEvent(event);
    qDebug()<<Q_FUNC_INFO<<event;

}

void VolumeWidget::enterEvent(QEvent *event)
{
    m_mouseIn = true;
    QFrame::enterEvent(event);
}

void VolumeWidget::leaveEvent(QEvent *event)
{
    m_mouseIn = false;
    this->deleyHide();
    QFrame::leaveEvent(event);
}

void VolumeWidget::showEvent(QShowEvent *event)
{
    m_mouseIn = true;
    QFrame::showEvent(event);
}

void VolumeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    QPainterPath path;

    auto penWidthf = 1.0;
    auto background =  this->m_background;
    auto borderColor = this->m_borderColor;

    const qreal radius = this->m_radius;;
    const qreal triHeight = 6;
    const qreal triWidth = 8;
    const qreal height = this->height() - triHeight;
    const qreal width = this->width();

    QRectF topRightRect(QPointF(0, 0),
                        QPointF(2 * radius, 2 * radius));
    QRectF bottomRightRect(QPointF(0, height - 2 * radius),
                           QPointF(2 * radius, height));
    QRectF topLeftRect(QPointF(width, 0),
                       QPointF(width - 2 * radius, 2 * radius));
    QRectF bottomLeftRect(QPointF(width, height),
                          QPointF(width - 2 * radius, height - 2 * radius));

//    qDebug()<<Q_FUNC_INFO<<" topRightRect "<<topRightRect
//           <<" bottomRightRect "<<bottomRightRect
//          <<" topLeftRect "<<topLeftRect
//         <<" bottomLeftRect "<<bottomLeftRect;

    path.moveTo(radius, 0.0);
    path.lineTo(width - radius, 0.0);
    path.arcTo(topLeftRect, 90.0, 90.0);
    path.lineTo(width, height - radius);
    path.arcTo(bottomLeftRect, 180.0, -90.0);
    path.lineTo(width / 2 + triWidth / 2, height);
    path.lineTo(width / 2, height + triHeight);
    path.lineTo(width / 2 - triWidth / 2, height);
    path.lineTo(radius, height);

    path.arcTo(bottomRightRect, 270.0, -90.0);
    path.lineTo(0.0, radius);
    path.arcTo(topRightRect, 180.0, -90.0);

    path.lineTo(radius, 0.0);

//    qDebug()<<Q_FUNC_INFO<<path;

    // FIXME: light: white
//    painter.fillPath(path, QColor(49, 49, 49));
    painter.fillPath(path, background);

    // FIXME: light: QColor(0, 0, 0, 51)
//    QPen pen(QColor(0, 0, 0, 0.1 * 255));
    QPen pen(borderColor);
    pen.setWidth(penWidthf);
    painter.strokePath(path, pen);
}

void VolumeWidget::deleyHide()
{
    m_mouseIn = false;
    m_timer->start();
}

void VolumeWidget::setRadius(int radius)
{
    m_radius = radius;
}

void VolumeWidget::setBackground(QBrush background)
{
    m_background = background;
}

void VolumeWidget::setBorderColor(QColor borderColor)
{
    m_borderColor = borderColor;
}
