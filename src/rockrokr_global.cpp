#include "rockrokr_global.h"

#include <QPixmap>
#include <QSize>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QImageReader>
#include <QGuiApplication>
#include <QScreen>
#include <qcompilerdetection.h>


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static qreal S_REF_DPI    = 96.0; //192.0;
const static qreal S_REF_HEIGHT = 900;//1080;////1600.0;
const static qreal S_REF_WIDTH  = 1600;// 1920;//2560.0;

static qreal S_RATIO        = -1.0;
static qreal S_RATIO_FONT   = -1.0;

inline void calcRatio() {
    QRect rect = QGuiApplication::primaryScreen()->geometry();
    qreal height = qMin(rect.width(), rect.height());
    qreal width = qMax(rect.width(), rect.height());
    qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
    S_RATIO = qMin(height/S_REF_HEIGHT, width/S_REF_WIDTH);
    S_RATIO_FONT = qMin(height*S_REF_DPI/(dpi*S_REF_HEIGHT), width*S_REF_DPI/(dpi*S_REF_WIDTH));
}

int _to_px(qint64 px)
{
    if (Q_UNLIKELY((S_RATIO <= 0) || (S_RATIO_FONT <= 0))) {
        calcRatio();
    }
    return px * S_RATIO;
}

int _to_font_px(qint64 px)
{
    if (Q_UNLIKELY((S_RATIO <= 0) || (S_RATIO_FONT <= 0))) {
        calcRatio();
    }
    return S_RATIO_FONT * px;
}

qreal _to_pxF(qint64 px)
{
    if (Q_UNLIKELY((S_RATIO <= 0) || (S_RATIO_FONT <= 0))) {
        calcRatio();
    }
    return px * S_RATIO;
}

qreal _to_font_pxF(qint64 px)
{
    if (Q_UNLIKELY((S_RATIO <= 0) || (S_RATIO_FONT <= 0))) {
        calcRatio();
    }
    return S_RATIO_FONT * px;
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
