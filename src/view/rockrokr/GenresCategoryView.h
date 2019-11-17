#ifndef GENRESCATEGORYVIEW_H
#define GENRESCATEGORYVIEW_H

#include "BaseCategoryView.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class GenresCategoryView : public BaseCategoryView
{
    Q_OBJECT
public:
    GenresCategoryView(QWidget *parent = Q_NULLPTR);
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // GENRESCATEGORYVIEW_H
