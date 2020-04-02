#include "LBListWidget.h"

#include "rockrokr_global.h"


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

LBListWidget::LBListWidget(QWidget *parent)
    : RKListWidget(parent)
{
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

