#ifndef MAINWINDOWBGVIEW_H
#define MAINWINDOWBGVIEW_H

#include <QWidget>

class MainWindowBGView : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowBGView(QWidget *parent = nullptr);
    virtual ~MainWindowBGView() override;

};

#endif // MAINWINDOWBGVIEW_H
