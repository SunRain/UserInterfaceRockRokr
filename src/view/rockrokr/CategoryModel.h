#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QAbstractListModel>

#include "AudioMetaObject.h"
#include "AudioMetaGroupObject.h"

#include "CategoryModelImageProvider.h"

namespace PhoenixPlayer {

    class RecentPlayedQueue;
    class PlayerCore;

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

class CategoryObject;
class BaseCategoryModel : public QAbstractListModel, ImageProviderProxy
{
    Q_OBJECT
public:
    explicit BaseCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~BaseCategoryModel() override;

    ///
    /// \brief initData call this for init model data, or will be an empty model
    ///
    void initData();

    ///
    /// \brief setSizeLimit max size of model
    /// \param limit -1 if no limit
    ///
    inline void setSizeLimit(int limit = 20)
    {
        m_sizeLimit = limit;
    }

    inline int sizeLimit() const
    {
        return m_sizeLimit;
    }

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    // ImageProviderProxy interface
public:
    void onImageFile(const QString &file, const QModelIndex &idx) Q_DECL_OVERRIDE;

public:
    enum ModelRoles {
        RoleCategoryName = Qt::UserRole + 1,
        RoleCategorySubName,
        RoleCategorySubNameList,
        RoleTrackNum,                           //size of tracks in this category
        RoleImageUri,
        RoleCurIdx,
        RolePlayingState,
        RoleTrackDuration,
        RoleTrackHash
    };

public slots:
    void setCurIdx(const QModelIndex &idx);

protected:
    virtual void onInitData() = 0;

protected:
    inline QList<CategoryObject> *dataListPtr()
    {
        return &m_dataList;
    }
    inline PlayerCore *playerCore() const
    {
        return m_playCore;
    }
    inline MusicLibrary::MusicLibraryManager  *libraryMgr() const
    {
        return m_libraryMgr;
    }

    void clearList();

private:
    CategoryModelImageProvider          *m_imgProvider  = Q_NULLPTR;
    PlayerCore                          *m_playCore     = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libraryMgr   = Q_NULLPTR;
    int                                 m_sizeLimit     = 20;
    QList<CategoryObject>               m_dataList;
    QModelIndex                         m_curIdx;
};

//    TypeRecentAlbum = 0x0,
//    TypeRecommendedAlbum,
//    TypeAllAlbum,
//    TypeRecentArtist,
//    TypeRecommendedArtis,
//    TypeAllArtis,
//    TypeRecentGenres,
//    TypeRecommendedGenres,
//    TypeAllGenres,


class RecentAlbumCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit RecentAlbumCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~RecentAlbumCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class RecentArtistCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit RecentArtistCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~RecentArtistCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class RecentGenresCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit RecentGenresCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~RecentGenresCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class RecommendedAlbumCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit RecommendedAlbumCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~RecommendedAlbumCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class RecommendedArtistCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit RecommendedArtistCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~RecommendedArtistCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class RecommendedGenresCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit RecommendedGenresCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~RecommendedGenresCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class AllAlbumCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit AllAlbumCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~AllAlbumCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class AllArtistCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit AllArtistCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~AllArtistCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};

class AllGenresCategoryModel : public BaseCategoryModel
{
    Q_OBJECT
public:
    explicit AllGenresCategoryModel(QAbstractListModel *parent = Q_NULLPTR);
    virtual ~AllGenresCategoryModel() override;

    // BaseCategoryModel interface
protected:
    void onInitData() Q_DECL_OVERRIDE;
};



} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // CATEGORYMODEL_H
