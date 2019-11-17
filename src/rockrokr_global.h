#ifndef ROCKROKR_GLOBAL_H
#define ROCKROKR_GLOBAL_H

#include <qglobal.h>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

inline int dpi_to_px(qint64 dip) {
    return dip;
}


#define MAIN_WINDOW_W                       dpi_to_px(1400)
#define MAIN_WINDOW_H                       dpi_to_px(/*800*/830)

#define HIGHLIGHT_BG_COLOR                  "#336CFB"
#define HIGHLIGHT_FONT_COLOR                "#F4F6FF"
#define LEFT_BAR_BG_COLOR                   "#FCFCFC"
#define MAIN_VIEW_BG_COLOR                  "#FFFFFF"
#define PLAY_BAR_BG_COLOR                   "#FDFDFD"
#define FONT_COLOR_TITLE                    "#1F2022"
#define FONT_COLOR_SECONDARY                "#BABBBB"

#define MAIN_WINDOW_TIP_H                   dpi_to_px(32)

/*
 * begin left bar
 */
#define LEFT_BAR_W                          dpi_to_px(240)
#define LEFT_BAR_H                          MAIN_WINDOW_H - dpi_to_px(85) // -PLAY_BAR_H
#define LEFT_BAR_CONTENT_BASE_MARGIN        dpi_to_px(20)
#define LEFT_BAR_CONTENT_LR_MARGIN          dpi_to_px(40)

#define LEFT_BAR_SCROLLBAR_W                dpi_to_px(10)

#define LEFT_BAR_BIG_TITLE_FONT_SIZE        dpi_to_px(22)
//#define LEFT_BAR_BT_LR_MARGIN               dpi_to_px(40) // big title left and right margin (240-160)/2
#define LEFT_BAR_BT_TB_MARGIN               dpi_to_px(20)

#define LEFT_BAR_FONT_SIZE_SECTION          dpi_to_px(14)
#define LEFT_BAR_FONT_SIZE_NORMAL           dpi_to_px(12)

#define LEFT_BAR_LISTVIEW_W                 (LEFT_BAR_W - LEFT_BAR_CONTENT_LR_MARGIN *2) //dpi_to_px(160)
#define LEFT_BAR_ITEM_W                     LEFT_BAR_LISTVIEW_W
#define LEFT_BAR_ITEM_H                     dpi_to_px(40)
//#define LEFT_BAR_LISTVIEW_LR_MARGIN         dpi_to_px(40) // left and right margin (240-160)/2
#define LEFT_BAR_LISTVIEW_SPACING           dpi_to_px(10)
#define LEFT_BAR_ITEM_CONTENT_MARGIN        dpi_to_px(7)
#define LEFT_BAR_ITEM_ICON_PART_W           dpi_to_px(20)
#define LEFT_BAR_ITEM_ICON_PART_H           LEFT_BAR_ITEM_ICON_PART_W
/*
 * end left bar
 */

#define RIGHT_PART_L_MARGIN                 dpi_to_px(80)  //left margin between leftbar and right part view

/*
 * begin play bar
 */
#define PLAY_BAR_W                          MAIN_WINDOW_W - LEFT_BAR_W
#define PLAY_BAR_H                          dpi_to_px(80)
#define PLAY_BAR_CONTENT_H_BASE_MARGIN      dpi_to_px(20)
#define PLAY_BAR_COVER_H                    PLAY_BAR_H
#define PLAY_BAR_COVER_W                    PLAY_BAR_COVER_H
#define PLAY_BAR_TITLE_W                    dpi_to_px(150)
#define PLAY_BAR_SLIDER_W                   dpi_to_px(420)
#define PLAY_BAR_SLIDER_H                   dpi_to_px(30)
#define PLAY_BAR_TIME_LABEL_W               dpi_to_px(50)
#define PLAY_BAR_SMALL_BTN_W                dpi_to_px(30)
#define PLAY_BAR_SMALL_BTN_H                PLAY_BAR_SMALL_BTN_W
#define PLAY_BAR_BIG_BTN_W                  dpi_to_px(50)
#define PLAY_BAR_BIG_BTN_H                  PLAY_BAR_BIG_BTN_W


/*
 * end play bar
 */

/*
 * begin title bar
 */
#define TITLE_BAR_W                         MAIN_WINDOW_H - LEFT_BAR_W
#define TITLE_BAR_H                         dpi_to_px(90)
#define TITIE_BAR_ICON_H                    dpi_to_px(48)
#define TITLE_BAR_SEARCH_VIEW_W             dpi_to_px(350)
#define TITLE_BAR_SEARCH_VIEW_H             dpi_to_px(48)
/*
 * end title bar
 */

