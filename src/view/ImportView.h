#ifndef IMPORTVIEW_H
#define IMPORTVIEW_H

#include <QFrame>

class QLabel;
class QPushButton;
class ImportView : public QFrame
{
    Q_OBJECT
public:
    explicit ImportView(QWidget *parent = nullptr);
    virtual ~ImportView() override;

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

#endif // IMPORTVIEW_H
