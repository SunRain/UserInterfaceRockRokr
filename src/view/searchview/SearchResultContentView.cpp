#include "SearchResultContentView.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QMenu>
#include <QStyleFactory>

#include <DThemeManager>
#include <DHiDPIHelper>
#include <DDesktopServices>

#include "PPUtility.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/PlayListMetaMgr.h"
#include "DataProvider/ITrackSearch.h"

#include "rockrokr_global.h"
#include "RKUtility.h"
#include "view/ViewUtility.h"
#include "view/rockrokr/CategoryModelImageProvider.h"
#include "widget/RKTableHeaderItem.h"
#include "widget/RKListView.h"

DWIDGET_USE_NAMESPACE

using namespace PhoenixPlayer::DataProvider;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

const static int S_FONT_SIZE = _to_font_px(16);

class SearchResultContentViewModel : public QAbstractListModel, ImageProviderProxy
{
    Q_OBJECT
public:
    enum ModelRoles {
        RoleMatchTypes = Qt::UserRole + 1,
        RoleMatchFilePathObject,
        RoleMatchTrackNameObject,
        RoleMatchArtistNameObject,
        RoleMatchAlbumNameObject,
//        RoleFileUri,
//        RoleMatchedStr,
//        RoleMatchedIndex,
//        RoleMatchedLength,
//        RoleQueryStr,
//        RoleAudioMetaObject,
        RoleAudioMetaObject,

