#ifndef LBLISTITEM_H
#define LBLISTITEM_H

#include <QFrame>
#include <QVariant>

class QLabel;
class QLineEdit;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {
    
class LBListItem : public QFrame
{
    Q_OBJECT
    Q_ENUMS(ItemType)
public:
    enum ItemType {
        TypeArtist = 0x0,
        TypeAlbums,
        TypeGenres,
        TypeSongs,
        TypeFavorites,
        TypePlaylist,
        TypeAddon,
        TypeService
    };
    explicit LBListItem(const ItemType &type, const QString &text, QWidget *parent = Q_NULLPTR);
    virtual ~LBListItem() override;

    QVariant extraData() const;
    void setExtraData(const QVariant &extraData);

    void keepHover(bool active);

    void setHoverState(bool hover);

    void setEditMode(bool editable);

    int itemType() const;

public:
    QString enumToStr(const QString &enumName, int enumValue);

    // QObject interface
protected:
//    bool event(QEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void leftBtnClicked(LBListItem *item);
    void rightBtnClicked(LBListItem *item,  const QPoint &pos);
    void rename(const QString &text);

private:
    QLineEdit       *m_textEdit;
    QLabel          *m_iconLabel;
    bool            m_editMode;
    bool            m_keepHover;
    ItemType        m_itemType;
    QString         m_text;
    QVariant        m_extraData;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // LBLISTITEM_H
