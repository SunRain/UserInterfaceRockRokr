#include "RKNavigationPage.h"

#include <QDebug>

#include <DThemeManager>
#include <dimagebutton.h>
#include <dwindowclosebutton.h>
#include <dwindowminbutton.h>
#include <dwindowoptionbutton.h>

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

RKNavigationPage::RKNavigationPage(QWidget *parent)
    : QFrame(parent)
{

    m_optionBtn = new DWindowOptionButton;
    connect(m_optionBtn, &DImageButton::clicked,
            this, [&](){
        qDebug()<<" option clicked ";
    });

    m_closeBtn = new DWindowCloseButton;
    connect(m_closeBtn, &DImageButton::clicked, this, [&] () {
//        UserInterface::UserInterfaceMgr mgr;
//        UserInterface::IUserInterface *us = mgr.usedInterface();
//        if (us) {
//            us->close();
//        }
    });

    m_minimizeBtn = new DWindowMinButton;
    connect(m_minimizeBtn, &DImageButton::clicked,
            this, [&](){
        qDebug()<<" m_minimizeBtn clicked ";
    });

    initUserInterface();
}

RKNavigationPage::~RKNavigationPage()
{

}

void RKNavigationPage::initUserInterface()
{

}



} // namespace RockRokr
} // namespace UserInterface
} // namespace PhoenixPlayer
