#include "ListModel.h"

#include <QDebug>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


const static char *KEY_ARTISTS      = "KEY_ARTISTS";
const static char *KEY_ALBUMS       = "KEY_ALBUMS";
const static char *KEY_SONGS        = "KEY_SONGS";
const static char *KEY_GENRES       = "KEY_GENRES";
const static char *KEY_FAVORITES    = "KEY_FAVORITES";

ListModel::ListModel(QAbstractListModel *parent)
    : QAbstractListModel(parent),
      m_curIdx(QModelIndex())
{

}

ListModel::~ListModel()
{

}

void ListModel::showMainPart()
{
    beginResetModel();
    m_DataMap.clear();
    endResetModel();

    m_DataMap.insert(KEY_ARTISTS, tr("KEY_ARTISTS"));
    m_DataMap.insert(KEY_ALBUMS, tr("KEY_ALBUMS"));
    m_DataMap.insert(KEY_SONGS, tr("KEY_SONGS"));
    m_DataMap.insert(KEY_GENRES, tr("KEY_GENRES"));
    m_DataMap.insert(KEY_FAVORITES, tr("KEY_FAVORITES"));
    for (int i = 0; i < m_DataMap.size(); ++i) {
        beginInsertRows(QModelIndex(), i, i);
        endInsertRows();
    }
}

void ListModel::showLibraryPart()
{
    beginResetModel();
    m_DataMap.clear();
    endResetModel();
    //TODO
}

int ListModel::count()
{
    return m_DataMap.size();
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return  m_DataMap.size();

}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    const int idx = index.row();
    if (idx < 0 || idx >= m_DataMap.size())
        return QVariant();
    switch (role) {
    case RoleKey:
        return m_DataMap.keys().at(idx);
    case RoleName:
        return m_DataMap.value(m_DataMap.keys().at(idx));
    case RoleCurIdx:
        return index == m_curIdx;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> role;
    role.insert(ModelRoles::RoleCurIdx, "RoleCurIdx");
    role.insert(ModelRoles::RoleKey, "RoleKey");
    role.insert(ModelRoles::RoleName, "RoleName");
    return role;
}

void ListModel::setCurIdx(QModelIndex idx)
{
    m_curIdx = idx;
    emit dataChanged(m_curIdx, m_curIdx);
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

