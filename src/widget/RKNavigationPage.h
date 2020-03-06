#ifndef RKNAVIGATIONPAGE_H
#define RKNAVIGATIONPAGE_H

#include <QFrame>

#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DImageButton;
class DWindowOptionButton;
class DWindowCloseButton;
class DWindowMinButton;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKNavigationPage : public QFrame
{
    Q_OBJECT
public:
    explicit RKNavigationPage(QWidget *parent = Q_NULLPTR);
    virtual ~RKNavigationPage();

protected:
    void initUserInterface();

private:
    DTK_WIDGET_NAMESPACE::DWindowOptionButton   *m_optionBtn    = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DWindowCloseButton    *m_closeBtn     = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DWindowMinButton      *m_minimizeBtn  = Q_NULLPTR;
};



} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer

#endif // RKNAVIGATIONPAGE_H
