#ifndef PLAYLISTDETAILVIEW_H
#define PLAYLISTDETAILVIEW_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>

#include <dtkwidget_global.h>

#include "PlayerCore/PlayListMeta.h"
#include "BaseTrackView.h"

DWIDGET_BEGIN_NAMESPACE
class DImageButton;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

            class RKImage;
            class RKTableHeaderItem;
            class TrackListModel;
            class RKLineEdit;

class PlayListDetailTrackView;
class PlayListDetailView : public QFrame
{
    Q_OBJECT
public:
    explicit PlayListDetailView(QWidget *parent = Q_NULLPTR);
    virtual ~PlayListDetailView() override;

    void showPlayList(const PlayListMeta &meta);

protected:
    void initUserInterface();

private:
    PlayListDetailTrackView             *m_trackView    = Q_NULLPTR;
    RKImage                             *m_imgView      = Q_NULLPTR;
    QLabel                              *m_title        = Q_NULLPTR;
    QLabel                              *m_time         = Q_NULLPTR;
    QLabel                              *m_taglb        = Q_NULLPTR;
    QLabel                              *m_antlb        = Q_NULLPTR;
    RKLineEdit                          *m_tagedt       = Q_NULLPTR;
    RKLineEdit                          *m_antedt       = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DImageButton  *m_tagBtn       = Q_NULLPTR;
    DTK_WIDGET_NAMESPACE::DImageButton  *m_antBtn       = Q_NULLPTR;

    PlayListMeta                        m_meta;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // PLAYLISTDETAILVIEW_H
