#include "CategoryRecentViewDelegate.h"

#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>
#include <QByteArray>
#include <QFile>

#include "PPCommon.h"
#include "PPUtility.h"

#include "rockrokr_global.h"
#include "RKUtility.h"
#include "CategoryModel.h"

using namespace PhoenixPlayer;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

CategoryRecentViewDelegate::CategoryRecentViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent),
    m_itemRadius(6),
    m_coverRadius(3),
    m_borderColor(QColor(255, 0, 0, 152)),
    m_shadowColor(QColor(0, 255, 0, 126))
{

}

CategoryRecentViewDelegate::~CategoryRecentViewDelegate()
{

}

void CategoryRecentViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::HighQualityAntialiasing |
                            QPainter::SmoothPixmapTransform);
    const QRect rect = option.rect;
    const int coverMargin = MAIN_VIEW_LISTITEM_CONTENT_MARGIN;
    const QSize coverSz(rect.height() - coverMargin, rect.height() - coverMargin);
    const int marginWidth = MAIN_VIEW_LISTITEM_CONTENT_MARGIN;

    int painterX = rect.x();

    QColor textColor;
    QColor timeTextColor;
    const bool isHover = index.data(BaseCategoryModel::ModelRoles::RoleCurIdx).toBool();
    if (isHover) {
        textColor.setNamedColor("#FFFFFF");
        timeTextColor.setNamedColor("#FFFFFF");

        QColor color;
        color.setNamedColor("#336CFB");

        painter->setPen(color);
        painter->setBrush(color);
        painter->drawRoundRect(rect, m_itemRadius, m_itemRadius);
    } else {
        textColor.setNamedColor("#1F2022");
        timeTextColor.setNamedColor("#1F2022");
        timeTextColor.setAlphaF(0.3);
        painter->fillRect(rect, Qt::GlobalColor::transparent);
    }

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
                          (double)m_coverRadius, (double)m_coverRadius);
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
    painterX += coverMargin/2;
    painter->drawImage(painterX,
                       rect.y() + (rect.height() - coverSz.height())/2,
                       canvas);


    //TODO draw play state icon
    const int playState = index.data(BaseCategoryModel::ModelRoles::RolePlayingState).toInt();
    if (playState == (int)PPCommon::PlayBackendState::PlayBackendPaused) {
    } else if (playState == (int)PPCommon::PlayBackendState::PlayBackendPlaying) {
    }

    painterX += coverSz.width();
    painterX += marginWidth;

    painter->setPen(textColor);
    painter->setBrush(textColor);

    QTextOption textOpt;
    textOpt.setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignLeft);
    textOpt.setWrapMode(QTextOption::WrapMode::WordWrap);

    QPointF textTopLeft;
    textTopLeft.setX(painterX);
    textTopLeft.setY(rect.y() + (rect.height() - _to_px(14))/2);
    {
        QString str = index.data(BaseCategoryModel::ModelRoles::RoleCategoryName).toString();
        if (str.isEmpty()) {
            str = tr("UnKnow");
        }
        const QSizeF sf(MAIN_VIEW_LISTITEM_LEFT_CONTENT_W, _to_px(14));
        QRectF tf(textTopLeft, sf);
        painter->drawText(tf, str, textOpt);


//        painter->setBrush(Qt::GlobalColor::red);
//        painter->drawRect(textTopLeft.x(), textTopLeft.y(), sf.width(), sf.height());
//        painter->setBrush(textColor);
    }
    painterX += MAIN_VIEW_LISTITEM_LEFT_CONTENT_W;
    painterX += marginWidth;
    textTopLeft.setX(painterX);
    {
        QString str = index.data(BaseCategoryModel::ModelRoles::RoleCategorySubName).toString();
        if (str.isEmpty()) {
            str = tr("UnKnow");
        }
        const QSizeF sf(MAIN_VIEW_LISTITEM_MIDDLE_CONTENT_W, MAIN_VIEW_HL_COVER_FONT_SIZE_SECONDARY);
        QRectF tf(textTopLeft, sf);
        painter->drawText(tf, str, textOpt);

//        painter->setBrush(Qt::GlobalColor::blue);
//        painter->drawRect(textTopLeft.x(), textTopLeft.y(), sf.width(), sf.height());
//        painter->setBrush(textColor);
    }
    painterX += MAIN_VIEW_LISTITEM_MIDDLE_CONTENT_W;
    painterX += marginWidth;
    textTopLeft.setX(painterX);
    {
        painter->setPen(timeTextColor);
        painter->setBrush(timeTextColor);
        QString str;
        const int duration = index.data(BaseCategoryModel::ModelRoles::RoleTrackDuration).toInt();
        if (duration <= 0) {
            str = "00:00";
        }
        str = PPUtility::formateSongDuration(duration);
        const QSizeF sf(MAIN_VIEW_LISTITEM_RIGHT_CONTENT_W, MAIN_VIEW_HL_COVER_FONT_SIZE_SECONDARY);
        QRectF tf(textTopLeft, sf);
        painter->drawText(tf, str, textOpt);

//        painter->setBrush(Qt::GlobalColor::green);
//        painter->drawRect(textTopLeft.x(), textTopLeft.y(), sf.width(), sf.height());
//        painter->setBrush(textColor);
    }
}

QSize CategoryRecentViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(MAIN_VIEW_V_LISTVIEW_W, MAIN_VIEW_LISTITEM_H);
}

void CategoryRecentViewDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
