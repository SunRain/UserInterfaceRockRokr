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

const static int CONTENT_MARGIN_L = 4;
const static int CONTENT_MARGIN_T = 0;
const static int CONTENT_MARGIN_R = CONTENT_MARGIN_L;
const static int CONTENT_MARGIN_B = CONTENT_MARGIN_T;

class SearchInnerLabel : public RKMarqueeLabel
{
    Q_OBJECT

    friend class SearchResultItem;
public:
    SearchInnerLabel(SearchResultItem *item, QWidget *parent = Q_NULLPTR)
        : RKMarqueeLabel(parent),
          m_item(item)
    {}
    virtual ~SearchInnerLabel() {}

    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE
    {
        Q_ASSERT(m_item);
        if (m_item->hoverState()) {
            RKMarqueeLabel::enterEvent(event);
        }
    }
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE
    {
        Q_ASSERT(m_item);
        if (m_item->hoverState()) {
            RKMarqueeLabel::enterEvent(event);
        } else {
            RKMarqueeLabel::leaveEvent(event);
        }
    }
private:
    SearchResultItem *m_item;
};

SearchResultItem::SearchResultItem(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("SearchResultItem");
    DThemeManager::instance()->registerWidget(this);

    QVBoxLayout *ly = new QVBoxLayout;
    ly->setContentsMargins(CONTENT_MARGIN_L, CONTENT_MARGIN_T, CONTENT_MARGIN_R, CONTENT_MARGIN_B);
    ly->setSpacing(0);

    m_wrapperWidget = new QWidget;
    m_wrapperWidget->setObjectName("Wrapper");

    ly->addWidget(m_wrapperWidget);

    {
        QVBoxLayout *vb = new QVBoxLayout;
        vb->setContentsMargins(0, 0, 0, 0);
        vb->setSpacing(0);

        m_wrapperWidget->setLayout(vb);

        m_lable = new SearchInnerLabel(this);
        m_lable->setObjectName("LabelText");
        m_lable->setTextColor(QColor(FONT_COLOR_TITLE));
        m_lable->setHoverTextColor(QColor(HIGHLIGHT_FONT_COLOR));
        m_lable->setBackgroundColor(QColor(LEFT_BAR_BG_COLOR));
        m_lable->setHoverBackgroundColor(QColor(HIGHLIGHT_BG_COLOR));

        m_subLabel = new SearchInnerLabel(this);
        m_subLabel->setObjectName("SubLabelText");
        m_subLabel->setTextColor(QColor(FONT_COLOR_TITLE));
        m_subLabel->setHoverTextColor(QColor(HIGHLIGHT_FONT_COLOR));
        m_subLabel->setBackgroundColor(QColor(LEFT_BAR_BG_COLOR));
        m_subLabel->setHoverBackgroundColor(QColor(HIGHLIGHT_BG_COLOR));

        vb->addStretch();
        vb->addWidget(m_lable, Qt::AlignLeft | Qt::AlignVCenter);
        vb->addStretch();
        vb->addWidget(m_subLabel , Qt::AlignLeft | Qt::AlignVCenter);
        vb->addStretch();
    }

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
    m_hoverState = hover;
    if (hover) {
        m_wrapperWidget->setProperty("hoverState", "active");
        m_lable->enterEvent(Q_NULLPTR);
        m_subLabel->enterEvent(Q_NULLPTR);

    } else {
        m_wrapperWidget->setProperty("hoverState", "");
        m_lable->leaveEvent(Q_NULLPTR);
        m_subLabel->leaveEvent(Q_NULLPTR);
    }
    this->style()->unpolish(m_wrapperWidget);
    this->style()->polish(m_wrapperWidget);
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

    m_wrapperWidget->setFixedSize(sz.width() - CONTENT_MARGIN_L - CONTENT_MARGIN_R,
                                  sz.height() - CONTENT_MARGIN_T - CONTENT_MARGIN_B);

    m_lable->setFixedSize(sz.width() - CONTENT_MARGIN_L - CONTENT_MARGIN_R,
                          sz.height() / 2 - CONTENT_MARGIN_T - CONTENT_MARGIN_B);
    m_subLabel->setFixedSize(sz.width() - CONTENT_MARGIN_L - CONTENT_MARGIN_R,
                             sz.height() *2/5 - CONTENT_MARGIN_T - CONTENT_MARGIN_B);

    QFrame::resizeEvent(event);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#include "SearchResultItem.moc"
