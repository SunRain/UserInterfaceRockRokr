#include "RKListWidget.h"

#include <QDebug>
#include <QResizeEvent>

#include <DThemeManager>

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKListWidget::RKListWidget(QWidget *parent)
    : QListWidget(parent),
      m_scrollBar(new QScrollBar(this)),
      m_animation(new QPropertyAnimation),
      m_timer(new QTimer),
      m_effect(new QGraphicsOpacityEffect(m_scrollBar))
{
    this->setObjectName("RKListWidget");
    DThemeManager::instance()->registerWidget(this);

    this->setDragEnabled(false);
    this->setSelectionMode(QListView::SingleSelection);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setResizeMode(QListView::ResizeMode::Adjust);

    m_effect->setOpacity(1.0);

    m_animation->setTargetObject(m_effect);
    m_animation->setPropertyName("opacity");
    m_animation->setDuration(2000);
    m_animation->setKeyValueAt(0.4, 1.0);
    m_animation->setKeyValueAt(0.8, 1.0);

    m_scrollBar->setGraphicsEffect(m_effect);
    m_scrollBar->setObjectName("InnerScrollBar");
    m_scrollBar->setOrientation(Qt::Orientation::Vertical);

    connect(m_scrollBar, &QScrollBar::valueChanged, this, [&](int value) {
        this->verticalScrollBar()->setValue(value);
        emit this->scrollValueChanged(value);
    });

    m_timer->setSingleShot(true);
    m_timer->setInterval(500);
    connect(m_timer, &QTimer::timeout, this, [&](){
        if (!m_mouseIn) {
            this->animateHideScrollbar(true);
        }
    });

}

RKListWidget::~RKListWidget()
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

void RKListWidget::addItem(QListWidgetItem *item)
{
    QListWidget::addItem(item);
    updateHeight();
}

void RKListWidget::insertItem(int row, QListWidgetItem *item)
{
    QListWidget::insertItem(row, item);
    updateHeight();
}

QListWidgetItem *RKListWidget::takeItem(int row)
{
    QListWidgetItem *item = QListWidget::takeItem(row);
    updateHeight();
    return item;
}

void RKListWidget::enableScrollBar(bool enable)
{
    m_enableScrollBar = enable;
    m_scrollBar->setVisible(enable);
}

int RKListWidget::contentHeight() const
{
    return m_contentHeight;
}

void RKListWidget::calculateContentHeight()
{
    updateHeight();
}

void RKListWidget::wheelEvent(QWheelEvent *event)
{
    QListWidget::wheelEvent(event);
    m_scrollBar->setValue(this->verticalScrollBar()->value());
}

void RKListWidget::enterEvent(QEvent *event)
{
    m_mouseIn = true;
    if (m_scrollBarState && m_enableScrollBar) {
        if (m_timer->isActive()) {
            m_timer->stop();
        }
        animateShowScrollbar(true);
    }
    QListWidget::enterEvent(event);
}

void RKListWidget::leaveEvent(QEvent *event)
{
    m_mouseIn = false;
    if (m_scrollBarState && !m_timer->isActive() && m_enableScrollBar) {
        m_timer->start();
    }
    QListWidget::leaveEvent(event);
}

void RKListWidget::resizeEvent(QResizeEvent *event)
{
    QListWidget::resizeEvent(event);
    static const int scW = LEFT_BAR_SCROLLBAR_W;
    const QSize size = event->size();
    m_scrollBar->resize(scW, size.height());
    m_scrollBar->move(size.width()-scW, 0);

    updateHeight();
}

void RKListWidget::updateHeight()
{
    if ((this->count() == 0) || (this->height() == 0)) {
        return;
    }

    int height = 0;
    for (int i = 0; i < this->count(); ++i) {
        QListWidgetItem *item = this->item(i);
        if (item) {
            //TODO item is hidden?
            height += item->sizeHint().height();
       }
    }
    if (this->count() > 0) {
        height += (this->count()-1) * this->spacing();
    }

    const QSize size = this->size();
    const int averageItemH = height / this->count();
    m_scrollBar->setSingleStep(1);
    m_scrollBar->setPageStep(size.height() / averageItemH);

    if (height > size.height()) {
        m_scrollBarState = true;
        m_scrollBar->setMaximum(this->count() - size.height()/averageItemH);
        animateShowScrollbar(false);
    } else {
        m_scrollBarState = false;
        animateHideScrollbar(false);
        m_scrollBar->setMaximum(0);
    }

    if (m_timer->isActive()) {
        m_timer->stop();
    }
    if (m_enableScrollBar) {
        m_timer->start();
    }

    if (height != m_contentHeight) {
        m_contentHeight = height;
        emit contentHeightChanged(m_contentHeight);
    }
}

void RKListWidget::animateShowScrollbar(bool animate)
{
    if (!m_enableScrollBar) {
        return;
    }
    if (!animate) {
        m_effect->setOpacity(1.0);
        return;
    }
    m_animation->setStartValue(0);
    m_animation->setEndValue(1.0);
    m_animation->start();
}

void RKListWidget::animateHideScrollbar(bool animate)
{
    if (!m_enableScrollBar) {
        return;
    }
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






