#include "RKExpand.h"

#include <QDebug>

#include <DThemeManager>
#include <QResizeEvent>
#include <dbaseexpand.h>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


class RKContentLoader : public ContentLoader
{
    Q_OBJECT
public:
    explicit RKContentLoader(QWidget *parent = Q_NULLPTR) : ContentLoader(parent)
    {

    }
    virtual ~RKContentLoader() override {}


    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override
    {
        ContentLoader::resizeEvent(event);
//        qDebug()<<Q_FUNC_INFO<<" size "<<event->size();
        emit sizeChanged(event->size());
    }

signals:
    void sizeChanged(QSize size);
};

RKExpand::RKExpand(QWidget *parent) : QWidget(parent)
{
    DThemeManager::registerWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_headerLayout = new QVBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setAlignment(Qt::AlignCenter);

    m_hSeparator = new DSeparatorHorizontal();
    m_bottom_separator = new DSeparatorHorizontal;
    m_bottom_separator->hide();

    connect(this, &RKExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);

    m_contentLoader = new RKContentLoader();
    m_contentLoader->setFixedHeight(0); // default to not expanded.
    connect(m_contentLoader, &RKContentLoader::sizeChanged,
            [&](QSize size){
//        qDebug()<<Q_FUNC_INFO<<" RKContentLoader::sizeChanged "<<size;
//        if (m_header) {
//            m_header->setFixedWidth(size.width());
//        }

        emit this->expandExpectedSizeChanged(size);
    });

    m_boxWidget = new DVBoxWidget;
    m_contentLayout = m_boxWidget->layout();

    QVBoxLayout *layout_contentLoader = new QVBoxLayout(m_contentLoader);

    layout_contentLoader->setMargin(0);
    layout_contentLoader->setSpacing(0);
    layout_contentLoader->addWidget(m_boxWidget);
    layout_contentLoader->addStretch();

    m_animation = new QPropertyAnimation(m_contentLoader, "height");
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::InSine);
    connect(m_animation, &QPropertyAnimation::valueChanged, this, [this] {
        setFixedHeight(sizeHint().height());
    });

    mainLayout->addLayout(m_headerLayout);
    mainLayout->addWidget(m_hSeparator);
    mainLayout->addWidget(m_contentLoader);
    mainLayout->addWidget(m_bottom_separator);

    setLayout(mainLayout);

    connect(m_boxWidget, &DBoxWidget::sizeChanged,
            this, [/*this*/&] (QSize size) {
        if (m_expand)
        {
            int endHeight = 0;
            endHeight = m_boxWidget->height();

            m_animation->setStartValue(m_contentLoader->height());
            m_animation->setEndValue(endHeight);
            m_animation->stop();
            m_animation->start();
        } else {
            m_expectedExpendSize = size;//m_boxWidget->height();
        }
    });
}

RKExpand::~RKExpand()
{
    if (m_headerLayout) {
        m_headerLayout->deleteLater();
    }
    if (m_contentLayout) {
        m_contentLayout->deleteLater();
    }
    if (m_contentLoader) {
        m_contentLoader->deleteLater();
    }
    if (m_animation) {
        m_animation->deleteLater();
    }
}

/**
 * \~chinese \brief 设置标题控件
 * \~chinese 标题控件会始终显示在布局里
 * \~chinese \param header 标题控件
 */
