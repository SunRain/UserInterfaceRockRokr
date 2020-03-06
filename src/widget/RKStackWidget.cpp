#include "RKStackWidget.h"

#include <QtGlobal>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QTransform>
#include <QVBoxLayout>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKStackedWidget::RKStackedWidget(QWidget *parent)
    : QStackedWidget (parent),
      d_ptr(new RKStackedWidgetPrivate(this))
{

}

RKStackedWidget::~RKStackedWidget()
{
}


void RKStackedWidget::setDuration(int duration)
{
    d_func()->m_animation->setDuration(duration);
}

int RKStackedWidget::duration() const
{
    return d_func()->m_animation->duration();
}

void RKStackedWidget::setFadeEnable(bool fade)
{
    d_func()->m_fade = fade;
}

bool RKStackedWidget::fadeEnable() const
{
    return d_func()->m_fade;
}

int RKStackedWidget::addWidget(QWidget *widget)
{
    if (!widget) {
        return -1;
    }
    widget->hide();
    return QStackedWidget::addWidget(widget);
}

int RKStackedWidget::previousIndex() const
{
    return d_func()->m_previousIndex;
}

int RKStackedWidget::currentIndex() const
{
    return d_func()->m_currentIndex;
}

QSize RKStackedWidget::sizeHint() const
{
    return QSize(100, 100);
}

