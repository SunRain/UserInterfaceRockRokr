#ifndef RKLISTWIDGET_H
#define RKLISTWIDGET_H

#include <QListWidget>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit RKListWidget(QWidget *parent = nullptr);
    virtual ~RKListWidget() override;

    void addItem(QListWidgetItem *item);

    void insertItem(int row, QListWidgetItem *item);

    QListWidgetItem *takeItem(int row);

    ///
    /// \brief contentHeight
    /// \return contentHeight + spacings in each items
    ///
    int contentHeight() const;

signals:
    void contentHeightChanged(int height);

protected:
    virtual void updateHeight();

private:
    int m_contentHeight;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKLISTWIDGET_H
