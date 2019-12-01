#ifndef RKICONBUTTON_H
#define RKICONBUTTON_H

#include <QPushButton>

class QtMaterialRippleOverlay;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKIconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RKIconButton(QWidget *parent = Q_NULLPTR);
    virtual ~RKIconButton() override;

    // QObject interface
public:
    bool event(QEvent *event) override;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    QtMaterialRippleOverlay         *m_overlay;
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKICONBUTTON_H
