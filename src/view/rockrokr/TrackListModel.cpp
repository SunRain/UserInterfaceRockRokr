#include "TrackListModel.h"

#include <QDebug>

#include "LibPhoenixPlayerMain.h"
#include "PPUtility.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/RecentPlayedQueue.h"
#include "MusicLibrary/MusicLibraryManager.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

TrackListModel::TrackListModel(QAbstractListModel *parent)
    : QAbstractListModel (parent)
{
    m_playerCore = new PlayerCore(this);
    m_libraryMgr = new MusicLibrary::MusicLibraryManager(this);
    m_imgProvider = new CategoryModelImageProvider(this, this);

    connect(m_playerCore, &PlayerCore::trackChanged, this, [&](){
        emit this->dataChanged(QModelIndex(), QModelIndex());
    });

    connect(m_libraryMgr, &MusicLibrary::MusicLibraryManager::libraryListSizeChanged,
            this, [&] () {
        switch (m_viewMode) {
        case ModeShowAllTracks:
            showAllTracks();
            break;
        case ModeShowFavorites:
            showFavorites();
            break;
        case ModeShowAlbumTracks:
            showAlbumTracks(m_keyForTrackGroupName, m_trackNumLimitForMode);
            break;
        case ModeShowArtistTracks:
            showArtistTracks(m_keyForTrackGroupName, m_trackNumLimitForMode);
            break;
        case ModeShowGenreTracks:
            showGenreTracks(m_keyForTrackGroupName, m_trackNumLimitForMode);
            break;
        default:
            break;
        }
    });
}

TrackListModel::~TrackListModel()
{
    if (m_playerCore) {
        m_playerCore->deleteLater();
        m_playerCore = Q_NULLPTR;
    }
    if (m_libraryMgr) {
        m_libraryMgr->deleteLater();
        m_libraryMgr = Q_NULLPTR;
    }
    if (m_imgProvider) {
        m_imgProvider->deleteLater();
        m_imgProvider = Q_NULLPTR;
    }
}

