#ifndef LEFTBAR_H
#define LEFTBAR_H

#include <QFrame>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DSeparatorHorizontal;
DWIDGET_END_NAMESPACE

class LBListItem;
class QListWidget;
class QGraphicsOpacityEffect;
class QPropertyAnimation;

class LBListWidget;
class QListWidgetItem;
class ItemFragment;
class LBPlaylistView;
class LeftBar : public QFrame
{
    Q_OBJECT
public:
    explicit LeftBar(QWidget *parent = nullptr);
    virtual ~LeftBar() override;

//    void insertAnchorsForChildren(const DAnchorsBase *anchors);

    // QWidget interface
//protected:
//    void paintEvent(QPaintEvent *event) override;

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

#endif // LEFTBAR_H
