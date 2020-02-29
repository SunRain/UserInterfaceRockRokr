#ifndef SEARCHRESULTITEM_H
#define SEARCHRESULTITEM_H

#include <QFrame>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class SearchInnerLabel;
class SearchResultItem : public QFrame
{
    Q_OBJECT
public:
    explicit SearchResultItem(QWidget *parent = Q_NULLPTR);
    virtual ~SearchResultItem();

    void setText(const QString &text);

    void setSubText(const QString &subText);

    void setHoverState(bool hover);

    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    SearchInnerLabel  *m_lable      = Q_NULLPTR;
    SearchInnerLabel  *m_subLabel   = Q_NULLPTR;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // SEARCHRESULTITEM_H
