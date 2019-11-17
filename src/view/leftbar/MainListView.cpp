#include "MainListView.h"

#include <QDebug>
#include <QMenu>
#include <QMoveEvent>

#include "rockrokr_global.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

MainListView::MainListView(QWidget *parent)
    : QListView(parent),
      m_popMenu(new QMenu(this)),
      m_contextMenuEnabled(false),
      m_curIdx(QModelIndex())
{
//    qDebug()<<Q_FUNC_INFO<<"-----------------------";
//    setFlow(QListView::Flow::TopToBottom);
    setLayoutMode(QListView::Batched);
    setResizeMode(QListView::Adjust);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setFrameStyle(QFrame::NoFrame);
    setMouseTracking(true);
//    setWrapping(true);
//    setSpacing(LEFT_BAR_LISTVIEW_SPACING);

//    m_popMenu->addAction(new QAction("aaaaa", this));
}

MainListView::~MainListView()
{

}

QMenu *MainListView::popMenu() const
{
    return m_popMenu;
}

void MainListView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex idx = indexAt(event->pos());

    if (m_curIdx != idx) {
        m_curIdx = idx;
        emit entered(m_curIdx);
    }
}

void MainListView::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_contextMenuEnabled) {
        if (this->indexAt(mapFromGlobal(QCursor::pos())).isValid()) {
            m_popMenu->exec(QCursor::pos());
        }
    } else {
        QListView::contextMenuEvent(event);
    }
}

bool MainListView::contextMenuEnabled() const
{
    return m_contextMenuEnabled;
}

void MainListView::setContextMenuEnabled(bool contextMenuEnabled)
{
    m_contextMenuEnabled = contextMenuEnabled;
}
} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

