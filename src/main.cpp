#include "MainWindow.h"

#include <DApplication>

#include <DThemeManager>

#include "libphoenixplayer_global.h"

#include "RockRokrApp.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    QScopedPointer<QApplication> app(new QApplication(argc, argv));
    app.data()->setOrganizationName("SunRain");
    app.data()->setApplicationName("RockRokr");
    app.data()->setApplicationDisplayName(QObject::tr("RockRokr Player"));
    app.data()->setApplicationVersion("1.0.0");

    PhoenixPlayer::LibPhoenixPlayer_Init();

    DThemeManager::instance()->setTheme("light");

//    MainWindow w;
//    w.show();
    RockRokrApp::instance()->init();
    RockRokrApp::instance()->show();

    return app->exec();
}
