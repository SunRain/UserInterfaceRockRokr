#include "qtmaterialiconbutton.h"

#include <QMouseEvent>
#include <QDebug>

#include "qtmaterialrippleoverlay.h"


///*!
// *  \class QtMaterialIconButtonPrivate
// *  \internal
// */

//QtMaterialIconButtonPrivate::QtMaterialIconButtonPrivate(QtMaterialIconButton *q)
//    : q_ptr(q)
//{
//}

//QtMaterialIconButtonPrivate::~QtMaterialIconButtonPrivate()
//{
//}

//void QtMaterialIconButtonPrivate::init()
//{
//    Q_Q(QtMaterialIconButton);

//    rippleOverlay  = new QtMaterialRippleOverlay(q->parentWidget());
//    color.setNamedColor("#9e9e9e");
//    disabledColor.setNamedColor("#616161");
//    rippleOverlay->installEventFilter(q);


//    QSizePolicy policy;
//    policy.setWidthForHeight(true);
//    q->setSizePolicy(policy);
//}

//void QtMaterialIconButtonPrivate::updateRipple()
//{
//    Q_Q(QtMaterialIconButton);

//    QRect r(q->rect());
//    r.setSize(QSize(q->width()*2, q->height()*2));
//    r.moveCenter(q->geometry().center());
//    rippleOverlay->setGeometry(r);
//}

///*!
// *  \class QtMaterialIconButton
// */

//QtMaterialIconButton::QtMaterialIconButton(const QIcon &icon, QWidget *parent)
//    : QAbstractButton(parent),
//      d_ptr(new QtMaterialIconButtonPrivate(this))
//{
//    d_func()->init();

//    setIcon(icon);
//}

//QtMaterialIconButton::QtMaterialIconButton(QWidget *parent)
//    : QAbstractButton(parent),
//      d_ptr(new QtMaterialIconButtonPrivate(this))
//{
//    d_func()->init();
//}

//QtMaterialIconButton::~QtMaterialIconButton()
//{
//}

///*!
// *  \reimp
// */
//QSize QtMaterialIconButton::sizeHint() const
//{
//    return iconSize();
//}

//void QtMaterialIconButton::setColor(const QColor &color)
//{
//    Q_D(QtMaterialIconButton);
//    d->color = color;
//    update();
//}

//QColor QtMaterialIconButton::color() const
//{
//    Q_D(const QtMaterialIconButton);

//     return d->color;
//}

//void QtMaterialIconButton::setDisabledColor(const QColor &color)
//{
//    Q_D(QtMaterialIconButton);
//    d->disabledColor = color;
//    update();
//}

//QColor QtMaterialIconButton::disabledColor() const
//{
//    Q_D(const QtMaterialIconButton);
//    return d->disabledColor;
//}

//QIcon QtMaterialIconButton::disabledIcon() const
//{
//    Q_D(const QtMaterialIconButton);
//    return d->disabledIcon;
//}

//void QtMaterialIconButton::setDisabledIcon(const QIcon &disabledIcon)
//{
//    Q_D(QtMaterialIconButton);
//    d->disabledIcon = disabledIcon;
//    update();
//}

//void QtMaterialIconButton::setIcon(const QIcon &icon)
//{
//    QAbstractButton::setIcon(icon);
//    update();
//}

//void QtMaterialIconButton::setDisabled(bool disable)
//{
//    Q_D(QtMaterialIconButton);
//    d->disabled = disable;
//    QAbstractButton::setDisabled(disable);
//}

//void QtMaterialIconButton::setIconSize(const QSize &size)
//{
//    QAbstractButton::setIconSize(size);
//    update();
//}

//QtMaterialIconButton::QtMaterialIconButton(QtMaterialIconButtonPrivate &d, QWidget *parent)
//    : QAbstractButton(parent),
//      d_ptr(&d)
//{
//    d_func()->init();
//}

///*!
// *  \reimp
// */
//bool QtMaterialIconButton::event(QEvent *event)
//{
//    Q_D(QtMaterialIconButton);

//    switch (event->type())
//    {
//    case QEvent::Move:
//    case QEvent::Resize:
//        d->updateRipple();
//        break;
//    case QEvent::ParentChange: {
//        QWidget *widget;
//        if ((widget = parentWidget())) {
//            d->rippleOverlay->setParent(widget);
//        }
//        break;
//    }
//    default:
//        break;
//    }
//    return QAbstractButton::event(event);
//}

///*!
// *  \reimp
// */
//bool QtMaterialIconButton::eventFilter(QObject *obj, QEvent *event)
//{
//    if (QEvent::Resize == event->type())
//    {
//        Q_D(QtMaterialIconButton);

//        d->updateRipple();
//    }
//    return QAbstractButton::eventFilter(obj, event);
//}

///*!
// *  \reimp
// */
//void QtMaterialIconButton::mousePressEvent(QMouseEvent *event)
//{
//    Q_D(QtMaterialIconButton);

//    d->rippleOverlay->addRipple(QPoint(d->rippleOverlay->width(),
//                                       d->rippleOverlay->height())/2,
//                                iconSize().width());
//    emit clicked();

//    QAbstractButton::mousePressEvent(event);
//}

///*!
// *  \reimp
// */
//void QtMaterialIconButton::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event)
//    Q_D(const QtMaterialIconButton);

//    QPainter painter(this);

//    QPixmap pixmap;
//    if (!this->isEnabled() || d->disabled) {
//        pixmap = disabledIcon().pixmap(iconSize());
//    } else {
//        pixmap = icon().pixmap(iconSize());
//    }
////    QPainter icon(&pixmap);
////    icon.setCompositionMode(QPainter::CompositionMode_SourceIn);
////    icon.fillRect(pixmap.rect(), isEnabled() ? color() : disabledColor());

//    QRect r(rect());
//    const qreal w = pixmap.width();
//    const qreal h = pixmap.height();
//    painter.drawPixmap(QRect((r.width()-w)/2, (r.height()-h)/2, w, h), pixmap);
//}

RKIconButton::RKIconButton(QWidget *parent)
    : QPushButton(parent),
      m_overlay(new QtMaterialRippleOverlay(parent))
{
}

RKIconButton::~RKIconButton()
{

}

bool RKIconButton::event(QEvent *event)
{
    if (event->type() == QEvent::ParentChange && parentWidget()) {
        m_overlay->setParent(parentWidget());
    }
    return QPushButton::event(event);
}

void RKIconButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
}

void RKIconButton::mousePressEvent(QMouseEvent *event)
{
    m_overlay->addRipple(QPoint(m_overlay->width(), m_overlay->height())/2,
                         rect().width() *4/5);
    QPushButton::mousePressEvent(event);
}

void RKIconButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
}

void RKIconButton::resizeEvent(QResizeEvent *event)
{
    QRect r = this->rect();
    r.setSize(QSize(this->width() + this->width()/2,
                    this->height() + this->height()/2));
    r.moveCenter(this->geometry().center());
    m_overlay->setGeometry(r);
    QPushButton::resizeEvent(event);
}


