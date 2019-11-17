#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QFrame>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DWaterProgress;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {

    namespace MusicLibrary {
        class MusicLibraryManager;
        class LocalMusicScanner;
    }

    namespace UserInterface {

        namespace RockRokr {

class LoadingWidget : public QFrame
{
    Q_OBJECT
public:
    explicit LoadingWidget(QWidget *parent = Q_NULLPTR);
    virtual ~LoadingWidget();

    void start();

    void stop();

private:
    DTK_WIDGET_NAMESPACE::DWaterProgress    *m_progress     = Q_NULLPTR;
    MusicLibrary::LocalMusicScanner         *m_localSC      = Q_NULLPTR;
    int m_totalFileSize         = 0;
    int m_remainingFileSize     = 0;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // LOADINGWIDGET_H
