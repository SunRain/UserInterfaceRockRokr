#ifndef MAINWINDOWBGVIEW_H
#define MAINWINDOWBGVIEW_H

#include <QWidget>


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class MainWindowBGView : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowBGView(QWidget *parent = nullptr);
    virtual ~MainWindowBGView() override;

};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // MAINWINDOWBGVIEW_H
