#include "SearchPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <DThemeManager>

#include "DataProvider/TrackSearchProvider.h"

#include "rockrokr_global.h"
#include "searchview/SRLeftBar.h"
#include "playbar/PlayBar.h"
#include "widget/RKTitleBar.h"
#include "widget/RKSearchEdit.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SPSearchEdit : public RKSearchEdit
{
    Q_OBJECT
public:
    SPSearchEdit(QWidget *parent = Q_NULLPTR)
        : RKSearchEdit(parent)
    {
        connect(this, &SPSearchEdit::focusOut,
                this, &SPSearchEdit::onFocusOut);

        connect(this, &SPSearchEdit::textChanged,
                this, &SPSearchEdit::onTextChanged);

        connect(this, &SPSearchEdit::returnPressed,
                this, &SPSearchEdit::onReturnPressed);
    }
    virtual ~SPSearchEdit()
    {

    }

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(TITLE_BAR_SEARCH_VIEW_W, TITLE_BAR_SEARCH_VIEW_H);
    }

protected:
    void onFocusOut()
    {

    }
    void onTextChanged()
    {
        auto text = QString(this->text()).remove(" ").remove("\r").remove("\n");
        if (text.length() >= 2) {
            auto searchtext = QString(this->text()).remove("\r").remove("\n");

        } else {
            onFocusOut();
        }
    }
    void onReturnPressed()
    {
        auto text = QString(this->text()).remove(" ").remove("\r").remove("\n");
        if (text.isEmpty()) {
            return;
        }
    //    onFocusOut();
        onTextChanged();
    }
};



SearchPage::SearchPage(QWidget *parent)
    : QFrame(parent)
{

    this->setObjectName("SearchPage");
    DThemeManager::instance()->registerWidget(this);

    m_leftBar = new SRLeftBar;
    m_leftBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    m_titlebar  = new RKTitleBar;
    m_titlebar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_titlebar->setDisplayedButtons(RKTitleBar::WindowOptionButton |
                                    RKTitleBar::WindowMinButton |
                                    RKTitleBar::WindowCloseButton);
//    m_titlebar->bindPopup(m_searchResultPopup);

    m_playbar   = new PlayBar;
    m_playbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_searchEdit = new SPSearchEdit;
    m_searchEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_searchEdit->setPlaceHolder(tr("Search"));

    m_animGroup = new QParallelAnimationGroup(this);
    m_leftAnim = new QPropertyAnimation;
    m_rightAnim = new QPropertyAnimation;
    m_animGroup->addAnimation(m_leftAnim);
    m_animGroup->addAnimation(m_rightAnim);

    m_leftAnim->setTargetObject(m_leftBar);
    m_leftAnim->setPropertyName("geometry");
    m_leftAnim->setEasingCurve(QEasingCurve::Linear);
    m_leftAnim->setDuration(1000);

    m_rightAnim->setTargetObject(m_titlebar);
    m_rightAnim->setPropertyName("geometry");
    m_rightAnim->setEasingCurve(QEasingCurve::Linear);
    m_rightAnim->setDuration(1000);

    initUserInterface();
}

SearchPage::~SearchPage()
{

}

void SearchPage::bindTrackSearchProvider(DataProvider::TrackSearchProvider *provider)
{
    m_searchProvider = provider;
    m_leftBar->setEnabledPlugins(provider->enabledPlugins());
}

void SearchPage::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    {
        QRect rcEnd = m_leftBar->geometry();
        QRect rcStart(rcEnd);
        rcStart.setX(-m_leftBar->width());

        m_leftAnim->setStartValue(rcStart);
        m_leftAnim->setEndValue(rcEnd);

        qDebug()<<rcStart<<rcEnd;
    }
    {
        QRect rcEnd = m_titlebar->geometry();
        QRect rcStart (rcEnd);
        rcEnd.setX(rcStart.x() + m_titlebar->width()/2);

        m_rightAnim->setStartValue(rcStart);
        m_rightAnim->setEndValue(rcEnd);

        qDebug()<<rcStart<<rcEnd;
    }
    m_animGroup->start();
}

void SearchPage::hideEvent(QHideEvent *event)
{
    QFrame::hideEvent(event);
}

void SearchPage::initUserInterface()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(m_leftBar);

    {
        // title bar
        {
            QHBoxLayout *hb = new QHBoxLayout;
            hb->setContentsMargins(0, 0, 0, 0);
            hb->setSpacing(0);
            hb->setAlignment(Qt::AlignVCenter);
//            m_titlebar->getContentWidget()->setFixedSize(TITLE_BAR_SEARCH_VIEW_W,
//                                                         TITLE_BAR_SEARCH_VIEW_H);
            m_titlebar->getContentWidget()->setFixedHeight(TITLE_BAR_SEARCH_VIEW_H);

            m_titlebar->getContentWidget()->setLayout(hb);
            hb->addWidget(m_searchEdit, Qt::AlignVCenter);
        }

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->setContentsMargins(0, LEFT_BAR_BT_TB_MARGIN, 0, 0);
        vbox->setSpacing(0);
        {
            QVBoxLayout *vv = new QVBoxLayout;
            vv->setContentsMargins(RIGHT_PART_L_MARGIN, 0, 0, 0);
            vv->setSpacing(0);
            vv->addWidget(m_titlebar);
            vv->addSpacing(10);
//            vv->addWidget(m_stack);
            vbox->addLayout(vv);
        }
        vbox->addWidget(m_playbar, 0, Qt::AlignBottom);
        layout->addLayout(vbox);
    }

}



} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer

#include "SearchPage.moc"
