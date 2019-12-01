#include "RKIconButton.h"

#include <QMouseEvent>
#include <QDebug>

#include "qtmaterialrippleoverlay.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


RKIconButton::RKIconButton(QWidget *parent)
    : QPushButton(parent),
      m_overlay(new QtMaterialRippleOverlay(parent))
{
}

RKIconButton::~RKIconButton()
{

}

bool RKIconButton::event(QEvent *event)
{
    if (event->type() == QEvent::ParentChange && parentWidget()) {
        m_overlay->setParent(parentWidget());
    }
    return QPushButton::event(event);
}

void RKIconButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
}

void RKIconButton::mousePressEvent(QMouseEvent *event)
{
    m_overlay->addRipple(QPoint(m_overlay->width(), m_overlay->height())/2,
                         rect().width() *4/5);
    QPushButton::mousePressEvent(event);
}

void RKIconButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
}

void RKIconButton::resizeEvent(QResizeEvent *event)
{
    QRect r = this->rect();
    r.setSize(QSize(this->width() + this->width()/2,
                    this->height() + this->height()/2));
    r.moveCenter(this->geometry().center());
    m_overlay->setGeometry(r);

    QPushButton::resizeEvent(event);
}

void RKIconButton::moveEvent(QMoveEvent *event)
{
    QRect r = this->rect();
    r.setSize(QSize(this->width() + this->width()/2,
                    this->height() + this->height()/2));
    r.moveCenter(this->geometry().center());
    m_overlay->setGeometry(r);

    QPushButton::moveEvent(event);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