void RKStackedWidget::paintEvent(QPaintEvent *event)
{
    Q_D(RKStackedWidget);
    if (d->m_animation->state() == QAbstractAnimation::State::Running) {
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


void RKStackedWidget::renderPreviousWidget(QPainter *painter, QTransform *transform)
{
    Q_D(RKStackedWidget);

    painter->save();
    switch (d->m_type) {
    case BottomToTop: {
        painter->translate(0, -(this->height() - qAbs(d->m_currentValue)));
        if (d->m_fade){
            painter->setOpacity(qAbs((qreal)d->m_currentValue)/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, d->m_privPixmap);
        break;
    }
    case TopToBottom: {
        painter->translate(0, d->m_currentValue);
        if (d->m_fade) {
            painter->setOpacity(1.0 - qAbs((qreal)d->m_currentValue)/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, d->m_privPixmap);
        break;
    }
    case LeftToRight:{
        painter->translate(d->m_currentValue, 0);
        if (d->m_fade) {
            painter->setOpacity(1.0 - (qreal)d->m_currentValue/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, d->m_privPixmap);
        break;
    }
    case RightToLeft: {
        painter->translate(qAbs(d->m_currentValue) - this->width(), 0);
        if (d->m_fade) {
            painter->setOpacity(qAbs((qreal)d->m_currentValue)/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, d->m_privPixmap);
        break;
    }
    case Center: {
        painter->drawPixmap(0, 0, d->m_privPixmap);
        break;
    }
    default:
        break;
    }
    painter->restore();
}

void RKStackedWidget::renderCurrentWidget(QPainter *painter, QTransform *transform)
{
    Q_D(RKStackedWidget);
    painter->save();
    switch (d->m_type) {
    case BottomToTop: {
        painter->translate(0, qAbs(d->m_currentValue));
        if (d->m_fade) {
            painter->setOpacity(1.0 - qAbs((qreal)d->m_currentValue)/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, d->m_currentPixmap);
        break;
    }
    case TopToBottom: {
        painter->translate(0, d->m_currentValue - this->height());
        if (d->m_fade) {
            painter->setOpacity(qAbs((qreal)d->currentValue())/(qreal)this->height());
        }
        painter->drawPixmap(0, 0, d->m_currentPixmap);
        break;
    }
    case LeftToRight:{
        painter->translate(d->m_currentValue - this->width(), 0);
        if (d->m_fade) {
            painter->setOpacity((qreal)d->m_currentValue/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, d->m_currentPixmap);
        break;
    }
    case RightToLeft: {
        painter->translate(qAbs(d->m_currentValue), 0);
        if (d->m_fade) {
            painter->setOpacity(1.0 - qAbs((qreal)d->m_currentValue)/(qreal)this->width());
        }
        painter->drawPixmap(0, 0, d->m_currentPixmap);
        break;
    }
    case Center: {
        const qreal ratio = d->m_currentValue / d->m_currentPixmap.height();
        const int width = (qreal)this->width() * ratio;
        const int height = (qreal)this->height() * ratio;
        const int x = (this->width() - width) /2;
        const int y = (this->height() - height) /2;
        painter->translate(x, y);
        if (d->m_fade) {
            painter->setOpacity(ratio);
        }
        QPixmap pixmap = d->m_currentPixmap.scaled(width, height);
        painter->drawPixmap(0, 0, pixmap);
    }
    default:
        break;
    }
    painter->restore();
}

void RKStackedWidget::setCurrentIndex(int index, RKStackedWidget::AnimationType type)
{
    Q_D(RKStackedWidget);
    if (index < 0 || index >= this->count()) { //invalid index
        return;
    }
    if (index == d->m_currentIndex) {
        return;
    }
    if (type == AnimationType::AnimationTypeNone) {
        d->m_previousIndex = d->m_currentIndex;
        d->m_currentIndex = index;
        if (d->m_animation->state() == QAbstractAnimation::Running) {
            stopAndResetAnimation();
        }
        auto w = widget(d->m_previousIndex);
        if (w) {
            w->hide();
        }
        w = widget(index);
        QStackedWidget::setCurrentWidget(w);
        w->show();
        return;
    }
    d->m_type = type;
    d->m_previousIndex = d->m_currentIndex;
    d->m_currentIndex = index;
    if (d->m_animation->state() == QAbstractAnimation::Running) {
        stopAndResetAnimation();

        auto w = widget(d->m_previousIndex);
        w->hide();

        w = widget(index);
        QStackedWidget::setCurrentWidget(w);
        w->show();
        return;
    }
    int animStart = 0;
    int animEnd = 0;
    switch (d->m_type) {
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
    case Center: {
        animStart = 0;
        animEnd = this->height();
    }
    default:
        break;
    }
    QWidget *w = this->widget(d->m_previousIndex);
    if (w) {
        d->m_privPixmap = w->grab(w->rect());
    } else {
        d->m_privPixmap = QPixmap();
    }

    w = this->widget(d->m_currentIndex);
    Q_ASSERT(w);
    QStackedWidget::setCurrentWidget(w);
    w->hide();
    d->m_currentPixmap = w->grab(w->rect());

    // use half size to make visual sense better
//    if (d->m_type == AnimationType::Center) {
//        animStart = d->m_currentPixmap.height() / 2;
//    }

    d->m_animation->setStartValue(animStart);
    d->m_animation->setEndValue(animEnd);
    d->m_animation->start();
}

void RKStackedWidget::setCurrentWidget(QWidget *w, RKStackedWidget::AnimationType type)
{
    if (!w) {
        return;
    }
    const int idx = this->indexOf(w);
    this->setCurrentIndex(idx, type);
}


void RKStackedWidget::stopAndResetAnimation()
{
    Q_D(RKStackedWidget);
    d->m_animation->stop();
    d->m_currentValue = 0;
}

RKStackedWidgetPrivate::RKStackedWidgetPrivate(RKStackedWidget *parent)
    : q_ptr(parent)
{
    m_animation = new QPropertyAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setPropertyName("currentValue");
    m_animation->setDuration(100);
    m_animation->setEasingCurve(m_curve);
    m_animation->setStartValue(0);
    m_animation->setEndValue(0);

    connect(m_animation, &QPropertyAnimation::finished,
            this, &RKStackedWidgetPrivate::animationFinished);

    connect(m_animation, &QPropertyAnimation::valueChanged,
            this, [&](const QVariant &value){
        Q_UNUSED(value);
        Q_Q(RKStackedWidget);
        q->update();
    });
}

RKStackedWidgetPrivate::~RKStackedWidgetPrivate()
{
    if (m_animation->state() == QPropertyAnimation::State::Running) {
        m_animation->stop();
        m_animation->deleteLater();
    }
}

void RKStackedWidgetPrivate::animationFinished()
{
    Q_Q(RKStackedWidget);
    q->widget(m_currentIndex)->show();
    m_currentValue = 0;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


