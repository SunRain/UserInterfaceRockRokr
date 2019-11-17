#ifndef CATEGORYMODELIMAGEPROVIDER_H
#define CATEGORYMODELIMAGEPROVIDER_H

#include <QModelIndex>

#include <QObject>
#include <QHash>
#include <QUrl>

namespace QCurl {
    class QCNetworkAccessManager;
    class QCNetworkAsyncReply;
}

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class ImageProviderProxy;
class CategoryModelImageProvider : public QObject
{
    Q_OBJECT
public:
    explicit CategoryModelImageProvider(ImageProviderProxy *proxy, QObject *parent = nullptr);

    virtual ~CategoryModelImageProvider();

    void startRequest(const QUrl &uri, const QModelIndex &idx);

private:
    QString getFile(const QUrl &uri) const;

private:
    ImageProviderProxy                              *m_proxy;
    QCurl::QCNetworkAccessManager                   *m_mgr;
    QMap<QModelIndex, QUrl>                         m_urlMap;
    QMap<QModelIndex, QUrl>                         m_ignoreMap;
    QMap<QModelIndex, QCurl::QCNetworkAsyncReply*>  m_replyMap;
    QString                                         m_dataCache;
};

class ImageProviderProxy
{
public:
    explicit ImageProviderProxy() {}
    virtual ~ImageProviderProxy();

    ///
    /// \brief onImageFile
    /// \param file the target file path
    /// \param idx
    ///
    virtual void onImageFile(const QString &file, const QModelIndex &idx) = 0;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // CATEGORYMODELIMAGEPROVIDER_H
