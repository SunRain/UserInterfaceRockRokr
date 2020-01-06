#ifndef CATEGORYDETAILVIEW_H
#define CATEGORYDETAILVIEW_H

#include <QFrame>

#include "BaseTrackView.h"

namespace PhoenixPlayer {

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

            class RKTableHeaderItem;
            class TrackListModel;

class CategoryDetailTrackView;
class CategoryDetailView : public QFrame
{
    Q_OBJECT
public:
    explicit CategoryDetailView(QWidget *parent = Q_NULLPTR);
    virtual ~CategoryDetailView() override;

    void showArtistTracks(const QString &artistName);
    void showAlbumTracks(const QString  &albumName);
    void showGenreTracks(const QString &genreName);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    void initUserInterface();


private:
    CategoryDetailTrackView     *m_trackView = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // CATEGORYDETAILVIEW_H
