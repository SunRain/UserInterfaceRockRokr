#ifndef RKSTACKWIDGET_H
#define RKSTACKWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

class RKStackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY(float value READ GetValue WRITE SetValue)
public:
    enum AnimationType
    {
        TopToBottom = 0x0,
        BottomToTop,
        LeftToRight,
        RightToLeft,
        RollInOut,
        FadeInOut,
        BlackInOut,
        SlideInOut,
        CoverInOutLeft,
        CoverInOutRight,
        FadeExchange,
        VerticalFlipRotate,
        VerticalFlipRotateOut,
        VerticalCubeRotateT2B,
        VerticalCubeRotateB2T,
        HorizontalFlipRotate
    };

    explicit RKStackedWidget(QWidget *parent = Q_NULLPTR);
    virtual ~RKStackedWidget() override;

    void start(int index);
    void setLength(int length, AnimationType type);

    void setCurve(QEasingCurve::Type curve);
    void setRevert(bool revert);

    void setDuration(int duration);
    int getDuration() const;

    void setFadeEnable(bool enable);
    void setAnimatEnable(bool animat);

    void addWidget(QWidget *widget);

    bool isAnimating();

    float GetValue() const;
    void SetValue(const float value);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

Q_SIGNALS:
    void pageChanged(int index);

public Q_SLOTS:
    void setCurrentIndex(int index);

private:
    void animationFinished();
    void renderPreviousWidget(QPainter *painter,  QTransform *transform);
    void renderCurrentWidget(QPainter  *painter, QTransform *transform);

private:
    int     m_currentIndex;
    int     m_previousIndex;
    bool    m_isAnimating;
    bool    m_fade;
    bool    m_animat;
    bool    m_revert;
    float   m_currentValue;
    float   m_rangeValue;
    float   m_startValue;
    float   m_endValue;
    QEasingCurve::Type      m_curve;
    AnimationType           m_type;
    QPropertyAnimation      *m_animation;

    QPixmap     m_privPixmap;
    QPixmap     m_currentPixmap;
};


#endif // RKSTACKWIDGET_H
