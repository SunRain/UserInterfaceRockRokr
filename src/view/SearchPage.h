#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QFrame>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {


class SearchPage : public QFrame
{
    Q_OBJECT
public:
    explicit SearchPage(QWidget *parent = Q_NULLPTR);
    virtual ~SearchPage();

protected:
    void initUserInterface();

};

} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer
#endif // SEARCHPAGE_H
