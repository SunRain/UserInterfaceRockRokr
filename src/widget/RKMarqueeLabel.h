#ifndef TTKMARQUEELABEL_H
#define TTKMARQUEELABEL_H

/* =================================================
 * This file is part of the TTK Widget Tools project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QtGlobal>
#include <QFrame>
#include <QColor>

#include "rockrokr_global.h"

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

/*!
* @author Greedysky <greedysky@163.com>
* 2020 - SunRain <41245110@qq.com>
*/
class RKMarqueeLabel : public QFrame
{
    Q_OBJECT
    Q_ENUMS(MoveStyle)
    Q_ENUMS(MarqueeStyle)
public:
    enum MoveStyle {
        LeftAndRight = 0,
        LeftToRight = 1,
        RightToLeft = 2
    };

    enum MarqueeStyle {
        MarqueeOnHover = 0x0,
        StopOnHover
    };

    explicit RKMarqueeLabel(QWidget *parent = Q_NULLPTR);
    virtual ~RKMarqueeLabel() override;

    void setText(const QString &text);

    void setInterval(int interval);

    void setTextColor(const QColor &color);

    void setHoverTextColor(const QString &color);

    void setBackgroundColor(const QColor &color);

    void setHoverBackgroundColor(const QColor &color);

    void setMoveStyle(MoveStyle moveStyle);

    void setMarqueeStyle(MarqueeStyle style);

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private:
    void timeout();
    void setMarqueeStyleText();
    void setStopStyleText();

private:
    QTimer *m_timer = Q_NULLPTR;

    int m_interval      = 350;
    int m_startIdx      = 0; // >0 -> leftToRight, <0 -> rightToLeft
    int m_marqueeCnt    = 0; //count for marquee loop
    int m_drawLength    = 0; //displayed text length
    int m_drawYPos      = 0;

    bool m_hoverState = false;

    MoveStyle m_moveStyle       = RightToLeft;
    MarqueeStyle m_marqueeStyle = MarqueeOnHover;

    QString m_text;

    QColor m_textColor      = QColor(FONT_COLOR_TITLE);
    QColor m_hoverTextColor = QColor(HIGHLIGHT_FONT_COLOR);
    QColor m_bgColor        = QColor(Qt::transparent);
    QColor m_hoverBgColor   = QColor(HIGHLIGHT_BG_COLOR);
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // TTKMARQUEELABEL_H
