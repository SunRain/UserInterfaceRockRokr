#ifndef USERINTERFACEROCKROKR_H
#define USERINTERFACEROCKROKR_H

#include "AudioMetaObject.h"
#include "UserInterface/IUserInterface.h"

namespace PhoenixPlayer {

    namespace MusicLibrary {
        class MusicLibraryManager;
    }

    namespace UserInterface {

        namespace RockRokr {

class RKMainWindow;
class LIBPHOENIXPLAYER_EXPORT UserInterfaceRockRokr : public IUserInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "PhoenixPlayer.UserInterface.RokrUserInterface" FILE "RockRokr.json")
    Q_INTERFACES(PhoenixPlayer::BasePlugin)
public:
    explicit UserInterfaceRockRokr(QObject *parent = Q_NULLPTR);
    virtual ~UserInterfaceRockRokr() override;

    RKMainWindow *mainWindow() const;

    // BasePlugin interface
public:
    PluginProperty property() const Q_DECL_OVERRIDE;

    QWidget *settingsUI() const Q_DECL_OVERRIDE
    {
        return Q_NULLPTR;
    }
    QWidget *aboutUI() const Q_DECL_OVERRIDE
    {
        return Q_NULLPTR;
    }

    // IUserInterface interface
public:
    bool initialize() Q_DECL_OVERRIDE;
    void show() Q_DECL_OVERRIDE;
    void hide() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    QStringList snapshots() const Q_DECL_OVERRIDE;

private:
    RKMainWindow                          *m_mainWindow = Q_NULLPTR;
    MusicLibrary::MusicLibraryManager   *m_libraryMgr = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // USERINTERFACEROCKROKR_H
