#ifndef MAINLISTVIEW_H
#define MAINLISTVIEW_H

#include <QListView>

class QMenu;
class MainListView : public QListView
{
    Q_OBJECT
public:
    MainListView(QWidget *parent = Q_NULLPTR);
    ~MainListView() override;

    QMenu *popMenu() const;

    ///
    /// \brief contextMenuEnabled default false;
    /// \return
    ///
    bool contextMenuEnabled() const;
    void setContextMenuEnabled(bool contextMenuEnabled);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QMenu       *m_popMenu;
    bool        m_contextMenuEnabled;
    QModelIndex m_curIdx;
};

#endif // MAINLISTVIEW_H
