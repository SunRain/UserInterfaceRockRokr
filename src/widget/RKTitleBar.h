#ifndef RKTITLEBAR_H
#define RKTITLEBAR_H

#include <QFrame>
#include <QHBoxLayout>

#include <dtkwidget_global.h>
#include <DGraphicsDropShadowEffect>
#include <DTitlebar>

DWIDGET_BEGIN_NAMESPACE
class DWindowOptionButton;
class DWindowCloseButton;
class DWindowMinButton;
class DArrowButton;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKTitleBar : public QFrame
{
    Q_OBJECT
public:
    enum DisplayedButton
    {
        WindowOptionButton = 0x1,
        WindowCloseButton = 0x2,
        WindowMinButton = 0x4,
        WindowBackButton = 0x8,
        WindowUndefined = 0x0
    };
    Q_DECLARE_FLAGS(DisplayedButtons, DisplayedButton)
    Q_FLAG(DisplayedButtons)

    explicit RKTitleBar(QWidget *parent = Q_NULLPTR);
    virtual ~RKTitleBar() override;

    void setDisplayedButtons(DisplayedButtons buttons);

    QWidget *getContentWidget() const
    {
        return m_contentWidget;
    }

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    void buttonClicked(DisplayedButton button);

private:
    DTK_WIDGET_NAMESPACE::DWindowOptionButton   *m_optionBtn    = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DWindowCloseButton    *m_closeBtn     = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DWindowMinButton      *m_minimizeBtn  = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DArrowButton          *m_backBtn      = Q_NULLPTR;

    QHBoxLayout *m_iconLayout       = Q_NULLPTR;
    QHBoxLayout *m_mainLayout       = Q_NULLPTR;

    QWidget     *m_contentWidget    = Q_NULLPTR;

    DisplayedButtons m_buttons = WindowUndefined;
 };

} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer

Q_DECLARE_OPERATORS_FOR_FLAGS(PhoenixPlayer::UserInterface::RockRokr::RKTitleBar::DisplayedButtons)

#endif // RKTITLEBAR_H
