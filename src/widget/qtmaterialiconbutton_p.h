#ifndef QTMATERIALICONBUTTON_P_H
#define QTMATERIALICONBUTTON_P_H

#include <QColor>
#include <QtGlobal>
#include <QIcon>

class QtMaterialIconButton;
class QtMaterialRippleOverlay;
class QColor;

class QtMaterialIconButtonPrivate
{
    Q_DISABLE_COPY(QtMaterialIconButtonPrivate)
    Q_DECLARE_PUBLIC(QtMaterialIconButton)

public:
    QtMaterialIconButtonPrivate(QtMaterialIconButton *q);
    virtual ~QtMaterialIconButtonPrivate();

    void init();
    void updateRipple();

    QtMaterialIconButton    *const q_ptr;
    QtMaterialRippleOverlay *rippleOverlay;
    QIcon                    disabledIcon;
    QColor                   color;
    QColor                   disabledColor;
    bool                     disabled;
};

#endif // QTMATERIALICONBUTTON_P_H
