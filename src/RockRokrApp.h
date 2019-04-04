#ifndef ROCKROKRAPP_H
#define ROCKROKRAPP_H

#include <QObject>

class MainWindow;
class RockRokrApp : public QObject
{
    Q_OBJECT
private:
    explicit RockRokrApp(QObject *parent = nullptr);
    static RockRokrApp *createInstance();

public:
    static RockRokrApp *instance();
    virtual ~RockRokrApp();

    void init();

    void show();

    MainWindow *mainWindow() const;

private:
    void initUI();

private:
    MainWindow      *m_mainWindow;

};

#endif // ROCKROKRAPP_H
