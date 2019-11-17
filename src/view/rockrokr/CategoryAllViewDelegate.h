#ifndef CATEGORYALLVIEWDELEGATE_H
#define CATEGORYALLVIEWDELEGATE_H

#include <QStyledItemDelegate>


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class CategoryAllViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CategoryAllViewDelegate(QObject *parent = Q_NULLPTR);
    virtual ~CategoryAllViewDelegate() override;

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    // QStyledItemDelegate interface
protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    int                         m_radius;
    QColor                      m_borderColor;
    QColor                      m_shadowColor;
    QPixmap                     m_coverPixmap;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // CATEGORYALLVIEWDELEGATE_H
