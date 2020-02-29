#include "RKMarqueeLabel.h"

#include <QTimer>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QtMath>

#include "rockrokr_global.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static int LABEL_INNER_MARGIN = 6;

RKMarqueeLabel::RKMarqueeLabel(QWidget *parent)
    : QFrame(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(m_interval);
    connect(m_timer, &QTimer::timeout, this, &RKMarqueeLabel::timeout);
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
        QFont f(this->font());
        f.setPixelSize(this->height() - _to_font_px(LABEL_INNER_MARGIN));
        this->setFont(f);

        const int textWidth = this->fontMetrics().horizontalAdvance(text);
        const int perTextWidth = qCeil((qreal)textWidth/(qreal)text.length());
        m_drawLength = this->rect().width() / perTextWidth - 3; // 3 for draw dot(...) when marquee
        m_marqueeCnt = text.length() - m_drawLength + 3;
        m_drawYPos = (this->rect().height() - fontMetrics().height()) /2;

        if (m_moveStyle == LeftToRight) {
            m_startIdx = -m_marqueeCnt;
        } else {
            m_startIdx = 0;
        }
        this->update();
    }
}

void RKMarqueeLabel::setInterval(int interval)
{
    if (m_interval != interval) {
        m_interval = interval;
        m_timer->setInterval(interval);
    }
}

void RKMarqueeLabel::setTextColor(const QColor &color)
{
    if (m_textColor != color) {
        m_textColor = color;
        this->update();
    }
}

void RKMarqueeLabel::setHoverTextColor(const QColor &color)
{
    if (m_hoverTextColor != color) {
        m_hoverTextColor = color;
        this->update();
    }
}

void RKMarqueeLabel::setBackgroundColor(const QColor &color)
{
    if (m_bgColor != color) {
        m_bgColor = color;
        this->update();
    }
}

void RKMarqueeLabel::setHoverBackgroundColor(const QColor &color)
{
    if (m_hoverBgColor != color) {
        m_hoverBgColor = color;
        this->update();
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

void RKMarqueeLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::HighQualityAntialiasing |
                           QPainter::SmoothPixmapTransform);

    if (m_hoverState) {
        painter.setPen(m_hoverBgColor);
        painter.setBrush(m_hoverBgColor);
    } else {
        painter.setPen(m_bgColor);
        painter.setBrush(m_bgColor);
    }
    painter.drawRect(this->rect());

    if (m_hoverState) {
        painter.setPen(m_hoverTextColor);
        painter.setBrush(m_hoverTextColor);
    } else {
        painter.setPen(m_textColor);
        painter.setBrush(m_textColor);
    }

    QString text = m_text.mid(m_startIdx, m_drawLength);
    if ((m_startIdx == 0) && (m_marqueeCnt > 0)) {
        text.append("...");
    }

    QTextOption topt;
    topt.setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignLeft);

    QRectF trf = this->rect();
    trf.setX(rect().x());
    trf.setY(m_drawYPos);

    painter.drawText(trf, text, topt);
}

void RKMarqueeLabel::timeout()
{
    if (m_marqueeCnt <= 0) {
        return;
    }

    this->update();

    if (qAbs(m_startIdx) >= m_marqueeCnt) {
        if (m_moveStyle == LeftAndRight) {
            m_startIdx = -m_marqueeCnt;
        } else if (m_moveStyle == RightToLeft) {
            m_startIdx = 0;
        } else if (m_moveStyle == LeftToRight) {
            m_startIdx = -m_marqueeCnt;
        }
    } else {
        m_startIdx++;
    }
}

void RKMarqueeLabel::setMarqueeStyleText()
{
    setCursor(Qt::PointingHandCursor);
    m_hoverState = true;
}

void RKMarqueeLabel::setStopStyleText()
{
    setCursor(Qt::PointingHandCursor);
    m_hoverState = false;
}

void RKMarqueeLabel::resizeEvent(QResizeEvent *event)
{
    QFont f(this->font());
    f.setPixelSize(event->size().height() - _to_font_px(LABEL_INNER_MARGIN));
    this->setFont(f);

    const int textWidth = this->fontMetrics().horizontalAdvance(m_text);
    const int perTextWidth = qCeil((qreal)textWidth/(qreal)m_text.length());
    m_drawLength = this->rect().width() / perTextWidth - 3; // 3 for draw dot(...) when marquee
    m_marqueeCnt = m_text.length() - m_drawLength + 3;
    m_drawYPos = (this->rect().height() - fontMetrics().height()) /2;

    this->update();
}

void RKMarqueeLabel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if (m_marqueeStyle == StopOnHover) {
        setStopStyleText();
        m_timer->start();
    }
    this->update();
}

void RKMarqueeLabel::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    m_timer->stop();
    this->update();
}

void RKMarqueeLabel::enterEvent(QEvent *event)
{
    if (m_marqueeStyle == MarqueeOnHover) {
        setMarqueeStyleText();
        m_timer->start();
    } else if (m_marqueeStyle == StopOnHover) {
        m_timer->stop();
        setStopStyleText();
    }
    this->update();
    QWidget::enterEvent(event);
}

void RKMarqueeLabel::leaveEvent(QEvent *event)
{
    if (m_marqueeStyle == MarqueeOnHover) {
        m_timer->stop();
        setStopStyleText();
    } else if (m_marqueeStyle == StopOnHover) {
        setMarqueeStyleText();
        m_timer->start();
    }
    this->update();
    QWidget::leaveEvent(event);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
