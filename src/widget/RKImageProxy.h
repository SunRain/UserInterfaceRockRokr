#ifndef RKIMAGEPROXY_H
#define RKIMAGEPROXY_H

#include <QObject>
#include <QHash>
#include <QUrl>
#include <QMap>

namespace QCurl {
    class QCNetworkAccessManager;
    class QCNetworkAsyncReply;
}

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


class RKImageProxy;
class RKImageProivder : public QObject
{
    Q_OBJECT
    friend class RKImageProxy;
protected:
    explicit RKImageProivder(QObject *parent = Q_NULLPTR);
    static RKImageProivder *createInstance();

public:
    static RKImageProivder *instance();
    virtual ~RKImageProivder();

protected:
    virtual void startRequest(const QUrl &uri, RKImageProxy *proxy);

    virtual void unRegisterProxy(RKImageProxy *proxy);


private:
    QCurl::QCNetworkAccessManager       *m_mgr;
    QList<RKImageProxy *>               *m_dataList;
};

class RKImageProxy
{
    friend class RKImageProivder;
public:
    explicit RKImageProxy();
    virtual ~RKImageProxy();

    void startRequest(const QUrl &uri);

protected:
    /*!
     * \brief keepRegister
     * \return true if not remove this from RKImageProivder
     */
    virtual bool keepRegister()
    {
        return false;
    }

    /*!
     * \brief cookieFile
     * \return empty if not use cookie or filepaht used to save cookie data
     */
    virtual QString cookieFile() const
    {
        return QString();
    }

    /*!
     * \brief onImageFile
     * \param data filePath or file data
     * \param proxy
     * \param isBinaryData indicate if data is file path or binary data
     * True for binary data, false for file path
     */
    virtual void onImageFile(const QByteArray &data, RKImageProxy *proxy, bool isBinaryData) = 0;

    /*!
     * \brief dataCachePath
     * \return dir to save dowloaded file
     * if return empty the onImageFile will file data, or the full path of file saved
     */
    virtual QString dataCachePath() const
    {
        return QString();
    }

    void addExtraCacheSearchPath(const QString &path);

    inline QStringList extraCacheSearchPath() const
    {
        return m_extraCachePath;
    }

    inline void setReply(QCurl::QCNetworkAsyncReply *reply)
    {
        m_reply = reply;
    }

    inline QCurl::QCNetworkAsyncReply *getReply()
    {
        return m_reply;
    }

    /*!
     * \brief setProvider
     * If no custom provider set, will use default provider
     * \param provider
     */
    inline void setProvider(RKImageProivder *provider) {
        m_provider = provider;
    }

    /*!
     * \brief getProvider
     * \return nullptr is no curstom provider set
     */
    inline RKImageProivder *getProvider()
    {
        return m_provider;
    }

private:
    QCurl::QCNetworkAsyncReply      *m_reply = Q_NULLPTR;
    RKImageProivder                 *m_provider = Q_NULLPTR;
    QStringList                     m_extraCachePath;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKIMAGEPROXY_H