        RoleArtistName,
        RoleArtistImageUri,
        RoleAlbumName,
        RoleAlbumImageUrl,
        RoleCoverArtSmall,
        RoleCoverArtLarge,
        RoleCoverArtMiddle,
        RoleMediaType,
        RoleSongTitle,
        RoleHash,
        RoleDuration,
        RoleTrackImageUri,
        RoleCurIdx,
        RolePlayingState,
    };
    SearchResultContentViewModel(QAbstractListModel *parent = Q_NULLPTR)
        : QAbstractListModel(parent)
    {

    }
    virtual ~SearchResultContentViewModel()
    {

    }

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(parent);
        return m_dataList.size();
    }
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE
    {
        if (index.row() < 0 || index.row() >= m_dataList.size()) {
            return QVariant();
        }
//        DataProvider::MatchObject obj = m_dataList.value(keyList.at(index.row()));
        QList<MatchObject> list = m_dataList.value(m_keyList.at(index.row()));

        switch (role) {

        case RoleMatchTypes: {
            ITrackSearch::MatchTypes t = ITrackSearch::MatchUndefined;
            foreach (const auto &it, list) {
                t |= it.matchType();
            }
            return (int)t;
        }
        case RoleMatchFilePathObject: {
            foreach (const auto &it, list) {
                if (it.matchType() == ITrackSearch::MatchFilePath) {
                    QVariant v;
                    v.setValue(it);
                    return v;
                }
            }
            return QVariant();
        }
        case RoleMatchTrackNameObject: {
            foreach (const auto &it, list) {
                if (it.matchType() == ITrackSearch::MatchTrackName) {
                    QVariant v;
                    v.setValue(it);
                    return v;
                }
            }
            return QVariant();
        }
        case RoleMatchArtistNameObject: {
            foreach (const auto &it, list) {
                if (it.matchType() == ITrackSearch::MatchArtistName) {
                    QVariant v;
                    v.setValue(it);
                    return v;
                }
            }
            return QVariant();
        }
        case RoleMatchAlbumNameObject: {
            foreach (const auto &it, list) {
                if (it.matchType() == ITrackSearch::MatchAlbumName) {
                    QVariant v;
                    v.setValue(it);
                    return v;
                }
            }
            return QVariant();
        }
        case RoleAudioMetaObject: {
            QVariant v;
            v.setValue(list.first().audioMetaObject());
            return v;
        }
        case RoleAlbumImageUrl: {
            //TODO
            return QVariant();
        }
        case RoleAlbumName: {
            return list.first().audioMetaObject().albumMeta().name();
        }
        case RoleArtistImageUri: {
            return QVariant();
        }
        case RoleArtistName: {
            return list.first().audioMetaObject().artistMeta().name();
        }
        case RoleCoverArtLarge: {
            return QVariant();
        }
        case RoleCoverArtMiddle:{
            return QVariant();
        }
        case RoleCoverArtSmall: {
            return QVariant();
        }
        case RoleHash: {
            return list.first().audioMetaObject().hash();
        }
        case RoleMediaType: {
            return QVariant();
        }
        case RoleSongTitle: {
            QString str = list.first().audioMetaObject().trackMeta().title();
            if (str.isEmpty()) {
                str = list.first().audioMetaObject().name();
            }
            return str;
        }
        case RoleTrackImageUri: {
            return list.first().audioMetaObject().queryImgUri();
        }

        case RoleDuration: {
            return PPUtility::formateSongDuration(list.first().audioMetaObject().trackMeta().duration());
        }
        case RolePlayingState: {
            //TODO
//            if (m_playerCore->curTrackMetaObject().hash() == obj.hash()) {
//                return m_playerCore->playBackendStateInt();
//            }
            return -1;
        }

        case RoleCurIdx:
            return index == m_curIdx;
        default:
            return QVariant();
        }
        return QVariant();
    }
    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE
    {
        QHash<int, QByteArray> role;
        role.insert(ModelRoles::RoleMatchTypes, "RoleMatchTypes");
        role.insert(ModelRoles::RoleMatchFilePathObject, "RoleMatchFilePathObject");
        role.insert(ModelRoles::RoleMatchTrackNameObject, "RoleMatchTrackNameObject");
        role.insert(ModelRoles::RoleMatchArtistNameObject, "RoleMatchArtistNameObject");
        role.insert(ModelRoles::RoleMatchAlbumNameObject, "RoleMatchAlbumNameObject");

        role.insert(ModelRoles::RoleAudioMetaObject, "RoleAudioMetaObject");

        role.insert(ModelRoles::RoleAlbumImageUrl, "albumImageUrl");
        role.insert(ModelRoles::RoleAlbumName, "albumName");
        role.insert(ModelRoles::RoleArtistImageUri, "artistImageUri");
        role.insert(ModelRoles::RoleArtistName, "artistName");
        role.insert(ModelRoles::RoleCoverArtLarge, "coverArtLarge");
        role.insert(ModelRoles::RoleCoverArtMiddle, "coverArtMiddle");
        role.insert(ModelRoles::RoleCoverArtSmall, "coverArtSmall");
        role.insert(ModelRoles::RoleHash, "hash");
        role.insert(ModelRoles::RoleMediaType, "mediaType");
        role.insert(ModelRoles::RoleSongTitle, "songTitle");
        role.insert(ModelRoles::RoleTrackImageUri, "trackImageUri");
        role.insert(ModelRoles::RoleDuration, "RoleDuration");
        role.insert(ModelRoles::RoleCurIdx, "RoleCurIdx");
        role.insert(ModelRoles::RolePlayingState, "RolePlayingState");
        return role;;
    }

    void setDataMap(const QMap<QString, QList<DataProvider::MatchObject>> &map) {
        beginResetModel();
        endResetModel();

        m_dataList.clear();
        m_dataList = map;

        m_keyList.clear();
        m_keyList.append(m_dataList.keys());

        beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
        endInsertRows();
    }
public Q_SLOTS:
    inline void setCurIdx(const QModelIndex &idx)
    {
        m_curIdx = idx;
        Q_EMIT dataChanged(m_curIdx, m_curIdx);
    }

    // ImageProviderProxy interface
public:
    virtual void onImageFile(const QString &file, const QModelIndex &idx) Q_DECL_OVERRIDE
    {
        //TODO
    }

private:
    QModelIndex                         m_curIdx;
//    QList<DataProvider::MatchObject> m_dataList;
    QMap<QString, QList<DataProvider::MatchObject>> m_dataList;
    QStringList m_keyList;

};

/**************************************************************************************************
 *
 *
 *
 *
 **************************************************************************************************/

class SearchResultContentViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SearchResultContentViewDelegate(QObject *parent = Q_NULLPTR)
        : QStyledItemDelegate(parent)
    {

    }
    virtual ~SearchResultContentViewDelegate() {}

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        const RKListView *view = qobject_cast<const RKListView*>(option.widget);
        Q_ASSUME(view != Q_NULLPTR);
        QMap<RKTableHeaderItem::HeaderColumn, int> columnMap = view->getTableHeaders();

        painter->setRenderHints(QPainter::Antialiasing |
                                QPainter::HighQualityAntialiasing |
                                QPainter::SmoothPixmapTransform);
        const QRect rect = option.rect;

        int painterX = rect.x();

        QColor textColor;
        QColor timeTextColor;

        const bool isHover = index.data(SearchResultContentViewModel::ModelRoles::RoleCurIdx).toBool()
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
        {
            const int coverMargin = MAIN_VIEW_LISTITEM_CONTENT_MARGIN;
            const int coverAreaWidth = columnMap.value(RKTableHeaderItem::HeaderCover);
            const int minSize = qMin(coverAreaWidth, rect.height()-coverMargin*2);
            const QSize coverSz(minSize, minSize);
            // cover
            QPixmap coverPixmap;
            {
                QString imgUri = index.data(SearchResultContentViewModel::ModelRoles::RoleTrackImageUri).toUrl().toString();
                if (imgUri.toLower().startsWith("qrc:")) {
                    imgUri = imgUri.mid(3, imgUri.length()-3);
                }
                if (imgUri.isEmpty()) {
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
            const int playState = index.data(SearchResultContentViewModel::RolePlayingState).toInt();
            if (playState == (int)PPCommon::PlayBackendState::PlayBackendPaused) {

            } else if (playState == (int)PPCommon::PlayBackendState::PlayBackendPlaying) {

            }

            painterX += coverAreaWidth;
        }
        painter->setPen(textColor);
        painter->setBrush(textColor);

        int titleX = painterX;
        int artistX = titleX + columnMap.value(RKTableHeaderItem::HeaderTitle);
        int albumX = artistX + columnMap.value(RKTableHeaderItem::HeaderArtist);
        int durationX = albumX + columnMap.value(RKTableHeaderItem::HeaderAlbum);
        int matchTypes = index.data(SearchResultContentViewModel::RoleMatchTypes).toInt();

#define INIT_BASE(titleRole, contentRole) \
    const int textWidth = columnMap.value(titleRole); \
    QString text = index.data(contentRole).toString(); \
    QFont font = option.font; \
    font.setPixelSize(S_FONT_SIZE); \
    font.setBold(false); \
    painter->setFont(font); \
    QFontMetrics fm(font); \
    qreal textY = rect.y(); \
    if (text.isEmpty()) { \
        text = tr("Unknow"); \
    }

#define DRAW_QUERY(matchObj, pos) \
    const QString queryStr = matchObj.queryStr(); \
    if (!text.contains(queryStr)) { \
        const QRectF tf(pos+_to_px(2), textY, textWidth - _to_px(4), rect.height()); \
        painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text); \
    } else { \
        const int idx = text.indexOf(queryStr); \
        const int lw = fm.horizontalAdvance(text.left(idx)); \
        const int mw = fm.horizontalAdvance(queryStr + " "); \
        const int rw = textWidth - lw - mw - _to_px(4); \
        int xpos = pos + _to_px(2); \
        { \
            const QRectF tf(xpos, textY, lw, rect.height()); \
            painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text.left(idx)); \
            xpos += lw; \
        } \
        { \
            font.setBold(true); \
            font.setItalic(true); \
            painter->setFont(font); \
            const QRectF tf(xpos, textY, mw, rect.height()); \
            painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, queryStr + " "); \
            xpos += mw; \
        } \
        { \
            font.setBold(false); \
            font.setItalic(false); \
            painter->setFont(font); \
            const QRectF tf(xpos, textY, rw, rect.height()); \
            painter->drawText(tf, \
                              Qt::AlignLeft | Qt::AlignVCenter, \
                              text.right(text.length() - idx - queryStr.length())); \
        } \
    }


        // title
        {
            INIT_BASE(RKTableHeaderItem::HeaderTitle, SearchResultContentViewModel::RoleSongTitle)
            if (((matchTypes & ITrackSearch::MatchTrackName) == ITrackSearch::MatchTrackName)
                    || ((matchTypes & ITrackSearch::MatchFilePath) == ITrackSearch::MatchFilePath)) {
                QVariant va = index.data(SearchResultContentViewModel::RoleMatchTrackNameObject);
                if (va.isNull() || !va.isValid()) {
                    va = index.data(SearchResultContentViewModel::RoleMatchFilePathObject);
                }
                MatchObject mo = va.value<MatchObject>();
                DRAW_QUERY(mo, titleX)
            } else {
                const QRectF tf(titleX+_to_px(2), textY, textWidth - _to_px(4), rect.height());
                painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text);
            }
        }
        // artist
        {
            INIT_BASE(RKTableHeaderItem::HeaderArtist, SearchResultContentViewModel::RoleArtistName)

            if ((matchTypes & ITrackSearch::MatchArtistName) == ITrackSearch::MatchArtistName) {
                QVariant va = index.data(SearchResultContentViewModel::RoleMatchArtistNameObject);
                if (va.isNull() || !va.isValid()) {
                    const QRectF tf(artistX+_to_px(2), textY, textWidth - _to_px(4), rect.height());
                    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text);
                } else {
                    MatchObject mo = va.value<MatchObject>();
                    DRAW_QUERY(mo, artistX)
                }
            } else {
                const QRectF tf(artistX+_to_px(2), textY, textWidth - _to_px(4), rect.height());
                painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text);
            }
        }
        //album
        {
            INIT_BASE(RKTableHeaderItem::HeaderAlbum, SearchResultContentViewModel::RoleAlbumName)

            if ((matchTypes & ITrackSearch::MatchAlbumName) == ITrackSearch::MatchAlbumName) {
                QVariant va = index.data(SearchResultContentViewModel::RoleMatchAlbumNameObject);
                if (va.isNull() || !va.isValid()) {
                    const QRectF tf(albumX+_to_px(2), textY, textWidth - _to_px(4), rect.height());
                    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text);
                } else {
                    MatchObject mo = va.value<MatchObject>();
                    DRAW_QUERY(mo, albumX)
                }
            } else {
                const QRectF tf(albumX+_to_px(2), textY, textWidth - _to_px(4), rect.height());
                painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, text);
            }
        }
        //duration
        {
            painter->setPen(timeTextColor);
            painter->setBrush(timeTextColor);
            const int textWidth = columnMap.value(RKTableHeaderItem::HeaderDuration);
            QString text = index.data(SearchResultContentViewModel::RoleDuration).toString();
            if (text.isEmpty()) {
                text = tr("Unknow");
            }
            QFont font = option.font;
            font.setPixelSize(S_FONT_SIZE);
            painter->setFont(font);
            QFontMetrics fm(font);
            const QString str = fm.elidedText(text, Qt::ElideRight, textWidth - _to_px(4));
            qreal textY = rect.y();
            const QRectF tf(durationX, textY, textWidth - _to_px(4), rect.height());
            painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
        }
    }
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        return QSize(option.widget->width(), MAIN_VIEW_LISTITEM_H);
    }

    // QStyledItemDelegate interface
