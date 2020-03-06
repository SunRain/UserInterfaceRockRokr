#ifndef PLAYBAR_H
#define PLAYBAR_H

#include <QFrame>

#include "PPCommon.h"

class QLabel;
class QPushButton;

namespace PhoenixPlayer {

    class PlayerCore;
    class PPCommon;
    class VolumeControl;

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace DataProvider {
        class TinySpectrumDataProvider;
    }

    namespace UserInterface {

        class UserInterfaceMgr;

        namespace RockRokr {

            class RKImage;
            class RKIconButton;
            class FilterObject;
            class VolumeWidget;
            class WaveformSlider;
            class UserInterfaceRockRokr;

class PlayBar : public QFrame
{
    Q_OBJECT
public:
    explicit PlayBar(QWidget *parent = Q_NULLPTR);
    virtual ~PlayBar() override;

    void showVolumeView();
    void deleyHideVolumeView();

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void initUserInterface();
    void changeVolume(int volume);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void playModeChanged(PhoenixPlayer::PPCommon::PlayMode mode);

private:
    QString         m_preVolumeIcon;

    RKImage                 *m_cover            = Q_NULLPTR;
    QLabel                  *m_title            = Q_NULLPTR;
    QLabel                  *m_artist           = Q_NULLPTR;
    QLabel                  *m_playedTime       = Q_NULLPTR;
    QLabel                  *m_totalTime        = Q_NULLPTR;
    RKIconButton            *m_btnPre           = Q_NULLPTR;
    RKIconButton            *m_btnNext          = Q_NULLPTR;
    RKIconButton            *m_btnSound         = Q_NULLPTR;
    RKIconButton            *m_btnShuffle       = Q_NULLPTR;
    RKIconButton            *m_btnOrderOrRepeat = Q_NULLPTR;
    RKIconButton            *m_playPausedIcon   = Q_NULLPTR;
    VolumeWidget            *m_volumeView       = Q_NULLPTR;
    WaveformSlider          *m_waveformSlider   = Q_NULLPTR;

    UserInterfaceRockRokr                   *m_ui           = Q_NULLPTR;
    FilterObject                            *m_filter       = Q_NULLPTR;
    PlayerCore                              *m_playCore     = Q_NULLPTR;
    PPCommon                                *m_ppCommon     = Q_NULLPTR;
    VolumeControl                           *m_volumeCtrl   = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager       *m_libraryMgr   = Q_NULLPTR;
    DataProvider::TinySpectrumDataProvider  *m_spekProvider = Q_NULLPTR;
    UserInterfaceMgr                        *m_uiMgr        = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // PLAYBAR_H
