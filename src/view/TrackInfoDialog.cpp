#include "TrackInfoDialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <DThemeManager>
#include <dimagebutton.h>

#include "PPUtility.h"

#include "rockrokr_global.h"
#include "ViewUtility.h"

DWIDGET_USE_NAMESPACE
using namespace  PhoenixPlayer;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

TrackInfoDialog::TrackInfoDialog(const PhoenixPlayer::AudioMetaObject &obj, QWidget *parent)
    : DAbstractDialog(parent)
{
    this->setObjectName("TrackInfoDialog");
    DThemeManager::instance()->registerWidget(this);

    this->setFixedWidth(dpi_to_px(320));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(dpi_to_px(5));

    {
        DImageButton *closeBtn = new DImageButton;
        closeBtn->setObjectName("CloseBtn");
        closeBtn->setFixedSize(dpi_to_px(27), dpi_to_px(23));
        closeBtn->setNormalPic(":/images/light/images/window/close_normal.svg");
        closeBtn->setHoverPic(":/images/light/images/window/close_hover.svg");
        closeBtn->setPressPic(":/images/light/images/window/close_press.svg");

        layout->addWidget(closeBtn, 0, Qt::AlignTop | Qt::AlignRight);
        layout->addSpacing(dpi_to_px(43));

        connect(closeBtn, &DImageButton::clicked,
                this, &DAbstractDialog::close);
    }
    {
        auto cover = new QLabel;
        cover->setContentsMargins(0, 0, 0, 0);
        cover->setObjectName("Cover");
        cover->setFixedSize(dpi_to_px(140), dpi_to_px(140));
        cover->setAlignment(Qt::AlignCenter);
        layout->addWidget(cover, 0, Qt::AlignCenter);
        layout->addSpacing(dpi_to_px(13));

        auto cimg = QImage(QString(":/light/image/default_cover.jpg"));
        QUrl uri = obj.queryImgUri();
        if (uri.isValid()) {
            QString f;
            if (uri.isRelative()) {
                f = uri.toString();
            } else {
                f = uri.toLocalFile();
            }
            auto img = QImage(f);
            if (!img.isNull()) {
                cimg = img;
            }
        }
        int ms = qMax(cimg.width(), cimg.height());
        if (ms > dpi_to_px(140)) {
            qreal ratio = 1.0;
            if (cimg.width() > cimg.height()) {
                ratio = cimg.width()/dpi_to_px(140);
            } else {
                ratio = cimg.height()/dpi_to_px(140);
            }
            qreal w = cimg.width()/ratio;
            qreal h = cimg.height()/ratio;
            cimg = cimg.scaled(w, h);
        }
        auto pixmap = QPixmap::fromImage(cimg);
        cover->setPixmap(pixmap);
    }
    {
        auto title = new QLabel;
        title->setObjectName("Title");
        title->setFixedWidth(dpi_to_px(300));
        title->setWordWrap(true);
        title->setAlignment(Qt::AlignCenter);

        layout->addWidget(title, 0, Qt::AlignCenter);
        layout->addSpacing(dpi_to_px(19));

        QString text = obj.trackMeta().title();
        if (text.isEmpty()) {
            text = obj.name();
        }
        title->setText(text);
    }
    {
        auto label = new QLabel;
        label->setObjectName("Split");
        label->setFixedSize(dpi_to_px(300), 1);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label, 0, Qt::AlignCenter);
        layout->addSpacing(dpi_to_px(19));
    }
#define ADD_SPACING layout->addSpacing(dpi_to_px(5));
    createInfoLabel(layout, tr("Title:"), obj.trackMeta().title().isEmpty()
                                              ? obj.name()
                                              : obj.trackMeta().title());
    ADD_SPACING
    createInfoLabel(layout, tr("Artist:"), obj.artistMeta().name());
    ADD_SPACING
    createInfoLabel(layout, tr("Album:"), obj.albumMeta().name());
    ADD_SPACING
    {
        int idx = obj.name().lastIndexOf(".");
        const QString str = obj.name().mid(idx+1, obj.name().length() - idx -1);
        createInfoLabel(layout, tr("Type:"), str);
    }
    ADD_SPACING
    createInfoLabel(layout, tr("Size:"), PPUtility::formateFileSize(obj.size()));
    ADD_SPACING
    createInfoLabel(layout, tr("Duration:"), PPUtility::formateSongDuration(obj.trackMeta().duration()));
    ADD_SPACING
    createInfoLabel(layout, tr("Path:"), QString("%1/%2").arg(obj.path()).arg(obj.name()));
    layout->addSpacing(dpi_to_px(10));
    layout->addStretch();
}

TrackInfoDialog::~TrackInfoDialog()
{

}

void TrackInfoDialog::createInfoLabel(QBoxLayout *parent, const QString &key, const QString &vale)
{
    QHBoxLayout *ly = new QHBoxLayout;
    ly->setContentsMargins(0, 0, 0, 0);
    ly->setSpacing(dpi_to_px(5));

    auto kl = new QLabel;
    kl->setObjectName("Key");
    kl->setMinimumHeight(dpi_to_px(18));
    kl->setAlignment(Qt::AlignRight);
    kl->setText(key);

    auto vl = new QLabel;
    vl->setObjectName("Value");
    vl->setAlignment(Qt::AlignLeft);
    vl->setWordWrap(true);
    vl->setMinimumHeight(dpi_to_px(18));
    vl->setMinimumWidth(dpi_to_px(200));
    vl->setMaximumWidth(dpi_to_px(220));
    vl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (vale.isEmpty()) {
        vl->setText(tr("Unknow"));
    } else {
        vl->setText(vale);
    }

    ly->addWidget(kl);
    ly->addWidget(vl);

    parent->addLayout(ly);
}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
