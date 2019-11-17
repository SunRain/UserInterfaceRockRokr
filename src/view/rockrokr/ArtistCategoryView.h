#ifndef ARTISTCATEGORYVIEW_H
#define ARTISTCATEGORYVIEW_H

#include "BaseCategoryView.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class ArtistCategoryView : public BaseCategoryView
{
    Q_OBJECT
public:
    ArtistCategoryView(QWidget *parent = Q_NULLPTR);
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // ARTISTCATEGORYVIEW_H
