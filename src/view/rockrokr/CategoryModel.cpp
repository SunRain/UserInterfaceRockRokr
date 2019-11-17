#include "CategoryModel.h"

#include <QDebug>
#include <QSharedData>
#include <QSharedDataPointer>

#include "LibPhoenixPlayerMain.h"
#include "PlayerCore/PlayerCore.h"
#include "PlayerCore/RecentPlayedQueue.h"
#include "MusicLibrary/MusicLibraryManager.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

QUrl getPreferImgUri(const AudioMetaGroupObject &obj)
{
    QList<QUrl> list = AudioMetaGroupObject::sortAndTrimImgs(obj);
    if (list.isEmpty()) {
        return QUrl();
    } else {
        return list.first();
    }
}

QString getPreferArtistName(const AudioMetaGroupObject &obj)
{
    if (obj.list().isEmpty()) {
        return  QObject::tr("UnKnown");
    } else {
        return obj.list().first().artistMeta().name();
    }
}

QString getPreferAlbumName(const AudioMetaGroupObject &obj)
{
    if (obj.list().isEmpty()) {
        return  QObject::tr("UnKnown");
    } else {
        return obj.list().first().artistMeta().name();
    }
}

class CategoryObject
{
public:
    CategoryObject()
        : d(new Priv())
    {
    }
    CategoryObject(const CategoryObject &other)
        : d(other.d)
    {

    }
    ~CategoryObject() {}

    inline CategoryObject &operator =(const CategoryObject &other)
    {
        if (this != &other)
            d.operator = (other.d);
        return *this;
    }
    inline bool operator == (const CategoryObject &other)
    {
        return other.d.data()->name == d.data()->name &&
                other.d.data()->imgUri == d.data()->imgUri &&
                other.d.data()->subName == d.data()->subName &&
                other.d.data()->trackNum == d.data()->trackNum &&
               other.d.data()->extraData == d.data()->extraData;
    }
    inline bool operator != (const CategoryObject &other)
    {
        return !operator == (other);
    }
    inline QString name() const
    {
        return d.data()->name;
    }
    inline void setName(const QString &name)
    {
        d.data()->name = name;
    }
    inline QString subName() const
    {
        return d.data()->subName;
    }
    inline void setSubName(const QString &name)
    {
        d.data()->subName = name;
    }
    inline QUrl imgUri() const
    {
        return d.data()->imgUri;
    }
    inline void setImgUri(const QUrl &uri)
    {
        d.data()->imgUri = uri;
    }
    inline int trackNum() const
    {
        return d.data()->trackNum;
    }
    inline void setTrackNum(int num)
    {
        d.data()->trackNum = num;
    }
    inline QVariant getExtraData() const
    {
        return d.data()->extraData;
    }
    inline void setExtraData(const QVariant &data)
    {
        d.data()->extraData = data;
    }

private:
    class Priv : public QSharedData
    {
    public:
        Priv(){}
        int     trackNum = 0;
        QString name = QString();
        QString subName = QString();
        QUrl    imgUri = QUrl();
        QVariant extraData = QVariant();
    };
    QSharedDataPointer<Priv> d;

};

BaseCategoryModel::BaseCategoryModel(QAbstractListModel *parent)
    : QAbstractListModel (parent)
{
    m_imgProvider = new CategoryModelImageProvider(this, this);
    m_playCore = new PlayerCore(this);
    m_libraryMgr = new MusicLibrary::MusicLibraryManager(this);

    connect(m_libraryMgr, &MusicLibrary::MusicLibraryManager::libraryListSizeChanged,
            this, &BaseCategoryModel::initData);

    connect(m_playCore, &PlayerCore::trackChanged, this, [&](){
        emit this->dataChanged(QModelIndex(), QModelIndex());
    });
}

BaseCategoryModel::~BaseCategoryModel()
{
    if (m_imgProvider) {
        m_imgProvider->deleteLater();
        m_imgProvider = Q_NULLPTR;
    }
    if (m_playCore) {
        m_playCore->deleteLater();
        m_playCore = Q_NULLPTR;
    }
    if (m_libraryMgr) {
        m_libraryMgr->deleteLater();
        m_libraryMgr = Q_NULLPTR;
    }
}

