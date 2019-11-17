#include "CategoryModelImageProvider.h"

#include <QDebug>
#include <QDir>
#include <QFile>

#include "SingletonPointer.h"
#include "PPUtility.h"

#include "QCNetworkAccessManager.h"
#include "QCNetworkRequest.h"
#include "QCNetworkAsyncReply.h"

#include "CategoryModel.h"
#include "RKUtility.h"

using namespace QCurl;
using namespace PhoenixPlayer;


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

CategoryModelImageProvider::CategoryModelImageProvider(ImageProviderProxy *proxy, QObject *parent)
    : QObject (parent),
      m_proxy(proxy),
      m_mgr(new QCNetworkAccessManager(this))
{
    m_dataCache = RKUtility::coverCacheDir();
    if (m_dataCache.endsWith("/")) {
        m_dataCache = m_dataCache.left(m_dataCache.size() - 1);
    }
}

CategoryModelImageProvider::~CategoryModelImageProvider()
{
    if (!m_replyMap.isEmpty()) {
        foreach(auto &repy, m_replyMap.values()) {
            if (repy->isRunning()) {
                repy->abort();
            }
            disconnect(repy);
            repy->deleteLater();
        }
        m_replyMap.clear();
    }
    if (!m_urlMap.isEmpty()) {
        m_urlMap.clear();
    }
}

void CategoryModelImageProvider::startRequest(const QUrl &uri, const QModelIndex &idx)
{
    if (!m_proxy) {
        qWarning()<<"proxy is nullptr !!!";
        return;
    }
    if (!uri.isValid()) {
        qWarning()<<"Invalid uri : "<<uri;
        m_ignoreMap.insert(idx, uri);
        m_proxy->onImageFile(QString(), idx);
        return;
    }

    {
        QUrl url = m_ignoreMap.value(idx, QUrl());
        if (url.isValid() && url == uri) {
            qDebug()<<"current QModelIndex ["<<idx<<"] for url ["<<uri<<"] is in ignore map";
            return;
        }
    }

    {
        const QString path = getFile(uri);
        if (QFile::exists(path)) {
            m_proxy->onImageFile(path.toUtf8(), idx);
            return;
        }
    }

    {
        auto reply = m_replyMap.value(idx, Q_NULLPTR);
        auto ul = m_urlMap.value(idx, QUrl());
        //check if another reply is running for this idx, and requested url is not same
        if (reply && reply->isRunning()) {
            if (ul.isValid() && (ul == uri)) {
                return;
            }
            reply->abort();
            QObject::disconnect(reply);
            reply->deleteLater();
            m_replyMap.remove(idx);
        }
    }
    QCNetworkRequest req(uri);
    auto reply = m_mgr->get(req);
    m_replyMap.insert(idx, reply);
    m_urlMap.insert(idx, uri);

    connect(reply, &QCNetworkAsyncReply::finished,
            this, [&, idx, uri, reply](){
                if (uri != m_urlMap.value(idx)) {
                    qWarning()<<"url not same ["<<uri<<"] <==> ["<<m_urlMap.value(idx)<<"]";
                }

                if (reply != m_replyMap.value(idx)) {
                    qWarning()<<"reply ptr not same ["<<reply<<"] <==> ["<<m_replyMap.value(idx)<<"]";
                }

                const NetworkError error = reply->error();
                const QString errorStr = reply->errorString();
                QByteArray data = reply->readAll();
                QByteArray hba = reply->rawHeaderData();
                if (reply->isRunning()) {
                    reply->abort();
                }
                reply->deleteLater();
                m_replyMap.remove(idx);
                m_urlMap.remove(idx);

                if (error == NetworkNoError) {
                    const int i = data.indexOf(hba);
                    if (i >= 0) {
                        data = data.remove(i, hba.size());
                    }
                    QString file = getFile(uri);
                    if (QFile::exists(file)) {
                        m_proxy->onImageFile(file, idx);
                        return;
                    } else {
                        QFile f(file);
                        if (!f.open(QIODevice::WriteOnly)) {
                            qDebug()<<"Open file error: "<<file;
                            m_proxy->onImageFile(QString(), idx);
                            return;
                        }
                        f.write(data);
                        f.flush();
                        f.close();
                        m_proxy->onImageFile(file, idx);
                    }
                } else {
                    m_ignoreMap.insert(idx, uri);
                    qDebug()<<"download from uri ["<<uri<<"] error: "<<reply->errorString();
                }
            });
    reply->perform();
}

QString CategoryModelImageProvider::getFile(const QUrl &uri) const
{
    const QString hash = PPUtility::calculateHash(m_dataCache + uri.toString());
    QString path = QString("%1/%2").arg(m_dataCache).arg(hash);
    return path;
}

ImageProviderProxy::~ImageProviderProxy()
{

}


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

