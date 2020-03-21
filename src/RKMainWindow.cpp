#include "RKMainWindow.h"

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
#include "DataProvider/TrackSearchProvider.h"
#include "DataProvider/ITrackSearch.h"

#include "rockrokr_global.h"
#include "widget/LoadingWidget.h"
#include "widget/RKStackWidget.h"
#include "widget/RKOverlayWidget.h"
#include "view/ImportPage.h"
#include "view/MainWindowBGView.h"
#include "view/RockRokrPage.h"
#include "view/rockrokr/CategoryDetailView.h"
#include "view/rockrokr/PlayListDetailView.h"
#include "view/SearchPage.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKMainWindow::RKMainWindow(QWidget *parent)
    : DMainWindow(parent),
      m_ppSettings(new PPSettings(this)),
//      m_libraryMgr(new MusicLibrary::MusicLibraryManager(this)),
      m_localMSC(new MusicLibrary::LocalMusicScanner(this)),
      m_searchProvider(new DataProvider::TrackSearchProvider(this)),
      m_defaultTitlebarH(this->titlebar()->height())
{
    this->setMouseTracking(true);

    m_mainWidget = new MainWindowBGView(this);
    m_mainWidget->resize(MAIN_WINDOW_W, MAIN_WINDOW_H);
    this->setCentralWidget(m_mainWidget);

    QVBoxLayout *ly = new QVBoxLayout(m_mainWidget);
    ly->setContentsMargins(0, 0, 0, 0);
    ly->setSpacing(0);

    m_stack = new RKStackedWidget(m_mainWidget);
    m_stack->setContentsMargins(0, 0, 0, 0);
    m_stack->setFadeEnable(true);
    m_stack->setDuration(600);
    ly->addWidget(m_stack);

    m_importView = new ImportPage;
    m_loadingWidget = new LoadingWidget;
    m_rkView = new RockRokrPage;
    m_rkView->bindTrackSearchProvider(m_searchProvider);
    m_searchPage = new SearchPage;
    m_searchPage->bindTrackSearchProvider(m_searchProvider);

    {
        m_overlayWidget = new RKOverlayWidget;

        m_rkView->categoryDetailView()->setFixedSize(MAIN_WINDOW_W *3/5, MAIN_WINDOW_H *3/5);
        m_overlayWidget->addWidget(m_rkView->categoryDetailView());

        m_rkView->playListDetailView()->setFixedSize(MAIN_WINDOW_W *3/5, MAIN_WINDOW_H *3/5);
        m_overlayWidget->addWidget(m_rkView->playListDetailView());
    }

    m_stack->addWidget(m_importView);
    m_stack->addWidget(m_loadingWidget);
    m_stack->addWidget(m_rkView);
    m_stack->addWidget(m_overlayWidget);
    m_stack->addWidget(m_searchPage);

    connect(m_importView, &ImportPage::scanStandardMusicPath,
            this, [&]() {
        m_stack->setCurrentWidget(m_loadingWidget, RKStackedWidget::LeftToRight);
        m_loadingWidget->start();
        m_localMSC->scanLocalMusic();
    });
    connect(m_importView, &ImportPage::importFiles,
            this, [&](const QStringList &list){
        m_stack->setCurrentWidget(m_loadingWidget, RKStackedWidget::LeftToRight);
        m_loadingWidget->start();
        m_localMSC->scarnDirs(list);
    });
    connect(m_importView, &ImportPage::showImportDlg,
            this, &RKMainWindow::showFileImportDlg);

    connect(m_localMSC, &MusicLibrary::LocalMusicScanner::searchingFinished,
            this, [&]() {
        m_loadingWidget->stop();
        m_stack->setCurrentWidget(m_rkView, RKStackedWidget::LeftToRight);
    });

    connect(m_stack, &QStackedWidget::currentChanged,
            this, [&](int idx) {
        if (m_stack->widget(idx) != m_loadingWidget) {
            m_loadingWidget->stop();
        }
        if ((m_stack->widget(idx) == m_rkView) ||
                (m_stack->widget(idx) == m_overlayWidget) ||
                (m_stack->widget(idx) == m_searchPage)) {
            this->setDefaultTitlebarHidden(true);
        } else {
            this->setDefaultTitlebarHidden(false);
        }
    });

    connect(m_overlayWidget, &RKOverlayWidget::mousePressOutsideContent, this, [&](){
        m_stack->setCurrentIndex(m_stack->previousIndex(),
                                 RKStackedWidget::AnimationType::AnimationTypeNone);
    });
}

