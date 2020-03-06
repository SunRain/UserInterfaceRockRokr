#ifndef ROCKROKRTITLEBAR_H
#define ROCKROKRTITLEBAR_H

#include <QFrame>

#include <widget/RKTitleBar.h>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SearchEdit;
class SearchResultView;
class RockrokrTitleBar : public RKTitleBar
{
    Q_OBJECT
public:
    explicit RockrokrTitleBar(QWidget *parent = Q_NULLPTR);
    virtual ~RockrokrTitleBar() override;

    void bindResultView(SearchResultView *view);

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void initUserInterface();

private:
    SearchEdit                                  *m_searchEdit   = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // ROCKROKRTITLEBAR_H
