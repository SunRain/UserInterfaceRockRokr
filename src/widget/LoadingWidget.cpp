#include "LoadingWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include <DWaterProgress>

#include <DThemeManager>

DWIDGET_USE_NAMESPACE

LoadingWidget::LoadingWidget(QWidget *parent)
    : QFrame(parent),
      m_progress(new DWaterProgress)
{
    this->setObjectName("LoadingWidget");

    DThemeManager::instance()->registerWidget(this);

    m_progress->setValue(50);
    m_progress->setTextVisible(false);

    auto label = new QLabel(tr("Loading..."));
    label->setObjectName("LoadWidgetLabel");

    auto vbox = new QVBoxLayout(this);
    setLayout(vbox);

    vbox->addStretch();
    vbox->addWidget(m_progress, 0, Qt::AlignCenter);
    vbox->addSpacing(10);
    vbox->addWidget(label, 0, Qt::AlignCenter);
    vbox->addStretch();
}

LoadingWidget::~LoadingWidget()
{

}

void LoadingWidget::start()
{
    m_progress->start();
}

void LoadingWidget::stop()
{
    m_progress->stop();
}
