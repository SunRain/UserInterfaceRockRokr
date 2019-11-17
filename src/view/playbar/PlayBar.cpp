#include "PlayBar.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QEvent>

#include <DThemeManager>

#include "LibPhoenixPlayerMain.h"
#include "PPCommon.h"
#include "PPUtility.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/VolumeControl.h"
#include "MusicLibrary/MusicLibraryManager.h"
#include "UserInterface/UserInterfaceMgr.h"
#include "UserInterface/IUserInterface.h"
#include "DataProvider/TinySpectrumDataProvider.h"
#include "DataProvider/ISpectrumGenerator.h"

#include "rockrokr_global.h"
#include "RKMainWindow.h"
#include "UserInterfaceRockRokr.h"

#include "widget/RKImage.h"
#include "widget/RKSlider.h"
#include "widget/RKIconButton.h"

#include "VolumeWidget.h"
#include "WaveformSlider.h"

DWIDGET_USE_NAMESPACE;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static char *S_PROPERTY_TIP_TEXT              = "S_PROPERTY_TIP_TEXT";
const static char *S_PROPERTY_TIP_HIDE_DELAY_MS     = "S_PROPERTY_TIP_HIDE_DELAY_MS";
const static char *S_OBJ_NAME_VOLUME_BTN            = "RKSoundBtn";
const static char *S_PROPERTY_VOLUME_BTN_ICON       = "iconName";
const static char *S_VOLUME_BTN_MUTED               = "iconMuted";
const static char *S_VOLUME_BTN_1_3                 = "icon1_3";
const static char *S_VOLUME_BTN_1_2                 = "icon1_2";
const static char *S_VOLUME_BTN_2_3                 = "icon2_3";
const static char *S_VOLUME_BTN_3                   = "icon3";


class FilterObject : public QObject
{
    Q_OBJECT
public:
    FilterObject(UserInterfaceRockRokr *ui, QObject *parent = Q_NULLPTR)
        : QObject(parent),
          m_ui(ui)
    {

    }
    virtual ~FilterObject() override {}

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        switch (event->type()) {
        case QEvent::Enter: {
            auto w = qobject_cast<QWidget *>(watched);
            if (!w) {
                break;
            }
            w->setCursor(QCursor(Qt::PointingHandCursor));
            if (w->objectName() == S_OBJ_NAME_VOLUME_BTN) {
                PlayBar *bar = qobject_cast<PlayBar *>(w->parentWidget());
                if (bar) {
                    bar->showVolumeView();
                }
                break;
            }
            const QString text = w->property(S_PROPERTY_TIP_TEXT).toString();
            if (!text.isEmpty()) {
                m_ui->mainWindow()->showTip(w, text);
            }
            break;
        }
        case QEvent::Leave: {
            auto w = qobject_cast<QWidget *>(watched);
            if (!w) {
                break;
            }
            w->unsetCursor();
            if (w->objectName() == S_OBJ_NAME_VOLUME_BTN) {
                PlayBar *bar = qobject_cast<PlayBar *>(w->parentWidget());
                if (bar) {
                    bar->deleyHideVolumeView();
                }
                break;
            }
            const QString text = w->property(S_PROPERTY_TIP_TEXT).toString();
            const int delay = w->property(S_PROPERTY_TIP_HIDE_DELAY_MS).toInt();
            if (!text.isEmpty()) {
                m_ui->mainWindow()->hideTip(delay);
            }
            break;
        }
        case QEvent::MouseButtonPress:
            m_ui->mainWindow()->hideTip();
            break;
        default:
            break;
        }
        return QObject::eventFilter(watched, event);
    }
private:
    UserInterfaceRockRokr *m_ui = Q_NULLPTR;
};


