#ifndef RKBOXWIDGET_H
#define RKBOXWIDGET_H

#include <QWidget>

#include <dboxwidget.h>


DWIDGET_USE_NAMESPACE

class RKBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit RKBoxWidget(QBoxLayout::Direction direction, QWidget *parent = Q_NULLPTR);
    virtual ~RKBoxWidget() override;

    // QObject interface
public:
    bool event(QEvent *event) override;

    // DBoxWidget interface
protected:
    void updateSize(const QSize &size) override;
};


class RKHBoxWidget : public RKBoxWidget
{
    Q_OBJECT
public:
    explicit RKHBoxWidget(QWidget *parent = Q_NULLPTR) : RKBoxWidget(QBoxLayout::LeftToRight, parent){}
};

class RKVBoxWidget : public RKBoxWidget
{
    Q_OBJECT
public:
    explicit RKVBoxWidget(QWidget *parent = Q_NULLPTR) : RKBoxWidget(QBoxLayout::TopToBottom, parent){}
};

#endif // RKBOXWIDGET_H
