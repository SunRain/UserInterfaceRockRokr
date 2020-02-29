#include "SearchResultItem.h"

#include <QEvent>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QDebug>
#include <QStyle>
#include <QColor>

#include <DThemeManager>

#include "widget/RKMarqueeLabel.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SearchInnerLabel : public RKMarqueeLabel
{
    Q_OBJECT

    friend class SearchResultItem;
public:
    SearchInnerLabel(QWidget *parent = Q_NULLPTR)
        : RKMarqueeLabel(parent)
    {}
    virtual ~SearchInnerLabel() {}

    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE
    {
        SearchResultItem *item = qobject_cast<SearchResultItem*>(this->parent());
        Q_ASSERT(item);
        if (item->property("hoverState") == "active") {
            RKMarqueeLabel::enterEvent(event);
        }
    }
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE
    {
        SearchResultItem *item = qobject_cast<SearchResultItem*>(this->parent());
        Q_ASSERT(item);
        if (item->property("hoverState") == "active") {
            RKMarqueeLabel::enterEvent(event);
        } else {
            RKMarqueeLabel::leaveEvent(event);
        }
    }
};

SearchResultItem::SearchResultItem(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("SearchResultItem");
    DThemeManager::instance()->registerWidget(this);

    QVBoxLayout *ly = new QVBoxLayout;
    ly->setContentsMargins(0, 0, 0, 0);
    ly->setSpacing(0);

    m_lable = new SearchInnerLabel;
    m_lable->setObjectName("LabelText");
    m_lable->setTextColor(QColor(FONT_COLOR_TITLE));
    m_lable->setHoverTextColor(Qt::transparent);
    m_lable->setBackgroundColor(Qt::transparent);
    m_lable->setHoverBackgroundColor(Qt::transparent);

    m_subLabel = new SearchInnerLabel;
    m_subLabel->setObjectName("SubLabelText");
    m_subLabel->setTextColor(QColor(FONT_COLOR_TITLE));
    m_subLabel->setHoverTextColor(Qt::transparent);
    m_subLabel->setBackgroundColor(Qt::transparent);
    m_subLabel->setHoverBackgroundColor(Qt::transparent);


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
        m_lable->setHoverTextColor(QColor(HIGHLIGHT_FONT_COLOR));
        m_lable->enterEvent(Q_NULLPTR);
        m_subLabel->setHoverTextColor(QColor(HIGHLIGHT_FONT_COLOR));
        m_subLabel->enterEvent(Q_NULLPTR);

    } else {
        this->setProperty("hoverState", "");
        m_lable->setHoverTextColor(QColor(FONT_COLOR_TITLE));
        m_lable->leaveEvent(Q_NULLPTR);
        m_subLabel->setHoverTextColor(QColor(FONT_COLOR_TITLE));
        m_subLabel->leaveEvent(Q_NULLPTR);
    }
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

void SearchResultItem::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    this->setHoverState(true);
}

void SearchResultItem::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    this->setHoverState(false);
}

void SearchResultItem::resizeEvent(QResizeEvent *event)
{
    const QSize sz = event->size();

    m_lable->setFixedSize(sz.width(), sz.height() / 2);
    m_subLabel->setFixedSize(sz.width(), sz.height() *2/5);

    QFrame::resizeEvent(event);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#include "SearchResultItem.moc"
