#ifndef PLAYBAR_H
#define PLAYBAR_H

#include <QFrame>

#include "PPCommon.h"

namespace PhoenixPlayer {
    class PlayerCore;
    class PPCommon;
    class VolumeControl;
}

class QLabel;
class QPushButton;
class RKImage;
class RKIconButton;
class FilterObject;
class VolumeWidget;
class WaveformSlider;
class PlayBar : public QFrame
{
    Q_OBJECT
public:
    explicit PlayBar(QWidget *parent = Q_NULLPTR);
    virtual ~PlayBar() override;

    void showVolumeView();
    void deleyHideVolumeView();

protected:
    void initUI();
    void volumeChanged(int volume);

private:
    void playModeChanged(PhoenixPlayer::PPCommon::PlayMode mode);

private:
    QString         m_preVolumeIcon;
    RKImage                 *m_cover;
    QLabel                  *m_title;
    QLabel                  *m_artist;
    QLabel                  *m_playedTime;
    QLabel                  *m_totalTime;
    RKIconButton    *m_btnPre;
    RKIconButton    *m_btnNext;
    RKIconButton    *m_btnSound;
    RKIconButton    *m_btnShuffle;
    RKIconButton    *m_btnOrderOrRepeat;
    VolumeWidget    *m_volumeView;
    WaveformSlider  *m_waveformSlider;

    FilterObject    *m_filter;
    PhoenixPlayer::PlayerCore       *m_playCore;
    PhoenixPlayer::PPCommon         *m_ppCommon;
    PhoenixPlayer::VolumeControl    *m_volumeCtrl;



};

#endif // PLAYBAR_H
