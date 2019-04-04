#include "MainWindow.h"

#include <QApplication>
#include <QMouseEvent>
#include <QScreen>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QTimer>

#include <DWidgetUtil>
#include <DTitlebar>
#include <QLabel>
#include <DToast>


#include "LibPhoenixPlayerMain.h"
#include "PPSettings.h"
#include "MusicLibrary/MusicLibraryManager.h"
#include "MusicLibrary/LocalMusicScanner.h"

#include "rockrokr_global.h"
#include "widget/LoadingWidget.h"
#include "widget/RKStackWidget.h"
#include "view/ImportView.h"
#include "view/MainWindowBGView.h"
#include "view/RockRokrView.h"
#include "view/titlebar/RKTitleBar.h"

DWIDGET_USE_NAMESPACE
using namespace PhoenixPlayer;
using namespace PhoenixPlayer::MusicLibrary;

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent),
      m_defaultTitlebarH(this->titlebar()->height())
{
    this->setMouseTracking(true);

    m_mainWidget = new MainWindowBGView(this);
    m_mainWidget->resize(MAIN_WINDOW_W, MAIN_WINDOW_H);
    this->setCentralWidget(m_mainWidget);


//    m_topStack = new QStackedLayout(m_mainWidget);
//    m_topStack->setContentsMargins(0, 0, 0, 0);
//    m_topStack->setMargin(0);
//    m_topStack->setSpacing(0);

    QVBoxLayout *ly = new QVBoxLayout(m_mainWidget);
    ly->setContentsMargins(0, 0, 0, 0);
    ly->setSpacing(0);
    m_stack = new RKStackedWidget(m_mainWidget);
    m_stack->setContentsMargins(0, 0, 0, 0);
    m_stack->setFadeEnable(true);
    m_stack->setAnimatEnable(true);
    m_stack->setDuration(2000);
    ly->addWidget(m_stack);


//    m_rkTitleBar = new RKTitleBar(this);
//    m_rkTitleBar->setFixedSize(MAIN_WINDOW_W, TITLE_BAR_H);
//    m_rkTitleBar->setStyleSheet(QString("QWidget{background-color:red;}"));


    m_importView = new ImportView;
    m_loadingWidget = new LoadingWidget;
    m_rkView = new RockRokrView;

    m_stack->addWidget(m_importView);
    m_stack->addWidget(m_loadingWidget);
    m_stack->addWidget(m_rkView);


    connect(m_importView, &ImportView::scanStandardMusicPath,
            this, [&]() {
        m_stack->setLength(1200, RKStackedWidget::AnimationType::LeftToRight);
//        m_stack->setCurrentWidget(m_loadingWidget);
        m_stack->setCurrentIndex(m_stack->indexOf(m_loadingWidget));
        m_loadingWidget->start();
        phoenixPlayerLib->libraryMgr()->localMusicScanner()->scanLocalMusic();
    });
    connect(m_importView, &ImportView::importFiles,
            this, [&](const QStringList &list){
//        m_stack->setCurrentWidget(m_loadingWidget);
        m_stack->setLength(1200, RKStackedWidget::AnimationType::LeftToRight);
        m_stack->setCurrentIndex(m_stack->indexOf(m_loadingWidget));
        m_loadingWidget->start();
        phoenixPlayerLib->libraryMgr()->localMusicScanner()->scarnDirs(list);
    });
    connect(m_importView, &ImportView::showImportDlg,
            this, &MainWindow::showFileImportDlg);

    connect(phoenixPlayerLib->libraryMgr()->localMusicScanner(), &LocalMusicScanner::searchingFinished,
            this, [&](){
        m_loadingWidget->stop();
//        m_stack->setCurrentWidget(m_rkView);
        m_stack->setLength(1200, RKStackedWidget::AnimationType::LeftToRight);
        m_stack->setCurrentIndex(m_stack->indexOf(m_rkView));
    });

    connect(m_stack, &QStackedWidget::currentChanged,
            this, [&](int idx) {
        if (m_stack->widget(idx) != m_loadingWidget) {
            m_loadingWidget->stop();
        }
        if (m_stack->widget(idx) != m_rkView) {
            this->setDefaultTitlebarHidden(false);
        } else {
            this->setDefaultTitlebarHidden(true);
        }
    });
}

MainWindow::~MainWindow()
{

}

