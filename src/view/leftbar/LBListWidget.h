#ifndef LBLISTWIDGET_H
#define LBLISTWIDGET_H

#include <QPropertyAnimation>

#include "widget/RKListWidget.h"

class QScrollBar;
class QTimer;
class QGraphicsOpacityEffect;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class LBListWidget : public RKListWidget
{
    Q_OBJECT
public:
    explicit LBListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~LBListWidget() override;

protected:
    virtual void showContextMenu(const QPoint &pos) {}
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // LBLISTWIDGET_H
