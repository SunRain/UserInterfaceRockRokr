#include "MainWindowBGView.h"

#include <QGraphicsDropShadowEffect>
#include <DThemeManager>

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

MainWindowBGView::MainWindowBGView(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("MainWindowBGView");
    DThemeManager::instance()->registerWidget(this);

//    <Canvas
//      Width="1400"
//      Height="800">
//      <Path
//        Data="M0 0L1400 0L1400 800L0 800L0 0Z"
//        Stretch="Fill"
//        Width="1401"
//        Height="800"
//        Fill="#FFFFFF">
//        <Path.Effect>
//          <DropShadowEffect
//            Color="#1F000000"
//            Opacity="0.12"
//            BlurRadius="80"
//            Direction="-90"
//            ShadowDepth="60" />
//        </Path.Effect>
//      </Path>
//    </Canvas>
    ///FIXME QGraphicsDropShadowEffect cause layout problem for inner widget why?
//    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
//    effect->setColor("31,00,00, 0.12");
//    effect->setBlurRadius(80);
//    effect->setOffset(0, 60);
//    this->setGraphicsEffect(effect);

}

MainWindowBGView::~MainWindowBGView()
{

}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
