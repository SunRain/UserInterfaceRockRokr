#include "LoadingWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include <DWaterProgress>
#include <DThemeManager>

#include "LibPhoenixPlayerMain.h"
#include "MusicLibrary/LocalMusicScanner.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

LoadingWidget::LoadingWidget(QWidget *parent)
    : QFrame(parent),
      m_progress(new DWaterProgress)
{
    this->setObjectName("LoadingWidget");

    DThemeManager::instance()->registerWidget(this);

    m_localSC = new MusicLibrary::LocalMusicScanner(this);

    m_progress->setValue(50);
    m_progress->setTextVisible(true);

    auto label = new QLabel(tr("Loading..."));
    label->setObjectName("LoadWidgetLabel");

    auto vbox = new QVBoxLayout(this);
    setLayout(vbox);

    vbox->addStretch();
    vbox->addWidget(m_progress, 0, Qt::AlignCenter);
    vbox->addSpacing(10);
    vbox->addWidget(label, 0, Qt::AlignCenter);
    vbox->addStretch();

    connect(m_localSC, &MusicLibrary::LocalMusicScanner::newFileListAdded,
            this, [&](const QStringList &list) {
        m_totalFileSize += list.size();
        if ((m_remainingFileSize == 0) || (m_totalFileSize == 0)) {
            m_progress->setValue(100);
        } else {
            m_progress->setValue((m_totalFileSize - m_remainingFileSize) * 100 / m_totalFileSize);
        }
    }, Qt::QueuedConnection);
    connect(m_localSC, &MusicLibrary::LocalMusicScanner::parsingFile,
            this, [&, label](const QString &file, int remainingSize){
        m_remainingFileSize = remainingSize;
        if ((m_remainingFileSize == 0) || (m_totalFileSize == 0)) {
            m_progress->setValue(100);
        } else {
            m_progress->setValue((m_totalFileSize - m_remainingFileSize) * 100 / m_totalFileSize);
        }
        label->setText(QString("%1 %2").arg(tr("parseing")).arg(file));
    }, Qt::QueuedConnection);
}

LoadingWidget::~LoadingWidget()
{
    if (m_localSC) {
        m_localSC->deleteLater();
        m_localSC = Q_NULLPTR;
    }
}

void LoadingWidget::start()
{
    m_progress->start();
}

void LoadingWidget::stop()
{
    m_progress->stop();
    m_totalFileSize = 0;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

