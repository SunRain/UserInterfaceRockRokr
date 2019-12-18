#ifndef RKOVERLAYWIDGET_H
#define RKOVERLAYWIDGET_H

#include <QWidget>
#include <QPixmap>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKOverlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RKOverlayWidget(QWidget *parent = Q_NULLPTR);
    virtual ~RKOverlayWidget() override;

    void setBackgroundPixmap(const QPixmap &pixmap);


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap m_bgPixmap;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKOVERLAYWIDGET_H
