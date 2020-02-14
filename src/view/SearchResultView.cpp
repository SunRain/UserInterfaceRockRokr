#include "SearchResultView.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QEvent>

#include <DThemeManager>

#include "rockrokr_global.h"
#include "PluginMgr.h"
#include "DataProvider/TrackSearchProvider.h"
#include "DataProvider/ITrackSearch.h"

DWIDGET_USE_NAMESPACE
using namespace PhoenixPlayer::DataProvider;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

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
    layout->setContentsMargins(0, 4, 0, 3);
    layout->setSpacing(0);
    this->setLayout(layout);

    m_resultView = new QListView;
    m_resultView->setObjectName("ResultView");
    m_resultView->setMouseTracking(true);
    m_resultView->setSelectionMode(QListView::SingleSelection);
    m_resultView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultView->setFocusPolicy(Qt::NoFocus);

    m_resultModel = new QStringListModel;
//    {
//        QStringList a;
//        for(int i=0; i<9; ++i) {
//            a.append(QString::number(i));
//        }
//        m_resultModel->setStringList(a);
//    }
    m_resultView->setModel(m_resultModel);

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
    m_searchByAllPluginBtn->setFixedHeight(_to_px(25));
//    m_searchByAllPluginBtn->setText(tr("Search \"%1\" in all plugin").);

    layout->addWidget(m_resultView, 0, Qt::AlignHCenter | Qt::AlignTop);
    layout->addSpacing(1);
    layout->addWidget(m_pluginView, 0, Qt::AlignHCenter | Qt::AlignTop);
    layout->addSpacing(1);
    layout->addWidget(m_searchByAllPluginBtn, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addSpacing(1);

    this->setMinimumHeight(_to_px(50));
    m_resultView->setMinimumHeight(_to_px(25));
    m_resultView->adjustSize();
    m_pluginView->setMinimumHeight(_to_px(25));
    m_pluginView->adjustSize();

    connect(m_resultView, &QListView::clicked,
            this, [&](const QModelIndex &idx) {
        //TODO
    });
    connect(m_resultView, &QListView::entered,
            this, [&](const QModelIndex &idx) {
        m_resultView->setCurrentIndex(idx);
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
    m_resultView->setFixedHeight(m_resultModel->rowCount() * (_to_px(25)) + 2);
    m_resultView->setFixedWidth(this->size().width() - 2);

    m_pluginView->setFixedHeight(m_pluginViewModel->rowCount() * (_to_px(25)) + 2);
    m_pluginView->setFixedWidth(this->size().width() - 2);

    m_searchByAllPluginBtn->setFixedWidth(this->size().width() - 2);

    qDebug() << m_searchByAllPluginBtn->rect() << m_resultView->rect();

    this->setFixedHeight(m_resultView->height() + m_pluginView->height() + 25 + 8 + 3);

    m_resultView->setVisible(!(0 == m_resultModel->rowCount()));

    this->adjustSize();

    qDebug() << this->height();

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

            m_resultObjList.clear();
            m_resultObjList.append(objList);

            QStringList ll;
            foreach (const auto &it, m_resultObjList) {
                ll.append(it.matchedStr());
            }
            m_resultModel->setStringList(ll);

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

