#ifndef ALBUMCATEGORYVIEW_H
#define ALBUMCATEGORYVIEW_H

#include "BaseCategoryView.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class BaseCategoryModel;
class AlbumCategoryView : public BaseCategoryView
{
    Q_OBJECT
public:
    AlbumCategoryView(QWidget *parent = Q_NULLPTR);
};


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // ALBUMCATEGORYVIEW_H
