#ifndef TRACKLISTVIEWDELEGATE_H
#define TRACKLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class TrackListViewDelegate : public QStyledItemDelegate
{
public:
    TrackListViewDelegate(QObject *parent = Q_NULLPTR);
    virtual ~TrackListViewDelegate() override;

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    // QStyledItemDelegate interface
protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    int                         m_itemRadius;
    int                         m_coverRadius;
    QColor                      m_borderColor;
    QColor                      m_shadowColor;
    QPixmap                     m_coverPixmap;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // TRACKLISTVIEWDELEGATE_H
