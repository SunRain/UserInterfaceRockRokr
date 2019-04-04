#ifndef QTMATERIALICONBUTTON_H
#define QTMATERIALICONBUTTON_H

#include <QPushButton>

//class QtMaterialIconButtonPrivate;

//class QtMaterialIconButton : public QAbstractButton
//{
//    Q_OBJECT
//    Q_PROPERTY(QColor color READ color WRITE setColor)
//    Q_PROPERTY(QColor disabledColor READ disabledColor WRITE setDisabledColor)
//    Q_PROPERTY(QIcon disabledIcon READ disabledIcon WRITE setDisabledIcon)

//public:
//    explicit QtMaterialIconButton(const QIcon &icon, QWidget *parent = Q_NULLPTR);
//    QtMaterialIconButton(QWidget *parent = Q_NULLPTR);
//    virtual ~QtMaterialIconButton() override;

//    QSize sizeHint() const Q_DECL_OVERRIDE;

//    void setColor(const QColor &color);
//    QColor color() const;

//    void setDisabledColor(const QColor &color);
//    QColor disabledColor() const;

//    QIcon disabledIcon() const;
//    void setDisabledIcon(const QIcon &disabledIcon);

//    void setIcon(const QIcon &icon);

//    Q_SLOT void setDisabled(bool disable);
//    Q_SLOT void setIconSize(const QSize &size);
//protected:
//    QtMaterialIconButton(QtMaterialIconButtonPrivate &d, QWidget *parent = nullptr);

//    bool event(QEvent *event) Q_DECL_OVERRIDE;
//    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
//    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

//    const QScopedPointer<QtMaterialIconButtonPrivate> d_ptr;

//private:
//    Q_DISABLE_COPY(QtMaterialIconButton)
//    Q_DECLARE_PRIVATE(QtMaterialIconButton)
//};

class QtMaterialRippleOverlay;
class RKIconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RKIconButton(QWidget *parent = Q_NULLPTR);
    virtual ~RKIconButton() override;

    // QObject interface
public:
    bool event(QEvent *event) override;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QtMaterialRippleOverlay         *m_overlay;
};

#endif // QTMATERIALICONBUTTON_H
