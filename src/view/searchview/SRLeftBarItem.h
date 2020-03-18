#ifndef SRLEFTBARITEM_H
#define SRLEFTBARITEM_H

#include <QVariant>
#include <QFrame>
#include <QLabel>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SRLeftBarItem : public QFrame
{
    Q_OBJECT
public:
    explicit SRLeftBarItem(QWidget *parent = Q_NULLPTR);
    virtual ~SRLeftBarItem() override;

    void setHoverState(bool hover);

    void setText(const QString &text);

    inline QVariant extraData() const
    {
        return m_extraData;
    }
    inline void setExtraData(const QVariant &extraData)
    {
        m_extraData = extraData;
    }

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel  *m_label = Q_NULLPTR;
    QVariant m_extraData;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // SRLEFTBARITEM_H
