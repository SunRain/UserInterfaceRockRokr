#ifndef CATEGORYVIEWDATAPROVIDER_H
#define CATEGORYVIEWDATAPROVIDER_H

#include <QtGlobal>

class QAbstractListModel;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class BaseCategoryModel;
class CategoryViewDataProvider
{
public:
    CategoryViewDataProvider() {}
    virtual ~CategoryViewDataProvider();

    virtual QAbstractListModel *recommendedModel() const = 0;
    virtual QAbstractListModel *recentModel() const = 0;
    virtual QAbstractListModel *allListModel() const = 0;
    virtual QString title() const = 0;
};

class AlbumCategoryDataProvider : public CategoryViewDataProvider
{
public:
    AlbumCategoryDataProvider();
    virtual ~AlbumCategoryDataProvider() override {}

    // CategoryViewDataProvider interface
public:
    QAbstractListModel *recommendedModel() const Q_DECL_OVERRIDE;
    QAbstractListModel *recentModel() const Q_DECL_OVERRIDE;
    QAbstractListModel *allListModel() const Q_DECL_OVERRIDE;
    QString title() const Q_DECL_OVERRIDE;

private:
    BaseCategoryModel       *m_recommendModel   = Q_NULLPTR;
    BaseCategoryModel       *m_recentModel      = Q_NULLPTR;
    BaseCategoryModel       *m_allListModel     = Q_NULLPTR;
};

class ArtistCategoryDataProvider : public CategoryViewDataProvider
{
public:
    ArtistCategoryDataProvider();
    virtual ~ArtistCategoryDataProvider() override {}

    // CategoryViewDataProvider interface
public:
    QAbstractListModel *recommendedModel() const Q_DECL_OVERRIDE;
    QAbstractListModel *recentModel() const Q_DECL_OVERRIDE;
    QAbstractListModel *allListModel() const Q_DECL_OVERRIDE;
    QString title() const Q_DECL_OVERRIDE;

private:
    BaseCategoryModel       *m_recommendModel   = Q_NULLPTR;
    BaseCategoryModel       *m_recentModel      = Q_NULLPTR;
    BaseCategoryModel       *m_allListModel     = Q_NULLPTR;
};

class GenresCategoryDataProvider : public CategoryViewDataProvider
{
public:
    GenresCategoryDataProvider();
    virtual ~GenresCategoryDataProvider() override {}

    // CategoryViewDataProvider interface
public:
    QAbstractListModel *recommendedModel() const Q_DECL_OVERRIDE;
    QAbstractListModel *recentModel() const Q_DECL_OVERRIDE;
    QAbstractListModel *allListModel() const Q_DECL_OVERRIDE;
    QString title() const Q_DECL_OVERRIDE;

private:
    BaseCategoryModel       *m_recommendModel   = Q_NULLPTR;
    BaseCategoryModel       *m_recentModel      = Q_NULLPTR;
    BaseCategoryModel       *m_allListModel     = Q_NULLPTR;
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // CATEGORYVIEWDATAPROVIDER_H
