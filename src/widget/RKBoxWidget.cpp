#include "RKBoxWidget.h"

#include <QDebug>
#include <QEvent>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKBoxWidget::RKBoxWidget(QBoxLayout::Direction direction, QWidget *parent)
    : DBoxWidget(direction, parent)
{

}

RKBoxWidget::~RKBoxWidget()
{

}

bool RKBoxWidget::event(QEvent *event)
{
//    if(event->type() == QEvent::Resize) {
//        qDebug()<<Q_FUNC_INFO<<" *****************- "<<event->type();
//    } else if(event->type() == QEvent::ChildAdded) {
//        qDebug()<<Q_FUNC_INFO<<" *****************- "<<event->type();
//    } else if(event->type() == QEvent::ChildRemoved) {
//        qDebug()<<Q_FUNC_INFO<<" *****************- "<<event->type();
//    } else if(event->type() == QEvent::Show) {
//        qDebug()<<Q_FUNC_INFO<<" *****************- "<<event->type();;
//    }
    return DBoxWidget::event(event);
}

void RKBoxWidget::updateSize(const QSize &size)
{
//     qDebug()<<Q_FUNC_INFO<<" *****************-  "<<size;
     DBoxWidget::updateSize(size);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
