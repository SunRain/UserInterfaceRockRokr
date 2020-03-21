#ifndef SEARCHRESULTCONTENTVIEW_H
#define SEARCHRESULTCONTENTVIEW_H


#include "DataProvider/ITrackSearch.h"

#include "view/rockrokr/BaseTrackView.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKTableHeaderItem;
class SearchResultContentViewModel;
class SearchResultContentViewDelegate;
class SearchResultContentViewDataProvider : public BaseTrackViewDataProvider
{
public:
    explicit SearchResultContentViewDataProvider();
    virtual ~SearchResultContentViewDataProvider();

    // BaseTrackViewDataProvider interface
public:
    virtual RKTableHeaderItem *headerItem() const Q_DECL_OVERRIDE;
    virtual QAbstractListModel *dataModel() const Q_DECL_OVERRIDE;
    virtual QStyledItemDelegate *delegate() const Q_DECL_OVERRIDE;
    virtual void resetDataModelToDefalutState() Q_DECL_OVERRIDE;

private:
    RKTableHeaderItem               *m_header  = Q_NULLPTR;
    SearchResultContentViewModel    *m_model = Q_NULLPTR;
    SearchResultContentViewDelegate *m_delegate = Q_NULLPTR;
};


class SearchResultContentView : public BaseTrackView
{
    Q_OBJECT
public:
    explicit SearchResultContentView(QWidget *parent = Q_NULLPTR);
    virtual ~SearchResultContentView() override;

    /*!
     * \brief setDataMap
     * \param map
     * key is uri in MatchObject, value is a list that have the same uri of MatchObject
     */
    void setDataMap(const QMap<QString, QList<DataProvider::MatchObject>> &map);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    // BaseTrackView interface
protected:
    virtual void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE;
    virtual void onClicked(const QModelIndex &index) Q_DECL_OVERRIDE;
};


} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer

#endif // SEARCHRESULTCONTENTVIEW_H
