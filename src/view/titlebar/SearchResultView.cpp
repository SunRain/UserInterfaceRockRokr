#include "SearchResultView.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QEvent>
#include <QListWidgetItem>

#include <DThemeManager>

#include "PluginMgr.h"
#include "DataProvider/TrackSearchProvider.h"
#include "DataProvider/ITrackSearch.h"

#include "rockrokr_global.h"
#include "widget/RKMarqueeLabel.h"
#include "widget/ItemFragment.h"

#include "SearchResultItem.h"

DWIDGET_USE_NAMESPACE
using namespace PhoenixPlayer::DataProvider;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static int RET_ITEM_H = 40;
const static int LY_TOP_MARGIN = 4;
const static int LY_BOTTOM_MARGIN = 3;
class BtnFiler : public QObject
{
    Q_OBJECT
public:
    explicit BtnFiler(SearchResultView *view, QObject *parent = Q_NULLPTR)
        : QObject(parent),
          m_view(view)
    {
    }
    virtual ~BtnFiler()
    {
    }

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE
    {
        if (event->type() == QEvent::Enter) {
            m_view->searchByAllPluginBtnEntered();
        } else if (event->type() == QEvent::Leave) {
            m_view->searchByAllPluginBtnLeave();
        }
        return QObject::eventFilter(watched, event);
    }

private:
    SearchResultView *m_view = Q_NULLPTR;
};

