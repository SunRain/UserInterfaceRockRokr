#ifndef ITEMFRAGMENT_H
#define ITEMFRAGMENT_H

#include <QWidget>
#include <QBoxLayout>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKBoxWidget;
class ItemFragment : public QWidget
{
    Q_OBJECT
public:
    enum LayoutOrientation {
        Horizontal = 0x0,
        Vertical
    };

    explicit ItemFragment(LayoutOrientation o = Horizontal, QWidget *parent = Q_NULLPTR);
    virtual ~ItemFragment();

    QBoxLayout *layout() const;

private:
    QBoxLayout          *m_layout;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // ITEMFRAGMENT_H
