#include "CategoryViewDataProvider.h"

#include <QObject>

#include "CategoryModel.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


CategoryViewDataProvider::~CategoryViewDataProvider()
{

}

AlbumCategoryDataProvider::AlbumCategoryDataProvider()
{
    m_recommendModel = new RecommendedAlbumCategoryModel;

    m_recentModel = new RecentAlbumCategoryModel;

    m_allListModel = new AllAlbumCategoryModel;
}

QAbstractListModel *AlbumCategoryDataProvider::recommendedModel() const
{
    return m_recommendModel;
}

QAbstractListModel *AlbumCategoryDataProvider::recentModel() const
{
    return m_recentModel;
}

QAbstractListModel *AlbumCategoryDataProvider::allListModel() const
{
    return m_allListModel;
}

QString AlbumCategoryDataProvider::title() const
{
    return QObject::tr("Album");
}

ArtistCategoryDataProvider::ArtistCategoryDataProvider()
{
    m_recommendModel = new RecommendedArtistCategoryModel;

    m_recentModel = new RecentArtistCategoryModel;

    m_allListModel = new AllArtistCategoryModel;
}

QAbstractListModel *ArtistCategoryDataProvider::recommendedModel() const
{
    return m_recommendModel;
}

QAbstractListModel *ArtistCategoryDataProvider::recentModel() const
{
    return m_recentModel;
}

QAbstractListModel *ArtistCategoryDataProvider::allListModel() const
{
    return m_allListModel;
}

QString ArtistCategoryDataProvider::title() const
{
    return QObject::tr("Artist");
}

GenresCategoryDataProvider::GenresCategoryDataProvider()
{
    m_recommendModel = new RecommendedGenresCategoryModel;

    m_recentModel = new RecentGenresCategoryModel;

    m_allListModel = new AllGenresCategoryModel;
}

QAbstractListModel *GenresCategoryDataProvider::recommendedModel() const
{
    return m_recommendModel;
}

QAbstractListModel *GenresCategoryDataProvider::recentModel() const
{
    return m_recentModel;
}

QAbstractListModel *GenresCategoryDataProvider::allListModel() const
{
    return m_allListModel;
}

QString GenresCategoryDataProvider::title() const
{
    return QObject::tr("Genres");
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
