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

#include <QWidget>
#include <QLabel>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

/*!
* @author Greedysky <greedysky@163.com>
* 2020 - SunRain <41245110@qq.com>
*/
class RKMarqueeLabel : public QWidget
{
    Q_OBJECT
    Q_ENUMS(MoveStyle)
    Q_ENUMS(MarqueeStyle)
public:
    enum MoveStyle {
        MoveStyleLeftAndRight = 0,
        MoveStyleLeftToRight = 1,
        MoveStyleRightToLeft = 2
    };

    enum MarqueeStyle {
        MarqueeOnHover = 0x0,
        StopOnHover
    };

    explicit RKMarqueeLabel(QWidget *parent = Q_NULLPTR);
    virtual ~RKMarqueeLabel() override;

    void setText(const QString &text);
    void setStep(int step);
    void setInterval(int interval);

    void setForeground(const QColor &foreground);
    void setBackground(const QColor &background);

    void setMoveStyle(MoveStyle moveStyle);

    void setMarqueeStyle(MarqueeStyle style);

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
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
    QLabel *m_labText   = Q_NULLPTR;
    QTimer *m_timer     = Q_NULLPTR;

    int m_step          = 2;
    int m_interval      = 20;
    int m_initX         = 0;
    int m_initY         = 0;

    MoveStyle m_moveStyle = MoveStyleLeftAndRight;
    MarqueeStyle m_marqueeStyle = MarqueeOnHover;

    QString m_text;
    QColor  m_foreground;
    QColor  m_background;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // TTKMARQUEELABEL_H
