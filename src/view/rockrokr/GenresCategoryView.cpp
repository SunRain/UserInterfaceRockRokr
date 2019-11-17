#include "GenresCategoryView.h"

#include "CategoryModel.h"
#include "CategoryViewDataProvider.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

GenresCategoryView::GenresCategoryView(QWidget *parent)
    : BaseCategoryView (new GenresCategoryDataProvider(), parent)
{

}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
