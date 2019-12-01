#ifndef WAVEFORMSLIDER_H
#define WAVEFORMSLIDER_H

#include <QAbstractSlider>

class QLabel;
class QTimer;
namespace PhoenixPlayer {
    class PlayerCore;

    namespace UserInterface {

        namespace RockRokr {

class WaveformSlider : public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
//    Q_PROPERTY(QBrush waveBackground READ waveBackground WRITE setWaveBackground)
//    Q_PROPERTY(QColor waveBorderColor READ waveBorderColor WRITE setWaveBorderColor)
//    Q_PROPERTY(QColor groovePlayingBackground READ groovePlayingColor WRITE setGroovePlayingColor)
//    Q_PROPERTY(QColor groovePausedBackground READ groovePausedColor WRITE setGroovePausedColor)
    Q_PROPERTY(QColor indicatorColor READ indicatorColor WRITE setIndicatorColor)
    Q_PROPERTY(double indicatorWidth READ indicatorWidth WRITE setIndicatorWidth)
    Q_PROPERTY(QColor waveColor READ waveColor WRITE setWaveColor)
    Q_PROPERTY(QColor waveHightlightColor READ waveHightlightColor WRITE setWaveHightlightColor)
    Q_PROPERTY(qreal waveWidth READ waveWidth WRITE setWaveWidth)
    Q_PROPERTY(qreal waveSpacing READ waveSpacing WRITE setWaveSpacing)

public:
    explicit WaveformSlider(QWidget *parent = nullptr);
    virtual ~WaveformSlider() override;

    void setPlayTickActual(quint64 sec);

    void setSpectrumData(const QList<QList<qreal>> &list, int trackDuration);

protected:
    virtual void updateTextLabel();
    QPoint limitLableGeometry(int x, int y, int z);

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

public:
    int radius() const;

    QBrush background() const;

//    QBrush waveBackground() const;

//    QColor waveBorderColor() const;

//    QColor groovePlayingColor() const;

//    QColor groovePausedColor() const;

    qreal waveWidth() const;

    qreal waveSpacing() const;

    QColor waveColor() const;

    QColor waveHightlightColor() const;

    QColor indicatorColor() const;

    double indicatorWidth() const;

    QColor borderColor() const;

signals:
//    void sliderMoved(qreal value);
    void sliderReleasedAt(qreal value);

public slots:
    void setRadius(int radius);

    void setBackground(const QBrush &background);

//    void setWaveBackground(const QBrush &waveBackground);

//    void setWaveBorderColor(const QColor &waveBorderColor);

//    void setGroovePlayingColor(const QColor &groovePlayingBackground);

//    void setGroovePausedColor(const QColor &groovePausedBackground);

    void setWaveWidth(qreal waveWidth);

    void setWaveSpacing(qreal waveSpacing);

    void setWaveColor(const QColor &waveColor);

    void setWaveHightlightColor(const QColor &waveHightlightColor);

    void setIndicatorColor(const QColor &indicatorColor);

    void setIndicatorWidth(double indicatorWidth);

    void setBorderColor(const QColor &borderColor);

private:
    bool m_mousePress;
    bool m_isMoving;
    bool m_needsUpdate;
    QSize m_oldSize;
    int m_oldBuilderIndex;
    float m_oldBuilderPos;

    int m_radius;
    QBrush m_background;
//    QBrush m_waveBackground;
//    QColor m_waveBorderColor;
//    QColor m_groovePlayingColor;
//    QColor m_groovePausedColor;

    QLabel                                  *m_textLabel    = Q_NULLPTR;
    QTimer                                  *m_labelTimer   = Q_NULLPTR;

    QList<qreal>  m_spekList;
    int           m_trackDuration;
    qreal         m_waveWidth;
    qreal         m_waveSpacing;
    QColor      m_waveColor;
    QColor      m_waveHightlightColor;

    double      m_valueScale;
    double      m_valueWhenMoving;
    QColor      m_indicatorColor;
    double      m_indicatorWidth;
    QColor      m_borderColor;
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // WAVEFORMSLIDER_H
