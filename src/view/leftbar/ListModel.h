#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QMap>
#include <QObject>
#include <QAbstractListModel>

class ListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListModel(QAbstractListModel *parent = nullptr);
    ~ListModel() override;

    void showMainPart();

    void showLibraryPart();

    int count();


public:
    enum ModelRoles {
        RoleCurIdx = Qt::UserRole + 1,
        RoleKey,
        RoleName
    };

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setCurIdx(QModelIndex idx);

private:
    QMap<QString, QString> m_DataMap;
    QModelIndex m_curIdx;
};

#endif // LISTMODEL_H
