#ifndef PLAYLISTDETAILVIEW_H
#define PLAYLISTDETAILVIEW_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>

#include "PlayerCore/PlayListMeta.h"

#include "BaseTrackView.h"

namespace PhoenixPlayer {

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

            class RKImage;
            class RKTableHeaderItem;
            class TrackListModel;

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
    PlayListDetailTrackView     *m_trackView    = Q_NULLPTR;
    RKImage                     *m_imgView      = Q_NULLPTR;
    QLabel                      *m_title        = Q_NULLPTR;
    QLabel                      *m_time         = Q_NULLPTR;
    QLabel                      *m_tag          = Q_NULLPTR;
    QLabel                      *m_ant          = Q_NULLPTR;



};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // PLAYLISTDETAILVIEW_H
