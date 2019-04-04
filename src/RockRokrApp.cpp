#include "RockRokrApp.h"




#include "SingletonPointer.h"
#include "LibPhoenixPlayerMain.h"
#include "PPSettings.h"
#include "MusicLibrary/MusicLibraryManager.h"
#include "MusicLibrary/LocalMusicScanner.h"

#include "MainWindow.h"

using namespace PhoenixPlayer;
using namespace PhoenixPlayer::MusicLibrary;


RockRokrApp::RockRokrApp(QObject *parent)
    : QObject(parent)
{

}

RockRokrApp *RockRokrApp::createInstance()
{
    return new RockRokrApp;
}

RockRokrApp *RockRokrApp::instance()
{
    return Singleton<RockRokrApp>::instance(RockRokrApp::createInstance);
}

RockRokrApp::~RockRokrApp()
{
    if (m_mainWindow) {
        m_mainWindow->deleteLater();
    }
    m_mainWindow = Q_NULLPTR;
}

void RockRokrApp::init()
{
    initUI();

#if 1
    if (phoenixPlayerLib->libraryMgr()->isEmpty()) {
        m_mainWindow->showImportView();
    } else {
        m_mainWindow->showRockRokrView();
    }
#else
    m_stack->setCurrentWidget(m_importView);
//m_topStack->setCurrentWidget(m_mainStack);
//m_mainStack->setCurrentWidget(m_rkView);
#endif



}

void RockRokrApp::show()
{
    m_mainWindow->show();
}

MainWindow *RockRokrApp::mainWindow() const
{
    return m_mainWindow;
}

void RockRokrApp::initUI()
{
    m_mainWindow = new MainWindow;
}


