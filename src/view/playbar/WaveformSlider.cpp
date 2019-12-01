#include "WaveformSlider.h"

#include <QTimer>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QLabel>

#include <DThemeManager>

#include "AudioMetaObject.h"
#include "LibPhoenixPlayerMain.h"
#include "PPSettings.h"
#include "PPUtility.h"
#include "PlayerCore/PlayerCore.h"

#include "rockrokr_global.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

WaveformSlider::WaveformSlider(QWidget *parent)
    : QAbstractSlider(parent)
{
    this->setObjectName("WaveformSlider");
    DThemeManager::instance()->registerWidget(this);

    this->setMinimum(0);
    this->setAcceptDrops(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->setMinimumSize(PLAY_BAR_SLIDER_W/2, PLAY_BAR_SMALL_BTN_H/2);

    this->setCursor(QCursor(Qt::PointingHandCursor));

    m_textLabel = new QLabel(this);
    m_textLabel->setObjectName("WaveTextLabel");
    m_textLabel->setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
    m_textLabel->setGeometry(0, 0, 40, 20);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setFocusPolicy(Qt::NoFocus);
//    m_textLabel->setStyleSheet("QLabel{ color:rgba(0, 0, 0, 0.5); background-color:#FCFCFC; border:1px solid #F3F3F3;}");

    m_labelTimer = new QTimer(this);
    m_labelTimer->setSingleShot(true);
    connect(m_labelTimer, &QTimer::timeout, this, [&]() {
        m_textLabel->hide();
    });

    this->m_mousePress = false;
    this->m_isMoving = false;

    this->m_needsUpdate = false;
    this->m_oldSize = QSize(0, 0);
    this->m_oldBuilderPos = -1;
    this->m_oldBuilderIndex = -1;

    this->m_radius = 4;
    this->m_background = QBrush(Qt::transparent);
    this->m_borderColor = QColor(Qt::transparent);
//    this->m_waveBackground = QBrush(Qt::transparent);
//    this->m_waveBorderColor = QColor(Qt::transparent);
//    this->m_groovePlayingColor = QColor(Qt::red);
//    this->m_groovePausedColor = QColor(Qt::yellow);
    this->m_indicatorColor      = QColor(Qt::transparent);
    this->m_indicatorWidth      = 1;
    this->m_waveColor           = QColor(218, 218, 218);
    this->m_waveHightlightColor = QColor(HIGHLIGHT_BG_COLOR);

    this->m_waveWidth = 3.0;
    this->m_waveSpacing = 3.0;

    this->m_valueScale = 1.0;
    this->m_valueWhenMoving = 0.0;
}

WaveformSlider::~WaveformSlider()
{

}

void WaveformSlider::setPlayTickActual(quint64 sec)
{
    if (!m_isMoving) {
        QAbstractSlider::setValue(sec);
    } else {
        m_valueWhenMoving = sec;
    }
    m_textLabel->setText(PPUtility::formateSongDuration(sec));
    this->update();
}

void WaveformSlider::setSpectrumData(const QList<QList<qreal> > &list, int trackDuration)
{
    m_spekList.clear();
    foreach (auto i, list) {
        qreal value = 0;
        foreach (auto j, i) {
            value += j;
        }
        m_spekList.append(value);
    }
    m_trackDuration = trackDuration;

    this->setMaximum(m_trackDuration);
    qDebug()<<" duration "<<m_trackDuration;
    m_valueScale = m_trackDuration/(double)this->width();

    this->update();
}

void WaveformSlider::updateTextLabel()
{
    const QPoint &curPos = mapFromGlobal(QCursor::pos());
    const QPoint &glbPos = mapToGlobal(QPoint(0, 0));
    const QSize &sizePos = size();
    QPoint changePos;
    changePos = limitLableGeometry(curPos.x(), glbPos.x(), sizePos.width());
    m_textLabel->move(changePos.x() - m_textLabel->width()/2,
                      (glbPos - QPoint(0, m_textLabel->height())).y() - 10);
    m_textLabel->setText(PPUtility::formateSongDuration(changePos.y()));
}

QPoint WaveformSlider::limitLableGeometry(int x, int y, int z)
{
    QPoint pt;
    if( 0 < x && x < z) {
        pt.setX(y + x);
        pt.setY(qint64(x)*maximum()/z);
    }
    if(x <= 0) {
        pt.setX(y);
        pt.setY(0);
    }
    if(x >= z) {
        pt.setX(y + z);
        pt.setY(maximum());
    }
    return pt;
}

QSize WaveformSlider::sizeHint() const
{
    return QSize(PLAY_BAR_SLIDER_W, PLAY_BAR_SLIDER_H);
}

void WaveformSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        return;
    }
    m_mousePress = true;
    m_isMoving = false;
    double value = event->pos().x() * m_valueScale;

    qDebug()<<event<<" value "<<value;

