#include "BaseCategoryView.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QLabel>

#include <DThemeManager>

#include "rockrokr_global.h"
#include "widget/RKListView.h"
#include "CategoryRecommendedViewDelegate.h"
#include "CategoryRecentViewDelegate.h"
#include "CategoryAllViewDelegate.h"
#include "CategoryModel.h"
#include "CategoryViewDataProvider.h"

DWIDGET_USE_NAMESPACE


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

BaseCategoryView::BaseCategoryView(CategoryViewDataProvider *pr, QWidget *parent)
    : QFrame(parent),
      m_dataProvider(pr)
{
    this->setObjectName("BaseCategoryView");
    DThemeManager::instance()->registerWidget(this);

    m_recommedViewDelegate = new CategoryRecommendedViewDelegate;
    m_recentViewDelegate = new CategoryRecentViewDelegate;
    m_allViewDelegate = new CategoryAllViewDelegate;

    {
        m_recommendedListView = new RKListView;
        initListViewParameter(m_recommendedListView);
        m_recommendedListView->setFixedHeight(MAIN_VIEW_HL_VIEW_H);
        m_recommendedListView->setDefaultItemSize(QSize(MAIN_VIEW_HL_COVER_W, MAIN_VIEW_HL_COVER_H));
        m_recommendedListView->setSpacing(10);
        m_recommendedListView->setViewMode(QListView::ViewMode::IconMode);
        m_recommendedListView->setDragEnabled(false);
        m_recommendedListView->setMovement(QListView::Movement::Static);
        m_recommendedListView->setFlow(QListView::Flow::TopToBottom);
        m_recommendedListView->setScrollBarOrientation(Qt::Horizontal);
        BaseCategoryModel *model = qobject_cast<BaseCategoryModel*>(m_dataProvider->recommendedModel());
        Q_ASSERT(model != Q_NULLPTR);
        model->initData();
        m_recommendedListView->setModel(model);
        m_recommendedListView->setItemDelegate(m_recommedViewDelegate);
    }
    {
        m_recentListView = new RKListView;
        initListViewParameter(m_recentListView);
        m_recentListView->setFixedWidth(MAIN_VIEW_V_LISTVIEW_W);
        m_recentListView->setSpacing(8);
        m_recentListView->setScrollBarOrientation(Qt::Vertical);
        m_recentListView->setDefaultItemSize(QSize(MAIN_VIEW_V_LISTVIEW_W, MAIN_VIEW_LISTITEM_H));
        BaseCategoryModel *model = qobject_cast<BaseCategoryModel*>(m_dataProvider->recentModel());
        Q_ASSERT(model != Q_NULLPTR);
        model->initData();
        m_recentListView->setModel(model);
        m_recentListView->setItemDelegate(m_recentViewDelegate);
    }
    {
        m_allListView = new RKListView;
        initListViewParameter(m_allListView);
//        m_allListView->setFixedHeight(MAIN_VIEW_HL_VIEW_H);
        m_allListView->setDefaultItemSize(QSize(MAIN_VIEW_R_VIEW_COVER_W, MAIN_VIEW_R_VIEW_COVER_H));
        m_allListView->setSpacing(10);
        m_allListView->setViewMode(QListView::ViewMode::IconMode);
        m_allListView->setDragEnabled(false);
        m_allListView->setMovement(QListView::Movement::Static);
        m_allListView->setFlow(QListView::Flow::TopToBottom);
        m_allListView->setScrollBarOrientation(Qt::Horizontal);
        BaseCategoryModel *model = qobject_cast<BaseCategoryModel*>(m_dataProvider->allListModel());
        Q_ASSERT(model != Q_NULLPTR);
        model->initData();
        m_allListView->setModel(model);
        m_allListView->setItemDelegate(m_allViewDelegate);
    }


    initUI();

    connect(m_recommendedListView, &RKListView::entered,
            this, [&](const QModelIndex &idx) {
//                qDebug()<<Q_FUNC_INFO<<" entered "<<idx;
                BaseCategoryModel *model = qobject_cast<BaseCategoryModel*>(m_dataProvider->recommendedModel());
                Q_ASSERT(model != Q_NULLPTR);
                model->setCurIdx(idx);
            });

    connect(m_recentListView, &RKListView::entered,
            this, [&](const QModelIndex &idx) {
//                qDebug()<<Q_FUNC_INFO<<" entered "<<idx;
                BaseCategoryModel *model = qobject_cast<BaseCategoryModel*>(m_dataProvider->recentModel());
                Q_ASSERT(model != Q_NULLPTR);
                model->setCurIdx(idx);
            });

    connect(m_allListView, &RKListView::entered,
            this, [&](const QModelIndex &idx) {
//                qDebug()<<Q_FUNC_INFO<<" entered "<<idx;
                BaseCategoryModel *model = qobject_cast<BaseCategoryModel*>(m_dataProvider->allListModel());
                Q_ASSERT(model != Q_NULLPTR);
                model->setCurIdx(idx);
            });


}

