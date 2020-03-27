#ifndef VIEWUTILITY_H
#define VIEWUTILITY_H

#include <QSize>
#include <QImage>

#include "AudioMetaObject.h"
#include "PlayerCore/PlayListMeta.h"

class QMenu;
namespace PhoenixPlayer {
    class PlayerCore;
    class PlayListMetaMgr;

    namespace DataProvider {
        class TrackSearchProvider;
    } // namespace DataProvider

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

    static void showCategoryDetailView();

    static void showPlaylistDetailView();

    static void menuAddToQueue(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj, PlayerCore *core);

    static void menuAddToPlaylist(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj, PlayListMetaMgr *mgr);

    static void menuShowInFileMgr(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);

    static void menuRemoveObject(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);

    static void menuTrackInfo(QMenu *menu, const PhoenixPlayer::AudioMetaObject &obj);

    static void showSearchPage();

    static void showRockRokrPage();
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // VIEWUTILITY_H
