#ifndef RKTITLEBAR_H
#define RKTITLEBAR_H

#include <QFrame>

#include <dtkwidget_global.h>
#include <DGraphicsDropShadowEffect>
#include <DTitlebar>

DWIDGET_BEGIN_NAMESPACE
class DTitlebar;
class DImageButton;
class DWindowOptionButton;
class DWindowCloseButton;
class DWindowMinButton;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SearchEdit;
class SearchResultView;
class RKTitleBar : public QFrame//DTK_WIDGET_NAMESPACE::DTitlebar
{
    Q_OBJECT
public:
    explicit RKTitleBar(QWidget *parent = Q_NULLPTR);
    virtual ~RKTitleBar() override;

    void bindResultView(SearchResultView *view);

private:
    DTK_WIDGET_NAMESPACE::DWindowOptionButton   *m_optionBtn    = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DWindowCloseButton    *m_closeBtn     = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DWindowMinButton      *m_minimizeBtn  = Q_NULLPTR;
    SearchEdit                                  *m_searchEdit   = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKTITLEBAR_H