PlayBar::PlayBar(QWidget *parent)
    : QFrame(parent),
      m_ppCommon(Q_NULLPTR)
{
    this->setObjectName("PlayBar");
    DThemeManager::instance()->registerWidget(this);

    this->setFrameStyle(QFrame::NoFrame);

    m_playCore      = new PlayerCore(this);
    m_volumeCtrl    = new VolumeControl(this);
    m_libraryMgr    = new MusicLibrary::MusicLibraryManager(this);
    m_uiMgr         = new UserInterface::UserInterfaceMgr(this);
    m_ui            = qobject_cast<UserInterfaceRockRokr*>(m_uiMgr->usedInterface());
    m_filter        = new FilterObject(m_ui, this);

    initUserInterface();

    this->changeVolume(m_volumeCtrl->volume());
    m_volumeView->slider()->setValue(m_volumeCtrl->volume());

    connect(m_playCore, &PlayerCore::playModeChanged, this, &PlayBar::playModeChanged);

    connect(m_playCore, &PlayerCore::playBackendStateChanged,
            this, [&](PPCommon::PlayBackendState state) {
        if (PPCommon::PlayBackendPlaying == state) {
            m_playPausedIcon->setProperty(S_PROPERTY_TIP_TEXT, tr("play"));
            m_playPausedIcon->setProperty("PlayBackendState", "play");
        } else if (PPCommon::PlayBackendStopped == state) {
            m_playPausedIcon->setProperty(S_PROPERTY_TIP_TEXT, tr("stop"));
            m_playPausedIcon->setProperty("PlayBackendState", "stop");
        } else {
            m_playPausedIcon->setProperty(S_PROPERTY_TIP_TEXT, tr("pause"));
            m_playPausedIcon->setProperty("PlayBackendState", "pause");
        }
        this->style()->unpolish(m_playPausedIcon);
        this->style()->polish(m_playPausedIcon);
    });
    connect(m_playCore, &PlayerCore::playTickActual, m_waveformSlider, &WaveformSlider::setPlayTickActual);

    connect(m_playCore, &PlayerCore::trackChanged,
            this, [&](const QVariantMap &map) {
                AudioMetaObject obj = AudioMetaObject::fromMap(map);
                if (obj.isHashEmpty() ||
                    obj.mediaType() == PPCommon::MediaType::MediaTypeUrl) {
                    return;
                }
                m_waveformSlider->setSpectrumData(m_libraryMgr->loadSpectrumData(obj),
                                                  obj.trackMeta().duration());
            });

    connect(m_waveformSlider, &WaveformSlider::valueChanged,
            this, [&](int value){
        m_playedTime->setText(PPUtility::formateSongDuration(value));
    });
    connect(m_waveformSlider, &WaveformSlider::sliderReleasedAt,
            this, [&](qreal value){
        m_playCore->setPosition(value, false);
    });

    connect(m_volumeCtrl, &VolumeControl::volumeChanged,
            this, [&](int volume) {
//        qDebug()<<Q_FUNC_INFO<<" VolumeControl::volumeChanged is slider moveing "<<m_volumeView->slider()->isMoving();
        if (m_volumeView->isVisible()) {
            m_volumeView->slider()->setValue(volume);
        }
        this->changeVolume(volume);
    });
    connect(m_volumeCtrl, &VolumeControl::mutedChanged,
            this, [&](bool muted) {
        if (muted) {
            m_btnSound->setProperty(S_PROPERTY_VOLUME_BTN_ICON, S_VOLUME_BTN_MUTED);
            m_preVolumeIcon = QString(S_VOLUME_BTN_MUTED);
            this->style()->unpolish(m_btnSound);
            this->style()->polish(m_btnSound);
        }
    });

    connect(m_btnOrderOrRepeat, &QPushButton::clicked,
            this, [&](){
        int mode = m_playCore->playModeInt();
        if (PPCommon::PlayMode::PlayModeShuffle == mode) {
            mode = (int)PPCommon::PlayModeOrder;
        } else {
            if(PPCommon::PlayMode::PlayModeShuffle == ++mode) {
                mode = (int)PPCommon::PlayModeOrder;
            }
        }
        m_playCore->setPlayModeInt(mode);
    });
    connect(m_btnShuffle, &QPushButton::clicked,
            this, [&](){
        int mode = m_playCore->playModeInt();
        if (PPCommon::PlayModeShuffle == mode) {
            m_playCore->setPlayMode(PPCommon::PlayModeOrder);
        } else {
            m_playCore->setPlayMode(PPCommon::PlayModeShuffle);
        }
    });
    connect(m_playPausedIcon, &QPushButton::clicked,
            this, [&](){
        m_playCore->togglePlayPause();
    });


    connect(m_volumeView->slider(), &RKSlider::valueChanged,
            this, [&](int value){
        if (value == 0) {
            m_volumeCtrl->setMuted(true);
        } else {
            m_volumeCtrl->setVolume(value);
        }
    });
}

PlayBar::~PlayBar()
{
    m_btnOrderOrRepeat->removeEventFilter(m_filter);
    m_btnShuffle->removeEventFilter(m_filter);
    m_btnSound->removeEventFilter(m_filter);
    m_btnPre->removeEventFilter(m_filter);
    m_btnNext->removeEventFilter(m_filter);
    m_playPausedIcon->removeEventFilter(m_filter);

    if (m_filter) {
        m_filter->deleteLater();
        m_filter = Q_NULLPTR;
    }

    if (m_ppCommon) {
        m_ppCommon->deleteLater();
        m_ppCommon = Q_NULLPTR;
    }
    if (m_playCore) {
        m_playCore->deleteLater();
        m_playCore = Q_NULLPTR;
    }
    if (m_volumeCtrl) {
        m_volumeCtrl->deleteLater();
        m_volumeCtrl = Q_NULLPTR;
    }
    if (m_libraryMgr) {
        m_libraryMgr->deleteLater();
        m_libraryMgr = Q_NULLPTR;
    }
    if (m_spekProvider) {
        m_spekProvider->deleteLater();
        m_spekProvider = Q_NULLPTR;
    }
    if (m_uiMgr) {
        m_uiMgr->deleteLater();
        m_uiMgr = Q_NULLPTR;
    }
}

