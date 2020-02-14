#ifndef RKSEARCHEDIT_H
#define RKSEARCHEDIT_H

#include <QFrame>
#include <QPropertyAnimation>

#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DImageButton;
DWIDGET_END_NAMESPACE


class QLabel;
class QLineEdit;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

// copy from DSearchEdit with tiny modifacation
class RKSearchEdit : public QFrame
{
    Q_OBJECT
public:
    explicit RKSearchEdit(QWidget *parent = Q_NULLPTR);
    ~RKSearchEdit();

    QSize sizeHint() const {return m_size;}
    QSize minimumSizeHint() const {return m_size;}
    const QString text() const;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

    inline void setAniDuration(const int duration) {m_animation->setDuration(duration);}
    inline void setAniShowCurve(const QEasingCurve curve) {m_showCurve = curve;}
    inline void setAniHideCurve(const QEasingCurve curve) {m_hideCurve = curve;}

    QLineEdit *getLineEdit() const;

public Q_SLOTS:
    void setText(const QString & text);
    void clear();
    void setPlaceHolder(const QString &text);

Q_SIGNALS:
    void textChanged();
    void returnPressed();
    void editingFinished();
    void focusOut();
    void focusIn();

private Q_SLOTS:
    void toEditMode();

private:
    void initInsideFrame();

protected:
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *e);

private:
    QSize                                       m_size;
    QLineEdit                                   *m_edt;
    DTK_WIDGET_NAMESPACE::DImageButton          *m_searchBtn;
    DTK_WIDGET_NAMESPACE::DImageButton          *m_clearBtn;
    QLabel                                      *m_placeHolder;
    QFrame                                      *m_insideFrame = Q_NULLPTR;

    QPropertyAnimation  *m_animation;
    QEasingCurve        m_showCurve = QEasingCurve::OutCubic;
    QEasingCurve        m_hideCurve = QEasingCurve::InCubic;
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKSEARCHEDIT_H
