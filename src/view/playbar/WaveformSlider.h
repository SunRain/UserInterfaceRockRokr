#ifndef WAVEFORMSLIDER_H
#define WAVEFORMSLIDER_H

#include <QAbstractSlider>


namespace PhoenixPlayer {
    class PlayerCore;
}

class WaveformSlider : public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(QBrush waveBackground READ waveBackground WRITE setWaveBackground)
    Q_PROPERTY(QColor waveBorderColor READ waveBorderColor WRITE setWaveBorderColor)
    Q_PROPERTY(QColor groovePlayingBackground READ groovePlayingColor WRITE setGroovePlayingColor)
    Q_PROPERTY(QColor groovePausedBackground READ groovePausedColor WRITE setGroovePausedColor)

public:
    explicit WaveformSlider(QWidget *parent = nullptr);
    virtual ~WaveformSlider() override;

    void setValue(qreal value);

protected:
    virtual void drawWaveform();

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

public:
    int radius() const;

    QBrush background() const;

    QBrush waveBackground() const;

    QColor waveBorderColor() const;

    QColor groovePlayingColor() const;

    QColor groovePausedColor() const;

signals:
    void sliderMoved(qreal value);

public slots:
    void setRadius(int radius);

    void setBackground(const QBrush &background);

    void setWaveBackground(const QBrush &waveBackground);

    void setWaveBorderColor(const QColor &waveBorderColor);

    void setGroovePlayingColor(const QColor &groovePlayingBackground);

    void setGroovePausedColor(const QColor &groovePausedBackground);

private:
    bool m_mousePress;
    bool m_isMoving;
    bool m_needsUpdate;
    QSize m_oldSize;
    int m_oldBuilderIndex;
    float m_oldBuilderPos;

    int m_radius;
    QBrush m_background;
    QBrush m_waveBackground;
    QColor m_waveBorderColor;
    QColor m_groovePlayingColor;
    QColor m_groovePausedColor;

    PhoenixPlayer::PlayerCore       *m_playcore;
    QString                         m_curFile;
};

#endif // WAVEFORMSLIDER_H
