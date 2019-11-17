#ifndef RKIMAGEPROXY_H
#define RKIMAGEPROXY_H

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

//    void registerProxy(RKImageProxy *proxy);

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
    ///
    /// \brief keepConnetction
    /// \return true if not remove this from RKImageProivder
    ///
    virtual bool keepRegister()
    {
        return false;
    }
    ///
    /// \brief cookieFile
    /// \return empty if not use cookie or filepaht used to save cookie data
    ///
    virtual QString cookieFile() const
    {
        return QString();
    }

    ///
    /// \brief onImageFile
    /// \param data filePath or file data
    /// if dataCachePath is empty, data will be file path, or the data of file
    /// \param proxy
    ///
    virtual void onImageFile(const QByteArray &data, RKImageProxy *proxy) = 0;

    ///
    /// \brief dataCachePath
    /// \return dir to save dowloaded file
    /// if return empty the onImageFile will file data, or the full path of file saved
    ///
    virtual QString dataCachePath() const
    {
        return QString();
    }

    inline void setReply(QCurl::QCNetworkAsyncReply *reply)
    {
        m_reply = reply;
    }

    inline QCurl::QCNetworkAsyncReply *getReply()
    {
        return m_reply;
    }

    ///
    /// \brief setProvider
    /// If no custom provider set, will use default provider
    /// \param provider if use custom provider
    ///
    inline void setProvider(RKImageProivder *provider) {
        m_provider = provider;
    }

    ///
    /// \brief getProvider
    /// \return nullptr is no curstom provider set
///
    inline RKImageProivder *getProvider()
    {
        return m_provider;
    }

private:
    QCurl::QCNetworkAsyncReply      *m_reply = Q_NULLPTR;
    RKImageProivder                 *m_provider = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKIMAGEPROXY_H
