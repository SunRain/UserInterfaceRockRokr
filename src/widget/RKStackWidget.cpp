#include "RKStackWidget.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QTransform>
#include <QVBoxLayout>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKStackedWidget::RKStackedWidget(QWidget *parent)
    : QStackedWidget (parent)
{
    m_curve = QEasingCurve::Linear;
    m_currentValue = 0;
    m_currentIndex = -1;
    m_previousIndex = -1;
    m_fade = true;
    m_type = AnimationType::LeftToRight;

    m_animation = new QPropertyAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setPropertyName("currentValue");
    m_animation->setDuration(100);
    m_animation->setEasingCurve(m_curve);
    m_animation->setStartValue(0);
    m_animation->setEndValue(0);

    connect(m_animation, &QPropertyAnimation::finished,
            this, &RKStackedWidget::animationFinished);

    connect(m_animation, &QPropertyAnimation::valueChanged,
            this, [&](const QVariant &value){
                this->update();
            });
}

RKStackedWidget::~RKStackedWidget()
{
    if (m_animation->state() == QPropertyAnimation::State::Running) {
        m_animation->stop();
        m_animation->deleteLater();
    }
}

qreal RKStackedWidget::currentValue() const
{
    return m_currentValue;
}

void RKStackedWidget::setDuration(int duration)
{
    m_animation->setDuration(duration);
}

int RKStackedWidget::duration() const
{
    return m_animation->duration();
}

void RKStackedWidget::setFadeEnable(bool fade)
{
    m_fade = fade;
}

bool RKStackedWidget::fadeEnable() const
{
    return m_fade;
}

void RKStackedWidget::addWidget(QWidget *widget)
{
    if (!widget) {
        return;
    }
    widget->hide();
    QStackedWidget::addWidget(widget);
}

void RKStackedWidget::paintEvent(QPaintEvent *event)
{
    if (m_animation->state() == QAbstractAnimation::State::Running) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::HighQualityAntialiasing |
                               QPainter::SmoothPixmapTransform);
        QTransform transform;
        renderPreviousWidget(&painter, &transform);
        renderCurrentWidget(&painter, &transform);
    } else {
        QStackedWidget::paintEvent(event);
    }
}

void RKStackedWidget::animationFinished()
{
    this->widget(m_currentIndex)->show();
    m_currentValue = 0;
}

void RKStackedWidget::renderPreviousWidget(QPainter *painter, QTransform *transform)
{
    painter->save();
    switch (m_type) {
    case BottomToTop: {
        painter->translate(0, -(this->height() - qAbs(m_currentValue)));
        if (m_fade){
            painter->setOpacity(qAbs((qreal)m_currentValue)/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, m_privPixmap);
        break;
    }
    case TopToBottom: {
        painter->translate(0, m_currentValue);
        if (m_fade) {
            painter->setOpacity(1.0 - qAbs((qreal)m_currentValue)/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, m_privPixmap);
        break;
    }
    case LeftToRight:{
        painter->translate(m_currentValue, 0);
        if (m_fade) {
            painter->setOpacity(1.0 - (qreal)m_currentValue/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, m_privPixmap);
        break;
    }
    case RightToLeft: {
        painter->translate(qAbs(m_currentValue) - this->width(), 0);
        if (m_fade) {
            painter->setOpacity(qAbs((qreal)m_currentValue)/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, m_privPixmap);
        break;
    }
    default:
        break;
    }
    painter->restore();
}

void RKStackedWidget::renderCurrentWidget(QPainter *painter, QTransform *transform)
{
    painter->save();
    switch (m_type) {
    case BottomToTop: {
        painter->translate(0, qAbs(m_currentValue));
        if (m_fade) {
            painter->setOpacity(1.0 - qAbs((qreal)m_currentValue)/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case TopToBottom: {
        painter->translate(0, m_currentValue - this->height());
        if (m_fade) {
            painter->setOpacity(qAbs((qreal)this->currentValue())/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case LeftToRight:{
        painter->translate(m_currentValue - this->width(), 0);
        if (m_fade) {
            painter->setOpacity((qreal)m_currentValue/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case RightToLeft: {
        painter->translate(qAbs(m_currentValue), 0);
        if (m_fade) {
            painter->setOpacity(1.0 - qAbs((qreal)m_currentValue)/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    default:
        break;
    }
    painter->restore();
}

void RKStackedWidget::setCurrentIndex(int index, RKStackedWidget::AnimationType type)
{
    if (index < 0 || index >= this->count()) { //invalid index
        return;
    }
    if (index == m_currentIndex) {
        return;
    }
    if (type == AnimationType::AnimationTypeNone) {
        m_previousIndex = m_currentIndex;
        m_currentIndex = index;
        if (m_animation->state() == QAbstractAnimation::Running) {
            stopAndResetAnimation();
        }
        auto w = widget(m_previousIndex);
        if (w) {
            w->hide();
        }
        w = widget(index);
        QStackedWidget::setCurrentWidget(w);
        w->show();
        return;
    }
    m_type = type;
    m_previousIndex = m_currentIndex;
    m_currentIndex = index;
    if (m_animation->state() == QAbstractAnimation::Running) {
        stopAndResetAnimation();

        auto w = widget(m_previousIndex);
        w->hide();

        w = widget(index);
        QStackedWidget::setCurrentWidget(w);
        w->show();
        return;
    }
    int animStart = 0;
    int animEnd = 0;
    switch (m_type) {
    case LeftToRight: {
        animStart = 0;
        animEnd = this->width();
        break;
    }
    case RightToLeft: {
        animStart = -this->width();
        animEnd = 0;
        break;
    }
    case TopToBottom: {
        animStart = 0;
        animEnd = this->height();
        break;
    }
    case BottomToTop: {
        animStart = -this->height();
        animEnd = 0;
        break;
    }
    default:
        break;
    }
    QWidget *w = this->widget(m_previousIndex);
    if (w) {
        m_privPixmap = w->grab(w->rect());
    } else {
        m_privPixmap = QPixmap();
    }

    w = this->widget(m_currentIndex);
    Q_ASSERT(w);
    QStackedWidget::setCurrentWidget(w);
    w->hide();
    m_currentPixmap = w->grab(w->rect());

    m_animation->setStartValue(animStart);
    m_animation->setEndValue(animEnd);
    m_animation->start();
}

void RKStackedWidget::setCurrentWidget(QWidget *w, RKStackedWidget::AnimationType type)
{
    if (!w) {
        return;
    }
    const int idx = this->indexOf(w);
    this->setCurrentIndex(idx, type);
}

void RKStackedWidget::setCurrentValue(int currentValue)
{
    m_currentValue = currentValue;
}

void RKStackedWidget::stopAndResetAnimation()
{
    m_animation->stop();
    m_currentValue = 0;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