protected:
    virtual void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QStyledItemDelegate::initStyleOption(option, index);
    }

private:
    int     m_itemRadius    = 0;
    int     m_coverRadius   = 3;
    QColor  m_borderColor   = QColor(255, 0, 0, 152);
    QColor  m_shadowColor   = QColor(0, 255, 0, 126);
    QPixmap m_coverPixmap;
};


/**************************************************************************************************
 *
 *
 *
 *
 **************************************************************************************************/


SearchResultContentViewDataProvider::SearchResultContentViewDataProvider()
{
    m_header = new RKTableHeaderItem;
    m_header->setFixedHeight(_to_px(30));
    m_header->setEnableClick(false);
    m_header->setEnableArrowIcon(false);
    m_header->setCoverColumnWidth(_to_px(50));
    m_header->setCoverColumnIcon(DHiDPIHelper::loadNxPixmap(":/light/image/ic_format_list_numbered.svg"),
                                 DHiDPIHelper::loadNxPixmap(":/light/image/ic_format_list_numbered_hover.svg"));
//    m_header->setCoverColumnToolTip(QObject::tr("Reset to default order"));
    m_header->setDisplayedColumns(RKTableHeaderItem::HeaderCover |
                                  RKTableHeaderItem::HeaderTitle |
                                  RKTableHeaderItem::HeaderArtist |
                                  RKTableHeaderItem::HeaderAlbum |
                                  RKTableHeaderItem::HeaderDuration);

    m_model = new  SearchResultContentViewModel;

    m_delegate = new  SearchResultContentViewDelegate;
}

