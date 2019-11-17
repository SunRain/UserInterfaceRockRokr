#ifndef RKEXPAND_H
#define RKEXPAND_H

#include <QPropertyAnimation>
#include <QWidget>
#include <dboxwidget.h>
#include <dseparatorhorizontal.h>

DWIDGET_USE_NAMESPACE


DWIDGET_BEGIN_NAMESPACE
class ContentLoader;
DWIDGET_END_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKContentLoader;
class RKExpand : public QWidget
{
    Q_OBJECT
public:
    explicit RKExpand(QWidget *parent = Q_NULLPTR);
    virtual ~RKExpand() override;

    void setHeader(QWidget *header);
    inline QWidget *getHeader() const {return m_header;}
    void setContent(QWidget *content, Qt::Alignment alignment = Qt::AlignHCenter);
    inline QWidget *getContent() const {return m_content;}
    void setHeaderHeight(int height);
    virtual void setExpand(bool value);
    bool expand() const;
    void setAnimationDuration(int duration);
    void setAnimationEasingCurve(QEasingCurve curve);
    void setSeparatorVisible(bool arg);
    void setExpandedSeparatorVisible(bool arg);

    inline DSeparatorHorizontal *getSeparator() const {return m_hSeparator;}
    inline DSeparatorHorizontal *getExpandedSeparato() const {return m_bottom_separator;}

protected:
    void updateExpandSize(QSize size);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

Q_SIGNALS:
    void expandChange(bool e);
    void expandExpectedSizeChanged(QSize expandSize);

private:
    QWidget                 *m_header = Q_NULLPTR;
    QWidget                 *m_content = Q_NULLPTR;
    DBoxWidget              *m_boxWidget = Q_NULLPTR;
    QVBoxLayout             *m_headerLayout = Q_NULLPTR;
    QBoxLayout              *m_contentLayout = Q_NULLPTR;
    RKContentLoader         *m_contentLoader = Q_NULLPTR;
    DSeparatorHorizontal    *m_hSeparator = Q_NULLPTR;
    DSeparatorHorizontal    *m_bottom_separator = Q_NULLPTR;
    QPropertyAnimation      *m_animation = Q_NULLPTR;
    bool m_expand = false;
    QSize m_expectedExpendSize = QSize();
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // RKEXPAND_H
