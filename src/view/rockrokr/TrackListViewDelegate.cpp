#include "TrackListViewDelegate.h"

#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>
#include <QByteArray>
#include <QFile>

#include "PPCommon.h"
#include "PPUtility.h"

#include "rockrokr_global.h"
#include "RKUtility.h"
#include "widget/RKListView.h"
#include "widget/RKTableHeaderItem.h"
#include "TrackListModel.h"

using namespace  PhoenixPlayer;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static int S_FONT_SIZE = dpi_to_px(16);

TrackListViewDelegate::TrackListViewDelegate(QObject *parent)
    : QStyledItemDelegate (parent),
    m_itemRadius(0),
    m_coverRadius(3),
    m_borderColor(QColor(255, 0, 0, 152)),
    m_shadowColor(QColor(0, 255, 0, 126))
{

}

TrackListViewDelegate::~TrackListViewDelegate()
{

}

void TrackListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const RKListView *view = qobject_cast<const RKListView*>(option.widget);
    Q_ASSUME(view != Q_NULLPTR);
    QMap<RKTableHeaderItem::HeaderColumn, int> columnMap = view->getTableHeaders();

    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::HighQualityAntialiasing |
                            QPainter::SmoothPixmapTransform);
    const QRect rect = option.rect;
    const int marginWidth = MAIN_VIEW_LISTITEM_CONTENT_MARGIN;

    int painterX = rect.x();

    QColor textColor;
    QColor timeTextColor;

    const bool isHover = index.data(TrackListModel::ModelRoles::RoleCurIdx).toBool()
            || option.state & QStyle::State_Selected
//            || option.state & QStyle::State_Active
            || option.state & QStyle::State_MouseOver;
    if (isHover) {
        textColor.setNamedColor("#FFFFFF");
        timeTextColor.setNamedColor("#FFFFFF");

        QColor color;
        color.setNamedColor("#336CFB");

        painter->setPen(color);
        painter->setBrush(color);
        painter->fillRect(rect, color);
    } else {
        textColor.setNamedColor("#1F2022");
        timeTextColor.setNamedColor("#1F2022");
        timeTextColor.setAlphaF(0.3);
        painter->fillRect(rect, Qt::GlobalColor::transparent);
    }

    //cover
    if (columnMap.contains(RKTableHeaderItem::HeaderCover)) {
        const int coverMargin = MAIN_VIEW_LISTITEM_CONTENT_MARGIN;
        const int coverAreaWidth = columnMap.value(RKTableHeaderItem::HeaderCover);
        const int minSize = qMin(coverAreaWidth, rect.height()-coverMargin*2);
        const QSize coverSz(minSize, minSize);
        // cover
        QPixmap coverPixmap;
        {
            QString imgUri = index.data(TrackListModel::ModelRoles::RoleTrackImageUri).toUrl().toString();
//            qDebug()<<Q_FUNC_INFO<<" req imgUri "<<imgUri;

            if (imgUri.toLower().startsWith("qrc:")) {
                imgUri = imgUri.mid(3, imgUri.length()-3);
            }
            if (imgUri.isEmpty()) {
//                qDebug()<<Q_FUNC_INFO<<" use default cover!!!";
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
        const int xpos = painterX + (coverAreaWidth - minSize)/2;
        painter->drawImage(xpos,
                           rect.y() + (rect.height() - minSize)/2,
                           canvas);

        //TODO draw play state icon
        const int playState = index.data(TrackListModel::ModelRoles::RolePlayingState).toInt();
        if (playState == (int)PPCommon::PlayBackendState::PlayBackendPaused) {

        } else if (playState == (int)PPCommon::PlayBackendState::PlayBackendPlaying) {

        }

        painterX += coverAreaWidth;
    }
    painter->setPen(textColor);
    painter->setBrush(textColor);


#define DRAW_TEXT(role, textWidth) \
    QString text = index.data(role).toString(); \
    if (text.isEmpty()) { \
        text = tr("Unknow"); \
    } \
    QFont font = option.font; \
    font.setPixelSize(dpi_to_px(S_FONT_SIZE)); \
    QFontMetrics fm(font); \
    const QString str = fm.elidedText(text, Qt::ElideRight, textWidth - dpi_to_px(4)); \
    qreal textY = rect.y() + (rect.height() - S_FONT_SIZE)/2; \
    const QRectF tf(painterX+dpi_to_px(2), textY, textWidth - dpi_to_px(4), S_FONT_SIZE); \
    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);

    // title
    if (columnMap.contains(RKTableHeaderItem::HeaderTitle)) {
        const int textWidth = columnMap.value(RKTableHeaderItem::HeaderTitle);
//        QString text = index.data(TrackListModel::RoleSongTitle).toString();
//        if (text.isEmpty()) {
//            text = tr("Unknow");
//        }
//        QFont font = option.font;
//        font.setPixelSize(dpi_to_px(S_FONT_SIZE));
//        QFontMetrics fm(font);
//        const QString str = fm.elidedText(text, Qt::ElideRight, textWidth - dpi_to_px(4));
//        qreal textY = rect.y() + (rect.height() - S_FONT_SIZE)/2;
//        const QRectF tf(painterX+dpi_to_px(2), textY, textWidth - dpi_to_px(4), S_FONT_SIZE);
//        painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
        DRAW_TEXT(TrackListModel::RoleSongTitle, textWidth)

        painterX += textWidth;
    }
    // artist
    if (columnMap.contains(RKTableHeaderItem::HeaderArtist)) {
        const int textWidth = columnMap.value(RKTableHeaderItem::HeaderArtist);
        DRAW_TEXT(TrackListModel::RoleArtistName, textWidth)
        painterX += textWidth;
    }
    //album
    if (columnMap.contains(RKTableHeaderItem::HeaderAlbum)) {
        const int textWidth = columnMap.value(RKTableHeaderItem::HeaderAlbum);
        DRAW_TEXT(TrackListModel::RoleAlbumName, textWidth)
        painterX += textWidth;
    }
    //duration
    if (columnMap.contains(RKTableHeaderItem::HeaderDuration)) {
        painter->setPen(timeTextColor);
        painter->setBrush(timeTextColor);
        const int textWidth = columnMap.value(RKTableHeaderItem::HeaderDuration);
        QString text = index.data(TrackListModel::RoleDuration).toString();
        if (text.isEmpty()) {
            text = tr("Unknow");
        }
        QFont font = option.font;
        font.setPixelSize(dpi_to_px(S_FONT_SIZE));
        QFontMetrics fm(font);
        const QString str = fm.elidedText(text, Qt::ElideRight, textWidth - dpi_to_px(4));
        qreal textY = rect.y() + (rect.height() - S_FONT_SIZE)/2;
        const QRectF tf(painterX+dpi_to_px(2), textY, textWidth - dpi_to_px(4), S_FONT_SIZE);
        painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
    }
}

QSize TrackListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.widget->width(), MAIN_VIEW_LISTITEM_H);
}

void TrackListViewDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
