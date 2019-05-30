#ifndef QTMATERIALOVERLAYWIDGET_H
#define QTMATERIALOVERLAYWIDGET_H

#include <QWidget>

class QtMaterialOverlayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QtMaterialOverlayWidget(QWidget *parent = nullptr);
    virtual ~QtMaterialOverlayWidget() override;

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    virtual QRect overlayGeometry() const;

private:
    Q_DISABLE_COPY(QtMaterialOverlayWidget)
};

#endif // QTMATERIALOVERLAYWIDGET_H
