#include "RKSlider.h"

#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>
#include <QLabel>

#include <DThemeManager>


DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKBaseSlider::RKBaseSlider(QWidget *parent)
    : RKBaseSlider(Qt::Horizontal, parent)
{

}

RKBaseSlider::RKBaseSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
    this->setObjectName("RKBaseSlider");
    DThemeManager::instance()->registerWidget(this);

    setCursor(QCursor(Qt::PointingHandCursor));
    m_mousePress = false;
    m_value = 0;
}

bool RKBaseSlider::event(QEvent *event)
{
    if(m_mousePress) {
        setValue(m_value);
    }
    return QSlider::event(event);
}

void RKBaseSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    if(event->button() == Qt::LeftButton) {
        m_mousePress = true;

        double pos;
        if(orientation() == Qt::Horizontal) {
            pos = event->pos().x()*1.0 / width();
            m_value = pos * (maximum() - minimum()) + minimum();
        } else {
            pos = event->pos().y()*1.0 / height();
            m_value = maximum() - pos * (maximum() - minimum());
        }
    }
}

void RKBaseSlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    if(m_mousePress) {
        if(orientation() == Qt::Horizontal) {
            int x = event->pos().x();
            if((x >= 0) && (x <= width())) {
                double pos = event->pos().x()*1.0 / width();
                m_value = pos * (maximum() - minimum()) + minimum();
                setValue(m_value);
            } else if(x < 0) {
                m_value = minimum();
            } else {
                m_value = maximum();
            }
        } else {
            int y = event->pos().y();
            if((y >= 0) && (y <= height())) {
                double pos = event->pos().y()*1.0 / height();
                m_value = maximum() - pos * (maximum() - minimum());
                setValue(m_value);
            } else if(y < 0) {
                m_value = maximum();
            } else {
                m_value = minimum();
            }
        }
    }
}

void RKBaseSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    m_mousePress = false;
}


RKSlider::RKSlider(QWidget *parent)
    : RKSlider(Qt::Horizontal, parent)
{

}

RKSlider::RKSlider(Qt::Orientation orientation, QWidget *parent)
    : RKBaseSlider(orientation, parent)
{
    this->setObjectName("RKSlider");
    DThemeManager::instance()->registerWidget(this);

    this->setMouseTracking(true);

    m_isMoving = false;
    m_textLabel = new QLabel(this);
    m_textLabel->setObjectName("RKTextLabel");
    m_textLabel->setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
    m_textLabel->setGeometry(0, 0, 40, 20);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setFocusPolicy(Qt::NoFocus);
//    m_textLabel->setStyleSheet("QLabel{ color:rgba(0, 0, 0, 0.5); background-color:#FCFCFC; border:1px solid #F3F3F3;}");
}

RKSlider::~RKSlider()
{
    m_textLabel->deleteLater();
    m_textLabel = Q_NULLPTR;
}

void RKSlider::setValue(int value)
{
    if(!m_isMoving) {
        RKBaseSlider::setValue(value);
    }
}

void RKSlider::mousePressEvent(QMouseEvent *event)
{
    RKBaseSlider::mousePressEvent(event);
    m_isMoving = false;
    emit sliderMoved( m_value );
    m_textLabel->show();
    m_textLabel->raise();
}

void RKSlider::mouseMoveEvent(QMouseEvent *event)
{
    RKBaseSlider::mouseMoveEvent(event);
    if(m_mousePress) {
        m_isMoving = true;
        emit sliderMoved(m_value);
    }

    const QPoint &curPos = mapFromGlobal(QCursor::pos());
    const QPoint &glbPos = mapToGlobal(QPoint(0, 0));
    const QSize &sizePos = size();
    QPoint changePos;

    if(orientation() == Qt::Vertical) {
        changePos = limitLableGeometry(curPos.y(), glbPos.y(), sizePos.height());
        m_textLabel->move((glbPos + QPoint(sizePos.width(), 0)).x(), changePos.x());
    } else {
        changePos = limitLableGeometry(curPos.x(), glbPos.x(), sizePos.width());
        m_textLabel->move(changePos.x(), (glbPos - QPoint(0, m_textLabel->height())).y());
    }
//    qDebug()<<Q_FUNC_INFO<<"move text label to "<<QPoint(changePos.x(), (glbPos - QPoint(0, m_textLabel->height())).y());
//    m_textLabel->setText(QDateTime::fromMSecsSinceEpoch(changePos.y()).toString("mm:ss"));
    m_textLabel->setText(QString("%1").arg(changePos.y()));
}

void RKSlider::mouseReleaseEvent(QMouseEvent *event)
{
    RKBaseSlider::mouseReleaseEvent(event);
    emit sliderReleasedAt(m_value);
    m_isMoving = false;
}

void RKSlider::enterEvent(QEvent *event)
{
    RKBaseSlider::enterEvent(event);
    m_textLabel->show();
}

void RKSlider::leaveEvent(QEvent *event)
{
    RKBaseSlider::leaveEvent(event);
    m_textLabel->hide();
}

QPoint RKSlider::limitLableGeometry(int x, int y, int z)
{
    QPoint pt;
    if( 0 < x && x < z) {
        pt.setX(y + x);
        pt.setY(qint64(x)*maximum()/z);
    }
    if(x <= 0) {
        pt.setX(y);
        pt.setY(0);
    }
    if(x >= z) {
        pt.setX(y + z);
        pt.setY(maximum());
    }
    return pt;
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