void PlayBar::showVolumeView()
{
    ///FIX fix parent widget of volumeview
    if (m_volumeView && !m_volumeView->parentWidget()) {
        m_volumeView->setParent(m_ui->mainWindow());
    }
    //TODO only calculate pos once ??
    auto centerPos = m_btnSound->mapToGlobal(m_btnSound->rect().center());
    m_volumeView->show();
    m_volumeView->adjustSize();

    auto sz = m_volumeView->size();
    centerPos.setX(centerPos.x()  - sz.width() / 2);
    //TODO use correct offset, current is 20 (just for tmp)
    centerPos.setY(centerPos.y() - sz.height() - 20);
    centerPos = m_volumeView->mapFromGlobal(centerPos);
    centerPos = m_volumeView->mapToParent(centerPos);
    m_volumeView->move(centerPos);
    m_volumeView->raise();
}

void PlayBar::deleyHideVolumeView()
{
    m_volumeView->deleyHide();
}

void PlayBar::initUserInterface()
{
    m_volumeView = new VolumeWidget;
    m_volumeView->hide();


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN);

    m_cover = new RKImage;
    m_cover->setObjectName("RKCover");
    m_cover->setFixedSize(PLAY_BAR_COVER_W, PLAY_BAR_COVER_H);

    layout->addWidget(m_cover);

    {
        QVBoxLayout *ly = new QVBoxLayout;
        ly->setContentsMargins(2, 0, 2, 0);
        ly->setSpacing(0);
        ly->setAlignment(Qt::AlignLeft);

        m_title = new QLabel;
        m_title->setObjectName("RKTitle");
        m_title->setMaximumWidth(PLAY_BAR_TITLE_W);
        m_title->setText(tr("Unknow Title"));

        m_artist = new QLabel;
        m_artist->setObjectName("RKArtist");
        m_artist->setMaximumWidth(PLAY_BAR_TITLE_W);
        m_artist->setText(tr("Unknow Artist"));

        ly->addStretch();
        ly->addWidget(m_title);
        ly->addWidget(m_artist);
        ly->addStretch();

        layout->addLayout(ly);
    }
    layout->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN *3);
    layout->addStretch();
    {
        m_waveformSlider = new WaveformSlider;
        m_waveformSlider->setObjectName("RKWaveformSlider");
//        m_waveformSlider->setFixedSize(PLAY_BAR_SLIDER_W, PLAY_BAR_SLIDER_H);
        m_waveformSlider->setFixedHeight(PLAY_BAR_SLIDER_H);
        layout->addWidget(m_waveformSlider, Qt::AlignVCenter);
    }
    {
        m_playedTime = new QLabel;
        m_playedTime->setObjectName("RKPlayedTime");
        m_playedTime->setMaximumWidth(PLAY_BAR_TIME_LABEL_W);
        m_playedTime->setText("00.00");

        m_totalTime = new QLabel;
        m_totalTime->setObjectName("RKTotalTime");
        m_totalTime->setMaximumWidth(PLAY_BAR_TIME_LABEL_W);
        m_totalTime->setText("00:00");

        QVBoxLayout *ly = new QVBoxLayout;
        ly->setContentsMargins(2, 0, 2, 0);
        ly->setSpacing(0);
        ly->setAlignment(Qt::AlignLeft);

        ly->addStretch();
        ly->addWidget(m_playedTime);
        ly->addWidget(m_totalTime);
        ly->addStretch();

        layout->addLayout(ly);
    }
    layout->addStretch();
    layout->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN *2);
    {
        QHBoxLayout *ly = new QHBoxLayout;
        ly->setContentsMargins(2, 0, 2, 0);
        ly->setSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN/4);
        ly->setAlignment(Qt::AlignLeft);

        m_btnOrderOrRepeat = new RKIconButton;
        m_btnOrderOrRepeat->setObjectName("RKRepeatBtn");
        m_btnOrderOrRepeat->setFixedSize(PLAY_BAR_SMALL_BTN_W, PLAY_BAR_SMALL_BTN_H);
        m_btnOrderOrRepeat->setProperty(S_PROPERTY_TIP_TEXT, tr("RKRepeatBtn"));
        m_btnOrderOrRepeat->installEventFilter(m_filter);

        m_btnShuffle = new RKIconButton;
        m_btnShuffle->setObjectName("RKShuffleBtn");
        m_btnShuffle->setFixedSize(PLAY_BAR_SMALL_BTN_W, PLAY_BAR_SMALL_BTN_H);
        m_btnShuffle->setProperty(S_PROPERTY_TIP_TEXT, tr("RKShuffleBtn"));
        m_btnShuffle->installEventFilter(m_filter);

        m_btnSound = new RKIconButton;
        m_btnSound->setObjectName(S_OBJ_NAME_VOLUME_BTN);
        m_btnSound->setFixedSize(PLAY_BAR_SMALL_BTN_W, PLAY_BAR_SMALL_BTN_H);
        m_btnSound->installEventFilter(m_filter);

        m_btnPre = new RKIconButton;
        m_btnPre->setObjectName("RKPreBtn");
        m_btnPre->setFixedSize(PLAY_BAR_SMALL_BTN_W, PLAY_BAR_SMALL_BTN_H);
        m_btnPre->setProperty(S_PROPERTY_TIP_TEXT, tr("RKPreBtn"));
        m_btnPre->installEventFilter(m_filter);

        m_btnNext = new RKIconButton;
        m_btnNext->setObjectName("RKNextBtn");
        m_btnNext->setFixedSize(PLAY_BAR_SMALL_BTN_W, PLAY_BAR_SMALL_BTN_H);
        m_btnNext->setProperty(S_PROPERTY_TIP_TEXT, tr("RKNextBtn"));
        m_btnNext->installEventFilter(m_filter);

        m_playPausedIcon = new RKIconButton;
        m_playPausedIcon->setObjectName("RKPlayPausedBtn");
        m_playPausedIcon->setFixedSize(PLAY_BAR_BIG_BTN_W, PLAY_BAR_BIG_BTN_H);
        m_playPausedIcon->setProperty(S_PROPERTY_TIP_TEXT, tr("Play"));
        m_playPausedIcon->installEventFilter(m_filter);

        ly->addWidget(m_btnOrderOrRepeat);
        ly->addWidget(m_btnShuffle);
        ly->addWidget(m_btnSound);
        ly->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN/2);
        ly->addWidget(m_btnPre);
        ly->addWidget(m_btnNext);
        ly->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN/2);
        ly->addWidget(m_playPausedIcon);

        layout->addLayout(ly);
    }
    layout->addStretch();

}

