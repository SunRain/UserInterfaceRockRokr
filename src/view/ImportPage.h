#ifndef IMPORTPAGE_H
#define IMPORTPAGE_H

#include <QFrame>

class QLabel;
class QPushButton;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class ImportPage : public QFrame
{
    Q_OBJECT
public:
    explicit ImportPage(QWidget *parent = Q_NULLPTR);
    virtual ~ImportPage() override;

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void scanStandardMusicPath();
    void showImportDlg();
    void importFiles(const QStringList &urilist);


private:
    QLabel          *m_label;
    QPushButton     *m_btn;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // IMPORTPAGE_H
