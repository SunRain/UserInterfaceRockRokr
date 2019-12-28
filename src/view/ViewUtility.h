#ifndef VIEWUTILITY_H
#define VIEWUTILITY_H

#include <QSize>
#include <QImage>

#include "AudioMetaObject.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class ViewUtility
{
public:
    static QImage cropRect(const QImage &image, QSize sz);

    static void showTrackRemoveDialog(const PhoenixPlayer::AudioMetaObject &obj);

    static void showTrackInfoDialog(const PhoenixPlayer::AudioMetaObject &obj);

    static void showToast(const QString &msg);

    static void showTip(QWidget *baseWidget, const QString &text, int delayShowMilliseconds = 0);

};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // VIEWUTILITY_H
