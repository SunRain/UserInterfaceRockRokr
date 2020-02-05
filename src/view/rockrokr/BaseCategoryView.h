#ifndef BASECATEGORYVIEW_H
#define BASECATEGORYVIEW_H

#include <QFrame>

class QListView;
class QAbstractListModel;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class BaseCategoryModel;
class RKListView;
class CategoryRecommendedViewDelegate;
class CategoryRecentViewDelegate;
class CategoryAllViewDelegate;
class CategoryViewDataProvider;
class BaseCategoryView : public QFrame
{
    Q_OBJECT
public:
    enum ViewType {
        ViewTypeRecommended = 0x0,
        ViewTypeRecent,
        ViewTypeAllList
    };
    Q_ENUM(ViewType)

    explicit BaseCategoryView(CategoryViewDataProvider *pr, QWidget *parent = Q_NULLPTR);
    virtual ~BaseCategoryView() override;

    void initUI();

    void initListViewParameter(QListView *view);

Q_SIGNALS:
    void viewClicked(ViewType type, BaseCategoryModel *model, const QModelIndex &index);

    void customContextMenuRequested(ViewType type, BaseCategoryModel *model,
                                    const QModelIndex &index, const QPoint &globalPos);

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
