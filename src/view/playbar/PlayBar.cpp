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
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/VolumeControl.h"

#include "rockrokr_global.h"
#include "RockRokrApp.h"
#include "MainWindow.h"

#include "widget/RKImage.h"
#include "widget/RKSlider.h"
#include "widget/qtmaterialiconbutton.h"

#include "VolumeWidget.h"
#include "WaveformSlider.h"


DWIDGET_USE_NAMESPACE;
using namespace PhoenixPlayer;


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
    FilterObject(QObject *parent = Q_NULLPTR)
        : QObject(parent)
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
                RockRokrApp::instance()->mainWindow()->showTip(w, text);
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
                RockRokrApp::instance()->mainWindow()->hideTip(delay);
            }
            break;
        }
        case QEvent::MouseButtonPress:
            RockRokrApp::instance()->mainWindow()->hideTip();
            break;
        default:
            break;
        }
        return QObject::eventFilter(watched, event);
    }
};


PlayBar::PlayBar(QWidget *parent)
    : QFrame(parent),
      m_filter(new FilterObject(this)),
      m_ppCommon(Q_NULLPTR)
{
    this->setObjectName("PlayBar");
    DThemeManager::instance()->registerWidget(this);

    this->setFrameStyle(QFrame::NoFrame);

    initUI();


    m_playCore = phoenixPlayerLib->playerCore();
    m_volumeCtrl = phoenixPlayerLib->volumeCtrl();

    this->volumeChanged(m_volumeCtrl->volume());
    m_volumeView->slider()->setValue(m_volumeCtrl->volume());


    connect(m_playCore, &PlayerCore::playModeChanged, this, &PlayBar::playModeChanged);

    connect(m_volumeCtrl, &VolumeControl::volumeChanged,
            this, [&](int volume) {
//        qDebug()<<Q_FUNC_INFO<<" VolumeControl::volumeChanged is slider moveing "<<m_volumeView->slider()->isMoving();
        if (m_volumeView->isVisible()) {
            m_volumeView->slider()->setValue(volume);
        }
        this->volumeChanged(volume);
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
    if (m_ppCommon) {
        m_ppCommon->deleteLater();
        m_ppCommon = Q_NULLPTR;
    }
    if (m_filter) {
        m_filter->deleteLater();
        m_filter = Q_NULLPTR;
    }
}

void PlayBar::showVolumeView()
{
    ///FIX fix parent widget of volumeview
    if (m_volumeView && !m_volumeView->parentWidget()) {
        m_volumeView->setParent(RockRokrApp::instance()->mainWindow());
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

void PlayBar::initUI()
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
    layout->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN *4);
    {
        m_waveformSlider = new WaveformSlider;
        m_waveformSlider->setObjectName("RKWaveformSlider");
        layout->addWidget(m_waveformSlider);
    }
//    layout->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN *4);
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
    layout->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN *4);
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

        ly->addWidget(m_btnOrderOrRepeat);
        ly->addWidget(m_btnShuffle);
        ly->addWidget(m_btnSound);
        ly->addSpacing(PLAY_BAR_CONTENT_H_BASE_MARGIN/2);
        ly->addWidget(m_btnPre);
        ly->addWidget(m_btnNext);

        layout->addLayout(ly);
    }
    layout->addStretch();

}

void PlayBar::volumeChanged(int volume)
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

void PlayBar::playModeChanged(PPCommon::PlayMode mode)
{
    if (!m_ppCommon) {
        m_ppCommon = new PPCommon;
    }
    const QString modeStr = m_ppCommon->enumToStr("PlayMode", (int)mode);
    if (PPCommon::PlayMode::PlayModeShuffle == mode) {
        m_btnShuffle->setProperty("PlayMode", modeStr);
        m_btnOrderOrRepeat->setProperty("PlayMode", "");
        RockRokrApp::instance()->mainWindow()->showTip(m_btnShuffle, QString("%1").arg(modeStr));
    } else if (PPCommon::PlayMode::PlayModeOrder == mode) {
        m_btnShuffle->setProperty("PlayMode", "");
        m_btnOrderOrRepeat->setProperty("PlayMode", "");
        RockRokrApp::instance()->mainWindow()->showTip(m_btnOrderOrRepeat, QString("%1").arg(modeStr));
    } else {
        m_btnShuffle->setProperty("PlayMode", "");
        m_btnOrderOrRepeat->setProperty("PlayMode", modeStr);
        RockRokrApp::instance()->mainWindow()->showTip(m_btnOrderOrRepeat, QString("%1").arg(modeStr));
    }
    this->style()->unpolish(m_btnShuffle);
    this->style()->unpolish(m_btnOrderOrRepeat);
    this->style()->polish(m_btnShuffle);
    this->style()->polish(m_btnOrderOrRepeat);
}







#include "PlayBar.moc"
