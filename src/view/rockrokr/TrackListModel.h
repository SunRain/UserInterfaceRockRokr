#ifndef TRACKLISTMODEL_H
#define TRACKLISTMODEL_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include "AudioMetaObject.h"
#include "PlayerCore/PlayListMeta.h"

#include "CategoryModelImageProvider.h"

namespace PhoenixPlayer {

    class PlayerCore;
    class RecentPlayedQueue;

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

class TrackListModel : public QAbstractListModel, ImageProviderProxy
{
    Q_OBJECT
public:
    explicit TrackListModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~TrackListModel() override;

    void showAllTracks();

    void showFavorites();

    void showArtistTracks(const QString &artistName, int limitNum = 0);

    void showAlbumTracks(const QString  &albumName, int limitNum = 0);

    void showGenreTracks(const QString &genreName, int limitNum = 0);

    void showPlaylist(const PlayListMeta &meta);

    /*!
     * \brief resetToDefalutState
     * Reset mode to current ViewMode type with current limitNum
     */
    void resetToDefalutState();

public:
    enum ModelRoles {
        RoleFilePath = Qt::UserRole + 1,
        RoleFileName,
        RoleArtistName,
        RoleArtistImageUri,
        RoleAlbumName,
        RoleAlbumImageUrl,
        RoleCoverArtSmall,
        RoleCoverArtLarge,
        RoleCoverArtMiddle,
        RoleMediaType,
        RoleSongTitle,
        RoleUserRating,
        RoleGenre,
        RoleHash,
        RoleDuration,
//        RoleTrackTitle,     //播放列表中第一行显示内容
//        RoleTrackSubTitle,   //播放列表中第二行显示内容
        /*
         * 歌曲封面图片，按照
         * CoverArtMiddle
         * CoverArtLarge
         * CoverArtSmall
         * AlbumImageUrl
         * ArtistImageUri
         * 顺序返回图片地址，如果都不存在，则返回空
         */
        RoleTrackImageUri,
        RoleCurIdx,
        RolePlayingState
    };

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    ///
    /// \brief sort
    /// \param column use ModelRoles
    /// \param order
    ///
    void sort(int column = (int)RoleSongTitle, Qt::SortOrder order = Qt::SortOrder::AscendingOrder) Q_DECL_OVERRIDE;

    // ImageProviderProxy interface
public:
    void onImageFile(const QString &file, const QModelIndex &idx) Q_DECL_OVERRIDE;

protected:
    void clearList();

public slots:
    void setCurIdx(const QModelIndex &idx);

private:
    enum ViewMode {
        ModeUndefined = 0x0,
        ModeShowAllTracks,
        ModeShowFavorites,
        ModeShowArtistTracks,
        ModeShowAlbumTracks,
        ModeShowGenreTracks,
        ModeShowPlaylist
    };

private:
    PlayerCore                          *m_playerCore       = Q_NULLPTR;
    CategoryModelImageProvider          *m_imgProvider      = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libraryMgr       = Q_NULLPTR;
    AudioMetaList                       m_dataList;
    QModelIndex                         m_curIdx;
    ViewMode                            m_viewMode          = ViewMode::ModeUndefined;
    int                                 m_trackNumLimitForMode = 0;
    QString                             m_keyForTrackGroupName;
    PlayListMeta                        m_plsMeta;
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // TRACKLISTMODEL_H
