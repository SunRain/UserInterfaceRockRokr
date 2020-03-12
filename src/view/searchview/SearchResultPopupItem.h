#ifndef SEARCHRESULTPOPUPITEM_H
#define SEARCHRESULTPOPUPITEM_H

#include <QFrame>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SearchInnerLabel;
class SearchResultPopupItem : public QFrame
{
    Q_OBJECT
public:
    explicit SearchResultPopupItem(QWidget *parent = Q_NULLPTR);
    virtual ~SearchResultPopupItem();

    void setText(const QString &text);

    void setSubText(const QString &subText);

    void setHoverState(bool hover);

    inline bool hoverState() const
    {
        return m_hoverState;
    }

    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    SearchInnerLabel  *m_lable          = Q_NULLPTR;
    SearchInnerLabel  *m_subLabel       = Q_NULLPTR;
    QWidget           *m_wrapperWidget  = Q_NULLPTR;
    bool    m_hoverState = false;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // SEARCHRESULTPOPUPITEM_H