RKMainWindow::~RKMainWindow()
{

}

void RKMainWindow::show()
{
    this->resize(MAIN_WINDOW_W, MAIN_WINDOW_H);

    DMainWindow::show();

    Dtk::Widget::moveToCenter(this);
}

void RKMainWindow::showImportView()
{
    m_stack->setCurrentIndex(m_stack->indexOf(m_importView), RKStackedWidget::AnimationTypeNone);
}

void RKMainWindow::showLoadingView()
{
    m_stack->setCurrentIndex(m_stack->indexOf(m_loadingWidget), RKStackedWidget::AnimationTypeNone);
    m_loadingWidget->start();
}

void RKMainWindow::showRockRokrView()
{
    m_stack->setCurrentIndex(m_stack->indexOf(m_rkView), RKStackedWidget::AnimationTypeNone);
}

void RKMainWindow::showSearchPage()
{
    m_searchPage->setBackgroundPixmap(m_stack->currentWidget()->grab());
    m_stack->setCurrentIndex(m_stack->indexOf(m_searchPage), RKStackedWidget::AnimationTypeNone);
}

void RKMainWindow::showPlayingView()
{
    //TODO
}

void RKMainWindow::setDefaultTitlebarHidden(bool hidden)
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

void RKMainWindow::showFileImportDlg()
{
    QFileDialog dlg(this);

    dlg.setDirectory(m_ppSettings->defaultMusicDir());

    dlg.setViewMode(QFileDialog::Detail);
    dlg.setFileMode(QFileDialog::DirectoryOnly);
    if (QFileDialog::Accepted == dlg.exec()) {
        m_stack->setCurrentWidget(m_loadingWidget, RKStackedWidget::BottomToTop);
        m_loadingWidget->start();
        m_localMSC->scanDir(dlg.directory().path());
    }
}

void RKMainWindow::showTip(QWidget *baseWidget, const QString &text, int delayShowMilliseconds)
{
    auto centerPos = baseWidget->mapToGlobal(baseWidget->rect().center());
    showTip(centerPos, text, delayShowMilliseconds);
}

void RKMainWindow::showTip(const QPoint &globalCenterPos, const QString &text, int delayShowMilliseconds)
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

    m_tip->show();
    m_tip->adjustSize();

    QPoint centerPos = globalCenterPos;

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

void RKMainWindow::hideTip(int delayMilliseconds)
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

void RKMainWindow::showToast(const QString &text)
{
    if (m_toast) {
        m_toast->deleteLater();
        m_toast = Q_NULLPTR;
    }
    m_toast = new DToast(this);
    m_toast->setText(text);
    QTimer::singleShot(500, [&]() {
        m_toast->pop();
        m_toast->move((this->width() - m_toast->width()) / 2,
                      (this->height() - m_toast->height()));
    });
}

void RKMainWindow::showCategoryDetailView()
{
    m_overlayWidget->setBackgroundPixmap(m_stack->grab(m_stack->rect()));

    m_stack->setCurrentWidget(m_overlayWidget, RKStackedWidget::AnimationType::AnimationTypeNone);
    m_overlayWidget->setCurrentWidget(m_rkView->categoryDetailView());
}

void RKMainWindow::showPlaylistDetailView()
{
    m_overlayWidget->setBackgroundPixmap(m_stack->grab(m_stack->rect()));

    m_stack->setCurrentWidget(m_overlayWidget, RKStackedWidget::AnimationType::AnimationTypeNone);
    m_overlayWidget->setCurrentWidget(m_rkView->playListDetailView());
}

void RKMainWindow::resizeEvent(QResizeEvent *event)
{
    m_stack->setFixedSize(event->size());
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