void BaseCategoryModel::initData()
{
    onInitData();
    beginInsertRows(QModelIndex(), 0, m_dataList.size() -1);
    endInsertRows();
}

int BaseCategoryModel::rowCount(const QModelIndex &parent) const
{
    return m_dataList.size();
}

QVariant BaseCategoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_dataList.size()) {
        return QVariant();
    }
    CategoryObject obj = m_dataList.at(index.row());
    switch (role) {
        case RoleCategoryName: {
            return obj.name();
        }
        case RoleCategorySubName: {
            return obj.subName();
        }
        case RoleTrackNum: {
            return obj.trackNum();
        }
        case RoleImageUri: {
            QUrl url = obj.imgUri();
//            qDebug()<<Q_FUNC_INFO<<"img url is "<<url;
            if (url.isRelative()) {
                if (QFile::exists(url.toString())) {
                    return url;
                } else {
                    return QUrl();
                }
            }
            if (url.scheme().toLower().startsWith("qrc")) {
                //FIXME should remove "qrc" tag??
                return url;
            }
            if (url.scheme().toLower().startsWith("http") ||
                url.scheme().toLower().startsWith("ftp")) {
                m_imgProvider->startRequest(url, index);
            }
            return QUrl();
        }
        case RoleCurIdx: {
            return index == m_curIdx;
        }
        case RolePlayingState: {
            QVariant data = obj.getExtraData();
            if (!data.isValid() || data.isNull()) {
                return -1;
            }
            AudioMetaObject obj = AudioMetaObject::fromMap(data.toMap());
            if (obj.isHashEmpty()) {
                return QVariant();
            }
            if (this->playerCore()->curTrackMetaObject().hash() == obj.hash()) {
                return this->playerCore()->playBackendStateInt();
            }
            return -1;
        }
        case RoleTrackDuration: {
            QVariant data = obj.getExtraData();
            if (!data.isValid() || data.isNull()) {
                return -1;
            }
            AudioMetaObject obj = AudioMetaObject::fromMap(data.toMap());
            if (obj.isHashEmpty()) {
                return -1;
            }
            return obj.trackMeta().duration();
        }
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> BaseCategoryModel::roleNames() const
{
     QHash<int, QByteArray> role;
     role.insert(ModelRoles::RoleImageUri, "RoleImageUri");
     role.insert(ModelRoles::RoleTrackNum, "RoleTrackNum");
     role.insert(ModelRoles::RoleCategoryName, "RoleCategoryName");
     role.insert(ModelRoles::RoleCategorySubName, "RoleCategorySubName");
     role.insert(ModelRoles::RoleCurIdx, "RoleCurIdx");
     role.insert(ModelRoles::RolePlayingState, "RolePlayingState");
     role.insert(ModelRoles::RoleTrackDuration, "RoleTrackDuration");
     return role;
}

void BaseCategoryModel::onImageFile(const QString &file, const QModelIndex &idx)
{
    if (file.isEmpty() || !QFile::exists(file)) {
        return;
    }
    CategoryObject obj = m_dataList.at(idx.row());
    obj.setImgUri(file);
    m_dataList.replace(idx.row(), obj);
    QVector<int> roles;
    roles.append(ModelRoles::RoleImageUri);

    qDebug()<<"now emit datachange for idx "<<idx<<" for role"<<roles<<" with url "<<file;
    emit dataChanged(idx, idx, roles);
}

void BaseCategoryModel::setCurIdx(const QModelIndex &idx)
{
    m_curIdx = idx;
    emit dataChanged(m_curIdx, m_curIdx);
}

void BaseCategoryModel::clearList()
{
    beginResetModel();
    m_dataList.clear();
    endResetModel();
}

RecentAlbumCategoryModel::RecentAlbumCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

RecentAlbumCategoryModel::~RecentAlbumCategoryModel()
{

}

void RecentAlbumCategoryModel::onInitData()
{
    clearList();
    QList<LastPlayedMeta> list = this->libraryMgr()->getLastPlayedByAlbum();
    foreach(const LastPlayedMeta &meta, list) {
        AudioMetaObject o = this->libraryMgr()->trackFromHash(meta.audioMetaObjHash());
        if (o.isHashEmpty()) {
            continue;
        }
        CategoryObject obj;
        obj.setName(o.trackMeta().title());
        obj.setSubName(o.albumMeta().name());
        obj.setTrackNum(1);
        obj.setImgUri(o.queryImgUri());
        obj.setExtraData(o.toMap());
        dataListPtr()->append(obj);
        if (sizeLimit() > 0 && dataListPtr()->size() >= sizeLimit()) {
            break;
        }
    }
    int delta = 0;
    if (sizeLimit() > 0 && sizeLimit() > list.size()) {
        delta = sizeLimit() - list.size();
    }
    if (delta > 0) {
        AudioMetaGroupList l = this->libraryMgr()->albumList();
        for (int i=0; i<delta; ++i) {
            if (i >= l.size()) {
                break;
            }
            AudioMetaGroupObject o = l.at(i);
            foreach(const AudioMetaObject &meta, o.list()) {
                CategoryObject obj;
                obj.setName(meta.trackMeta().title());
                obj.setSubName(meta.albumMeta().name());
                obj.setImgUri(meta.queryImgUri());
                obj.setTrackNum(1);
                obj.setExtraData(meta.toMap());
                dataListPtr()->append(obj);
            }
        }
    }
}

RecommendedAlbumCategoryModel::RecommendedAlbumCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

RecommendedAlbumCategoryModel::~RecommendedAlbumCategoryModel()
{

}

void RecommendedAlbumCategoryModel::onInitData()
{
    clearList();
    QStringList hashlist = this->libraryMgr()->trackHashListByPlayedCount();
    QStringList abnList;
    foreach(const QString &hash, hashlist) {
        AudioMetaObject o = this->libraryMgr()->trackFromHash(hash);
        if (o.isHashEmpty()) {
            continue;
        }
        if (!abnList.contains(o.albumMeta().name())) {
            abnList.append(o.albumMeta().name());
        }
        if (sizeLimit() > 0 && abnList.size() >= sizeLimit()) {
            break;
        }
    }
    AudioMetaGroupList list = this->libraryMgr()->albumList();

    foreach(const QString &abn, abnList) {
        CategoryObject ca;
        ca.setName(abn);

        AudioMetaGroupObject go;
        foreach(const AudioMetaGroupObject &o, list) {
            if (o.name() == abn) {
                go = o;
                break;
            }
        }
        ca.setSubName(getPreferArtistName(go));
        ca.setTrackNum(go.list().size());
        ca.setImgUri(getPreferImgUri(go));
        dataListPtr()->append(ca);
    }
    if (sizeLimit() > abnList.size()) {
        foreach(const AudioMetaGroupObject &o, list) {
            if (abnList.contains(o.name())) {
                continue;
            }
            abnList.append(o.name());
            CategoryObject obj;
            obj.setName(o.name());
            obj.setSubName(getPreferArtistName(o));
            obj.setImgUri(getPreferImgUri(o));
            obj.setTrackNum(o.list().size());
            dataListPtr()->append(obj);
            if (abnList.size() >= sizeLimit()) {
                break;
            }
        }
    }
}

AllAlbumCategoryModel::AllAlbumCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

AllAlbumCategoryModel::~AllAlbumCategoryModel()
{

}

void AllAlbumCategoryModel::onInitData()
{
    clearList();
    AudioMetaGroupList list = this->libraryMgr()->albumList();
    foreach(const AudioMetaGroupObject &o, list) {
        CategoryObject obj;
        obj.setName(o.name());
        obj.setSubName(getPreferArtistName(o));
        obj.setImgUri(getPreferImgUri(o));
        obj.setTrackNum(o.list().size());
        dataListPtr()->append(obj);
    }
}

RecentArtistCategoryModel::RecentArtistCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

RecentArtistCategoryModel::~RecentArtistCategoryModel()
{

}

void RecentArtistCategoryModel::onInitData()
{
    clearList();
    QList<LastPlayedMeta> list = this->libraryMgr()->getLastPlayedByArtist();
    foreach(const LastPlayedMeta &meta, list) {
        AudioMetaObject o = this->libraryMgr()->trackFromHash(meta.audioMetaObjHash());
        if (o.isHashEmpty()) {
            continue;
        }
        CategoryObject obj;
        obj.setName(o.trackMeta().title());
        obj.setSubName(o.artistMeta().name());
        obj.setTrackNum(1);
        obj.setImgUri(o.queryImgUri());
        obj.setExtraData(o.toMap());
        dataListPtr()->append(obj);
        if (sizeLimit() > 0 && dataListPtr()->size() >= sizeLimit()) {
            break;
        }
    }
    int delta = 0;
    if (sizeLimit() > 0 && sizeLimit() > list.size()) {
        delta = sizeLimit() - list.size();
    }
    if (delta > 0) {
        AudioMetaGroupList l = this->libraryMgr()->artistList();
        for (int i=0; i<delta; ++i) {
            if (i >= l.size()) {
                break;
            }
            AudioMetaGroupObject o = l.at(i);
            foreach(const AudioMetaObject &meta, o.list()) {
                CategoryObject obj;
                obj.setName(meta.trackMeta().title());
                obj.setSubName(meta.artistMeta().name());
                obj.setImgUri(meta.queryImgUri());
                obj.setTrackNum(1);
                obj.setExtraData(meta.toMap());
                dataListPtr()->append(obj);
            }
        }
    }
}

RecentGenresCategoryModel::RecentGenresCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

RecentGenresCategoryModel::~RecentGenresCategoryModel()
{

}

void RecentGenresCategoryModel::onInitData()
{
    clearList();
    QList<LastPlayedMeta> list = this->libraryMgr()->getLastPlayedByGenres();
    if (sizeLimit() > 0 && sizeLimit() > list.size()) {
        foreach(const auto &meta, this->libraryMgr()->getLastPlayedMeta()) {
            if (list.contains(meta)) {
                continue;
            }
            list.append(meta);
            if (list.size() >= sizeLimit()) {
                break;
            }
        }
    }
    foreach(const LastPlayedMeta &meta, list) {
        AudioMetaObject o = this->libraryMgr()->trackFromHash(meta.audioMetaObjHash());
        if (o.isHashEmpty()) {
            continue;
        }
        CategoryObject obj;
        obj.setName(o.trackMeta().title());
        obj.setSubName(o.trackMeta().genre());
        obj.setTrackNum(1);
        obj.setImgUri(o.queryImgUri());
        obj.setExtraData(o.toMap());
        dataListPtr()->append(obj);
        if (sizeLimit() > 0 && dataListPtr()->size() >= sizeLimit()) {
            break;
        }
    }
}

RecommendedArtistCategoryModel::RecommendedArtistCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

RecommendedArtistCategoryModel::~RecommendedArtistCategoryModel()
{

}

void RecommendedArtistCategoryModel::onInitData()
{
    clearList();
    QStringList hashlist = this->libraryMgr()->trackHashListByPlayedCount();
    QStringList abnList;
    foreach(const QString &hash, hashlist) {
        AudioMetaObject o = this->libraryMgr()->trackFromHash(hash);
        if (o.isHashEmpty()) {
            continue;
        }
        if (!abnList.contains(o.artistMeta().name())) {
            abnList.append(o.artistMeta().name());
        }
        if (sizeLimit() > 0 && abnList.size() >= sizeLimit()) {
            break;
        }
    }
    AudioMetaGroupList list = this->libraryMgr()->artistList();

    foreach(const QString &abn, abnList) {
        CategoryObject ca;
        ca.setName(abn);

        AudioMetaGroupObject go;
        foreach(const AudioMetaGroupObject &o, list) {
            if (o.name() == abn) {
                go = o;
                break;
            }
        }
        ca.setSubName(getPreferAlbumName(go));
        ca.setTrackNum(go.list().size());
        ca.setImgUri(getPreferImgUri(go));
        dataListPtr()->append(ca);
    }
    if (sizeLimit() > abnList.size()) {
        foreach(const AudioMetaGroupObject o, list) {
            if (abnList.contains(o.name())) {
                continue;
            }
            abnList.append(o.name());
            CategoryObject obj;
            obj.setName(o.name());
            obj.setSubName(getPreferAlbumName(o));
            obj.setImgUri(getPreferImgUri(o));
            obj.setTrackNum(o.list().size());
            dataListPtr()->append(obj);
            if (abnList.size() >= sizeLimit()) {
                break;
            }
        }
    }
}

RecommendedGenresCategoryModel::RecommendedGenresCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

RecommendedGenresCategoryModel::~RecommendedGenresCategoryModel()
{

}

void RecommendedGenresCategoryModel::onInitData()
{
    clearList();
    QStringList hashlist = this->libraryMgr()->trackHashListByPlayedCount();
    QStringList abnList;
    foreach(const QString &hash, hashlist) {
        AudioMetaObject o = this->libraryMgr()->trackFromHash(hash);
        if (o.isHashEmpty()) {
            continue;
        }
        if (!abnList.contains(o.trackMeta().genre())) {
            abnList.append(o.trackMeta().genre());
        }
        if (sizeLimit() > 0 && abnList.size() >= sizeLimit()) {
            break;
        }
    }
    AudioMetaGroupList list = this->libraryMgr()->genreList();

    foreach(const QString &abn, abnList) {
        CategoryObject ca;
        ca.setName(abn);

        AudioMetaGroupObject go;
        foreach(const AudioMetaGroupObject &o, list) {
            if (o.name() == abn) {
                go = o;
                break;
            }
        }
        ca.setSubName(getPreferArtistName(go));
        ca.setTrackNum(go.list().size());
        ca.setImgUri(getPreferImgUri(go));
        dataListPtr()->append(ca);
    }
    if (sizeLimit() > abnList.size()) {
        foreach(const AudioMetaGroupObject o, list) {
            if (abnList.contains(o.name())) {
                continue;
            }
            abnList.append(o.name());
            CategoryObject obj;
            obj.setName(o.name());
            obj.setSubName(getPreferArtistName(o));
            obj.setImgUri(getPreferImgUri(o));
            obj.setTrackNum(o.list().size());
            dataListPtr()->append(obj);
            if (abnList.size() >= sizeLimit()) {
                break;
            }
        }
    }
}

AllArtistCategoryModel::AllArtistCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

AllArtistCategoryModel::~AllArtistCategoryModel()
{

}

void AllArtistCategoryModel::onInitData()
{
    clearList();
    AudioMetaGroupList list = this->libraryMgr()->artistList();
    foreach(const AudioMetaGroupObject &o, list) {
        CategoryObject obj;
        obj.setName(o.name());
        obj.setSubName(getPreferAlbumName(o));
        obj.setImgUri(getPreferImgUri(o));
        obj.setTrackNum(o.list().size());
        dataListPtr()->append(obj);
    }
}

AllGenresCategoryModel::AllGenresCategoryModel(QAbstractListModel *parent)
    : BaseCategoryModel (parent)
{

}

AllGenresCategoryModel::~AllGenresCategoryModel()
{

}

void AllGenresCategoryModel::onInitData()
{
    clearList();
    AudioMetaGroupList list = this->libraryMgr()->genreList();
    foreach(const AudioMetaGroupObject &o, list) {
        CategoryObject obj;
        obj.setName(o.name());
        obj.setSubName(getPreferArtistName(o));
        obj.setImgUri(getPreferImgUri(o));
        obj.setTrackNum(o.list().size());
        dataListPtr()->append(obj);
    }
}
} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