void RKExpand::setHeader(QWidget *header)
{
    if (!header) {
        return;
    }

    QLayoutItem *child;
    while ((child = m_headerLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_headerLayout->addWidget(header);
    m_header = header;
}

/**
 * \~chinese \property RKExpand::getContent
 * \~chinese \brief 获取内容控件对象
 * \~chinese \return 内容控件对象
 */

/**
 * \~chinese \brief 设置内容控件
 * \~chinese 内容控件默认是隐藏的，调用 RKExpand::setExpand 设置其可见性
 * \~chinese \param content 内容控件
 * \~chinese \param alignment 内容控件在布局中的对齐方式
 */
void RKExpand::setContent(QWidget *content, Qt::Alignment alignment)
{
    if (!content) {
        return;
    }

    QLayoutItem *child;
    while ((child = m_contentLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_contentLayout->addWidget(content, 1, alignment);
    m_contentLayout->addStretch(1);
    m_content = content;
}

/**
 * \~chinese \brief 设置标题控件的高度
 * \~chineseQSize
 * \~chinese \param height 指定的高度
 */
void RKExpand::setHeaderHeight(int height)
{
    if (m_header) {
        m_header->setFixedHeight(height);
    }
}

/**
 * \~chinese \brief 设置内容控件的可见性
 * \~chinese
 * \~chinese \param value 为 true 则内容控件可见，反之则反
 */
void RKExpand::setExpand(bool value)
{
    if (m_expand == value) {
        return;
    }

    m_expand = value;
    Q_EMIT expandChange(value);

    m_boxWidget->resize(m_expectedExpendSize);

    if (value) {
        m_animation->setStartValue(0);
        m_animation->setEndValue(/*m_boxWidget->height()*/m_expectedExpendSize.height());
    } else {
        m_animation->setStartValue(/*m_boxWidget->height()*/m_expectedExpendSize.height());
        m_animation->setEndValue(0);
    }

    m_animation->stop();
    m_animation->start();
}

/**
 * \~chinese \brief 获取当前内容控件的可见性
 * \~chinese
 * \~chinese \return 当前内容控件的可见性
 */
bool RKExpand::expand() const
{
    return m_expand;
}

/**
 * \~chinese \brief 设置内容控件的可见性改变时动画的时间
 * \~chinese
 * \~chinese \param duration 指定动画时间
 */
void RKExpand::setAnimationDuration(int duration)
{
    m_animation->setDuration(duration);
}

/**
 * \~chinese \brief 设置内容控件的可见性改变时动画的样式
 * \~chinese
 * \~chinese \param curve 指定动画样式
 */
void RKExpand::setAnimationEasingCurve(QEasingCurve curve)
{
    m_animation->setEasingCurve(curve);
}

/**
 * \~chinese \brief 设置是否允许标题控件与内容控件之间的分割线
 * \~chinese
 * \~chinese \param arg 为 ture 则显示分割线，反之则反
 */
void RKExpand::setSeparatorVisible(bool arg)
{
    m_hSeparator->setVisible(arg);
}

/**
 * \~chinese \brief 设置是否允许内容控件下的分割线
 * \~chinese
 * \~chinese \param arg 为 ture 则显示分割线，反之则反
 */
void RKExpand::setExpandedSeparatorVisible(bool arg)
{
    if (arg) {
        connect(this, &RKExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);
        m_bottom_separator->show();
    } else {
        disconnect(this, &RKExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);
        m_bottom_separator->hide();
    }
}

void RKExpand::updateExpandSize(QSize size)
{
    if (m_expand) {
        m_boxWidget->setFixedSize(size);
//        emit expandSizeChanged(size);
    } /*else {
        m_expectedExpendSize = size;
    }*/
    m_expectedExpendSize = size;
    emit expandExpectedSizeChanged(size);
//    int w = size.width();
//    int h = size.height();
//    h += m_header->height();
//    h += m_hSeparator->height();
//    h += m_bottom_separator->height();

//    emit expandSizeChanged(m_expectedExpendSize, QSize(w, h));
}

void RKExpand::resizeEvent(QResizeEvent *e)
{
//    qDebug()<<Q_FUNC_INFO<<" --- "<<e;

    if (m_content) {
        m_content->setFixedWidth(e->size().width());
    }
//    if (m_header) {
//        m_header->setFixedWidth(e->size().width());
//    }
    QWidget::resizeEvent(e);
//    emit expandSizeChanged(m_expectedExpendSize, e->size());
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#include "RKExpand.moc"

