#ifndef RKIMAGE_H
#define RKIMAGE_H

#include <QUrl>
#include <QFrame>

#include "RKImageProxy.h"

class QFile;

namespace QCurl {
    class QCNetworkAsyncReply;
}

namespace PhoenixPlayer {

    class PPSettings;

    namespace UserInterface {

        namespace RockRokr {

// copy from Cover in deepin-music with some modifaction
class RKImage : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)

public:
    explicit RKImage(QWidget *parent = Q_NULLPTR);
    virtual ~RKImage() override;
    int radius() const;
    QColor borderColor() const;
    QColor shadowColor() const;

public slots:
    void setPixmap(const QPixmap &pixmap);
    void setUri(const QUrl &uri);
    void setRadius(int radius);
    void setBorderColor(QColor borderColor);
    void setShadowColor(QColor shadowColor);

signals:
    void shadowColorChanged(QColor shadowColor);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

protected:
    void downloadFile(const QUrl &uri);

private:
//    PhoenixPlayer::PPSettings   *m_settings = Q_NULLPTR;
    QCurl::QCNetworkAsyncReply  *m_reply    = Q_NULLPTR;
    QFile                       *m_file     = Q_NULLPTR;
    int                         m_radius;
    QColor                      m_borderColor;
    QColor                      m_shadowColor;
    QPixmap                     m_Background;
    QMarginsF                   outterMargins;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKIMAGE_H
