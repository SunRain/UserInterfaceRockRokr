#include "WaveformSlider.h"

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include <DThemeManager>

#include "AudioMetaObject.h"
#include "LibPhoenixPlayerMain.h"
#include "PlayerCore/PlayerCore.h"

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE
using namespace PhoenixPlayer;

WaveformSlider::WaveformSlider(QWidget *parent)
    : QAbstractSlider(parent)
{
    this->setObjectName("WaveformSlider");
    DThemeManager::instance()->registerWidget(this);

    this->setMinimum(0);
//    this->setMaximum(10000);
    this->setAcceptDrops(true);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    this->setMinimumSize(PLAY_BAR_SLIDER_W/2, PLAY_BAR_SMALL_BTN_H/2);

//    this->setValue(50);

    this->m_mousePress = false;
    this->m_isMoving = false;

    this->m_needsUpdate = false;
    this->m_oldSize = QSize(0, 0);
    this->m_oldBuilderPos = -1;
    this->m_oldBuilderIndex = -1;

    this->m_radius = 4;
    this->m_background = QBrush(Qt::darkBlue);
    this->m_waveBackground = QBrush(Qt::darkGreen);
    this->m_waveBorderColor = QColor(Qt::green);
    this->m_groovePlayingColor = QColor(Qt::red);
    this->m_groovePausedColor = QColor(Qt::yellow);

    m_playcore = phoenixPlayerLib->playerCore();
    connect(m_playcore, &PlayerCore::trackChanged,
            this, [&](QVariantMap &map){
        AudioMetaObject obj = AudioMetaObject::fromMap(map);
        if (obj.isEmpty() ||
                obj.mediaType() == PPCommon::MediaType::MediaTypeUrl) {
            m_curFile = QString();
        } else {
            //FIXME
            QUrl url = obj.uri();
            if (!url.scheme().isEmpty()) {
                m_curFile = obj.uri().toLocalFile().remove(0, url.scheme().length() + 3);
            } else {
                m_curFile = url.toLocalFile();
            }
        }
    });

}

WaveformSlider::~WaveformSlider()
{

}

void WaveformSlider::setValue(qreal value)
{
    if (!m_isMoving) {
        QAbstractSlider::setValue(value * maximum());
    }
}

void WaveformSlider::drawWaveform()
{

}

QSize WaveformSlider::sizeHint() const
{
    return QSize(PLAY_BAR_SLIDER_W, PLAY_BAR_SLIDER_H);
}

void WaveformSlider::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<event;

    if (event->button() == Qt::RightButton) {
        return;
    }
    m_mousePress = true;
    m_isMoving = false;
    qreal value = event->pos().x() * 1.0 / width();

    emit sliderMoved(value);
    setValue(value);
}

void WaveformSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseReleaseEvent(event);
    m_mousePress = false;
    m_isMoving = false;
}

void WaveformSlider::mouseMoveEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseMoveEvent(event);

    if (m_mousePress) {
        m_isMoving = true;
        int x = event->pos().x();
        if (x>=0 && x<=this->width()) {
            qreal value = x / width();
            this->setValue(value);
        } else if (x < 0) {
            this->setValue(this->minimum());
        } else {
            this->setValue(this->maximum());
        }
    }
}

void WaveformSlider::wheelEvent(QWheelEvent *event)
{
    QAbstractSlider::wheelEvent(event);
}

void WaveformSlider::resizeEvent(QResizeEvent *event)
{
    QAbstractSlider::resizeEvent(event);
    this->drawWaveform();
}

void WaveformSlider::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange)
        m_needsUpdate = true;
    QWidget::changeEvent(event);
}

void WaveformSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QImage baseImg(this->size(), QImage::Format_ARGB32_Premultiplied);
#if 1
    int x = qRound((qreal)value() / maximum() * width());
    qDebug()<<Q_FUNC_INFO<<" x "<<x
           <<" rect "<<rect()
          <<" value "<<value()
         <<" maximun "<<maximum()
        <<" width "<<width();

    QRect left = rect().adjusted(0, 0, x - width(), 0);

//    painter.drawImage(left, baseImg , left);

    QRect right = rect().adjusted(x, 0, 0, 0);
//    painter.drawImage(right, baseImg, right);

    QColor grooveColor = m_groovePlayingColor;// m_pausedState ? m_groovePausedColor : m_groovePlayingColor;
    if (grooveColor != Qt::transparent) {
        painter.setPen(grooveColor);
        painter.drawLine(x, 0, x, height());
    }
#else
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_background);
    painter.drawRoundedRect(rect(), m_radius, m_radius);
#endif
}

int WaveformSlider::radius() const
{
    return m_radius;
}

QBrush WaveformSlider::background() const
{
    return m_background;
}

QBrush WaveformSlider::waveBackground() const
{
    return m_waveBackground;
}

QColor WaveformSlider::waveBorderColor() const
{
    return m_waveBorderColor;
}

QColor WaveformSlider::groovePlayingColor() const
{
    return m_groovePlayingColor;
}

QColor WaveformSlider::groovePausedColor() const
{
    return m_groovePausedColor;
}

void WaveformSlider::setRadius(int radius)
{
    m_radius = radius;
}

void WaveformSlider::setBackground(const QBrush &background)
{
    m_background = background;
}

void WaveformSlider::setWaveBorderColor(const QColor &waveBorderColor)
{
    m_waveBorderColor = waveBorderColor;
}

void WaveformSlider::setGroovePlayingColor(const QColor &groovePlayingBackground)
{
    m_groovePlayingColor = groovePlayingBackground;
}

void WaveformSlider::setGroovePausedColor(const QColor &groovePausedBackground)
{
    m_groovePausedColor = groovePausedBackground;
}

void WaveformSlider::setWaveBackground(const QBrush &wave_background)
{
    m_waveBackground = wave_background;
}
