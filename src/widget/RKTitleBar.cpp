#include "RKTitleBar.h"

#include <QResizeEvent>

#include <DThemeManager>
#include <dwindowoptionbutton.h>
#include <dimagebutton.h>
#include <dwindowclosebutton.h>
#include <dwindowminbutton.h>
#include <DArrowButton>

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static int ICON_LY_SPACING = 10;
const static int MAIN_LY_R_SPACING = 10;


RKTitleBar::RKTitleBar(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("RKTitleBar");
    DThemeManager::instance()->registerWidget(this);

    m_backBtn = new DArrowButton;
    m_backBtn->setObjectName("BackBtn");
    m_backBtn->setFixedSize(0, 0);
    m_backBtn->setEnabled(false);
    m_backBtn->setArrowDirection(DArrowButton::ArrowLeft);
    connect(m_backBtn, &DArrowButton::mousePress, this, [&]() {
        Q_EMIT buttonClicked(WindowBackButton);
    });

    m_contentWidget = new QWidget;

    m_optionBtn = new DWindowOptionButton;
    m_optionBtn->setEnabled(false);
    connect(m_optionBtn, &DImageButton::clicked, this, [&]() {
        Q_EMIT buttonClicked(WindowOptionButton);
    });

    m_closeBtn = new DWindowCloseButton;
    m_closeBtn->setEnabled(false);
    connect(m_closeBtn, &DImageButton::clicked, this, [&] () {
        Q_EMIT buttonClicked(WindowCloseButton);
    });

    m_minimizeBtn = new DWindowMinButton;
    m_minimizeBtn->setEnabled(false);
    connect(m_minimizeBtn, &DImageButton::clicked, this, [&]() {
        Q_EMIT buttonClicked(WindowMinButton);
    });

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setAlignment(Qt::AlignVCenter);

    m_iconLayout = new QHBoxLayout;
    m_iconLayout->setContentsMargins(0, 0, 0, 0);
    m_iconLayout->setSpacing(ICON_LY_SPACING);
    m_iconLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_mainLayout->addWidget(m_backBtn, 0, Qt::AlignLeft);
    m_mainLayout->addWidget(m_contentWidget, Qt::AlignVCenter);
    m_mainLayout->addLayout(m_iconLayout);
    m_mainLayout->addSpacing(MAIN_LY_R_SPACING);

    this->setLayout(m_mainLayout);
}

RKTitleBar::~RKTitleBar()
{

}

void RKTitleBar::setDisplayedButtons(DisplayedButtons buttons)
{
    m_buttons = buttons;

    int contentWidth = this->width();

    m_mainLayout->removeWidget(m_backBtn);
    m_backBtn->setEnabled(false);
    m_backBtn->hide();

    {
        QList<QWidget*> list;
        list << m_optionBtn << m_minimizeBtn << m_closeBtn;
        foreach (QWidget *w, list) {
            m_iconLayout->removeWidget(w);
            w->setEnabled(false);
            w->hide();
        }
    }

    if ((buttons & WindowBackButton) == WindowBackButton) {
        m_backBtn->setEnabled(true);
        m_backBtn->show();
        m_mainLayout->insertWidget(0, m_backBtn);

        contentWidth -= m_backBtn->width();
        contentWidth -= ICON_LY_SPACING;
    }
    if ((buttons & WindowCloseButton) == WindowCloseButton) {
        m_closeBtn->setEnabled(true);
        m_closeBtn->show();
        m_iconLayout->insertWidget(0, m_closeBtn);

        contentWidth -= m_closeBtn->width();
        contentWidth -= ICON_LY_SPACING;
    }
    if ((buttons & WindowMinButton) == WindowMinButton) {
        m_minimizeBtn->setEnabled(true);
        m_minimizeBtn->show();
        m_iconLayout->insertWidget(0, m_minimizeBtn);
        contentWidth -= m_minimizeBtn->width();

        contentWidth -= ICON_LY_SPACING;
    }
    if ((buttons & WindowOptionButton) == WindowOptionButton) {
        m_optionBtn->setEnabled(true);
        m_optionBtn->show();
        m_iconLayout->insertWidget(0, m_optionBtn);

        contentWidth -= m_optionBtn->width();
        contentWidth -= ICON_LY_SPACING;
    }

    contentWidth -= MAIN_LY_R_SPACING;

    m_contentWidget->setFixedSize(contentWidth, this->height());
}

QSize RKTitleBar::sizeHint() const
{
    return QSize(TITLE_BAR_W, TITLE_BAR_H);
}

void RKTitleBar::resizeEvent(QResizeEvent *event)
{
    m_backBtn->setFixedSize(m_optionBtn->size());
    setDisplayedButtons(m_buttons);
    QFrame::resizeEvent(event);
}

} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer
