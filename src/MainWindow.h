#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
//#include <DAnchors>
//#include "dborderlesswindow.h"

//#include "widget/dwindowframe.h"

DWIDGET_BEGIN_NAMESPACE
class DToast;
DWIDGET_END_NAMESPACE

class QStackedWidget;
class QStackedLayout;
class QTimer;
class LeftBar;
class ImportView;
class LoadingWidget;
class RockRokrView;
class RKTitleBar;
class RKStackedWidget;
class MainWindow : public DTK_WIDGET_NAMESPACE::DMainWindow
{
    Q_OBJECT

    friend class RockRokrApp;
protected:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    Q_SLOT void show();

public:
    void showImportView();

    void showLoadingView();

    void showRockRokrView();

    void showPlayingView();

    void setDefaultTitlebarHidden(bool hidden);

    void showFileImportDlg();

    void showTip(QWidget *baseWidget, const QString &text, int delayShowMilliseconds = 0);
    void hideTip(int delayMilliseconds = 0);

private:
    QWidget                             *m_mainWidget;
    QTimer                              *m_tipTimer         = Q_NULLPTR;
//    QStackedLayout                    *m_topStack;
//    QStackedWidget                    *m_mainStack;
    RKStackedWidget                     *m_stack;
    LeftBar                             *m_leftBar;
    ImportView                          *m_importView;
    LoadingWidget                       *m_loadingWidget;
    RockRokrView                        *m_rkView;
    RKTitleBar                          *m_rkTitleBar;
    DTK_WIDGET_NAMESPACE::DToast        *m_tip              = Q_NULLPTR;
    const int                           m_defaultTitlebarH;



};

#endif // MAINWINDOW_H
