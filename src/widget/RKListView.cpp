#include "RKListView.h"

#include <QScrollBar>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include "QPropertyAnimation"

#include <DThemeManager>

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKListView::RKListView(QWidget *parent)
    : QListView(parent)
{
    this->setObjectName("RKListView");
    DThemeManager::instance()->registerWidget(this);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setResizeMode(QListView::ResizeMode::Adjust);
    this->setMouseTracking(true);

    m_defaultItemSize = QSize(MAIN_VIEW_HL_COVER_W, MAIN_VIEW_HL_COVER_H);

    m_scrollBar = new QScrollBar(this);
    m_scrollBar->setObjectName("RKScrollBar");
    m_scrollBar->setSingleStep(1);

    m_effect = new QGraphicsOpacityEffect(m_scrollBar);
    m_scrollBar->setGraphicsEffect(m_effect);

    m_animation = new QPropertyAnimation;
    m_animation->setTargetObject(m_effect);
    m_animation->setPropertyName("opacity");
    m_animation->setDuration(1000);
    m_animation->setKeyValueAt(0.4, 1.0);
    m_animation->setKeyValueAt(0.8, 1.0);

    connect(m_scrollBar, &QScrollBar::valueChanged,
            this, [&](int value) {
                qDebug()<<" QScrollBar::valueChanged "<<value;

        this->getInnerScrollBar()->setValue(value);
        emit this->scrollValueChanged(value);
    });

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(500);
    connect(m_timer, &QTimer::timeout,
            this, [&](){
        qDebug()<<" timer to hide scrollbar ";
        if (!m_mouseIn) {
            this->animateHideScrollbar(true);
        }
    });

}

RKListView::~RKListView()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_timer->deleteLater();
    m_timer = Q_NULLPTR;
    if (m_animation->state() == QPropertyAnimation::State::Running) {
        m_animation->stop();
    }
    m_animation->deleteLater();
    m_animation = Q_NULLPTR;
}

void RKListView::setScrollBarOrientation(Qt::Orientation orientation)
{
    m_scrollBar->setOrientation(orientation);
    this->style()->unpolish(m_scrollBar);
    this->style()->polish(m_scrollBar);
    updateScrollBar();
}

void RKListView::wheelEvent(QWheelEvent *event)
{
    QListView::wheelEvent(event);
    m_scrollBar->setSliderPosition(this->getInnerScrollBar()->sliderPosition());
}

void RKListView::enterEvent(QEvent *event)
{
    m_mouseIn = true;
    if (m_scrollBarState) {
        if (m_timer->isActive()) {
            m_timer->stop();
        }
        animateShowScrollbar(true);
    }
//    QListView::enterEvent(event);
}

void RKListView::leaveEvent(QEvent *event)
{
    m_mouseIn = false;
    if (m_scrollBarState && !m_timer->isActive()) {
        m_timer->start();
    }
    QListView::leaveEvent(event);
    emit entered(QModelIndex());
}

void RKListView::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);
    static const int scSize = MAIN_VIEW_SCROLLBAR_SIZE;
    const QSize size = event->size();
    if (m_scrollBar->orientation() == Qt::Orientation::Vertical) {
        m_scrollBar->resize(scSize, size.height());
        m_scrollBar->move(size.width()-scSize, 0);
    } else {
        m_scrollBar->resize(size.width(), scSize);
        m_scrollBar->move(0, size.height()-scSize);
    }
    updateScrollBar();
}

void RKListView::mouseMoveEvent(QMouseEvent *event)
{
    QListView::mouseMoveEvent(event);
    QModelIndex idx = indexAt(event->pos());
    emit entered(idx);
}

void RKListView::setModel(QAbstractItemModel *model)
{
    qDebug()<<" row count "<<model->rowCount();

    QListView::setModel(model);
    updateScrollBar();
}

void RKListView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    QListView::setSelectionModel(selectionModel);
}

void RKListView::setDefaultItemSize(const QSize &size)
{
    m_defaultItemSize = size;
    updateScrollBar();
}

void RKListView::setTableHeaders(const QMap<RKTableHeaderItem::HeaderColumn, int> &map)
{
    m_map.clear();
    m_map = map;
    this->update();
}

QMap<RKTableHeaderItem::HeaderColumn, int> RKListView::getTableHeaders() const
{
    return m_map;
}

QScrollBar *RKListView::getInnerScrollBar() const
{
    if (m_scrollBar->orientation() == Qt::Orientation::Vertical) {
        return this->verticalScrollBar();
    } else {
        return this->horizontalScrollBar();
    }
}

void RKListView::updateScrollBar()
{
    if (!this->model() || !m_defaultItemSize.isValid()) {
        qDebug()<<"no model or invalid defaultItemSize";
        return;
    }
    const QSize size = this->size();
    int itemSize = 0;
    if (this->model()->rowCount() > 0) {
        itemSize += (this->model()->rowCount() - 1) * this->spacing();
    }
    qDebug()<<" this size "<<size
             <<" m_defaultItemSize "<<m_defaultItemSize
             <<" orientation "<<m_scrollBar->orientation()
             <<" model row count "<<model()->rowCount()
             <<" model columnCount "<<model()->columnCount();

    if (m_scrollBar->orientation() == Qt::Orientation::Vertical) {
        itemSize += m_defaultItemSize.height() * this->model()->rowCount();
        if (itemSize > size.height()) {
            m_scrollBarState = true;
//            m_scrollBar->setPageStep(size.height());
//            m_scrollBar->setSingleStep(size.height());
//            m_scrollBar->setMaximum(itemSize - size.height());
            m_scrollBar->setPageStep(size.height()/m_defaultItemSize.height());
            m_scrollBar->setMaximum(this->model()->rowCount() - size.height()/m_defaultItemSize.height());
            m_scrollBar->setSingleStep(1);
        }
    } else {
        int delta = size.height()/m_defaultItemSize.height();
        int count = this->model()->rowCount();
        if (delta > 0) {
            count = count / delta;
        }
        itemSize += m_defaultItemSize.width() *count;
        if (itemSize > size.width()) {
            m_scrollBarState = true;
            m_scrollBar->setPageStep(size.width());
            m_scrollBar->setSingleStep(size.width());
            m_scrollBar->setMaximum(itemSize - size.width());
        }
    }
    if (m_scrollBarState) {
        animateShowScrollbar(false);
    } else {
        animateHideScrollbar(false);
        m_scrollBar->setMaximum(0);
    }
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_timer->start();
}

void RKListView::animateShowScrollbar(bool animate)
{
    if (!animate) {
        m_effect->setOpacity(1.0);
        return;
    }
    m_animation->setStartValue(0);
    m_animation->setEndValue(1.0);
    m_animation->start();
}

void RKListView::animateHideScrollbar(bool animate)
{
    if (!animate) {
        m_effect->setOpacity(0);
        return;
    }
    m_animation->setStartValue(1.0);
    m_animation->setEndValue(0);
    m_animation->start();
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
