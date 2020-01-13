#ifndef RKSTACKWIDGET_H
#define RKSTACKWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKStackedWidgetPrivate;
class RKStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    enum AnimationType
    {
        TopToBottom = 0x0,
        BottomToTop,
        LeftToRight,
        RightToLeft,
        Center,

        AnimationTypeNone
    };
    explicit RKStackedWidget(QWidget *parent = Q_NULLPTR);
    virtual ~RKStackedWidget() override;

    void setDuration(int duration);
    int duration() const;

    void setFadeEnable(bool fade);
    bool fadeEnable() const;

    int addWidget(QWidget *widget);

    int previousIndex() const;

    int currentIndex() const;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected:
    void renderPreviousWidget(QPainter *painter,  QTransform *transform);
    void renderCurrentWidget(QPainter  *painter, QTransform *transform);

public Q_SLOTS:
    void setCurrentIndex(int index, AnimationType type);
    void setCurrentWidget(QWidget *w, AnimationType type);

private:
    void stopAndResetAnimation();

private:
    RKStackedWidgetPrivate *const d_ptr;
    Q_DISABLE_COPY(RKStackedWidget)
    Q_DECLARE_PRIVATE(RKStackedWidget)
};


//FIXME why build failure when moving this into source file??
class RKStackedWidgetPrivate : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue)
public:
    explicit RKStackedWidgetPrivate(RKStackedWidget *parent);
    ~RKStackedWidgetPrivate();
    int                             m_currentIndex  = -1;
    int                             m_previousIndex = -1;
    qreal                           m_currentValue  = 0;
    bool                            m_fade          = true;
    QEasingCurve::Type              m_curve         = QEasingCurve::Linear;
    RKStackedWidget::AnimationType  m_type          = RKStackedWidget::LeftToRight;
    QPropertyAnimation              *m_animation    = Q_NULLPTR;

    QPixmap                         m_privPixmap;
    QPixmap                         m_currentPixmap;

    inline int currentValue() const
    {
        return m_currentValue;
    }
public slots:
    inline void setCurrentValue(int currentValue)
    {
        m_currentValue = currentValue;
    }

protected:
    void animationFinished();

private:
    RKStackedWidget *const q_ptr;
    Q_DECLARE_PUBLIC(RKStackedWidget)
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKSTACKWIDGET_H