BaseCategoryView::~BaseCategoryView()
{
    if (m_dataProvider) {
        delete m_dataProvider;
        m_dataProvider = Q_NULLPTR;
    }
}

void BaseCategoryView::initUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    {
        QLabel *label = new QLabel;
        label->setObjectName("TitleLabel");
        label->setText(tr("Recommended") + " " + m_dataProvider->title());
        layout->addWidget(label);
    }
    layout->addSpacing(20);
    {
        layout->addWidget(m_recommendedListView);
    }
    layout->addSpacing(33);
    {
        //bottom part label
        QHBoxLayout *hb = new QHBoxLayout;
        hb->setContentsMargins(0, 0, 0, 0);
        {
            //left part
            QVBoxLayout *vb = new QVBoxLayout;
            vb->setContentsMargins(0, 0, 0, 0);
            vb->setSpacing(0);
            {
                QLabel *label = new QLabel;
                label->setObjectName("TitleLabel");
                label->setText(tr("Recently Released"));
                vb->addWidget(label);
                vb->addSpacing(13);
            }
            {
                //title header for listview
                QHBoxLayout *hh = new QHBoxLayout;
                hh->setContentsMargins(0, 0, 0, 0);
                hh->setSpacing(0);
                QLabel *label = new QLabel;
                label->setObjectName("ListTitleLabel");
                label->setText(tr("Song"));
                label->setFixedWidth(MAIN_VIEW_LISTITEM_LEFT_CONTENT_W - MAIN_VIEW_LISTITEM_CONTENT_MARGIN);
                hh->addWidget(label);
                hh->addSpacing(MAIN_VIEW_LISTITEM_CONTENT_MARGIN*3);

                label = new QLabel;
                label->setObjectName("ListTitleLabel");
                label->setText(m_dataProvider->title());
                label->setFixedWidth(MAIN_VIEW_LISTITEM_MIDDLE_CONTENT_W - MAIN_VIEW_LISTITEM_CONTENT_MARGIN*2);
                hh->addWidget(label);

                label = new QLabel;
                label->setObjectName("ListTitleLabel");
                label->setText(tr("time"));
                label->setFixedWidth(MAIN_VIEW_LISTITEM_RIGHT_CONTENT_W);
                hh->addWidget(label);

                vb->addLayout(hh);
            }
            {
                vb->addWidget(m_recentListView);
            }
            hb->addLayout(vb);
        }
        {
            // right part
            QVBoxLayout *vb = new QVBoxLayout;
            vb->setContentsMargins(0, 0, 0, 0);
            vb->setSpacing(0);

            QLabel *label = new QLabel;
            label->setObjectName("TitleLabel");
            label->setText(tr("All") + " " + m_dataProvider->title());
            vb->addWidget(label);
            vb->addSpacing(13);

            vb->addWidget(m_allListView);

            hb->addLayout(vb);
        }
        layout->addLayout(hb);
    }
    this->setLayout(layout);

}

void BaseCategoryView::initListViewParameter(QListView *view)
{
    if (!view) {
        return;
    }
    view->setDragEnabled(false);
    view->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    view->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    view->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