//    emit sliderMoved(value);
    setPlayTickActual(value);
    m_textLabel->show();
    m_textLabel->raise();

//    const QPoint &curPos = mapFromGlobal(QCursor::pos());
//    const QPoint &glbPos = mapToGlobal(QPoint(0, 0));
//    const QSize &sizePos = size();
//    QPoint changePos;
//    changePos = limitLableGeometry(curPos.x(), glbPos.x(), sizePos.width());
//    m_textLabel->move(changePos.x() - m_textLabel->width()/2,
//                      (glbPos - QPoint(0, m_textLabel->height())).y() - 10);
//    m_textLabel->setText(PPUtility::formateSongDuration(changePos.y()));
    updateTextLabel();
}

void WaveformSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseReleaseEvent(event);
    m_mousePress = false;
    m_isMoving = false;
    m_labelTimer->start(2000);
    emit sliderReleasedAt(value());
}

void WaveformSlider::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<Q_FUNC_INFO<<event;
    if (m_labelTimer->isActive()) {
        m_labelTimer->stop();
    }

    if (m_mousePress) {
        m_isMoving = true;
        int x = event->pos().x();
        if (x>=0 && x<=this->width()) {
            qreal value = x * m_valueScale;
            this->setPlayTickActual(value);
        } else if (x < 0) {
            this->setPlayTickActual(0);
        } else {
            this->setPlayTickActual(this->maximum());
        }
    }
//    const QPoint &curPos = mapFromGlobal(QCursor::pos());
//    const QPoint &glbPos = mapToGlobal(QPoint(0, 0));
//    const QSize &sizePos = size();
//    QPoint changePos;
//    changePos = limitLableGeometry(curPos.x(), glbPos.x(), sizePos.width());
//    m_textLabel->move(changePos.x() - m_textLabel->width()/2,
//                      (glbPos - QPoint(0, m_textLabel->height())).y() - 10);
//    m_textLabel->setText(PPUtility::formateSongDuration(changePos.y()));
    updateTextLabel();
}

void WaveformSlider::wheelEvent(QWheelEvent *event)
{
    qDebug()<<event;

    QAbstractSlider::wheelEvent(event);
}

void WaveformSlider::resizeEvent(QResizeEvent *event)
{
    QAbstractSlider::resizeEvent(event);

    this->setFixedSize(event->size());
    m_valueScale = m_trackDuration/(double)this->width();
    this->update();
}

void WaveformSlider::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange) {
        m_needsUpdate = true;
    }
    QWidget::changeEvent(event);
}

void WaveformSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

    painter.setPen(m_borderColor);
    painter.setBrush(m_background);
    painter.drawRoundedRect(rect(), m_radius, m_radius);

    int hightlightAt = 0;
    double hightlightPos = 0;
    const double spekScale = (double)m_spekList.size() / (double)this->width();
    if (m_mousePress && m_isMoving) {
        hightlightPos = m_valueWhenMoving / m_valueScale;
    } else {
        hightlightPos = (double)this->value() / m_valueScale;
    }
    hightlightAt = qRound(hightlightPos * spekScale);
    const int height = this->height();
    const double widthScale = (double)this->width()/(double)m_spekList.size()/(m_waveWidth+m_waveSpacing);
    const double waveWidth =  m_waveWidth * widthScale;
    const double waveSpacing = m_waveSpacing *widthScale;

