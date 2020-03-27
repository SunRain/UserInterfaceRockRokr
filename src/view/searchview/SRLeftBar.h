#ifndef SRLEFTBAR_H
#define SRLEFTBAR_H

#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <dtkwidget_global.h>

#include "PluginMgr.h"

DWIDGET_BEGIN_NAMESPACE
class DSeparatorHorizontal;
class DImageButton;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKListWidget;
class SRLeftBar : public QFrame
{
    Q_OBJECT
public:
    explicit SRLeftBar(QWidget *parent = Q_NULLPTR);
    virtual ~SRLeftBar() override;

    void setEnabledPlugins(const QList<PluginMetaData> &list);

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void clickedBackBtn();

protected:
    void initUserInterface();

private:
    DTK_WIDGET_NAMESPACE::DSeparatorHorizontal  *m_hSep         = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DImageButton          *m_backBtn      = Q_NULLPTR;
    QPropertyAnimation                          *m_sepAnimation = Q_NULLPTR;
    QGraphicsOpacityEffect                      *m_sepEffect    = Q_NULLPTR;
    RKListWidget                                *m_pluginView   = Q_NULLPTR;

};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // SRLEFTBAR_H
