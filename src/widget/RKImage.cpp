#include "RKImage.h"

#include <QPainter>
#include <QDebug>
#include <QFile>

#include <DThemeManager>

#include "LibPhoenixPlayerMain.h"
#include "PPUtility.h"

#include "QCNetworkAccessManager.h"
#include "QCNetworkRequest.h"
#include "QCNetworkAsyncReply.h"

#include "RKUtility.h"

DWIDGET_USE_NAMESPACE;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

static auto borderPenWidth = 1.0;

RKImage::RKImage(QWidget *parent)
    : QFrame(parent),
      m_radius(4),
      m_borderColor(QColor(255, 0, 0, 152)),
      m_shadowColor(QColor(0, 255, 0, 126)),
      outterMargins(QMarginsF(borderPenWidth, borderPenWidth, borderPenWidth, borderPenWidth))
{
    this->setObjectName("RKImage");
    DThemeManager::instance()->registerWidget(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

RKImage::~RKImage()
{
    qDebug()<<"-----------------------";
}
int RKImage::radius() const
{
    return  m_radius ;
}

QColor RKImage::borderColor() const
{
    return  m_borderColor ;
}

QColor RKImage::shadowColor() const
{
    return  m_shadowColor ;
}

void RKImage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    auto radius = m_radius;
    auto innerBorderColor = m_borderColor;
    auto outerBorderColor = m_shadowColor;
    auto backgroundColor = QColor(255, 255, 255, 255);
    auto penWidthf = borderPenWidth;

    // draw background
    auto backgroundRect = QRectF(rect()).marginsRemoved(outterMargins);
    if (m_Background.isNull()) {
        QPainterPath backgroundPath;
        backgroundPath.addRoundedRect(backgroundRect, radius, radius);
        painter.fillPath(backgroundPath, backgroundColor);
    } else {
        painter.drawPixmap(backgroundRect.toRect(), m_Background);
    }

    // draw border
    QPainterPath innerBorderPath;
    QRectF borderRect = QRectF(rect()).marginsRemoved(outterMargins);
    auto borderRadius = radius;
    QMarginsF borderMargin(penWidthf / 2, penWidthf / 2, penWidthf / 2, penWidthf / 2);
    borderRadius -= penWidthf / 2;
    borderRect = borderRect.marginsRemoved(borderMargin);
    innerBorderPath.addRoundedRect(borderRect, borderRadius, borderRadius);
    QPen innerBorderPen(innerBorderColor);
    innerBorderPen.setWidthF(penWidthf);
    painter.strokePath(innerBorderPath, innerBorderPen);

    QPainterPath outerBorderPath;
    borderRect = QRectF(rect()).marginsRemoved(outterMargins);
    borderRadius = radius;
    borderRadius += penWidthf / 2;
    borderRect = borderRect.marginsAdded(borderMargin);

    outerBorderPath.addRoundedRect(borderRect, borderRadius, borderRadius);
    QPen outerBorderPen(outerBorderColor);
    outerBorderPen.setWidthF(penWidthf);
    painter.strokePath(outerBorderPath, outerBorderPen);
}

void RKImage::onImageFile(const QByteArray &data, RKImageProxy *proxy, bool isBinaryData)
{
    if (proxy != this) {
        qWarning()<<"Not same proxy ?? This "<<this<<" proxy "<<proxy;
    }
    if (data.isEmpty()) {
        qWarning()<<"Empty data !";
        return;
    }
    QImage img;
    if (isBinaryData) {
        img = QImage::fromData(data);
    } else if (!img.load(data)) {
        qWarning()<<"Can't load file ["<<data<<"] as image .";
        return;
    }
    if (img.isNull()) {
        qWarning()<<"Finally image is null";
        return;
    }
    QPixmap pixmap = QPixmap::fromImage(img);
    if (pixmap.isNull()) {
        qWarning()<<"Can't load pixmap from image.";
        return;
    }
    this->setPixmap(pixmap);
}

void RKImage::setPixmap(const QPixmap &pixmap)
{

    int radius = m_radius;
    auto ratio = this->devicePixelRatioF();
    auto coverRect = QRectF(rect()).marginsRemoved(outterMargins);
    auto sz = coverRect.size().toSize() * ratio;

    QPixmap backgroundPixmap = pixmap.scaled(sz, Qt::KeepAspectRatioByExpanding);
    // TODO: fix ratio by Dtk::Widget::DHiDPIHelper
    backgroundPixmap.setDevicePixelRatio(1);
    QPixmap maskPixmap(sz);
    maskPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, sz.width(), sz.height()), double(radius), double(radius));
    QPainter bkPainter(&maskPixmap);
    bkPainter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    bkPainter.fillPath(path, QBrush(Qt::red));

    QPainter::CompositionMode mode = QPainter::CompositionMode_SourceIn;
    QImage resultImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, maskPixmap.toImage());
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, backgroundPixmap.toImage());
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.end();

    m_Background = QPixmap::fromImage(resultImage);

    this->update();
}

void RKImage::setUri(const QUrl &uri)
{
    if (uri.isEmpty() || !uri.isValid()) {
        qWarning()<<" uri is invalid";
        return;
    }
    QString path;
    if (uri.isLocalFile()) {
        path = uri.toLocalFile();
    }
    if (path.isEmpty()) {
        path = uri.toString();
    }
    if (path.isEmpty()) {
        qWarning()<<"No suitable path for uri ["<<uri<<"]";
        return;
    }
//    if (uri.scheme().startsWith("http") || uri.scheme().startsWith("ftp")) { //url
////        downloadFile(uri);
//        startRequest(uri);
//        return;
//    }
    //remove qrc: from file uri to fix image load
    if (path.startsWith("qrc:")) {
        path = path.mid(3, path.length() - 3);
        QImage img(path);
        if (img.isNull()) {
            qWarning()<<"Can't load img for uri "<<uri
                     <<" with path "<<path;
            return;
        }
        QPixmap pixmap = QPixmap::fromImage(img);
        if (pixmap.isNull()) {
            qWarning()<<"Can't load pixmap from image uri "<<uri;
            return;
        }
        this->setPixmap(pixmap);
    } else {
        startRequest(uri);
    }
}

void RKImage::setRadius(int radius)
{

    m_radius = radius;
}

void RKImage::setBorderColor(QColor borderColor)
{

    m_borderColor = borderColor;
}

void RKImage::setShadowColor(QColor shadowColor)
{

    m_shadowColor = shadowColor;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

