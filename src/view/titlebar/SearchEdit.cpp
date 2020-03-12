#include "SearchEdit.h"

#include <QTimer>
#include <QDebug>
#include <QResizeEvent>

#include "rockrokr_global.h"
#include "view/searchview/SearchResultPopup.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

SearchEdit::SearchEdit(QWidget *parent)
    : RKSearchEdit(parent)
{
    connect(this, &SearchEdit::focusOut,
            this, &SearchEdit::onFocusOut);

    connect(this, &SearchEdit::textChanged,
            this, &SearchEdit::onTextChanged);

    connect(this, &SearchEdit::returnPressed,
            this, &SearchEdit::onReturnPressed);
}

SearchEdit::~SearchEdit()
{

}

void SearchEdit::bindPopup(SearchResultPopup *view)
{
    m_resultPopup = view;
    m_resultPopup->hide();

    connect(m_resultPopup, &SearchResultPopup::searchMatched,
            this, [&]() {
        m_resultPopup->setFixedWidth(this->width() + _to_px(40));

        m_resultPopup->calToResize();
        m_resultPopup->show();

        m_resultPopup->move(m_topLeftToGlobal.x(),
                           m_topLeftToGlobal.y() + this->rect().height());
        m_resultPopup->setFocusPolicy(Qt::StrongFocus);
        m_resultPopup->raise();
    });
}

QSize SearchEdit::sizeHint() const
{
    return QSize(TITLE_BAR_SEARCH_VIEW_W, TITLE_BAR_SEARCH_VIEW_H);
}

void SearchEdit::onFocusOut()
{
    QTimer::singleShot(50, [this] (){
        m_resultPopup->hide();
    });
}

void SearchEdit::onTextChanged()
{
    auto text = QString(this->text()).remove(" ").remove("\r").remove("\n");
    if (text.length() >= 2) {
        auto searchtext = QString(this->text()).remove("\r").remove("\n");
        m_resultPopup->setSearchString(searchtext);
        m_resultPopup->doSearch();

//        m_resultView->setFixedWidth(this->width() + _to_px(40));

//        m_resultView->calToResize();
//        m_resultView->show();

//        m_resultView->move(m_topLeftToGlobal.x(),
//                           m_topLeftToGlobal.y() + this->rect().height());
//        m_resultView->setFocusPolicy(Qt::StrongFocus);
//        m_resultView->raise();
    } else {
        onFocusOut();
    }
}

void SearchEdit::onReturnPressed()
{
    auto text = QString(this->text()).remove(" ");
    if (text.isEmpty()) {
        return;
    }

//    onFocusOut();

    onTextChanged();
}

void SearchEdit::resizeEvent(QResizeEvent *event)
{
    RKSearchEdit::resizeEvent(event);
    m_topLeftToGlobal = mapToGlobal(this->geometry().topLeft());
}



} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer
