#include "RKOverlayWidget.h"

#include <QAbstractTransition>
#include <QCoreApplication>
#include <QEvent>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStackedLayout>

class WidgetWindow : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int offset WRITE setOffset READ offset)
public:
    explicit WidgetWindow(RKOverlayWidget *ly, QWidget *parent = Q_NULLPTR)
                    : QWidget(parent), m_overlay(ly)
    {

    }
    virtual ~WidgetWindow() override {}

    void setOffset(int offset)
    {
        QMargins margins = m_overlay->layout()->contentsMargins();
        margins.setBottom(offset);
        m_overlay->layout()->setContentsMargins(margins);
    }
    int offset() const
    {
        return m_overlay->layout()->contentsMargins().bottom();
    }

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event)
        QPainter painter(this);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
//        brush.setColor(Qt::GlobalColor::red);
        brush.setColor(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.setBrush(brush);
        painter.drawRect(rect());
    }

private:
    RKOverlayWidget *const m_overlay;
};


class WidgetProxy : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity WRITE setOpacity READ opacity)
public:
    enum TransparencyMode {
        Transparent = 0x0,
        SemiTransparent,
        Opaque,
    };
    WidgetProxy(WidgetWindow *source,
                          QStackedLayout *layout,
                          RKOverlayWidget *dialog,
                          QWidget *parent = Q_NULLPTR)
        : QWidget(parent), m_source(source), m_layout(layout),m_dialog(dialog),
          m_opacity(0), m_mode(Transparent)
    {

    }
    virtual ~WidgetProxy() override {}

    void setOpacity(qreal opacity)
    {
        m_opacity = opacity;
        m_mode = SemiTransparent;
        update();
        m_dialog->update();
    }
    inline qreal opacity() const
    {
        return m_opacity;
    }

    void makeOpaque()
    {
        m_dialog->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_layout->setCurrentIndex(0);
        m_opacity = 1.0;
        m_mode = Opaque;
        update();
    }
    void makeTransparent()
    {
        m_opacity = 0.0;
        m_mode = Transparent;
        update();
    }

    // QObject interface
public:
    bool event(QEvent *event) override
    {
        qDebug()<<Q_FUNC_INFO<<event->type();
        const QEvent::Type type = event->type();

        if (QEvent::Move == type || QEvent::Resize == type) {
            m_source->setGeometry(geometry());
        }
        return QWidget::event(event);
    }

    // QWidget interface
public:
    QSize sizeHint() const override
    {
        return m_source->sizeHint();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event)
        QPainter painter(this);
        qDebug()<<Q_FUNC_INFO<<"  m_mode "<<m_mode;
        if (Transparent == m_mode) {
            return;
        } else if (Opaque != m_mode) {
            painter.setOpacity(m_opacity);
        }
        QPixmap pm = m_source->grab(m_source->rect());
        painter.drawPixmap(0, 0, pm);
    }
private:
    Q_DISABLE_COPY(WidgetProxy)
    WidgetWindow            *const m_source;
    QStackedLayout          *const m_layout;
    RKOverlayWidget         *const m_dialog;
    qreal                         m_opacity;
    TransparencyMode              m_mode;
};

enum OverlayTransitionType {
    OverlayShowTransition = 0x0,
    OverlayHideTransition,
};

class OverlayStateTransitionEvent : public QEvent
{
    Q_GADGET
public:
    OverlayStateTransitionEvent(OverlayTransitionType type)
        : QEvent(QEvent::Type(QEvent::User + 1)),
          type(type)
    {
    }
    virtual ~OverlayStateTransitionEvent() override {}
    OverlayTransitionType type;
};

class OverlayStateTransition : public QAbstractTransition
{
    Q_OBJECT
public:
    explicit OverlayStateTransition(OverlayTransitionType type) : m_type(type) {}
    virtual ~OverlayStateTransition() override {}

    // QAbstractTransition interface
protected:
    bool eventTest(QEvent *event) override
    {
        if (event->type() != QEvent::Type(QEvent::User + 1)) {
            return false;
        }
        OverlayStateTransitionEvent *transition = static_cast<OverlayStateTransitionEvent *>(event);
        return (m_type == transition->type);
    }
    void onTransition(QEvent *event) override
    {
    }

private:
    OverlayTransitionType m_type;
};

