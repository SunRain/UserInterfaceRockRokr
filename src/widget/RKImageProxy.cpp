#include "RKImageProxy.h"

#include <QDebug>
#include <QDir>
#include <QFile>

#include "SingletonPointer.h"
#include "PPUtility.h"

#include "QCNetworkAccessManager.h"
#include "QCNetworkRequest.h"
#include "QCNetworkAsyncReply.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKImageProivder::RKImageProivder(QObject *parent)
    : QObject (parent),
      m_mgr(new QCurl::QCNetworkAccessManager(this))
{

}

RKImageProivder *RKImageProivder::createInstance()
{
    return new RKImageProivder();
}

RKImageProivder *RKImageProivder::instance()
{
    return Singleton<RKImageProivder>::instance(RKImageProivder::createInstance);
}

RKImageProivder::~RKImageProivder()
{
    if (!m_dataList->isEmpty()) {
        m_dataList->clear();
    }
}

void RKImageProivder::registerAvailableDataCachePath(RKImageProxy *proxy, const QString &path)
{
    if (!proxy || path.isEmpty()) {
        return;
    }
    if (m_cachePathList->keys().contains(proxy)) {
        QStringList list = m_cachePathList->value(proxy);
        if (!list.contains(path)) {
            list.append(path);
            m_cachePathList->insert(proxy, list);
        }
    } else {
        QStringList list;
        list.append(path);
        m_cachePathList->insert(proxy, list);
    }
}

void RKImageProivder::unRegisterAvailableDataCachePath(RKImageProxy *proxy)
{
    if (!proxy) {
        return;
    }
    if (m_cachePathList->keys().contains(proxy)) {
        m_cachePathList->remove(proxy);
    }
}

void RKImageProivder::startRequest(const QUrl &uri, RKImageProxy *proxy)
{
    if (!uri.isValid()) {
        qWarning()<<"Invalid uri : "<<uri;
        proxy->onImageFile(QByteArray(), proxy);
        return;
    }
    if (!proxy) {
        qWarning()<<"proxy is nullptr !!!";
        return;
    }
    {
        QStringList paths;
        if (m_cachePathList->contains(proxy)) {
            paths.append(m_cachePathList->value(proxy));
        }
        if (!proxy->dataCachePath().isEmpty()) {
            paths.append(proxy->dataCachePath());
        }
        foreach (const auto &dirPath, paths) {
            if (!dirPath.isEmpty()) {
                const QString hash = PPUtility::calculateHash(dirPath + uri.toString());
                QString path;
                if (dirPath.endsWith("/")) {
                    path = QString("%1%2").arg(dirPath).arg(hash);
                } else {
                    path = QString("%1/%2").arg(dirPath).arg(hash);;
                }
                if (QFile::exists(path)) {
                    proxy->onImageFile(path.toUtf8(), proxy);
                    return;
                }
            }
        }
    }

    RKImageProxy *pp = Q_NULLPTR;
    const int idx = m_dataList->indexOf(proxy);
    if (idx < 0) {
        pp = proxy;
        m_dataList->append(pp);
    } else {
        pp = m_dataList->at(idx);
    }
    {
        //TODO add previous requests to an extra service??
        QCurl::QCNetworkAsyncReply *reply = pp->getReply();
        if (reply) {
            if (reply->isRunning()) {
                reply->abort();
            }
            reply->deleteLater();
            pp->setReply(Q_NULLPTR);
        }
    }
    QCurl::QCNetworkRequest req(uri);
    if (!proxy->cookieFile().isEmpty()) {
        m_mgr->setCookieFilePath(proxy->cookieFile());
    }
    auto reply = m_mgr->get(req);
    pp->setReply(reply);

    connect(reply, &QCurl::QCNetworkAsyncReply::finished,
            this, [&, pp, reply](){
        const QCurl::NetworkError error = reply->error();
        QByteArray data;
        if (error == QCurl::NetworkNoError) {
            qDebug()<<"download from uri ["<<uri<<"] error: "<<reply->errorString();

            data = reply->readAll();
            QByteArray hba = reply->rawHeaderData();

            if (reply->isRunning()) {
                reply->abort();
            }
            reply->deleteLater();
            pp->setReply(Q_NULLPTR);
            if (!pp->keepRegister()) {
                const int idx = m_dataList->indexOf(pp);
                if (idx >= 0)  {
                    m_dataList->removeAt(idx);
                }
            }
            const int idx = data.indexOf(hba);
            if (idx >= 0) {
                data = data.remove(idx, hba.size());
            }
            if (!pp->dataCachePath().isEmpty()) {
                const QString path = pp->dataCachePath();
                const QString hash = PPUtility::calculateHash(path + uri.toString());
                QString file;
                if (path.endsWith("/")) {
                    file = QString("%1%2").arg(path).arg(hash);
                } else {
                    file = QString("%1/%2").arg(path).arg(hash);;
                }
                if (QFile::exists(file)) {
                    pp->onImageFile(file.toUtf8(), pp);
                    return;
                } else {
                    QFile f(file);
                    if (!f.open(QIODevice::WriteOnly)) {
                        qDebug()<<"Open file error: "<<file;
                        pp->onImageFile(data, pp);
                        return;
                    }
                    f.write(data);
                    f.flush();
                    f.close();
                    pp->onImageFile(file.toUtf8(), pp);
                }
            } else {
                pp->onImageFile(data, pp);
            }
        } else {
            reply->deleteLater();
            pp->setReply(Q_NULLPTR);
        }
    });
    reply->perform();
}

void RKImageProivder::unRegisterProxy(RKImageProxy *proxy)
{
    if (m_dataList->contains(proxy)) {
        m_dataList->removeAll(proxy);
    }
}

RKImageProxy::RKImageProxy()
{
    qDebug()<<" ---------------";
}

RKImageProxy::~RKImageProxy()
{
    qDebug()<<" ---------------";
    if (getProvider()) {
        getProvider()->unRegisterProxy(this);
        getProvider()->unRegisterAvailableDataCachePath(this);
    } else {
        RKImageProivder::instance()->unRegisterProxy(this);
        RKImageProivder::instance()->unRegisterAvailableDataCachePath(this);
    }
}

void RKImageProxy::startRequest(const QUrl &uri)
{
    if (getProvider()) {
        getProvider()->startRequest(uri, this);
    } else {
        RKImageProivder::instance()->startRequest(uri, this);
    }
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

