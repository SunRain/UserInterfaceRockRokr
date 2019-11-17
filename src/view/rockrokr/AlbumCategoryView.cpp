#include "AlbumCategoryView.h"


#include "CategoryModel.h"
#include "CategoryViewDataProvider.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

AlbumCategoryView::AlbumCategoryView(QWidget *parent)
    : BaseCategoryView (new AlbumCategoryDataProvider(), parent)
{

}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
