#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QFrame>
#include <dtkwidget_global.h>


DWIDGET_BEGIN_NAMESPACE
class DWaterProgress;
DWIDGET_END_NAMESPACE

class LoadingWidget : public QFrame
{
    Q_OBJECT
public:
    explicit LoadingWidget(QWidget *parent = nullptr);
    virtual ~LoadingWidget();

    void start();

    void stop();

private:
    DTK_WIDGET_NAMESPACE::DWaterProgress *m_progress;
};

#endif // LOADINGWIDGET_H
