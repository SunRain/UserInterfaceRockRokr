#include "SRLeftBarItem.h"

#include <QEvent>
#include <QResizeEvent>
#include <QStyle>
#include <QHBoxLayout>

#include <DThemeManager>

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

SRLeftBarItem::SRLeftBarItem(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("SRLeftBarItem");
    DThemeManager::instance()->registerWidget(this);

    m_label = new QLabel;
    m_label->setObjectName("SRLeftBarItemTextLabel");

    QHBoxLayout *ly = new QHBoxLayout(this);
    ly->setContentsMargins(LEFT_BAR_ITEM_CONTENT_MARGIN, //0,
                           0,
                           LEFT_BAR_ITEM_CONTENT_MARGIN, //0,
                           0);
    ly->setSpacing(0);
    ly->addWidget(m_label, Qt::AlignCenter);
}

SRLeftBarItem::~SRLeftBarItem()
{

}

void SRLeftBarItem::setHoverState(bool hover)
{
    if (hover) {
        this->setProperty("hoverState", "active");
        m_label->setProperty("hoverState", "active");
    } else {
        this->setProperty("hoverState", "");
        m_label->setProperty("hoverState", "");
    }
    this->style()->unpolish(m_label);
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->style()->polish(m_label);
    this->update();
}

void SRLeftBarItem::setText(const QString &text)
{
    m_label->setText(text);
}

QSize SRLeftBarItem::sizeHint() const
{
    return QSize(LEFT_BAR_ITEM_W, LEFT_BAR_ITEM_H);
}

void SRLeftBarItem::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    this->setHoverState(true);
}

void SRLeftBarItem::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    this->setHoverState(false);
}

void SRLeftBarItem::resizeEvent(QResizeEvent *event)
{


    QFrame::resizeEvent(event);
}



} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

