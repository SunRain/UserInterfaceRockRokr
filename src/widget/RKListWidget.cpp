#include "RKListWidget.h"

#include <QDebug>

#include <DThemeManager>

DWIDGET_USE_NAMESPACE

RKListWidget::RKListWidget(QWidget *parent)
    : QListWidget(parent),
      m_contentHeight(0)
{
    this->setObjectName("RKListWidget");
    DThemeManager::instance()->registerWidget(this);

    this->setDragEnabled(false);
    this->setSelectionMode(QListView::SingleSelection);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
}

RKListWidget::~RKListWidget()
{

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

int RKListWidget::contentHeight() const
{
    return m_contentHeight;
}

void RKListWidget::updateHeight()
{
    m_contentHeight = 0;
    for (int i = 0; i < this->count(); ++i) {
        QListWidgetItem *item = this->item(i);
        if (item) {
            //TODO item is hidden?
            m_contentHeight += item->sizeHint().height();
            qDebug()<<Q_FUNC_INFO<<" item "<<i<<" height "<<item->sizeHint().height();
        }
    }
    if (this->count() > 0) {
        m_contentHeight += (this->count()-1) * this->spacing();
    }
    emit contentHeightChanged(m_contentHeight);
}









