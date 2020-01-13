#ifndef LBPLAYLISTVIEW_H
#define LBPLAYLISTVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QFrame>

#include <dbaseexpand.h>

//#include "PlayerCore/PlayListMeta.h"

#include "widget/RKExpand.h"



class QLabel;

DWIDGET_BEGIN_NAMESPACE
class DArrowButton;
class DBoxWidget;
class DVBoxWidget;
class DImageButton;
class DToast;
DWIDGET_END_NAMESPACE

DWIDGET_USE_NAMESPACE


namespace PhoenixPlayer {

    class PlayListMetaMgr;

    namespace UserInterface {

        namespace RockRokr {

class RKVBoxWidget;
class LBListItem;
class LBPlaylistView : public RKExpand
{
    Q_OBJECT
public:
    explicit LBPlaylistView(QWidget *parent = nullptr);
    virtual ~LBPlaylistView() override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

    // RKExpand interface
public:
    void setExpand(bool value) override;

protected:
    void showPlaylistRightClickMenu(LBListItem *item, const QPoint &pos);

private:
    void showDeleteDlg(LBListItem *item);
    void connectLBListItemSignals(LBListItem *item);

    LBListItem *findLBListItem(const QVariant &value);

private:
    QWidget                             *m_headerLine;
    DArrowButton                        *m_arrowBtn;
    DImageButton                        *m_ImgBtn;
    RKVBoxWidget                        *m_listWidget;
    PhoenixPlayer::PlayListMetaMgr      *m_plsMetaMgr;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // LBPLAYLISTVIEW_H