void PlayBar::changeVolume(int volume)
{
    if (volume == 0) {
        m_volumeCtrl->setMuted(true);
    } else if (volume > 0 && volume < 25) {
        m_btnSound->setProperty(S_PROPERTY_VOLUME_BTN_ICON, S_VOLUME_BTN_1_3);
    } else if (volume >= 25 && volume < 50) {
        m_btnSound->setProperty(S_PROPERTY_VOLUME_BTN_ICON, S_VOLUME_BTN_1_2);
    } else if (volume >=50 && volume < 75) {
        m_btnSound->setProperty(S_PROPERTY_VOLUME_BTN_ICON, S_VOLUME_BTN_2_3);
    } else {
        m_btnSound->setProperty(S_PROPERTY_VOLUME_BTN_ICON, S_VOLUME_BTN_3);
    }
    if (m_preVolumeIcon != m_btnSound->property(S_PROPERTY_VOLUME_BTN_ICON).toString()) {
        m_preVolumeIcon = m_btnSound->property(S_PROPERTY_VOLUME_BTN_ICON).toString();
        this->style()->unpolish(m_btnSound);
        this->style()->polish(m_btnSound);
    }
}

void PlayBar::resizeEvent(QResizeEvent *event)
{

}

void PlayBar::playModeChanged(PPCommon::PlayMode mode)
{
    if (!m_ppCommon) {
        m_ppCommon = new PPCommon;
    }
    const QString modeStr = m_ppCommon->enumToStr("PlayMode", (int)mode);
    if (PPCommon::PlayMode::PlayModeShuffle == mode) {
        m_btnShuffle->setProperty("PlayMode", modeStr);
        m_btnOrderOrRepeat->setProperty("PlayMode", "");
        m_ui->mainWindow()->showTip(m_btnShuffle, modeStr);
    } else if (PPCommon::PlayMode::PlayModeOrder == mode) {
        m_btnShuffle->setProperty("PlayMode", "");
        m_btnOrderOrRepeat->setProperty("PlayMode", "");
        m_ui->mainWindow()->showTip(m_btnOrderOrRepeat, modeStr);
    } else {
        m_btnShuffle->setProperty("PlayMode", "");
        m_btnOrderOrRepeat->setProperty("PlayMode", modeStr);
        m_ui->mainWindow()->showTip(m_btnOrderOrRepeat, modeStr);
    }
    this->style()->unpolish(m_btnShuffle);
    this->style()->unpolish(m_btnOrderOrRepeat);
    this->style()->polish(m_btnShuffle);
    this->style()->polish(m_btnOrderOrRepeat);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#include "PlayBar.moc"