/*
 * begin main view
 */
/******************** common ***************/
#define MAIN_VIEW_LEFT_MARGIN               dpi_to_px(80)
#define MAIN_VIEW_FONT_SIZE_TITLE           dpi_to_px(28)
//#define MAIN_VIEW_FONT_SIZE_PRIMARY         dpi_to_px(12)
//#define MAIN_VIEW_FONT_SIZE_SECONDARY       dpi_to_px(10)
#define MAIN_VIEW_TITLE_BAR_H               dpi_to_px(56) //maybe bigger than MAIN_VIEW_FONT_SIZE_TITLE
#define MAIN_VIEW_TITLE_BAR_ICON_H          dpi_to_px(32)
#define MAIN_VIEW_TITLE_BAR_FONT_COLOR      "#1F2022"
#define MAIN_VIEW_SCROLLBAR_SIZE            dpi_to_px(8);

// buttom left part vertical listview
#define MAIN_VIEW_V_LISTVIEW_W              dpi_to_px(500) //vertical LIST VIEW PART
#define MAIN_VIEW_LISTITEM_H                dpi_to_px(58)
#define MAIN_VIEW_LISTITEM_MARGIN           dpi_to_px(6)  //margin for listitems
#define MAIN_VIEW_LISTITEM_CONTENT_MARGIN   dpi_to_px(10)  //margin for content in listitem
#define MAIN_VIEW_LISTITEM_ICON_PART_W      MAIN_VIEW_LISTITEM_H
#define MAIN_VIEW_LISTITEM_ICON_PART_ICON_W dpi_to_px(40) //40 is ok for MAIN_VIEW_LISTITEM_CONTENT_MARGIN
//#define MAIN_VIEW_LISTITEM_ICON_H           dpi_to_px(42)
//#define MAIN_VIEW_LISTITEM_ICON_L_MARGIN    dpi_to_px(10)
//#define MAIN_VIEW_LISTITEM_CONTENT_MARGIN   dpi_to_px(20)
#define MAIN_VIEW_LISTITEM_LEFT_CONTENT_W   dpi_to_px(200) //160 OR 150
#define MAIN_VIEW_LISTITEM_MIDDLE_CONTENT_W dpi_to_px(140)
#define MAIN_VIEW_LISTITEM_RIGHT_CONTENT_W  dpi_to_px(40) //or less?

// horizontal listview
//#define MAIN_VIEW_HL_COVER_ITEM_MARGIN         dpi_to_px(14) //margin for horizontal listitems
//#define MAIN_VIEW_HL_COVER_ITEM_CONTENT_MARGIN dpi_to_px(6) // (154-142)/2
#define MAIN_VIEW_HL_VIEW_H                    dpi_to_px(210)
#define MAIN_VIEW_HL_VIEW_W                    dpi_to_px(1080)
#define MAIN_VIEW_HL_SPACING                   dpi_to_px(30)
#define MAIN_VIEW_HL_COVER_W                   dpi_to_px(141)
#define MAIN_VIEW_HL_COVER_H                   dpi_to_px(190)
//#define MAIN_VIEW_HL_COVER_ICON_W              dpi_to_px(137)
//#define MAIN_VIEW_HL_COVER_ICON_H              MAIN_VIEW_HL_COVER_ICON_W
#define MAIN_VIEW_HL_COVER_FONT_SIZE_PRIMARY   dpi_to_px(16)
#define MAIN_VIEW_HL_COVER_FONT_SIZE_SECONDARY dpi_to_px(12)
#define MAIN_VIEW_HL_COVER_FONT_COLOR          "#19202C"


#define MAIN_VIEW_R_VIEW_SPACING                dpi_to_px(30)
#define MAIN_VIEW_R_VIEW_COVER_W                dpi_to_px(141)
#define MAIN_VIEW_R_VIEW_COVER_H                dpi_to_px(130)
//#define MAIN_VIEW_R_VIEW_COVER_ICON_W           MAIN_VIEW_R_VIEW_COVER_W
#define MAIN_VIEW_R_VIEW_COVER_ICON_H           dpi_to_px(85)
#define MAIN_VIEW_R_VIEW_FONT_SIZE_PRIMARY      dpi_to_px(16)
#define MAIN_VIEW_R_VIEW_FONT_SIZE_SECONDARY    dpi_to_px(11)

/*
 * end main view
 */

#define MUSIC_LIST_ITEM_DURATION_R_MARGIN   dpi_to_px(30)


} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // ROCKROKR_GLOBAL_H
