#include "SearchResultItem.h"

#include <QEvent>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QDebug>
#include <QStyle>

#include <DThemeManager>

#include "widget/RKMarqueeLabel.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

SearchResultItem::SearchResultItem(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("SearchResultItem");
    DThemeManager::instance()->registerWidget(this);

    QVBoxLayout *ly = new QVBoxLayout;
    ly->setContentsMargins(0, 0, 0, 0);
    ly->setSpacing(0);

    m_lable = new RKMarqueeLabel;
    m_lable->setObjectName("LabelText");
    m_subLabel = new RKMarqueeLabel;
    m_subLabel->setObjectName("SubLabelText");

    ly->addStretch();
    ly->addWidget(m_lable, Qt::AlignLeft);
    ly->addStretch();
    ly->addWidget(m_subLabel , Qt::AlignLeft);
    ly->addStretch();

    this->setLayout(ly);
}

SearchResultItem::~SearchResultItem()
{

}

void SearchResultItem::setText(const QString &text)
{
    m_lable->setText(text);
}

void SearchResultItem::setSubText(const QString &subText)
{
    m_subLabel->setText(subText);
}

void SearchResultItem::setHoverState(bool hover)
{
    if (hover) {
        this->setProperty("hoverState", "active");
        m_lable->setForeground(QColor(252, 252, 252));
        m_subLabel->setForeground(QColor(252, 252, 252));
    } else {
        this->setProperty("hoverState", "");
        m_lable->setForeground(QColor(31, 32, 34));
        m_subLabel->setForeground(QColor(31, 32, 34));
    }
    this->style()->unpolish(this);
    this->style()->unpolish(m_lable);
    this->style()->unpolish(m_subLabel);
    this->style()->polish(this);
    this->style()->polish(m_lable);
    this->style()->polish(m_subLabel);
    this->update();
}

void SearchResultItem::enterEvent(QEvent *event)
{
    qDebug()<<" ----------------- ";
    this->setHoverState(true);
}

void SearchResultItem::leaveEvent(QEvent *event)
{
    qDebug()<<" ------------------";
    this->setHoverState(false);
}

void SearchResultItem::resizeEvent(QResizeEvent *event)
{
    qDebug()<<" --- size "<<event->size();
    const QSize sz = event->size();

    m_lable->setFixedSize(sz.width(), sz.height() / 2);
    m_subLabel->setFixedSize(sz.width(), sz.height() *2/5);

    QFrame::resizeEvent(event);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
