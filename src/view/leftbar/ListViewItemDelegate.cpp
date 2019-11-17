#include "ListViewItemDelegate.h"

#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPixmap>

#include "rockrokr_global.h"
#include "RKUtility.h"
#include "view/leftbar/ListModel.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

ListViewItemDelegate::ListViewItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

ListViewItemDelegate::~ListViewItemDelegate()
{

}

void ListViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    qDebug()<<Q_FUNC_INFO<<"-----------------------";
    if (!index.isValid()) {
        qDebug()<<"Invalid index";
        return;
    }
    const QRect rect = option.rect;
//    qDebug()<<Q_FUNC_INFO<<"rect width "<<rect.width()<<" height "<<rect.height();

    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setBrush(QColor(Qt::GlobalColor::transparent));
    const bool isHover = index.data(ListModel::ModelRoles::RoleCurIdx).toBool();
    if (isHover) {
        QColor color(HIGHLIGHT_BG_COLOR);
//        color.setAlpha(95);
        painter->setPen(color);
        painter->setBrush(color);
        painter->drawRoundedRect(rect, 5, 5);

//        painter->setBrush(QColor(Qt::GlobalColor::transparent));
//        painter->setPen(QColor(0, 0, 0, 20));
        color = QColor(HIGHLIGHT_FONT_COLOR);
//        color.setAlpha(20);
        painter->setPen(color);
        color.setAlpha(20);
        painter->setBrush(color);
    } else {
        painter->setBrush(QColor(Qt::GlobalColor::transparent));
        painter->setPen(QColor(/*Qt::GlobalColor::black*/FONT_COLOR_TITLE));
    }
    QString icon = QString(":resources/%1.svg").arg(index.data(ListModel::ModelRoles::RoleKey).toString());
    const bool drawIcon = QFile::exists(icon);
    QPointF textTopLeft;
    QSizeF textSize;
    if (drawIcon) {
        const double iconMargin = (LEFT_BAR_ITEM_H - LEFT_BAR_ITEM_ICON_PART_H) / 2;
        const QSize iconSize(LEFT_BAR_ITEM_ICON_PART_W, LEFT_BAR_ITEM_ICON_PART_H);
        const QRect iconRect = QRect(rect.topLeft()+QPoint(LEFT_BAR_ITEM_CONTENT_MARGIN, iconMargin),
                                     iconSize);
        textTopLeft.setX(rect.x() + iconSize.width() + LEFT_BAR_ITEM_CONTENT_MARGIN*2);
        //FIXME LEFT_BAR_ITEM_CONTENT_MARGIN *4??
        textSize.setWidth(rect.width() - iconSize.width() - LEFT_BAR_ITEM_CONTENT_MARGIN*3);
//        textTopLeft.setY(rect.y() + iconMargin);
        const QPixmap ip = RKUtility::svgToPixmap(icon, iconSize);
        if (!icon.isNull()) {
            painter->drawPixmap(iconRect, ip);
        } else {
            textTopLeft.setX(rect.x() + LEFT_BAR_ITEM_CONTENT_MARGIN);
//            textTopLeft.setY(rect.y() + LEFT_BAR_ITEM_CONTENT_MARGIN);
            textSize.setWidth(rect.width() - LEFT_BAR_ITEM_CONTENT_MARGIN*2);
        }
    } else {
        textTopLeft.setX(rect.x() + LEFT_BAR_ITEM_CONTENT_MARGIN);
//        textTopLeft.setY(rect.y() + LEFT_BAR_ITEM_CONTENT_MARGIN);
        textSize.setWidth(rect.width() - LEFT_BAR_ITEM_CONTENT_MARGIN*2);
    }
    const int textTBmargin = (rect.height() - LEFT_BAR_FONT_SIZE_NORMAL) /2;
    textTopLeft.setY(rect.y() + textTBmargin);
    textSize.setHeight(/*rect.height() - textTBmargin *2*/LEFT_BAR_FONT_SIZE_NORMAL);

    QTextOption topt;
    topt.setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignLeft);
    topt.setWrapMode(QTextOption::WrapMode::WordWrap);
    QRectF textRect(textTopLeft, textSize);
    QString text = index.data(ListModel::ModelRoles::RoleName).toString();
//    painter->setBrush(QBrush(Qt::GlobalColor::transparent));
//    painter->setPen(QColor(Qt::GlobalColor::black));
    painter->drawText(textRect, text, topt);
}

QSize ListViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(LEFT_BAR_ITEM_W, LEFT_BAR_ITEM_H);
}
} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

