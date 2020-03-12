#include "ImportPage.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <DThemeManager>


DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


const static char *linkTemplate = "<a href='%1' style='text-decoration: none; color: #336CFB; '>%2</a>";

ImportPage::ImportPage(QWidget *parent)
    : QFrame(parent),
      m_label(new QLabel),
      m_btn(new QPushButton)
{
    this->setObjectName("ImportPage");
    DThemeManager::instance()->registerWidget(this);
    this->setAcceptDrops(true);

    QVBoxLayout *layout= new QVBoxLayout(this);
    layout->setSpacing(0);


    QLabel *logo = new QLabel;
    logo->setObjectName("IVLogo");
    logo->setFixedSize(128, 128);

    m_btn = new QPushButton;
    m_btn->setObjectName("IVButton");
    m_btn->setFixedSize(142, 42);
    m_btn->setText(tr("Add folder"));

    m_label = new QLabel;
    m_label->setObjectName("IVText");
    m_label->setFixedHeight(24);
    m_label->setOpenExternalLinks(false);
    QString linkText = QString(linkTemplate).arg(tr("Scan")).arg(tr("Scan"));
    m_label->setText(QString(tr("%1 default music directory or drag & drop music file to add music")).arg(linkText));

    layout->addStretch();
    layout->addWidget(logo, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(m_btn, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(m_label, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(m_btn, &QPushButton::clicked,
            this, [&]{
        emit this->showImportDlg();
    });

    connect(m_label, &QLabel::linkActivated,
            this, [&]{
        emit this->scanStandardMusicPath();
    });
}

ImportPage::~ImportPage()
{

}

void ImportPage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->setDropAction(Qt::CopyAction);
        event->acceptProposedAction();
        return;
    }

    QFrame::dragEnterEvent(event);
}

void ImportPage::dragMoveEvent(QDragMoveEvent *event)
{
    QFrame::dragMoveEvent(event);
}

void ImportPage::dragLeaveEvent(QDragLeaveEvent *event)
{
    QFrame::dragLeaveEvent(event);
}

void ImportPage::dropEvent(QDropEvent *event)
{
    qDebug()<<event->mimeData();
    if (!event->mimeData()->hasFormat("text/uri-list")) {
        return;
    }

    auto urls = event->mimeData()->urls();
    qDebug()<<" urls "<<urls;
    QStringList localpaths;
    for (auto &url : urls) {
        localpaths << url.toLocalFile();
    }
    if (!localpaths.isEmpty()) {
        emit importFiles(localpaths);
    }
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