SearchResultView::SearchResultView(QWidget *parent)
    : QFrame(parent)
{
    this->setObjectName("SearchResultView");
    DThemeManager::instance()->registerWidget(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, LY_TOP_MARGIN, 0, LY_BOTTOM_MARGIN);
    layout->setSpacing(2);
    this->setLayout(layout);

    m_resultView = new QListWidget;
    m_resultView->setObjectName("ResultView");
    m_resultView->setMouseTracking(true);
    m_resultView->setSelectionMode(QListView::SingleSelection);
    m_resultView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultView->setFocusPolicy(Qt::NoFocus);

    m_pluginView = new QListView;
    m_pluginView->setObjectName("PluginView");
    m_pluginView->setMouseTracking(true);
    m_pluginView->setSelectionMode(QListView::SingleSelection);
    m_pluginView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pluginView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pluginView->setFocusPolicy(Qt::NoFocus);

    m_pluginViewModel = new QStringListModel;
    m_pluginView->setModel(m_pluginViewModel);

    m_searchByAllPluginBtn = new QPushButton;
    m_searchByAllPluginBtn->installEventFilter(new BtnFiler(this, m_searchByAllPluginBtn));
    m_searchByAllPluginBtn->setCheckable(true);
    m_searchByAllPluginBtn->setObjectName("SearchBtn");
    m_searchByAllPluginBtn->setFixedHeight(_to_px(RET_ITEM_H/2));
//    m_searchByAllPluginBtn->setText(tr("Search \"%1\" in all plugin").);

    layout->addWidget(m_resultView, 0, Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(m_pluginView, 0, Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(m_searchByAllPluginBtn, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    this->setMinimumHeight(_to_px(RET_ITEM_H));
    m_resultView->setMinimumHeight(_to_px(RET_ITEM_H));
    m_resultView->adjustSize();

    connect(m_resultView, &QListWidget::itemClicked,
            this, [&](QListWidgetItem *item) {
        //TODO

        qDebug()<<" ----------------  m_resultView clicked "<<item;;
    });
    connect(m_resultView, &QListWidget::itemEntered,
            this, [&](QListWidgetItem *item) {

        qDebug()<<" ----------------  m_resultView entered "<<item;;

        m_resultView->setCurrentItem(item);
        m_pluginView->setCurrentIndex(QModelIndex());
        m_searchByAllPluginBtn->setChecked(false);
    });

    connect(m_pluginView, &QListView::clicked,
            this, [&](const QModelIndex &idx) {
        PluginMetaData meta = m_usedPlugins.at(idx.row());
        m_searchProvider->searchBy(m_searchStr, ITrackSearch::MatchAll, meta);
    });
    connect(m_pluginView, &QListView::entered,
            this, [&](const QModelIndex &idx) {

        qDebug()<<" ----------------  m_pluginView entered "<<idx;

        m_pluginView->setCurrentIndex(idx);
        m_resultView->setCurrentIndex(QModelIndex());
        m_searchByAllPluginBtn->setChecked(false);
    });

    connect(m_searchByAllPluginBtn, &QPushButton::clicked,
            this, [&]() {
        m_searchProvider->search(m_searchStr, ITrackSearch::MatchAll);
    });

    initProvider();
}

SearchResultView::~SearchResultView()
{

}

void SearchResultView::calToResize()
{
    const int maxH = qMin(m_resultObjList.size() * (_to_px(RET_ITEM_H))
                          + m_pluginViewModel->rowCount() * (_to_px(RET_ITEM_H))
                          + m_searchByAllPluginBtn->height()
                          + this->layout()->spacing() * this->layout()->count()
                          + LY_TOP_MARGIN
                          + LY_BOTTOM_MARGIN
                          ,
                          this->maximumHeight());
    this->setFixedHeight(maxH);

    int pvH = 0;
    if (m_pluginViewModel->rowCount() > 1) {
        pvH = m_pluginViewModel->rowCount() * (_to_px(RET_ITEM_H)) + 2;
    }
    int srH = maxH - pvH - m_searchByAllPluginBtn->height()
              - LY_TOP_MARGIN
              - LY_BOTTOM_MARGIN
              - this->layout()->spacing() * 3; // 3 part sub view (search ret, plugin, button)

    m_resultView->setFixedHeight(srH);
    m_resultView->setFixedWidth(this->size().width());

    for(int i=0; i<m_resultView->count(); ++i) {
        QListWidgetItem *item = m_resultView->item(i);
        item->setSizeHint(QSize(this->size().width(), _to_px(RET_ITEM_H)));
        SearchResultItem *sm = qobject_cast<SearchResultItem*>(m_resultView->itemWidget(item));
        sm->setFixedSize(this->width(), _to_px(RET_ITEM_H));
    }

    m_pluginView->setFixedSize(this->size().width(), pvH);
    m_searchByAllPluginBtn->setFixedWidth(this->size().width());

    m_resultView->setVisible(!m_resultObjList.isEmpty());
    m_pluginView->setVisible(m_pluginViewModel->rowCount() > 1);

    this->adjustSize();

    m_resultView->raise();
}

void SearchResultView::setSearchString(const QString &str)
{
    m_searchStr = str;
    m_searchByAllPluginBtn->setText(tr("Search \"%1\" in all plugin").arg(str));
}

void SearchResultView::doSearch()
{
    if (m_searchStr.isEmpty()) {
        qWarning()<<"Can't search with empty string, ignore!!";
        return;
    }
    m_searchProvider->search(m_searchStr, ITrackSearch::MatchAll);
}

void SearchResultView::initProvider()
{
    if (!m_searchProvider) {
        m_searchProvider = new TrackSearchProvider(this);
        connect(m_searchProvider, &TrackSearchProvider::matched,
                this, [&](const QString &pattern, const QList<MatchObject> &objList) {
            if (m_searchStr != pattern) {
                return;
            }
            m_resultView->clear();
            m_resultObjList.clear();
            m_resultObjList.append(objList);

            foreach(const auto &it, m_resultObjList) {
                QListWidgetItem *item = new QListWidgetItem(m_resultView);
                m_resultView->addItem(item);

                SearchResultItem *sm = new SearchResultItem;
                sm->setObjectName("SearchResultItem");
                sm->setFixedHeight(_to_px(RET_ITEM_H));
                sm->setText(it.uri());
                sm->setSubText(it.matchedStr());
                m_resultView->setItemWidget(item, sm);

            }
            Q_EMIT searchMatched();
        });
    }
    m_usedPlugins.clear();
    m_usedPlugins.append(m_searchProvider->enabledPlugins());
    QStringList ll;
    foreach (const auto &it, m_usedPlugins) {
        ll.append(tr("Search by \"%1\"").arg(it.property.name));
    }
    m_pluginViewModel->setStringList(ll);
}

void SearchResultView::searchByAllPluginBtnEntered()
{
    m_resultView->setCurrentIndex(QModelIndex());
    m_pluginView->setCurrentIndex(QModelIndex());
    m_searchByAllPluginBtn->setChecked(true);
}

void SearchResultView::searchByAllPluginBtnLeave()
{
    m_searchByAllPluginBtn->setChecked(false);
}

void SearchResultView::leaveEvent(QEvent *event)
{
    m_resultView->setCurrentIndex(QModelIndex());
    m_pluginView->setCurrentIndex(QModelIndex());
    m_searchByAllPluginBtn->setChecked(false);
    QFrame::leaveEvent(event);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#include "SearchResultView.moc"

