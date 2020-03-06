#ifndef LEFTBAR_H
#define LEFTBAR_H

#include <QFrame>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DSeparatorHorizontal;
DWIDGET_END_NAMESPACE

class QListWidgetItem;
class QListWidget;
class QGraphicsOpacityEffect;
class QPropertyAnimation;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class LBListItem;
class LBListWidget;
class ItemFragment;
class LBPlaylistView;
class LeftBar : public QFrame
{
    Q_OBJECT
public:
    explicit LeftBar(QWidget *parent = nullptr);
    virtual ~LeftBar() override;

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

signals:
    void itemClicked(LBListItem *item);
    void playlistAddClicked();

private:
    void initUI();
    void addSpacingListWidgetItem(int height);
    QListWidgetItem *createListWidgetItem(int height);
    ItemFragment *createItemFragment(int height);

private:
    LBListWidget                                *m_listWidget;
    LBPlaylistView                              *m_plsView;
    DTK_WIDGET_NAMESPACE::DSeparatorHorizontal  *m_hSep;
    QGraphicsOpacityEffect                      *m_sepEffect;
    QPropertyAnimation                          *m_sepAnimation;
    bool                                        m_isSepHidden;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // LEFTBAR_H
