#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QFrame>

class QTimer;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {
class RKSlider;

class VolumeWidget : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
public:
    explicit VolumeWidget(QWidget *parent = nullptr);
    virtual ~VolumeWidget() override;

    // QWidget interface
    int radius() const;

    QBrush background() const;

    QColor borderColor() const;

    RKSlider *slider() const;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public slots:
    void deleyHide();

    void setRadius(int radius);

    void setBackground(QBrush background);

    void setBorderColor(QColor borderColor);

private:
    RKSlider                *m_slider;
    QTimer                  *m_timer;
    bool                    m_mouseIn;
    int                     m_radius;
    QBrush                  m_background;
    QColor                  m_borderColor;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // VOLUMEWIDGET_H
