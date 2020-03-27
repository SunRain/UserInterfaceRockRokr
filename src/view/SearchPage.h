#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QFrame>
#include <QLabel>

#include "PluginMgr.h"
#include "DataProvider/ITrackSearch.h"

class QParallelAnimationGroup;
class QPropertyAnimation;

namespace PhoenixPlayer {
    namespace DataProvider {
        class TrackSearchProvider;
    } // namespace DataProvider

namespace UserInterface {
namespace RockRokr {

class RKStackedWidget;
class SRLeftBar;
class PlayBar;
class RKTitleBar;
class SPSearchEdit;
class SearchPage : public QFrame
{
    Q_OBJECT
public:
    explicit SearchPage(QWidget *parent = Q_NULLPTR);
    virtual ~SearchPage();

    void bindTrackSearchProvider(DataProvider::TrackSearchProvider *provider);

    void setBackgroundPixmap(const QPixmap &pm);

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    void initUserInterface();
    void showHideAnimation();

private:
    SRLeftBar       *m_leftBar      = Q_NULLPTR;
    RKTitleBar      *m_titlebar     = Q_NULLPTR;
    SPSearchEdit    *m_searchEdit   = Q_NULLPTR;
    PlayBar         *m_playbar      = Q_NULLPTR;
    DataProvider::TrackSearchProvider *m_searchProvider = Q_NULLPTR;
    QWidget         *m_rPart        = Q_NULLPTR;
    QLabel          *m_bgLabel      = Q_NULLPTR;
    RKStackedWidget *m_stack        = Q_NULLPTR;


    QParallelAnimationGroup *m_animGroup = Q_NULLPTR;
    QPropertyAnimation      *m_leftAnim = Q_NULLPTR;
    QPropertyAnimation      *m_rightAnim = Q_NULLPTR;

    bool m_hideState = false;

    QRect m_leftGeometry;
    QRect m_rightGeometry;

    QList<PluginMetaData> m_enabledPlugins;

    //key PluginMetaData->PluginProperty->name
    QMap<QString, QList<DataProvider::MatchObject>> m_matchRetMap;
};

} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer
#endif // SEARCHPAGE_H
