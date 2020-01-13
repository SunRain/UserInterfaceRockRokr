#ifndef RKOVERLAYWIDGET_H
#define RKOVERLAYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QStackedWidget>
#include <QPropertyAnimation>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

/*!
 * \brief The RKOverlayWidget class
 * Use QStackedLayout to manager added widget
 */
class RKOverlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RKOverlayWidget(QWidget *parent = Q_NULLPTR);
    virtual ~RKOverlayWidget() override;

    void setBackgroundPixmap(const QPixmap &pixmap);

    int addWidget(QWidget *widget);

    int currentIndex() const;

    QWidget *currentWidget() const;

    QWidget *widget(int index) const;

    void removeWidget(QWidget *widget);

signals:
    void mousePressOutsideContent();

    void currentChanged(int index);

    void widgetRemoved(int index);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

public slots:
    void setCurrentIndex(int index);

    void setCurrentWidget(QWidget *widget);

private:
    QWidget             *m_backgroundWidget = Q_NULLPTR;
    QWidget             *m_shadowWidget     = Q_NULLPTR;
    QStackedWidget      *m_stack            = Q_NULLPTR;
    QPropertyAnimation  *m_animation        = Q_NULLPTR;
    QPixmap             m_bgPixmap;

    Q_DISABLE_COPY(RKOverlayWidget)
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKOVERLAYWIDGET_H
