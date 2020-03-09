#ifndef RKLISTWIDGET_H
#define RKLISTWIDGET_H

#include <QTimer>
#include <QScrollBar>
#include <QListWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit RKListWidget(QWidget *parent = nullptr);
    virtual ~RKListWidget() override;

    void addItem(QListWidgetItem *item);

    void insertItem(int row, QListWidgetItem *item);

    QListWidgetItem *takeItem(int row);

    void enableScrollBar(bool enable = true);

    /*!
     * \brief contentHeight
     * \return contentHeight + spacings in each items
     */
    int contentHeight() const;

    void calculateContentHeight();

    // QWidget interface
protected:
    virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    virtual void updateHeight();
    virtual void animateShowScrollbar(bool animate = true);
    virtual void animateHideScrollbar(bool animate = true);

signals:
    void contentHeightChanged(int height);
    void scrollValueChanged(int value);

private:
    QScrollBar              *m_scrollBar    = Q_NULLPTR;
    QPropertyAnimation      *m_animation    = Q_NULLPTR;
    QTimer                  *m_timer        = Q_NULLPTR;
    QGraphicsOpacityEffect  *m_effect       = Q_NULLPTR;

    bool                    m_scrollBarState    = false;
    bool                    m_mouseIn           = false;
    bool                    m_enableScrollBar   = true;
    int m_contentHeight                         = 0;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKLISTWIDGET_H