void MainWindow::show()
{
//    QScreen *screen = QApplication::primaryScreen();
//    QRect  geometry = screen->geometry();
//    setMinimumSize(QSize(geometry.width()*1/2, geometry.height()*1/2));
//    setMinimumWidth(geometry.width()/2);
//    setMinimumHeight(geometry.height()/2);
//    resize(geometry.width()/2, geometry.height()/2);
    this->resize(MAIN_WINDOW_W, MAIN_WINDOW_H);

    DMainWindow::show();

    Dtk::Widget::moveToCenter(this);
}

void MainWindow::showImportView()
{
    m_stack->setCurrentIndex(m_stack->indexOf(m_importView));
}

void MainWindow::showLoadingView()
{
    m_stack->setCurrentIndex(m_stack->indexOf(m_loadingWidget));
    m_loadingWidget->start();
}

void MainWindow::showRockRokrView()
{
    m_stack->setCurrentIndex(m_stack->indexOf(m_rkView));
}

void MainWindow::showPlayingView()
{
    //TODO
}

void MainWindow::setDefaultTitlebarHidden(bool hidden)
{
    if (hidden) {
        this->titlebar()->setEnabled(false);
        this->titlebar()->setVisible(false);
        this->titlebar()->setFixedHeight(0);
    } else {
        this->titlebar()->setEnabled(true);
        this->titlebar()->setVisible(true);
        this->titlebar()->setFixedHeight(m_defaultTitlebarH);
    }
}

#if 1
void MainWindow::showFileImportDlg()
{
    QFileDialog dlg(this);

    dlg.setDirectory(phoenixPlayerLib->settings()->defaultMusicDir());

    dlg.setViewMode(QFileDialog::Detail);
    dlg.setFileMode(QFileDialog::DirectoryOnly);
    if (QFileDialog::Accepted == dlg.exec()) {
//        m_topStack->setCurrentWidget(m_loadingWidget);
        m_loadingWidget->start();
        phoenixPlayerLib->libraryMgr()->localMusicScanner()->scanDir(dlg.directory().path());
    }
}

void MainWindow::showTip(QWidget *baseWidget, const QString &text, int delayShowMilliseconds)
{
    if (!m_tipTimer) {
        m_tipTimer = new QTimer(this);
        m_tipTimer->setSingleShot(true);
        connect(m_tipTimer, &QTimer::timeout,
                this, [&](){
            if (m_tip) {
                m_tip->pop();
            }
        });
    }
    if (!m_tip) {
        m_tip = new DToast(this);
        m_tip->layout()->setContentsMargins(10, 0, 10, 0);
        m_tip->setFixedHeight(MAIN_WINDOW_TIP_H);
    }
    if (m_tipTimer->isActive()) {
        m_tipTimer->stop();
    }
    m_tip->pack();
    m_tip->lower();
    m_tip->setText(text);

    auto centerPos = baseWidget->mapToGlobal(baseWidget->rect().center());
    m_tip->show();
    m_tip->adjustSize();

    auto sz = m_tip->size();
    centerPos.setX(centerPos.x()  - sz.width() / 2);
    centerPos.setY(centerPos.y() - MAIN_WINDOW_TIP_H - sz.height());
    centerPos = m_tip->mapFromGlobal(centerPos);
    centerPos = m_tip->mapToParent(centerPos);
    m_tip->move(centerPos);
    m_tip->raise();

    QTimer::singleShot(10, [&, /*baseWidget, */delayShowMilliseconds](){
        if (delayShowMilliseconds > 0) {
            m_tipTimer->start(delayShowMilliseconds);
            return;
        }
        m_tip->pop();
    });
}

void MainWindow::hideTip(int delayMilliseconds)
{
    if (m_tip) {
        if (delayMilliseconds > 0) {
            QTimer::singleShot(delayMilliseconds, [&](){
                m_tip->pack();
            });
        } else {
            m_tip->pack();
        }
    }
}

#else

#include "widget/RKOverlayWidget.h"
void MainWindow::showFileImportDlg()
{
//    QWidget *w = new QWidget;
//    w->setStyleSheet("QWidget{background-color:transparent;}");
////    m_topStack->addWidget(w);
////    m_topStack->setCurrentWidget(w);
//    w->raise();
//    w->show();


    QWidget *w = new QWidget(m_mainWidget);
    w->resize(400, 400);
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(30, 30, 30, 30);

    RKOverlayWidget *v = new RKOverlayWidget;
    v->setFixedSize(300, 300);
    l->addWidget(v, Qt::AlignCenter);
    w->raise();
    w->show();

    QLabel *label = new QLabel("aaaaa");
    QVBoxLayout *ly = new QVBoxLayout;
    v->setWindowLayout(ly);
    v->windowLayout()->addWidget(label);
    v->showOverlay();
}

#endif
