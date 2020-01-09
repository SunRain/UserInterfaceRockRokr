#ifndef CATEGORYDETAILVIEW_H
#define CATEGORYDETAILVIEW_H

#include <QFrame>
#include <QLabel>

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

class CategoryDetailTrackView;
class CategoryDetailView : public QFrame
{
    Q_OBJECT
public:
    explicit CategoryDetailView(QWidget *parent = Q_NULLPTR);
    virtual ~CategoryDetailView() override;

    void showArtistTracks(const QString &artistName, const QString &subName, const QUrl &preferImg, int totalNum);
    void showAlbumTracks(const QString  &albumName, const QString &subName, const QUrl &preferImg, int totalNum);
    void showGenreTracks(const QString &genreName, const QString &subName, const QUrl &preferImg, int totalNum);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    void initUserInterface();
    void setTitle(const QString &text);
    void setText(const QString &text);

private:
    CategoryDetailTrackView     *m_trackView    = Q_NULLPTR;
    RKImage                     *m_imageView    = Q_NULLPTR;
    QLabel                      *m_titleLabel   = Q_NULLPTR;
    QLabel                      *m_textLabel    = Q_NULLPTR;
    QLabel                      *m_numLabel     = Q_NULLPTR;
    QString                     m_name;
    QUrl                        m_imgUri;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // CATEGORYDETAILVIEW_H