//    qDebug()<<Q_FUNC_INFO<<"width "<<this->width()
//           <<" value "<<this->value()
//            <<" widthScale "<<widthScale
//          <<" waveWidth "<<waveWidth
//         <<" waveSpacing "<<waveSpacing
//              <<" m_valueWhenMoving "<<m_valueWhenMoving
//           <<" m_valueScale "<<m_valueScale
//          <<" hightlightPos "<<hightlightPos
//         <<" m_spek->levels.size() "<<m_spek->levels.size()
//          <<"  this->maximum() "<< this->maximum()
//         <<"  spekScale "<<spekScale
//        <<" hightlightAt  "<<hightlightAt;

    double x = 0.0;
    double indicatiorPosX = 0.0;
    qreal scale = 1.0;
    if (!m_spekList.isEmpty()) {
        qreal maxSpek = 0;
        foreach (const auto &i, m_spekList) {
            if (i >= maxSpek) {
                maxSpek = i;
            }
        }
        scale = (this->rect().height() - _to_px(2)) / maxSpek;
    }
    {
        QPainterPath path;
        for(int i = 0; i<hightlightAt; ++i) {
            const double h = (double)m_spekList.at(i) * scale;
            double py = (height - h)/2;
            path.addRect(x, py, waveWidth, h);
            x += (waveWidth + waveSpacing);
//            qDebug()<<Q_FUNC_INFO<<" x "<<x<<" i "<<i;
        }
        indicatiorPosX = x;
        path.setFillRule(Qt::FillRule::WindingFill);
        painter.setPen(m_waveHightlightColor);
        painter.setBrush(m_waveHightlightColor);
        painter.drawPath(path);
    }
    {
        QPainterPath path;
        for(int i = hightlightAt; i<m_spekList.size(); ++i) {
            const double h = (double)m_spekList.at(i) * scale;
            double py = (height - h)/2;
            path.addRect(x, py, waveWidth, h);
            x += (waveWidth + waveSpacing);
        }
        path.setFillRule(Qt::FillRule::WindingFill);
        painter.setPen(m_waveColor);
        painter.setBrush(m_waveColor);
        painter.drawPath(path);
    }
    if (m_mousePress && m_isMoving) {
        painter.setPen(m_indicatorColor);
        painter.setBrush(m_indicatorColor);
        painter.drawRect(QRectF(indicatiorPosX-(m_indicatorWidth/2), 0, m_indicatorWidth, this->height()));

    }
}

void WaveformSlider::enterEvent(QEvent *event)
{
    QAbstractSlider::enterEvent(event);
    if (m_mousePress) {
        m_textLabel->show();
    }
}

void WaveformSlider::leaveEvent(QEvent *event)
{
    QAbstractSlider::leaveEvent(event);
    m_labelTimer->start(1000);
}

int WaveformSlider::radius() const
{
    return m_radius;
}

QBrush WaveformSlider::background() const
{
    return m_background;
}

//QBrush WaveformSlider::waveBackground() const
//{
//    return m_waveBackground;
//}

//QColor WaveformSlider::waveBorderColor() const
//{
//    return m_waveBorderColor;
//}

//QColor WaveformSlider::groovePlayingColor() const
//{
//    return m_groovePlayingColor;
//}

//QColor WaveformSlider::groovePausedColor() const
//{
//    return m_groovePausedColor;
//}

qreal WaveformSlider::waveWidth() const
{
    return m_waveWidth;
}

qreal WaveformSlider::waveSpacing() const
{
    return m_waveSpacing;
}

QColor WaveformSlider::waveColor() const
{
    return m_waveColor;
}

QColor WaveformSlider::waveHightlightColor() const
{
    return m_waveHightlightColor;
}

QColor WaveformSlider::indicatorColor() const
{
    return m_indicatorColor;
}

double WaveformSlider::indicatorWidth() const
{
    return m_indicatorWidth;
}

QColor WaveformSlider::borderColor() const
{
    return m_borderColor;
}

void WaveformSlider::setRadius(int radius)
{
    m_radius = radius;
}

void WaveformSlider::setBackground(const QBrush &background)
{
    m_background = background;
}

//void WaveformSlider::setWaveBorderColor(const QColor &waveBorderColor)
//{
//    m_waveBorderColor = waveBorderColor;
//}

//void WaveformSlider::setGroovePlayingColor(const QColor &groovePlayingBackground)
//{
//    m_groovePlayingColor = groovePlayingBackground;
//}

//void WaveformSlider::setGroovePausedColor(const QColor &groovePausedBackground)
//{
//    m_groovePausedColor = groovePausedBackground;
//}

void WaveformSlider::setWaveWidth(qreal waveWidth)
{
    m_waveWidth = waveWidth;
}

void WaveformSlider::setWaveSpacing(qreal waveSpacing)
{
    m_waveSpacing = waveSpacing;
}

void WaveformSlider::setWaveColor(const QColor &waveColor)
{
    m_waveColor = waveColor;
}

void WaveformSlider::setWaveHightlightColor(const QColor &waveHightlightColor)
{
    m_waveHightlightColor = waveHightlightColor;
}

void WaveformSlider::setIndicatorColor(const QColor &pausedIndicatorColor)
{
    m_indicatorColor = pausedIndicatorColor;
}

void WaveformSlider::setIndicatorWidth(double pausedIndicatorWidth)
{
    m_indicatorWidth = pausedIndicatorWidth;
}

void WaveformSlider::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
}

//void WaveformSlider::setWaveBackground(const QBrush &wave_background)
//{
//    m_waveBackground = wave_background;
//}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

