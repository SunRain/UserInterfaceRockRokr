#include "CategoryAllViewDelegate.h"

#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>
#include <QByteArray>
#include <QFile>

#include "rockrokr_global.h"
#include "RKUtility.h"
#include "CategoryModel.h"


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


CategoryAllViewDelegate::CategoryAllViewDelegate(QObject *parent)
    : QStyledItemDelegate (parent),
    m_radius(4),
    m_borderColor(QColor(255, 0, 0, 152)),
    m_shadowColor(QColor(0, 255, 0, 126))
{

}

CategoryAllViewDelegate::~CategoryAllViewDelegate()
{

}

void CategoryAllViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::HighQualityAntialiasing |
                            QPainter::SmoothPixmapTransform);
    const QRect rect = option.rect;
    const int margin = 2;
    const QSize coverSz(rect.width() - margin*2, MAIN_VIEW_R_VIEW_COVER_ICON_H);
    const QSize innerSize(rect.width() - margin*2, rect.height() - margin*2);

    //draw background
//    painter->fillRect(option.rect, Qt::GlobalColor::transparent);
    const bool isHover = index.data(BaseCategoryModel::ModelRoles::RoleCurIdx).toBool();
    if (isHover) {
        QColor color;
        color.setNamedColor("#336CFB");
        painter->setPen(color);
        painter->setBrush(color);
        painter->drawRoundRect(rect, m_radius, m_radius);
    } else {
        painter->fillRect(option.rect, Qt::GlobalColor::transparent);
    }

    QPixmap background(rect.size());
    {
        background.fill(Qt::transparent);
        QPainter p(&background);
        p.setRenderHints(QPainter::Antialiasing |
                         QPainter::HighQualityAntialiasing |
                         QPainter::SmoothPixmapTransform);
        QPainterPath pt;
        pt.addRoundedRect(QRectF(0, 0, innerSize.width(), innerSize.height()),
                          (double)m_radius, (double)m_radius);
        if (isHover) {
            p.fillPath(pt, Qt::GlobalColor::transparent);
        } else {
            QColor color;
            color.setAlphaF(0.2);
            color.setNamedColor("#F3F3F3");
            p.fillPath(pt, color);
        }
        p.end();
    }
    painter->drawPixmap(rect.x()+margin, rect.y()+margin, background);

    // cover
    QPixmap coverPixmap;
    {
        QString imgUri = index.data(BaseCategoryModel::ModelRoles::RoleImageUri).toUrl().toString();
//        qDebug()<<Q_FUNC_INFO<<" req imgUri "<<imgUri;

        if (imgUri.toLower().startsWith("qrc:")) {
            imgUri = imgUri.mid(3, imgUri.length()-3);
        }
        if (imgUri.isEmpty()) {
//            qDebug()<<Q_FUNC_INFO<<" use default cover!!!";
            coverPixmap = RKUtility::defaultCoverPixmap(coverSz);
        } else {
            QImage img(imgUri);
            if (img.isNull()) {
                coverPixmap = RKUtility::defaultCoverPixmap(coverSz);
            } else {
                coverPixmap = QPixmap::fromImage(img);
            }
        }
        coverPixmap = coverPixmap.scaled(coverSz, Qt::KeepAspectRatioByExpanding);
    }
    QPixmap maskPixmap(coverSz);
    {
        maskPixmap.fill(Qt::transparent);
        QPainter p(&maskPixmap);
        p.setRenderHints(QPainter::Antialiasing |
                         QPainter::HighQualityAntialiasing |
                         QPainter::SmoothPixmapTransform);
        QPainterPath pt;
        pt.addRoundedRect(QRectF(0, 0, coverSz.width(), coverSz.height()),
                          (double)m_radius, (double)m_radius);
        p.fillPath(pt, Qt::GlobalColor::green);
        p.end();
    }
    QImage canvas = QImage(coverSz, QImage::Format_ARGB32_Premultiplied);
    {
        QPainter p(&canvas);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(canvas.rect(), Qt::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        p.drawImage(0, 0, maskPixmap.toImage());
        p.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p.drawImage(0, 0, coverPixmap.toImage());
        p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        p.end();
    }
    painter->drawImage(rect.x()+margin, rect.y()+margin, canvas);

    // text
    QTextOption textOpt;
    textOpt.setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignLeft);
    textOpt.setWrapMode(QTextOption::WrapMode::WordWrap);

    const int textWidth = /*rect.width()*/innerSize.width() - dpi_to_px(4);
    const int innerMargin = /*rect.height()*/innerSize.height()
                            - coverSz.height()
                            - MAIN_VIEW_R_VIEW_FONT_SIZE_PRIMARY
                            - MAIN_VIEW_R_VIEW_FONT_SIZE_SECONDARY
                            - dpi_to_px(4);
    int textMargin = 0;
    if (innerMargin > 2) {
        textMargin = 2;
    }
    QPointF textTopLeft;
    textTopLeft.setX(rect.x()+margin + (rect.width() - margin - textWidth)/2);
    // name
    {
        QString str = index.data(BaseCategoryModel::ModelRoles::RoleCategoryName).toString();
        if (str.isEmpty()) {
            str = tr("UnKnow");
        }
        const int y = rect.y() + margin + coverSz.height() + innerMargin - textMargin -dpi_to_px(4);
        textTopLeft.setY(y);
        const QSizeF sf(textWidth, MAIN_VIEW_R_VIEW_FONT_SIZE_PRIMARY);
        QRectF tf(textTopLeft, sf);
        QColor color;
        if (isHover) {
            color.setNamedColor("#FFFFFF");
        } else {
            color.setNamedColor("#19202C");
        }
        painter->setPen(color);
        painter->setBrush(color);
        painter->drawText(tf, str, textOpt);
    }
    // subname
    {
        QString str = index.data(BaseCategoryModel::ModelRoles::RoleCategorySubName).toString();
        if (str.isEmpty()) {
            str = tr("UnKnow");
        }
        const int y = textTopLeft.y() + MAIN_VIEW_R_VIEW_FONT_SIZE_PRIMARY + textMargin;
        textTopLeft.setY(y);
        const QSizeF sf(textWidth, MAIN_VIEW_R_VIEW_FONT_SIZE_SECONDARY);
        QRectF tf(textTopLeft, sf);
        QColor color;
        if (isHover) {
            color.setNamedColor("#FFFFFF");
        } else {
            color.setNamedColor("#19202C");
        }
        color.setAlphaF(0.5);
        painter->setPen(color);
        painter->setBrush(color);
        painter->drawText(tf, str, textOpt);
    }

}

QSize CategoryAllViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(MAIN_VIEW_R_VIEW_COVER_W, MAIN_VIEW_R_VIEW_COVER_H);
}

void CategoryAllViewDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