SearchResultContentViewDataProvider::~SearchResultContentViewDataProvider()
{

}

RKTableHeaderItem *SearchResultContentViewDataProvider::headerItem() const
{
    return m_header;
}

QAbstractListModel *SearchResultContentViewDataProvider::dataModel() const
{
    return m_model;
}

QStyledItemDelegate *SearchResultContentViewDataProvider::delegate() const
{
    return m_delegate;
}

void SearchResultContentViewDataProvider::resetDataModelToDefalutState()
{
    // dummy
}

/**************************************************************************************************
 *
 *
 *
 *
 **************************************************************************************************/

SearchResultContentView::SearchResultContentView(QWidget *parent)
    : BaseTrackView(new SearchResultContentViewDataProvider, parent)
{
    this->setObjectName("SearchResultContentView");
    DThemeManager::instance()->registerWidget(this);

    m_playerCore = new PlayerCore(this);
    m_plsMetaMgr = new PlayListMetaMgr(this);
}

SearchResultContentView::~SearchResultContentView()
{

}

void SearchResultContentView::setDataMap(const QMap<QString, QList<MatchObject> > &map)
{
    BaseTrackViewDataProvider *dp = this->dataProvider();
    SearchResultContentViewModel *model = qobject_cast<SearchResultContentViewModel*>(dp->dataModel());
    model->setDataMap(map);
}

void SearchResultContentView::resizeEvent(QResizeEvent *event)
{
    return BaseTrackView::resizeEvent(event);
}

void SearchResultContentView::showContextMenu(const QPoint &pos)
{
    const QModelIndex idx = indexAtPos(pos);
    if (!idx.isValid()) {
        qWarning()<<"Invalid QModelIndex!!";
        return;
    }

    QVariant va = this->getModel()->data(idx, SearchResultContentViewModel::RoleAudioMetaObject);
    if (va.isNull() || !va.isValid()) {
        ViewUtility::showToast(tr("Invalid audio meta object !!"));
        return;
    }
    AudioMetaObject obj = va.value<AudioMetaObject>();

    QMenu menu;
    menu.setStyle(QStyleFactory::create("dlight"));

    ViewUtility::menuAddToQueue(&menu, obj, m_playerCore);
    ViewUtility::menuAddToPlaylist(&menu, obj, m_plsMetaMgr);
    menu.addSeparator();
    ViewUtility::menuRemoveObject(&menu, obj);
    ViewUtility::menuShowInFileMgr(&menu, obj);
    menu.addSeparator();
    ViewUtility::menuTrackInfo(&menu, obj);

    menu.exec(this->mapToGlobal(pos));
}

void SearchResultContentView::onClicked(const QModelIndex &index)
{
    QVariant va = this->getModel()->data(index, SearchResultContentViewModel::RoleAudioMetaObject);
    if (va.isNull() || !va.isValid()) {
        ViewUtility::showToast(tr("Invalid audio meta object !!"));
        return;
    }
    AudioMetaObject obj = va.value<AudioMetaObject>();
    m_playerCore->playFromLibrary(obj.hash());
}


} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer

#include "SearchResultContentView.moc"
