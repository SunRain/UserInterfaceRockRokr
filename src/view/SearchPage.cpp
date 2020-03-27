#include "SearchPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>

#include <DThemeManager>

#include "DataProvider/TrackSearchProvider.h"

#include "rockrokr_global.h"
#include "searchview/SRLeftBar.h"
#include "searchview/SearchResultContentView.h"
#include "playbar/PlayBar.h"
#include "widget/RKTitleBar.h"
#include "widget/RKSearchEdit.h"
#include "widget/RKStackWidget.h"
#include "view/ViewUtility.h"

DWIDGET_USE_NAMESPACE

using namespace PhoenixPlayer::DataProvider;

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
    void bindTrackSearchProvider(DataProvider::TrackSearchProvider *provider)
    {
        m_provider = provider;
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
            if (m_provider) {
                m_provider->search(searchtext, ITrackSearch::MatchAll);
            }
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
private:
    DataProvider::TrackSearchProvider *m_provider = Q_NULLPTR;
};

SearchPage::SearchPage(QWidget *parent)
    : QFrame(parent)
{

    this->setObjectName("SearchPage");
    DThemeManager::instance()->registerWidget(this);

    m_leftBar = new SRLeftBar(this);
    m_leftBar->setFixedWidth(LEFT_BAR_W);

    m_rPart = new QWidget(this);

    m_bgLabel = new QLabel(this);

    m_stack = new RKStackedWidget;

    m_titlebar  = new RKTitleBar;
    m_titlebar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_titlebar->setDisplayedButtons(RKTitleBar::WindowOptionButton |
                                    RKTitleBar::WindowMinButton |
                                    RKTitleBar::WindowCloseButton);

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
    m_leftAnim->setDuration(500);

    m_rightAnim->setTargetObject(m_rPart);
    m_rightAnim->setPropertyName("geometry");
    m_rightAnim->setEasingCurve(QEasingCurve::Linear);
    m_rightAnim->setDuration(500);

    connect(m_animGroup, &QParallelAnimationGroup::finished,
            this, [&](){
        if (!m_hideState) {
            m_bgLabel->lower();
            m_bgLabel->hide();
        } else {
            ViewUtility::showRockRokrPage();
        }
    });

    connect(m_leftBar, &SRLeftBar::clickedBackBtn,
            this, &SearchPage::showHideAnimation);

    initUserInterface();

    m_bgLabel->lower();
    m_leftBar->raise();
    m_rPart->raise();
}

SearchPage::~SearchPage()
{

}

void SearchPage::bindTrackSearchProvider(DataProvider::TrackSearchProvider *provider)
{
    if (m_searchProvider == provider) {
        return;
    }
    if (m_searchProvider) {
        m_searchProvider->disconnect(this);
    }
    if (!provider) {
        return;
    }
    m_searchEdit->bindTrackSearchProvider(provider);

    m_searchProvider = provider;
    m_enabledPlugins.clear();
    m_enabledPlugins.append(provider->enabledPlugins());

    m_leftBar->setEnabledPlugins(m_enabledPlugins);

    for (int i = 0; i < m_stack->count(); ++i) {
        QWidget *w = m_stack->widget(i);
        m_stack->removeWidget(w);
        w->deleteLater();
        w = Q_NULLPTR;
    }

    for (int i = 0; i < m_enabledPlugins.count(); ++i) {
        SearchResultContentView *v = new SearchResultContentView;
        m_stack->addWidget(v);
    }

    connect(m_searchProvider, &TrackSearchProvider::matched,
            this, [&](const QString &pattern, const QList<MatchObject> &objList) {

        if (m_searchEdit->getLineEdit()->text() != pattern) {
            m_searchEdit->getLineEdit()->setText(pattern);
        }

        m_matchRetMap.clear();
        for (int i = 0; i < m_enabledPlugins.count(); ++i) {
            auto plugin = m_enabledPlugins.at(i);
            QList<MatchObject> list;
            const QString key = plugin.property.name;
            foreach (const auto &obj, objList) {
                if (obj.pluginProperty().name == key) {
                    list.append(obj);
                }
            }
            m_matchRetMap.insert(key, list);

            SearchResultContentView *widget = qobject_cast<SearchResultContentView*>(m_stack->widget(i));
            widget->setDataMap(TrackSearchProvider::trimByUri(list));
        }
    });
}

void SearchPage::setBackgroundPixmap(const QPixmap &pm)
{
    m_bgLabel->setPixmap(pm);
}

void SearchPage::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
    m_hideState = false;

    {
        QRect rcEnd = m_leftGeometry;
        QRect rcStart(rcEnd);
        rcStart.setX(-m_leftBar->width());

        m_leftAnim->setStartValue(rcStart);
        m_leftAnim->setEndValue(rcEnd);
    }
    {
        QRect rcEnd = m_rightGeometry;
        QRect rcStart (rcEnd);
        rcStart.setX(rcStart.x() + m_rPart->width()/2);

        m_rightAnim->setStartValue(rcStart);
        m_rightAnim->setEndValue(rcEnd);
    }
    m_bgLabel->show();
    m_leftBar->show();
    m_leftBar->raise();
    m_rPart->show();
    m_rPart->raise();
    m_animGroup->start();
}

void SearchPage::hideEvent(QHideEvent *event)
{
    QFrame::hideEvent(event);
    m_hideState = true;
}

void SearchPage::resizeEvent(QResizeEvent *event)
{
    m_bgLabel->setFixedSize(event->size());

    m_leftBar->setFixedHeight(event->size().height());
    m_leftGeometry = QRect(0, 0, m_leftBar->width(), m_leftBar->height());

    m_rPart->setFixedWidth(event->size().width() - m_leftBar->width());
    m_rPart->setFixedHeight(event->size().height());
    m_rightGeometry = QRect(m_leftBar->width(), 0, m_rPart->width(), m_rPart->height());

    QFrame::resizeEvent(event);
}

void SearchPage::initUserInterface()
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

        QVBoxLayout *vbox = new QVBoxLayout(m_rPart);
        vbox->setContentsMargins(0, LEFT_BAR_BT_TB_MARGIN, 0, 0);
        vbox->setSpacing(0);
        {
            QVBoxLayout *vv = new QVBoxLayout;
            vv->setContentsMargins(RIGHT_PART_L_MARGIN, 0, 0, 0);
            vv->setSpacing(0);
            vv->addWidget(m_titlebar);
            vv->addSpacing(10);
            vv->addWidget(m_stack);
            vbox->addLayout(vv);
        }
        vbox->addWidget(m_playbar, 0, Qt::AlignBottom);
}

void SearchPage::showHideAnimation()
{
    m_hideState = true;

    {
        QRect rcStart = m_leftGeometry;
        QRect rcEnd(rcStart);
        rcEnd.setX(-m_leftBar->width());

        m_leftAnim->setStartValue(rcStart);
        m_leftAnim->setEndValue(rcEnd);
    }
    {
        QRect rcStart = m_rightGeometry;
        QRect rcEnd (rcStart);
        rcEnd.setX(rcStart.x() + m_rPart->width()/2);

        m_rightAnim->setStartValue(rcStart);
        m_rightAnim->setEndValue(rcEnd);
    }
    m_bgLabel->show();
    m_leftBar->show();
    m_leftBar->raise();
    m_rPart->show();
    m_rPart->raise();
    m_animGroup->start();
}



} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer

#include "SearchPage.moc"
