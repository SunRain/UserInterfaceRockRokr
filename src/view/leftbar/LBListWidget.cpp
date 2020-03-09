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
    : RKListWidget(parent)
{
    this->setObjectName("LBListWidget");
    DThemeManager::instance()->registerWidget(this);


    this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, &LBListWidget::customContextMenuRequested,
            this, &LBListWidget::showContextMenu);
}

LBListWidget::~LBListWidget()
{

}




} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

