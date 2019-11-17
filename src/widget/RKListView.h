#ifndef RKLISTVIEW_H
#define RKLISTVIEW_H

#include <QListView>
#include <QMap>

#include "RKTableHeaderItem.h"

class QScrollBar;
class QTimer;
class QGraphicsOpacityEffect;
class QPropertyAnimation;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKListView : public QListView
{
    Q_OBJECT
public:
    explicit RKListView(QWidget *parent = nullptr);
    virtual ~RKListView() Q_DECL_OVERRIDE;
    void setScrollBarOrientation(Qt::Orientation orientation);

    ///
    /// \brief setDefaultItemSize used for show/hide scrollbar
    /// \param size
    ///
    void setDefaultItemSize(const QSize &size);

    ///
    /// \brief setTableHeaders
    /// \param map header columns with its width
    ///
    void setTableHeaders(const QMap<RKTableHeaderItem::HeaderColumn, int> &map);

    QMap<RKTableHeaderItem::HeaderColumn, int> getTableHeaders() const;

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // QAbstractItemView interface
public:
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;
    void setSelectionModel(QItemSelectionModel *selectionModel) Q_DECL_OVERRIDE;

protected:
    virtual QScrollBar *getInnerScrollBar() const;
    virtual void updateScrollBar();
    virtual void animateShowScrollbar(bool animate = true);
    virtual void animateHideScrollbar(bool animate = true);

signals:
    void scrollValueChanged(int value);

private:
    QScrollBar              *m_scrollBar        = Q_NULLPTR;
    QPropertyAnimation      *m_animation        = Q_NULLPTR;
    QTimer                  *m_timer            = Q_NULLPTR;
    QGraphicsOpacityEffect  *m_effect           = Q_NULLPTR;
    bool                    m_scrollBarState    = false;
    bool                    m_mouseIn           = false;
    QSize                   m_defaultItemSize;
    QMap<RKTableHeaderItem::HeaderColumn, int>  m_map;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKLISTVIEW_H
