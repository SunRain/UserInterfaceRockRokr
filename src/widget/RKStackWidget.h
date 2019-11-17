#ifndef RKSTACKWIDGET_H
#define RKSTACKWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKStackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue)
public:
    enum AnimationType
    {
        TopToBottom = 0x0,
        BottomToTop,
        LeftToRight,
        RightToLeft,

        AnimationTypeNone
    };
    explicit RKStackedWidget(QWidget *parent = Q_NULLPTR);
    virtual ~RKStackedWidget() override;

    qreal currentValue() const;

    void setDuration(int duration);
    int duration() const;

    void setFadeEnable(bool fade);
    bool fadeEnable() const;

    void addWidget(QWidget *widget);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected:
    void animationFinished();
    void renderPreviousWidget(QPainter *painter,  QTransform *transform);
    void renderCurrentWidget(QPainter  *painter, QTransform *transform);

public Q_SLOTS:
    void setCurrentIndex(int index, AnimationType type);
    void setCurrentWidget(QWidget *w, AnimationType type);

    void setCurrentValue(int currentValue);

private:
    void stopAndResetAnimation();

private:
    Q_DISABLE_COPY(RKStackedWidget)

    int                     m_currentIndex;
    int                     m_previousIndex;
    int                     m_currentValue;
    bool                    m_fade;
    QEasingCurve::Type      m_curve;
    AnimationType           m_type;
    QPropertyAnimation      *m_animation;

    QPixmap                 m_privPixmap;
    QPixmap                 m_currentPixmap;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKSTACKWIDGET_H
