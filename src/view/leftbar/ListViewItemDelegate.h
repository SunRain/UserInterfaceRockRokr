#ifndef LISTVIEWITEMDELEGATE_H
#define LISTVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class ListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ListViewItemDelegate(QObject *parent = nullptr);
    ~ListViewItemDelegate() override;

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // LISTVIEWITEMDELEGATE_H
