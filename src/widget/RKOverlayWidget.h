#ifndef RKOVERLAYWIDGET_H
#define RKOVERLAYWIDGET_H

#include <QStackedLayout>
#include <QStateMachine>
#include <QWidget>
#include <QDialog>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class WidgetWindow;
class WidgetProxy;
class RKOverlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RKOverlayWidget(QWidget *parent = nullptr);
    virtual ~RKOverlayWidget() override;

    QLayout *windowLayout() const;
    void setWindowLayout(QLayout *layout);

protected:
    virtual QRect overlayGeometry() const;

    // QObject interface
protected:
    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void showOverlay();
    void hideOverlay();

private:
    WidgetWindow        *m_dialogWindow;
    QStackedLayout      *m_proxyStack;
    QStateMachine       *m_stateMachine;
    WidgetProxy         *m_proxy;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKOVERLAYWIDGET_H
