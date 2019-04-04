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


class RKSearchEdit;
class RKTitleBar : public QFrame//DTK_WIDGET_NAMESPACE::DTitlebar
{
    Q_OBJECT
public:
    explicit RKTitleBar(QWidget *parent = Q_NULLPTR);
    virtual ~RKTitleBar() override;



private:
    DTK_WIDGET_NAMESPACE::DWindowOptionButton   *m_optionBtn;
    DTK_WIDGET_NAMESPACE::DWindowCloseButton    *m_closeBtn;
    DTK_WIDGET_NAMESPACE::DWindowMinButton      *m_minimizeBtn;
    RKSearchEdit                                *m_searchEdit;

};

#endif // RKTITLEBAR_H
