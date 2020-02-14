#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include "widget/RKSearchEdit.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SearchResultView;
class SearchEdit : public RKSearchEdit
{
    Q_OBJECT
public:
    explicit SearchEdit(QWidget *parent = Q_NULLPTR);
    virtual ~SearchEdit();

    void bindResultView(SearchResultView *view);

protected:
//    void onFocusIn();
    void onFocusOut();
    void onTextChanged();
    void onReturnPressed();

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    SearchResultView    *m_resultView = Q_NULLPTR;
    QPoint              m_topLeftToGlobal;
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // SEARCHEDIT_H
