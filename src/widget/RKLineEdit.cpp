#include "RKLineEdit.h"

#include <QMouseEvent>
#include <QPainter>

#include <DThemeManager>

DWIDGET_USE_NAMESPACE;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKLineEdit::RKLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
//    this->setObjectName("RKLineEdit");
//    DThemeManager::instance()->registerWidget(this);
    this->setStyleSheet("border:none");

//    m_lineColor = QColor("51, 108, 251, 1");
    m_lineColor.setNamedColor("#336CFB");

    m_animation = new QPropertyAnimation(this, QByteArray());
    m_animation->setDuration(1000);
    m_animation->setEasingCurve(QEasingCurve::InCubic);
    m_animation->setStartValue(0.0f);
    m_animation->setEndValue(0.0f);

    connect(m_animation, &QPropertyAnimation::valueChanged,
            this, [&](const QVariant &value) {
        m_currentValue = value.toFloat();
        this->update();
    });

    connect(m_animation, &QPropertyAnimation::finished,
            this, [&]() {
        m_currentValue = 0;
    });

}

RKLineEdit::~RKLineEdit()
{
    if (m_animation->state() != QAbstractAnimation::State::Stopped) {
        m_animation->stop();
        m_animation->deleteLater();
        m_animation = Q_NULLPTR;
    }
}

QColor RKLineEdit::lineColor() const
{
    return m_lineColor;
}

void RKLineEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) {
        return QLineEdit::mousePressEvent(e);
    }
    m_animation->setStartValue(0);
    m_animation->setEndValue(width()/2);
    m_animation->start();

//    e->accept();
}

void RKLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    if (this->isReadOnly() || !this->isEnabled()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(QBrush(m_lineColor), 1, Qt::SolidLine));

    if (m_animation->state() == QAbstractAnimation::State::Running) {
        int hw = width()/2;
        painter.drawLine(hw - m_currentValue - 1, height() - 1, hw, height() - 1);
        painter.drawLine(hw + 1, height() - 1, hw + m_currentValue, height() - 1);
    } else {
        painter.drawLine(0, height() - 1, width(), height() - 1);
    }
}

void RKLineEdit::setLineColor(const QColor &lineColor)
{
    if (m_lineColor == lineColor)
        return;

    m_lineColor = lineColor;
    emit lineColorChanged(m_lineColor);
}



} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
