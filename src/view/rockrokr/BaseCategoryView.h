#ifndef BASECATEGORYVIEW_H
#define BASECATEGORYVIEW_H

#include <QFrame>

class QListView;
class QAbstractListModel;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKListView;
class CategoryRecommendedViewDelegate;
class CategoryRecentViewDelegate;
class CategoryAllViewDelegate;
class CategoryViewDataProvider;
class BaseCategoryView : public QFrame
{
    Q_OBJECT
public:
    explicit BaseCategoryView(CategoryViewDataProvider *pr, QWidget *parent = Q_NULLPTR);
    virtual ~BaseCategoryView() override;

    void initUI();

    void initListViewParameter(QListView *view);


private:
    RKListView                          *m_recommendedListView      = Q_NULLPTR;
    RKListView                          *m_recentListView           = Q_NULLPTR;
    RKListView                          *m_allListView              = Q_NULLPTR;
    CategoryRecommendedViewDelegate     *m_recommedViewDelegate     = Q_NULLPTR;
    CategoryRecentViewDelegate          *m_recentViewDelegate       = Q_NULLPTR;
    CategoryAllViewDelegate             *m_allViewDelegate          = Q_NULLPTR;
    CategoryViewDataProvider            *m_dataProvider             = Q_NULLPTR;

};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // BASECATEGORYVIEW_H