void TrackListModel::showAllTracks()
{
    m_viewMode = ViewMode::ModeShowAllTracks;

    clearList();
    m_dataList.append(m_libraryMgr->allTracks());

    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

void TrackListModel::showFavorites()
{
    m_viewMode = ViewMode::ModeShowFavorites;

    clearList();
    AudioMetaList list = m_libraryMgr->allTracks();
    foreach(const auto &obj, list) {
        if (m_libraryMgr->isLike(obj)) {
            m_dataList.append(obj);
        }
    }
    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

void TrackListModel::showArtistTracks(const QString &artistName, int limitNum)
{
    m_trackNumLimitForMode = limitNum;
    m_keyForTrackGroupName = artistName;
    m_viewMode = ViewMode::ModeShowArtistTracks;

    clearList();
    m_dataList.append(m_libraryMgr->artistTracks(artistName, limitNum));

    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

void TrackListModel::showAlbumTracks(const QString &albumName, int limitNum)
{
    m_trackNumLimitForMode = limitNum;
    m_keyForTrackGroupName = albumName;
    m_viewMode = ViewMode::ModeShowAlbumTracks;

    clearList();
    m_dataList.append(m_libraryMgr->albumTracks(albumName, limitNum));

    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

void TrackListModel::showGenreTracks(const QString &genreName, int limitNum)
{
    m_trackNumLimitForMode = limitNum;
    m_keyForTrackGroupName = genreName;
    m_viewMode = ViewMode::ModeShowGenreTracks;

    clearList();
    m_dataList.append(m_libraryMgr->genreTracks(genreName, limitNum));

    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

void TrackListModel::resetToDefalutState()
{
    switch (m_viewMode) {
    case ModeShowAllTracks:
        showAllTracks();
        break;
    case ModeShowFavorites:
        showFavorites();
        break;
    case ModeShowAlbumTracks:
        showAlbumTracks(m_keyForTrackGroupName, m_trackNumLimitForMode);
        break;
    case ModeShowArtistTracks:
        showArtistTracks(m_keyForTrackGroupName, m_trackNumLimitForMode);
        break;
    case ModeShowGenreTracks:
        showGenreTracks(m_keyForTrackGroupName, m_trackNumLimitForMode);
        break;
    default:
        break;
    }
}

int TrackListModel::rowCount(const QModelIndex &parent) const
{
    return m_dataList.size();
}

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_dataList.size()) {
        return QVariant();
    }
    AudioMetaObject obj = m_dataList.at(index.row());
    switch (role) {
    case RoleCurIdx:
        return index == m_curIdx;
    case RolePlayingState: {
        if (m_playerCore->curTrackMetaObject().hash() == obj.hash()) {
            return m_playerCore->playBackendStateInt();
        }
        return -1;
    }
    case RoleHash:
        return obj.hash();
    case RoleGenre:
        return obj.trackMeta().genre();
    case RoleDuration:
        return PPUtility::formateSongDuration(obj.trackMeta().duration());
    case RoleFileName:
        return obj.name();
    case RoleFilePath:
        return obj.path();
    case RoleAlbumName:
        return obj.albumMeta().name();
    case RoleMediaType:
        return obj.mediaType();
    case RoleSongTitle: {
        QString str = obj.trackMeta().title();
        if (str.isEmpty()) {
            str = obj.name();
        }
        return str;
    }
    case RoleArtistName:
        return obj.artistMeta().name();
    case RoleUserRating:
        return obj.trackMeta().userRating();
    case RoleAlbumImageUrl: {
        //TODO use image provider
        return obj.albumMeta().imgUri();
    }
    case RoleCoverArtLarge: {
        //TODO use image provider
        return obj.coverMeta().largeUri();
    }
    case RoleCoverArtSmall: {
        //TODO use image provider
        return obj.coverMeta().smallUri();
    }
    case RoleCoverArtMiddle: {
        //TODO use image provider
        return obj.coverMeta().middleUri();
    }
    case RoleTrackImageUri: {
        //TODO use image provider
        return obj.queryImgUri();
    }
    case RoleArtistImageUri: {
        //TODO use image provider
        return obj.artistMeta().imgUri();
    }
    default:
        return QVariant();
    }
}


QHash<int, QByteArray> TrackListModel::roleNames() const
{
    QHash<int, QByteArray> role;
    role.insert(ModelRoles::RoleAlbumImageUrl, "albumImageUrl");
    role.insert(ModelRoles::RoleAlbumName, "albumName");
    role.insert(ModelRoles::RoleArtistImageUri, "artistImageUri");
    role.insert(ModelRoles::RoleArtistName, "artistName");
    role.insert(ModelRoles::RoleCoverArtLarge, "coverArtLarge");
    role.insert(ModelRoles::RoleCoverArtMiddle, "coverArtMiddle");
    role.insert(ModelRoles::RoleCoverArtSmall, "coverArtSmall");
    role.insert(ModelRoles::RoleFileName, "fileName");
    role.insert(ModelRoles::RoleFilePath, "filePath");
    role.insert(ModelRoles::RoleGenre, "genre");
    role.insert(ModelRoles::RoleHash, "hash");
    role.insert(ModelRoles::RoleMediaType, "mediaType");
    role.insert(ModelRoles::RoleSongTitle, "songTitle");
    role.insert(ModelRoles::RoleUserRating, "userRating");
//    role.insert(ModelRoles::RoleTrackTitle, "trackTitle");
//    role.insert(ModelRoles::RoleTrackSubTitle, "trackSubTitle");
    role.insert(ModelRoles::RoleTrackImageUri, "trackImageUri");
    role.insert(ModelRoles::RoleDuration, "RoleDuration");
    role.insert(ModelRoles::RoleCurIdx, "RoleCurIdx");
    role.insert(ModelRoles::RolePlayingState, "RolePlayingState");
    return role;;
}

void TrackListModel::sort(int column, Qt::SortOrder order)
{
    //TODO use pinyin
    switch (column) {
    case (int)ModelRoles::RoleSongTitle: {
        if (order == Qt::SortOrder::DescendingOrder) {
            std::sort(m_dataList.begin(), m_dataList.end(),
                      [](const AudioMetaObject &a, AudioMetaObject &b) -> bool {
                          return a.trackMeta().title() > b.trackMeta().title();
                      });
        } else {
            std::sort(m_dataList.begin(), m_dataList.end(),
                      [](const AudioMetaObject &a, AudioMetaObject &b) -> bool {
                          return a.trackMeta().title() < b.trackMeta().title();
                      });
        }
        break;
    }
    case (int)ModelRoles::RoleAlbumName: {
        if (order == Qt::SortOrder::DescendingOrder) {
            std::sort(m_dataList.begin(), m_dataList.end(),
                      [](const AudioMetaObject &a, AudioMetaObject &b) -> bool {
                          return a.albumMeta().name() > b.albumMeta().name();
                      });
        } else {
            std::sort(m_dataList.begin(), m_dataList.end(),
                      [](const AudioMetaObject &a, AudioMetaObject &b) -> bool {
                          return a.albumMeta().name() < b.albumMeta().name();
                      });
        }
        break;
    }
    case (int)ModelRoles::RoleArtistName: {
        if (order == Qt::SortOrder::DescendingOrder) {
            std::sort(m_dataList.begin(), m_dataList.end(),
                      [](const AudioMetaObject &a, AudioMetaObject &b) -> bool {
                          return a.artistMeta().name() > b.artistMeta().name();
                      });
        } else {
            std::sort(m_dataList.begin(), m_dataList.end(),
                      [](const AudioMetaObject &a, AudioMetaObject &b) -> bool {
                          return a.artistMeta().name() < b.artistMeta().name();
                      });
        }
        break;
    }
    default:
        break;
    }
    beginResetModel();
    endResetModel();

    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

void TrackListModel::onImageFile(const QString &file, const QModelIndex &idx)
{

}

void TrackListModel::clearList()
{
    beginResetModel();
    m_dataList.clear();
    endResetModel();
}

void TrackListModel::setCurIdx(const QModelIndex &idx)
{
    m_curIdx = idx;
    emit dataChanged(m_curIdx, m_curIdx);
}

//TrackSortFilterProxyModel::TrackSortFilterProxyModel(QObject *parent)
//    : QSortFilterProxyModel (parent)
//{

//}

//TrackSortFilterProxyModel::~TrackSortFilterProxyModel()
//{

//}

//bool TrackSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
//{
//    qDebug()<<Q_FUNC_INFO<<" left "<<source_left<<" right "<<source_right;
//    QVariant left = sourceModel()->data(source_left);
//    QVariant right = sourceModel()->data(source_right);

//    qDebug()<<Q_FUNC_INFO<<" left data "<<left<<" right  data "<<right;
//    return true;

//}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
