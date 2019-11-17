#include "LBListWidget.h"

#include <QScrollBar>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>

#include <DThemeManager>

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

LBListWidget::LBListWidget(QWidget *parent)
    : RKListWidget(parent),
//      m_itemsHeight(0),
      m_scrollBar(new QScrollBar(this)),
      m_animation(new QPropertyAnimation),
      m_timer(new QTimer),
      m_effect(new QGraphicsOpacityEffect(m_scrollBar)),
      m_scrollBarState(false),
      m_mouseIn(false)
{
    this->setObjectName("LBListWidget");
    DThemeManager::instance()->registerWidget(this);

//    setStyleSheet(QString("QWidget{background-color:gray;}"));

    m_effect->setOpacity(1.0);
    m_scrollBar->setGraphicsEffect(m_effect);

    m_animation->setTargetObject(m_effect);
    m_animation->setPropertyName("opacity");
    m_animation->setDuration(2000);
    m_animation->setKeyValueAt(0.4, 1.0);
    m_animation->setKeyValueAt(0.8, 1.0);
//    m_animation->setEasingCurve(QEasingCurve::OutInQuad);

    this->setDragEnabled(false);
    this->setSelectionMode(QListView::SingleSelection);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setResizeMode(QListView::ResizeMode::Adjust);

    this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, &LBListWidget::customContextMenuRequested,
            this, &LBListWidget::showContextMenu);

    m_scrollBar->setObjectName("LBScrollBar");
    m_scrollBar->setOrientation(Qt::Orientation::Vertical);
//    m_scrollBar->raise();

    connect(m_scrollBar, &QScrollBar::valueChanged,
            this, [&](int value) {
        this->verticalScrollBar()->setValue(value);
        emit this->scrollValueChanged(value);
    });

    m_timer->setSingleShot(true);
    m_timer->setInterval(/*m_animation->duration()*/500);
    connect(m_timer, &QTimer::timeout,
            this, [&](){
        qDebug()<<" timer to hide scrollbar ";
        if (!m_mouseIn) {
            this->animateHideScrollbar(true);
        }
    });
}

LBListWidget::~LBListWidget()
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

void LBListWidget::wheelEvent(QWheelEvent *event)
{
    QListWidget::wheelEvent(event);
    m_scrollBar->setValue(this->verticalScrollBar()->value());
}

void LBListWidget::resizeEvent(QResizeEvent *event)
{
//    qDebug()<<Q_FUNC_INFO<<" -------------- "<<event
//           <<"    "<<this->geometry();

//    qDebug()<<Q_FUNC_INFO<<" this point "<<this->geometry().topLeft()
//           << "this global  "<<this->mapToGlobal(this->geometry().topLeft())
//          <<" scrollbar "<<m_scrollBar->geometry()
//         <<" scrollbar global "<<m_scrollBar->mapToGlobal(m_scrollBar->geometry().topLeft());

    QListWidget::resizeEvent(event);
    static const int scW = LEFT_BAR_SCROLLBAR_W;
    const QSize size = event->size();
    m_scrollBar->resize(scW, size.height());
    m_scrollBar->move(size.width()-scW, 0);

//    qDebug()<<Q_FUNC_INFO<<" scrollbar "<<m_scrollBar->geometry();

    updateScrollBar();
}

void LBListWidget::enterEvent(QEvent *event)
{
    m_mouseIn = true;
    if (m_scrollBarState) {
        if (m_timer->isActive()) {
            m_timer->stop();
        }
        animateShowScrollbar(true);
    }
    QListWidget::enterEvent(event);
}

void LBListWidget::leaveEvent(QEvent *event)
{
    m_mouseIn = false;
    if (m_scrollBarState && !m_timer->isActive()) {
        m_timer->start();
    }
    QListWidget::leaveEvent(event);
}

void LBListWidget::updateHeight()
{
    RKListWidget::updateHeight();
    updateScrollBar();
}

void LBListWidget::updateScrollBar()
{
//    static const int scW = LEFT_BAR_SCROLLBAR_W;
    int itemH = contentHeight();
    const QSize size = this->size();
    const int averageItemH = itemH/this->count();
//    m_scrollBar->resize(scW, size.height());
//    m_scrollBar->move(size.width()-scW, 0);
    m_scrollBar->setSingleStep(1);
    m_scrollBar->setPageStep(size.height() / averageItemH);

//            qDebug()<<Q_FUNC_INFO<<" item h "<<itemH<<" size h "<<size.height();

    if (itemH > size.height()) {
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
    m_timer->start();
}

void LBListWidget::showContextMenu(const QPoint &pos)
{

}

void LBListWidget::animateShowScrollbar(bool animate)
{
    qDebug()<<"----------";

//    if (m_scrollBar->isVisible()) {
//        return;
//    }
//    m_scrollBar->show();
    if (!animate) {
//        m_scrollBar->setEnabled(true);
//        m_scrollBar->show();
//        m_scrollBar->setVisible(true);
        m_effect->setOpacity(1.0);
//        m_scrollBar->raise();
        return;
    }

//    if (m_animation->state() != QPropertyAnimation::State::Stopped) {
////        m_animation->stop();
////        if (!m_scrollBar->isVisible()) {
////            animateShowScrollbar(animate);
////        }
//        return;
//    }

//    m_scrollBar->setEnabled(true);
//    m_scrollBar->show();
//    m_scrollBar->setVisible(true);
//    m_scrollBar->raise();

    m_animation->setStartValue(0);
    m_animation->setEndValue(1.0);
    m_animation->start();
}

void LBListWidget::animateHideScrollbar(bool animate)
{
//    if (!m_scrollBar->isVisible()) {
//        return;
//    }
    qDebug()<<"----------";

    if (!animate) {
//        m_scrollBar->setEnabled(false);
//        m_scrollBar->setVisible(false);
        m_effect->setOpacity(0);
//        m_scrollBar->hide();
//        m_scrollBar->lower();
        return;
    }
//    if (m_animation->state() != QPropertyAnimation::State::Stopped) {
////        m_animation->stop();
////        if (m_scrollBar->isVisible()) {
////            animateHideScrollbar(animate);
////        }
//        return;
//    }

//    m_scrollBar->setEnabled(false);
//    m_scrollBar->lower();

    m_animation->setStartValue(1.0);
    m_animation->setEndValue(0);
    m_animation->start();
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