RKOverlayWidget::RKOverlayWidget(QWidget *parent)
    : QWidget(parent)
{
    if (parent) {
        parent->installEventFilter(this);
    }
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    m_dialogWindow = new WidgetWindow(this);
    m_proxyStack   = new QStackedLayout;
    m_stateMachine = new QStateMachine(this);
    m_proxy        = new WidgetProxy(m_dialogWindow, m_proxyStack, this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);

    QWidget *widget = new QWidget;
    widget->setLayout(m_proxyStack);
    widget->setMinimumWidth(200);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
//    effect->setColor(QColor(0, 0, 0, 200));
    effect->setColor(Qt::GlobalColor::cyan);
    effect->setBlurRadius(64);
    effect->setOffset(0, 13);
    widget->setGraphicsEffect(effect);

    layout->addWidget(widget, Qt::AlignCenter);

    m_proxyStack->addWidget(m_dialogWindow);
    m_proxyStack->addWidget(m_proxy);
    m_proxyStack->setCurrentIndex(1);

    QState *hiddenState = new QState;
    QState *visibleState = new QState;

    m_stateMachine->addState(hiddenState);
    m_stateMachine->addState(visibleState);
    m_stateMachine->setInitialState(hiddenState);

    OverlayStateTransition *transition;

    transition = new OverlayStateTransition(OverlayShowTransition);
    transition->setTargetState(visibleState);
    hiddenState->addTransition(transition);

    transition = new OverlayStateTransition(OverlayHideTransition);
    transition->setTargetState(hiddenState);
    visibleState->addTransition(transition);

    visibleState->assignProperty(m_proxy, "opacity", 1);
    visibleState->assignProperty(effect, "color", QColor(0, 0, 0, 200));
    visibleState->assignProperty(m_dialogWindow, "offset", 0);
    hiddenState->assignProperty(m_proxy, "opacity", 0);
    hiddenState->assignProperty(effect, "color", QColor(0, 0, 0, 0));
    hiddenState->assignProperty(m_dialogWindow, "offset", 200);

    QPropertyAnimation *animation;

    animation = new QPropertyAnimation(m_proxy, "opacity", this);
    animation->setDuration(280);
    m_stateMachine->addDefaultAnimation(animation);

    animation = new QPropertyAnimation(effect, "color", this);
    animation->setDuration(280);
    m_stateMachine->addDefaultAnimation(animation);

    animation = new QPropertyAnimation(m_dialogWindow, "offset", this);
    animation->setDuration(280);
    animation->setEasingCurve(QEasingCurve::OutCirc);
    m_stateMachine->addDefaultAnimation(animation);

    QObject::connect(visibleState, &QState::propertiesAssigned,
                     m_proxy, &WidgetProxy::makeOpaque);
    QObject::connect(hiddenState, &QState::propertiesAssigned,
                     m_proxy, &WidgetProxy::makeTransparent);

    m_stateMachine->start();
    QCoreApplication::processEvents();
}

RKOverlayWidget::~RKOverlayWidget()
{
     qDebug()<<Q_FUNC_INFO<<"----------------";
}

QLayout *RKOverlayWidget::windowLayout() const
{
    return m_dialogWindow->layout();
}

void RKOverlayWidget::setWindowLayout(QLayout *layout)
{
    m_dialogWindow->setLayout(layout);
}

QRect RKOverlayWidget::overlayGeometry() const
{
    QWidget *widget = parentWidget();
    if (!widget) {
        return QRect();
    }
    return widget->rect();
}

bool RKOverlayWidget::event(QEvent *event)
{
    if (!parent()) {
        return QWidget::event(event);
    }
    qDebug()<<Q_FUNC_INFO<<event->type();

    switch (event->type()) {
        case QEvent::ParentChange: {
            parent()->installEventFilter(this);
            setGeometry(overlayGeometry());
            break;
        }
        case QEvent::ParentAboutToChange: {
            parent()->removeEventFilter(this);
            break;
        }
        default:
            break;
    }
    return QWidget::event(event);
}

bool RKOverlayWidget::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Move:
        case QEvent::Resize:
            setGeometry(overlayGeometry());
            break;
        default:
            break;
    }
    return QWidget::eventFilter(watched, event);
}

void RKOverlayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    qDebug()<<Q_FUNC_INFO<<event;

    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::GlobalColor::green);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(this->m_proxy->opacity()/2.4);
    painter.drawRect(rect());
}

void RKOverlayWidget::showOverlay()
{
    qDebug()<<Q_FUNC_INFO<<"----------------";
    this->m_stateMachine->postEvent(new OverlayStateTransitionEvent(OverlayShowTransition));
    this->raise();
}

void RKOverlayWidget::hideOverlay()
{
    this->m_stateMachine->postEvent(new OverlayStateTransitionEvent(OverlayHideTransition));
    setAttribute(Qt::WA_TransparentForMouseEvents);
    this->m_proxyStack->setCurrentIndex(1);
}


#include "RKOverlayWidget.moc"
