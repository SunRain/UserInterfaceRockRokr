#ifndef LISTVIEWITEMDELEGATE_H
#define LISTVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

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

#endif // LISTVIEWITEMDELEGATE_H
