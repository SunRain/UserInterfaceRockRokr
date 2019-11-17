#include "ArtistCategoryView.h"

#include "CategoryModel.h"
#include "CategoryViewDataProvider.h"


namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

ArtistCategoryView::ArtistCategoryView(QWidget *parent)
    : BaseCategoryView (new ArtistCategoryDataProvider(), parent)
{

}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
