#include "ItemFragment.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

ItemFragment::ItemFragment(ItemFragment::LayoutOrientation o, QWidget *parent)
    : QWidget(parent)
{
    if (o == Horizontal) {
        m_layout = new QHBoxLayout(this);
    } else {
        m_layout = new QVBoxLayout(this);
    }
    this->setStyleSheet("QWidget{background-color:transparent;}");
}

ItemFragment::~ItemFragment()
{

}

QBoxLayout *ItemFragment::layout() const
{
    return m_layout;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
