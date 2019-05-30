#include "RKStackWidget.h"

#include <cmath>
#include <QPixmap>
#include <QPainter>
#include <QTransform>

RKStackedWidget::RKStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    m_revert = false;
    m_curve = QEasingCurve::Linear;
    m_isAnimating = false;
    m_currentValue = 0;
    m_currentIndex = 0;
    m_previousIndex = 0;
    m_type = FadeExchange;

    m_fade = false;
    m_animat = false;

    m_animation = new QPropertyAnimation(this, "value", this);
    m_animation->setPropertyName("value");
    m_animation->setDuration(200);
    m_animation->setEasingCurve(m_curve);
    m_animation->setStartValue(0);
    m_animation->setEndValue(0);

    connect(m_animation, &QPropertyAnimation::finished, this, &RKStackedWidget::animationFinished);

    setLength(500, m_type);
}

RKStackedWidget::~RKStackedWidget()
{
    delete m_animation;
}

void RKStackedWidget::paintEvent(QPaintEvent * event)
{
    if(m_isAnimating)
    {
        QPainter painter(this);
        QTransform transform;

        renderPreviousWidget(&painter, &transform);
        renderCurrentWidget(&painter, &transform);
    }
    else
    {
        QWidget::paintEvent(event);
    }
}

void RKStackedWidget::renderPreviousWidget(QPainter *painter, QTransform *transform)
{
    painter->save();
    switch(m_type)
    {
    case BottomToTop :
    {
        painter->translate(0, m_currentValue);
        painter->drawPixmap(0, -height() / 2, m_privPixmap);
        break;
    }
    case TopToBottom :
    {
        painter->translate(0, m_currentValue);
        painter->drawPixmap(0, height() / 2, m_privPixmap);
        break;
    }
    case LeftToRight :
    {
        if(m_previousIndex > m_currentIndex && m_revert)
        {
            transform->translate(0 - m_currentValue, 0);
            painter->setTransform(*transform);

            if(m_fade)
            {
                painter->setOpacity(1 - (m_currentValue - m_startValue) / m_rangeValue);
            }

            painter->drawPixmap(-width() / 2, 0, m_privPixmap);
        }
        else
        {
            painter->translate(m_currentValue, 0);
            if(m_fade)
            {
                painter->setOpacity(1 - (m_currentValue - m_startValue) / m_rangeValue);
            }

            painter->drawPixmap(width() / 2, 0, m_privPixmap);
        }

        break;
    }
    case RightToLeft :
    {
        if(m_previousIndex > m_currentIndex && m_revert)
        {
            painter->translate(-m_currentValue, 0);
            if(m_fade)
            {
                painter->setOpacity((m_endValue - m_currentValue) / m_rangeValue);
            }

            painter->drawPixmap(width() / 2, 0, m_privPixmap);
        }
        else
        {
            painter->translate(m_currentValue, 0);
            if(m_fade)
            {
                painter->setOpacity(1 - (m_currentValue - m_startValue) / m_rangeValue);
            }

            painter->drawPixmap(-width() / 2, 0, m_privPixmap);
        }

        break;
    }
    case RollInOut:
    {
        if(m_fade)
        {
            painter->setOpacity((float)(m_currentValue - m_endValue) / (float)std::abs(m_rangeValue));
        }

        painter->drawPixmap(0 , m_startValue - m_currentValue, m_privPixmap);
        break;
    }
    case FadeInOut:
    {
        const float opt = (float)(m_currentValue - m_endValue) / (float)std::abs(m_rangeValue) / 2.5;
        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0 , 0, m_privPixmap);
        break;
    }
    case FadeExchange:
    {
        const float opt = (float)(m_currentValue - m_endValue) / (float)std::abs(m_rangeValue);
        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0 , 0, m_privPixmap);
        break;
    }
    case BlackInOut:
    {
        float opt = (m_currentValue - (float)std::abs(m_rangeValue) / 2.0) / (float)((float)std::abs(m_rangeValue) / 2.0);
        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0 , 0, m_privPixmap);

        opt = opt < 0 ? 0 : (float)(1.0 - opt);

        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0 , 0, m_privPixmap);
        break;
    }
    case CoverInOutRight:
    {
        if(m_fade)
        {
            painter->setOpacity(1 - (m_currentValue - m_startValue) / m_rangeValue);
        }

        painter->drawPixmap(0 , 0, m_privPixmap);
        break;
    }
    case CoverInOutLeft:
    {
        if(m_fade)
        {
            painter->setOpacity((m_currentValue - m_startValue) / m_rangeValue);
        }

        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case VerticalFlipRotate:
    {
        const float degree = ((m_currentValue - m_startValue) / m_rangeValue) * 180;
        if(degree <= 90)
        {
            if(m_fade)
            {
                painter->setOpacity((90 - degree) / 90);
            }

            painter->setTransform(QTransform().translate(width() / 2, height() / 2).rotate(degree, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
            painter->drawPixmap(0, 0, m_privPixmap);
        }
        break;
    }
    case VerticalFlipRotateOut:
    {
        break;
    }
    case VerticalCubeRotateT2B:
    {
        const float percent = (m_currentValue - m_startValue) / m_rangeValue;
        const float degree = percent * 90;
        const float pos = percent * height();

        painter->setTransform(QTransform().translate(0, pos/ 2).translate(width() / 2, height() / 2).rotate(-degree, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
        painter->drawPixmap(0, 0, m_privPixmap);
        break;
    }
    case VerticalCubeRotateB2T:
    {
        const float percent = (m_currentValue - m_startValue) / m_rangeValue;
        const float degree = percent * 90;
        const float pos = percent * height();

        painter->setTransform(QTransform().translate(0, -pos/ 2).translate(width() / 2, height() / 2).rotate(degree, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
        painter->drawPixmap(0, 0, m_privPixmap);
        break;
    }
    case HorizontalFlipRotate:
    {
        const float degree = ((m_currentValue - m_startValue) / m_rangeValue) * 180;
        if(degree <= qAbs(90))
        {
            if(m_fade)
            {
                painter->setOpacity((90 - degree) / 90);
            }

            painter->setTransform(QTransform().translate(width() / 2, height() / 2).rotate(degree, Qt::YAxis).translate(-width() / 2, -height() / 2), false);
            painter->drawPixmap(0, 0, m_privPixmap);
        }
        break;
    }
    default: break;
    }

    painter->restore();
}

void RKStackedWidget::renderCurrentWidget(QPainter *painter, QTransform *transform)
{
    painter->save();
    switch(m_type)
    {
    case BottomToTop :
    {
        transform->translate(0, m_currentValue);
        painter->setTransform(*transform);
        painter->drawPixmap(0, height() / 2, m_currentPixmap);
        break;
    }
    case TopToBottom :
    {
        transform->translate(0, m_currentValue);
        painter->setTransform(*transform);
        painter->drawPixmap(0, -height() / 2, m_currentPixmap);
        break;
    }
    case LeftToRight :
    {
        if(m_previousIndex > m_currentIndex && m_revert)
        {
            painter->translate(-m_currentValue, 0);

            if(m_fade)
            {
                painter->setOpacity(1 - (m_endValue - m_currentValue) / m_rangeValue);
            }

            painter->drawPixmap(width() / 2, 0, m_currentPixmap);
        }
        else
        {
            transform->translate(m_currentValue, 0);
            painter->setTransform(*transform);

            if(m_fade)
            {
                painter->setOpacity((m_currentValue - m_startValue) / m_rangeValue);
            }

            painter->drawPixmap(-width() / 2, 0, m_currentPixmap);
        }
        break;
    }
    case RightToLeft :
    {
        if(m_previousIndex > m_currentIndex && m_revert)
        {
            transform->translate(0 - m_currentValue, 0);
            painter->setTransform(*transform);

            if(m_fade)
            {
                painter->setOpacity((m_currentValue - m_startValue) / m_rangeValue);
            }

            painter->drawPixmap(-width() / 2, 0, m_currentPixmap);
        }
        else
        {
            painter->translate(m_currentValue, 0);

            if(m_fade)
            {
                painter->setOpacity((m_currentValue - m_startValue) / m_rangeValue);
            }

            painter->drawPixmap(width() / 2, 0, m_currentPixmap);
        }
        break;
    }
    case RollInOut:
    {
        painter->translate(m_currentValue, 0);

        if(m_fade)
        {
            painter->setOpacity((m_startValue - m_currentValue) / (float)std::abs(m_rangeValue));
        }

        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case FadeInOut:
    {
        const float opt = 1.0 - (float)(m_currentValue - m_endValue) / (float)std::abs(m_rangeValue);

        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case FadeExchange:
    {
        const float opt = 1.0 - (float)(m_currentValue - m_endValue) / (float)std::abs(m_rangeValue);

        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case BlackInOut:
    {
        float opt = ((float)std::abs(m_rangeValue) / 2.0 - m_currentValue) / ((float)std::abs(m_rangeValue) / 2.0);

        if(m_fade)
        {
            painter->setOpacity(opt);
        }
        painter->drawPixmap(0, 0, m_currentPixmap);

        opt = opt > 0 ? (float)(1.0 - opt) : 0;

        if(m_fade)
        {
            painter->setOpacity(opt);
        }

        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case CoverInOutRight:
    {
        painter->translate(m_currentValue, 0);

        if(m_fade)
        {
            painter->setOpacity((m_currentValue - m_startValue) / m_rangeValue);
        }

        painter->drawPixmap(width() / 2, 0, m_currentPixmap);
        break;
    }
    case CoverInOutLeft:
    {
        painter->translate(m_currentValue, 0);
        if(m_fade)
        {
            painter->setOpacity(1 - (m_currentValue - m_startValue) / m_rangeValue);
        }

        painter->drawPixmap(width() / 2, 0, m_privPixmap);
        break;
    }
    case VerticalFlipRotate:
    {
        const float degree = ((m_currentValue - m_startValue) / m_rangeValue) * 180;
        if(degree > 90)
        {
            if(m_fade)
            {
                painter->setOpacity((degree - 90) / 90);
            }

            painter->setTransform(QTransform().translate(width() / 2, height() / 2).rotate(180 - degree, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
            painter->drawPixmap(0, 0, m_currentPixmap);
        }
        break;
    }
    case VerticalFlipRotateOut:
    {
        const float degree = ((m_currentValue - m_startValue) / m_rangeValue) * 90;
        if(m_fade)
        {
            painter->setOpacity(degree / 90);
        }

        painter->setTransform(QTransform().translate(width() / 2, height() / 2).rotate(90 - degree, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case VerticalCubeRotateT2B:
    {
        const float percent = (m_currentValue - m_startValue) / m_rangeValue;
        const float degree = percent * 90;
        const float pos = (1 - percent) * height();

        painter->setTransform(QTransform().translate(0, -pos / 2 ).translate(width() / 2, height() / 2).rotate(90 - degree, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case VerticalCubeRotateB2T:
    {
        const float percent = (m_currentValue - m_startValue) / m_rangeValue;
        const float degree = percent * 90;
        const float pos = (1 - percent) * height();

        painter->setTransform(QTransform().translate(0, pos / 2 ).translate(width() / 2, height() / 2).rotate(degree - 90, Qt::XAxis).translate(-width() / 2, -height() / 2), false);
        painter->drawPixmap(0, 0, m_currentPixmap);
        break;
    }
    case HorizontalFlipRotate:
    {
        const float degree = ((m_currentValue - m_startValue) / m_rangeValue) * 180;
        if(degree > qAbs(90))
        {
            if(m_fade)
            {
                painter->setOpacity((degree - 90) / 90);
            }

            painter->setTransform(QTransform().translate(width() / 2, height() / 2).rotate(degree - 180, Qt::YAxis).translate(-width() / 2, -height() / 2), false);
            painter->drawPixmap(0, 0, m_currentPixmap);
        }
        break;
    }
    default: break;
    }

    painter->restore();
}

void RKStackedWidget::start(int index)
{
    if(index >= count())
    {
        return;
    }

    m_previousIndex = currentIndex();

    m_currentIndex = index;

    if(m_isAnimating)
    {
        disconnect(m_animation, &QPropertyAnimation::finished, this, &RKStackedWidget::animationFinished);
        m_animation->stop();
        animationFinished();
        connect(m_animation, &QPropertyAnimation::finished, this, &RKStackedWidget::animationFinished);
    }

    switch(m_type)
    {
    case LeftToRight:
    case RightToLeft:
    case SlideInOut:
    case CoverInOutLeft:
    case CoverInOutRight:
    {
        m_startValue = width() / 2;
        break;
    }
    case TopToBottom:
    case BottomToTop:
    {
        m_startValue = height() / 2;
        break;
    }
    case VerticalCubeRotateT2B:
    {
        m_startValue = 0;
        m_endValue = height();
        break;
    }
    case VerticalCubeRotateB2T:
    {
        m_startValue = 0;
        m_endValue = height();
        break;
    }
    default:
    {
        break;
    }
    }

    if(m_type == RightToLeft || m_type == LeftToRight)
    {
        setLength(std::abs(m_startValue * 2), m_type);
    }
    else if(m_type == CoverInOutLeft || m_type == CoverInOutRight)
    {
        if(m_previousIndex < m_currentIndex)
        {
            setLength(std::abs(m_startValue * 2), CoverInOutRight);
        }
        else if(m_previousIndex > m_currentIndex)
        {
            setLength(std::abs(m_startValue * 2), CoverInOutLeft);
        }
    }
    else if(m_type == VerticalCubeRotateT2B || m_type == VerticalCubeRotateB2T)
    {
        if(m_previousIndex < m_currentIndex)
        {
            setLength(std::abs(m_endValue), m_revert ? VerticalCubeRotateT2B : m_type);
        }
        else if(m_previousIndex > m_currentIndex)
        {
            setLength(std::abs(m_endValue), m_revert ? VerticalCubeRotateB2T : m_type);
        }
    }
    else if(m_type == TopToBottom || m_type == BottomToTop)
    {
        if(m_previousIndex < m_currentIndex)
        {
            setLength(std::abs(m_startValue * 2), m_revert ? TopToBottom : BottomToTop);
        }
        else if(m_previousIndex > m_currentIndex)
        {
            setLength(std::abs(m_endValue * 2), m_revert ? BottomToTop : TopToBottom);
        }
    }
    else if(m_type == HorizontalFlipRotate)
    {
        if(m_previousIndex < m_currentIndex)
        {
            setLength(m_revert ? -std::abs(m_startValue) : std::abs(m_startValue), HorizontalFlipRotate);
        }
        else if(m_previousIndex > m_currentIndex)
        {
            setLength(m_revert ? std::abs(m_startValue) : -std::abs(m_startValue), HorizontalFlipRotate);
        }
    }

    const int offsetX = frameRect().width();
    const int offsetY = frameRect().height();
    widget(m_currentIndex)->setGeometry(0, 0, offsetX, offsetY);

    currentWidget()->hide();
    m_isAnimating = true;

    QWidget *w = widget(m_currentIndex);
    w->hide();
    QStackedWidget::setCurrentWidget(w);
    w->hide();

    QWidget *previousWidget = widget(m_previousIndex);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_privPixmap = previousWidget->grab(previousWidget->rect());
#else
    m_privPixmap = QPixmap::grabWidget(previousWidget);
#endif

    previousWidget = widget(m_currentIndex);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_privPixmap = previousWidget->grab(previousWidget->rect());
#else
    m_privPixmap = QPixmap::grabWidget(previousWidget);
#endif

    m_animation->start();
}

void RKStackedWidget::setLength(int length, AnimationType type)
{
    switch(m_type = type)
    {
    case BottomToTop :
    {
        m_animation->setStartValue(length / 2);
        m_animation->setEndValue(-length / 2);
        break;
    }
    case LeftToRight :
    {
        m_animation->setStartValue(-length / 2);
        m_animation->setEndValue(length / 2);
        break;
    }
    case TopToBottom :
    {
        m_animation->setStartValue(-length / 2);
        m_animation->setEndValue(length / 2);
        break;
    }
    case RightToLeft :
    {
        m_animation->setStartValue(length / 2);
        m_animation->setEndValue(-length / 2);
        break;
    }
    case RollInOut:
    {
        m_animation->setStartValue(length);
        m_animation->setEndValue(0);
        break;
    }
    case FadeInOut:
    case BlackInOut:
    case FadeExchange:
    {
        m_animation->setStartValue(length);
        m_animation->setEndValue(0);
        break;
    }
    case SlideInOut:
    {
        m_animation->setStartValue(-length / 2);
        m_animation->setEndValue(length / 2);
        break;
    }
    case CoverInOutRight:
    {
        m_animation->setStartValue(length / 2);
        m_animation->setEndValue(-length / 2);
        break;
    }
    case CoverInOutLeft:
    {
        m_animation->setStartValue(-length / 2);
        m_animation->setEndValue(length / 2);
        break;
    }
    case VerticalFlipRotate:
    {
        m_animation->setStartValue(length);
        m_animation->setEndValue(0);
        break;
    }
    case VerticalFlipRotateOut:
    {
        m_animation->setStartValue(0);
        m_animation->setEndValue(length);
        break;
    }
    case VerticalCubeRotateT2B:
    {
        m_animation->setStartValue(0);
        m_animation->setEndValue(length);
        break;
    }
    case VerticalCubeRotateB2T:
    {
        m_animation->setStartValue(0);
        m_animation->setEndValue(length);
        break;
    }
    case HorizontalFlipRotate:
    {
        m_animation->setStartValue(length);
        m_animation->setEndValue(0);
        break;
    }

    default: break;
    }

    m_rangeValue = m_animation->endValue().toFloat() - m_animation->startValue().toFloat();
    m_startValue = m_animation->startValue().toFloat();
    m_endValue = m_animation->endValue().toFloat();
}

void RKStackedWidget::setDuration(int duration)
{
    m_animation->setDuration(duration);
}

int RKStackedWidget::getDuration() const
{
    return m_animation->duration();
}

void RKStackedWidget::setCurve(QEasingCurve::Type curve)
{
    m_curve = curve;
    m_animation->setEasingCurve(m_curve);
}

void RKStackedWidget::setRevert(bool revert)
{
    m_revert = revert;
}

void RKStackedWidget::setFadeEnable(bool enable)
{
    m_fade = enable;
}

void RKStackedWidget::setAnimatEnable(bool animat)
{
    m_animat = animat;
}

bool RKStackedWidget::isAnimating()
{
    return (m_animation->state() == QAbstractAnimation::Running);
}

void RKStackedWidget::addWidget(QWidget *widget)
{
    widget->setAttribute(Qt::WA_TranslucentBackground);
    QStackedWidget::addWidget(widget);
}

void RKStackedWidget::setCurrentIndex(int index)
{
    if(m_animat && isVisible() && index != m_currentIndex)
    {
        start(index);
    }
    else
    {
        QStackedWidget::setCurrentIndex(index);
    }
}

float RKStackedWidget::GetValue() const
{
    return m_currentValue;
}

void RKStackedWidget::SetValue(const float value)
{
    m_currentValue = value;
    update();
}

void RKStackedWidget::animationFinished()
{
    m_currentValue = 0;
    m_isAnimating = false;

    QWidget *w = widget(m_currentIndex);
    if(w != nullptr)
    {
        w->show();
        w->raise();
        QStackedWidget::setCurrentWidget( w );
    }

    update();
    emit pageChanged(m_currentIndex);
}
