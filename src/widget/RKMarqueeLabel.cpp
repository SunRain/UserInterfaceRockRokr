#include "RKMarqueeLabel.h"

#include <QTimer>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QVBoxLayout>

#include "rockrokr_global.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static int LABEL_INNER_MARGIN = 6;

RKMarqueeLabel::RKMarqueeLabel(QWidget *parent)
    : QWidget(parent)
{
    m_labText = new QLabel(this);
    m_labText->setAlignment(Qt::AlignVCenter);
    m_labText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_timer = new QTimer(this);
    m_timer->setInterval(m_interval);
    connect(m_timer, &QTimer::timeout, this, &RKMarqueeLabel::timeout);

    setForeground(QColor(100, 184, 255));
    setBackground(QColor(222, 222, 222));
}

RKMarqueeLabel::~RKMarqueeLabel()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void RKMarqueeLabel::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        m_labText->setText(text);

        QFont f(m_labText->font());
        f.setPixelSize(this->height() - _to_font_px(LABEL_INNER_MARGIN));
        m_labText->setFont(f);

        const int textWidth = m_labText->fontMetrics().horizontalAdvance(text);
        const int textHeight = m_labText->fontMetrics().height();

        m_labText->setFixedSize(textWidth, textHeight);
        m_initY = (this->height() - m_labText->height()) /2;

        if (m_moveStyle == MoveStyleLeftAndRight) {
            m_initX = 0;
        } else if (m_moveStyle == MoveStyleLeftToRight) {
            m_initX = -m_labText->width();
        } else if (m_moveStyle == MoveStyleRightToLeft) {
            m_initX = width();
        }
    }
}

void RKMarqueeLabel::setStep(int step)
{
    if (m_step != step) {
        m_step = step;
    }
}

void RKMarqueeLabel::setInterval(int interval)
{
    if (m_interval != interval) {
        m_interval = interval;
        m_timer->setInterval(interval);
    }
}

void RKMarqueeLabel::setForeground(const QColor &foreground)
{
    if (m_foreground != foreground) {
        m_foreground = foreground;

        qDebug()<<" set to  "<<foreground;

        QPalette pt = m_labText->palette();
        pt.setColor(QPalette::WindowText, foreground);
        m_labText->setPalette(pt);
    }
}

void RKMarqueeLabel::setBackground(const QColor &background)
{
    if (m_background != background) {
        m_background = background;
        QPalette pt = palette();
        pt.setColor(QPalette::Window, background);
        setPalette(pt);
    }
}

void RKMarqueeLabel::setMoveStyle(RKMarqueeLabel::MoveStyle moveStyle)
{
    if (m_moveStyle != moveStyle) {
       m_moveStyle = moveStyle;
    }
}

void RKMarqueeLabel::setMarqueeStyle(RKMarqueeLabel::MarqueeStyle style)
{
    m_marqueeStyle = style;
}

QSize RKMarqueeLabel::sizeHint() const
{
    return QSize(200, 30);
}

void RKMarqueeLabel::timeout()
{
    int textWidth = m_labText->fontMetrics().horizontalAdvance(m_text);
    if (textWidth <= this->width()) {
        return;
    }

    if (m_moveStyle == MoveStyleLeftAndRight) {
        bool moveRight = false;
        if (textWidth <= width()) {
            if ((m_initX + textWidth) > width()) {
                moveRight = false;
            } else if (m_initX <= 0) {
                moveRight = true;
            }
        } else {
            if (m_initX == 10) {
                moveRight = false;
            } else if(m_initX <= width() - textWidth - 10) {
                moveRight = true;
            }
        }

        if (moveRight) {
            m_initX += m_step;
        } else {
            m_initX -= m_step;
        }

        m_labText->move(m_initX, m_initY);
    } else if (m_moveStyle == MoveStyleLeftToRight) {
        if (m_initX > width()) {
            m_initX = -textWidth;
        }

        m_initX += m_step;
        m_labText->move(m_initX, m_initY);
    } else if (m_moveStyle == MoveStyleRightToLeft) {
        if (m_initX < -textWidth) {
            m_initX = width();
        }
        m_initX -= m_step;
        m_labText->move(m_initX, m_initY);
    }
}

void RKMarqueeLabel::setMarqueeStyleText()
{
    QFont font = m_labText->font();
//    font.setUnderline(true);
    m_labText->setFont(font);
    setCursor(Qt::PointingHandCursor);
}

void RKMarqueeLabel::setStopStyleText()
{
    QFont font = m_labText->font();
//    font.setUnderline(true);
    m_labText->setFont(font);
    setCursor(Qt::PointingHandCursor);
}

void RKMarqueeLabel::resizeEvent(QResizeEvent *event)
{
    QFont f(m_labText->font());
    f.setPixelSize(event->size().height() - _to_font_px(LABEL_INNER_MARGIN));
    m_labText->setFont(f);

    const int textWidth = m_labText->fontMetrics().horizontalAdvance(m_text);
    const int textHeight = m_labText->fontMetrics().height();
    m_labText->setFixedSize(textWidth, textHeight);
    m_initY = (this->height() - m_labText->height()) /2;
}

void RKMarqueeLabel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if (m_marqueeStyle == StopOnHover) {
        setStopStyleText();
        m_timer->start();
    }
}

void RKMarqueeLabel::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    m_timer->stop();
}

void RKMarqueeLabel::enterEvent(QEvent *event)
{
    qDebug()<<" ---------------- "<<this;

    if (m_marqueeStyle == MarqueeOnHover) {
        setMarqueeStyleText();
        m_timer->start();
    } else if (m_marqueeStyle == StopOnHover) {
        m_timer->stop();
        setStopStyleText();
    }

    QWidget::enterEvent(event);
}

void RKMarqueeLabel::leaveEvent(QEvent *event)
{
    qDebug()<<" ---------------- "<<this;

    if (m_marqueeStyle == MarqueeOnHover) {
        m_timer->stop();
        setStopStyleText();
    } else if (m_marqueeStyle == StopOnHover) {
        setMarqueeStyleText();
        m_timer->start();
    }

    QWidget::leaveEvent(event);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
