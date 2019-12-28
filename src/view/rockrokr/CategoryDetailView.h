#ifndef CATEGORYDETAILVIEW_H
#define CATEGORYDETAILVIEW_H

#include <QFrame>

#include "BaseTrackView.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

    class RKTableHeaderItem;
    class TrackListModel;

class CategoryDetailViewDataProvider : public BaseTrackViewDataProvider
{
public:
    explicit CategoryDetailViewDataProvider();
    virtual ~CategoryDetailViewDataProvider() override;

    // BaseTrackViewDataProvider interface
public:
    RKTableHeaderItem *headerItem() const Q_DECL_OVERRIDE;
    TrackListModel *dataModel() const Q_DECL_OVERRIDE;
    void resetDataModelToDefalutState() const Q_DECL_OVERRIDE;

private:
    RKTableHeaderItem           *m_header  = Q_NULLPTR;
    TrackListModel              *m_dataModel = Q_NULLPTR;
};

class CategoryDetailTrackView : public BaseTrackView
{
    Q_OBJECT
public:
    explicit CategoryDetailTrackView(QWidget *parent = Q_NULLPTR);
    virtual ~CategoryDetailTrackView() override;

    // BaseTrackView interface
protected:
    void showContextMenu(const QPoint &pos) Q_DECL_OVERRIDE;
    void onClicked(const QModelIndex &index) Q_DECL_OVERRIDE;
};

class CategoryDetailView : public QFrame
{
    Q_OBJECT
public:
    explicit CategoryDetailView(QWidget *parent = Q_NULLPTR);
    virtual ~CategoryDetailView() override;

protected:
    void initUserInterface();


private:
    CategoryDetailTrackView       *m_trackView = Q_NULLPTR;

};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // CATEGORYDETAILVIEW_H
