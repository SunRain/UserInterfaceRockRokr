#ifndef LBLISTWIDGET_H
#define LBLISTWIDGET_H

#include <QPropertyAnimation>

#include "widget/RKListWidget.h"

class QScrollBar;
class QTimer;
class QGraphicsOpacityEffect;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class LBListWidget : public RKListWidget
{
    Q_OBJECT
public:
    explicit LBListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~LBListWidget() override;

//    void addItem(QListWidgetItem *item);

//    void insertItem(int row, QListWidgetItem *item);

//    QListWidgetItem *takeItem(int row);

protected:
    void updateScrollBar();

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void scrollValueChanged(int value);

    // RKListWidget interface
public:
    void updateHeight() override;

protected:
    virtual void showContextMenu(const QPoint &pos);
    virtual void animateShowScrollbar(bool animate = true);
    virtual void animateHideScrollbar(bool animate = true);

private:
//    int                      m_itemsHeight;
    QScrollBar              *m_scrollBar;
    QPropertyAnimation      *m_animation;
    QTimer                  *m_timer;    
    QGraphicsOpacityEffect  *m_effect;
    bool                    m_scrollBarState;
    bool                    m_mouseIn;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // LBLISTWIDGET_H
