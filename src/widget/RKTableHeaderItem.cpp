#include "RKTableHeaderItem.h"

#include <QDebug>
#include <DHiDPIHelper>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include "UserInterface/UserInterfaceMgr.h"

#include "rockrokr_global.h"
#include "RKMainWindow.h"
#include "UserInterfaceRockRokr.h"

DWIDGET_USE_NAMESPACE

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

#define SET_BG(p, isHover, isClicked, rect) \
    if (isHover) { \
        p->setPen(m_hoverBorderColor); \
        p->setBrush(m_hoverBGColor); \
    } else if (isClicked) {\
        p->setPen(m_selectedBorderColor); \
        p->setBrush(m_selectedBGColor); \
    } else { \
        p->setPen(m_borderColor); \
        p->setBrush(m_backgroundColor); \
    } \
    p->drawRoundRect(rect, 4, 4);


#define SET_TEXT_PAINTER(p, isHover, isClicked) \
    if (isHover) { \
        p->setPen(m_hoverTextColor); \
        p->setBrush(m_hoverTextColor); \
    } else if (isClicked) {\
        p->setPen(m_selectedTextColor); \
        p->setBrush(m_selectedTextColor); \
    } else { \
        p->setPen(m_textColor); \
        p->setBrush(m_textColor); \
    }

#define DRAW_ARROW_ICON(painter, rect, textW, isHover, column) \
    const qreal iconH = rect.height() - dpi_to_px(4); \
    QPixmap icon; \
    if (isHover) { \
        icon = (m_arrowMap.value(column) == DirectionUp \
                ? m_arrowUpLightHoverImage \
                : m_arrowDownLightHoverImage); \
    } else { \
        icon = (m_arrowMap.value(column) == DirectionUp \
                    ? m_arrowUpLightNormalImage \
                    : m_arrowDownLightNormalImage);  \
    } \
    if (icon.height() > iconH) { \
        qreal i = iconH/icon.height(); \
        const qreal w = icon.width() * i; \
        const QSize iconSize(w, iconH); \
        icon = icon.scaled(iconSize, Qt::KeepAspectRatio); \
    } \
    textW = textW - icon.width() - dpi_to_px(16); \
    const qreal iconX = rect.x() + textW + dpi_to_px(8); \
    const qreal iconY = rect.y() + (rect.height() - icon.height())/2; \
    painter->drawPixmap(iconX, iconY, icon);

#define DRAW_TEXT(painter, text, textW, rect) \
    QFont f = this->font(); \
    f.setPixelSize(m_fontSize); \
    QFontMetrics fm(f); \
    const QString str = fm.elidedText(text, Qt::ElideRight, textW - dpi_to_px(4)); \
    qreal textY = (rect.height() - m_fontSize)/2; \
    const QRectF tf(rect.x() + dpi_to_px(2), textY, textW - dpi_to_px(4), m_fontSize); \
    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);


RKTableHeaderItem::RKTableHeaderItem(RKTableHeaderItem::ArrowIconDirection direction, QWidget *parent)
    : QWidget(parent)
{
    this->installEventFilter(this);
    this->setMouseTracking(true);

    m_radius = 0;
    m_fontSize = dpi_to_px(12);
    m_coverWidth = dpi_to_px(60);
    m_coverTextAlignment = Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignHCenter;
//    m_hoverState = false;
    m_enableClick = true;
    m_enableArrowIcon = true;
    m_borderColor = QColor(233, 233, 233);//Qt::GlobalColor::transparent;//QColor("#19000000");
    m_hoverBorderColor = QColor("#336CFB");
    m_backgroundColor = Qt::GlobalColor::transparent;//QColor("#FFFFFF");
    m_hoverBGColor = QColor("#336CFB");
    m_textColor = QColor("#19202C");
    m_hoverTextColor = QColor("#FFFFFF");

    m_selectedBorderColor = QColor(233, 233, 233); //#E9E9E9
    m_selectedTextColor = QColor("#19202C");//QColor(233, 233, 233);
    m_selectedBGColor = QColor(233, 233, 233);

    m_arrowUpLightNormalImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_up_normal.svg");
    m_arrowUpLightHoverImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_up_hover.svg");
    m_arrowUpLightPressImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_up_press.svg");
    m_arrowDownLightNormalImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_down_normal.svg");
    m_arrowDownLightHoverImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_down_hover.svg");
    m_arrowDownLightPressImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_down_press.svg");

    m_hoverColumn = HeaderUndefined;
    m_clickedColumn = HeaderUndefined;
    m_arrowMap.insert(HeaderAlbum, DirectionUndefined);
    m_arrowMap.insert(HeaderTitle, DirectionUndefined);
    m_arrowMap.insert(HeaderArtist, DirectionUndefined);
}

RKTableHeaderItem::~RKTableHeaderItem()
{

}

void RKTableHeaderItem::setDisplayedColumns(HeaderColumns colums)
{
//    m_HeaderColums = colums;
    m_displayedColumn.clear();
    if ((colums & HeaderCover) == HeaderCover) {
        m_displayedColumn.insert(HeaderCover, m_coverWidth);
    }
    if ((colums & HeaderAlbum) == HeaderAlbum) {
        m_displayedColumn.insert(HeaderAlbum, -1);
    }
    if ((colums & HeaderTitle) == HeaderTitle) {
        m_displayedColumn.insert(HeaderTitle, -1);
    }
    if ((colums & HeaderArtist) == HeaderArtist) {
        m_displayedColumn.insert(HeaderArtist, -1);
    }
    if ((colums & HeaderDuration) == HeaderDuration) {
        m_displayedColumn.insert(HeaderDuration, -1);
    }
    calcColumnsWidth();
    this->update();
}

void RKTableHeaderItem::setDisplayAllColumns()
{
    setDisplayedColumns(RKTableHeaderItem::HeaderColumn::HeaderAlbum |
                        RKTableHeaderItem::HeaderColumn::HeaderCover |
                        RKTableHeaderItem::HeaderColumn::HeaderTitle |
                        RKTableHeaderItem::HeaderColumn::HeaderArtist |
                        RKTableHeaderItem::HeaderColumn::HeaderDuration);
}

void RKTableHeaderItem::setDefaultArrowDirection(RKTableHeaderItem::HeaderColumn column, RKTableHeaderItem::ArrowIconDirection direction)
{
    m_arrowMap.insert(column, direction);
}

QMap<RKTableHeaderItem::HeaderColumn, int> RKTableHeaderItem::displayedColumnsWidth() const
{
    return m_displayedColumn;
}

int RKTableHeaderItem::radius() const
{
    return m_radius;
}

QColor RKTableHeaderItem::borderColor() const
{
    return m_borderColor;
}

QColor RKTableHeaderItem::hoverBorderColor() const
{
    return m_hoverBorderColor;
}

QColor RKTableHeaderItem::backgroundColor() const
{
    return m_backgroundColor;
}

QColor RKTableHeaderItem::hoverBGColor() const
{
    return m_hoverBGColor;
}

QColor RKTableHeaderItem::textColor() const
{
    return m_textColor;
}

QColor RKTableHeaderItem::hoverTextColor() const
{
    return m_hoverTextColor;
}

int RKTableHeaderItem::fontSize() const
{
    return m_fontSize;
}

void RKTableHeaderItem::mousePressEvent(QMouseEvent *event)
{
    if (!m_enableClick) {
        return;
    }
    if (event->button() == Qt::MouseButton::LeftButton) {
        HeaderColumn c = getFocusedColumn(event);
        if (m_arrowMap.contains(c)) {
            m_clickedColumn = c;
            //set clicked column direction
            const ArrowIconDirection a = m_arrowMap.value(c);
            if (a == DirectionDown) {
                m_arrowMap.insert(c, DirectionUp);
            } else {
                m_arrowMap.insert(c, DirectionDown);
            }
            //set other column direction to Undefined
            foreach (HeaderColumn h, m_arrowMap.keys()) {
                if (h == c) {
                    continue;
                }
                m_arrowMap.insert(h, DirectionUndefined);
            }
        } else {
            m_clickedColumn = HeaderUndefined;
        }
        this->update();
//        qDebug()<<Q_FUNC_INFO<<" -- "<<event
//                 <<"  "<<c<<"  "<<m_arrowMap.value(c);
        emit clicked(c, m_arrowMap.value(c));
    }
}

void RKTableHeaderItem::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

static bool s_mouse_keep_in_cover = false;
static RKMainWindow *s_mainWindow = Q_NULLPTR;
void RKTableHeaderItem::mouseMoveEvent(QMouseEvent *event)
{
    m_hoverColumn = getFocusedColumn(event);
    if (m_hoverColumn == HeaderCover && !m_toolTipText.isEmpty()) {
        const int x = this->rect().x() + m_displayedColumn.value(HeaderCover)/2;
        const int y = this->rect().y() + this->rect().height()/2;
        const QPoint pos = this->mapToGlobal(QPoint(x, y));
        if (!s_mouse_keep_in_cover) {
            if (Q_UNLIKELY(!s_mainWindow)) {
                UserInterfaceMgr mgr;
                UserInterfaceRockRokr *rk = qobject_cast<UserInterfaceRockRokr*>(mgr.usedInterface());
                if (rk) {
                    s_mainWindow = rk->mainWindow();
                }
            }
            s_mainWindow->showTip(pos, m_toolTipText);
            s_mouse_keep_in_cover = true;
        }
    } else if (!m_toolTipText.isEmpty()) {
        if (Q_UNLIKELY(!s_mainWindow)) {
            UserInterfaceMgr mgr;
            UserInterfaceRockRokr *rk = qobject_cast<UserInterfaceRockRokr*>(mgr.usedInterface());
            if (rk) {
                s_mainWindow = rk->mainWindow();
            }
        }
        s_mainWindow->hideTip(1500);
        s_mouse_keep_in_cover = false;
    }
    this->update();
}

void RKTableHeaderItem::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
//    qDebug()<<Q_FUNC_INFO<<"  m_hoverColumn "<<m_hoverColumn;
//    if (m_hoverColumn == HeaderCover && !m_toolTipText.isEmpty()) {
//        qDebug()<<Q_FUNC_INFO<<" show tool tip "<<m_toolTipText;
//        RockRokrApp::instance()->mainWindow()->showTip(this, m_toolTipText);
//    }
    this->update();
}

void RKTableHeaderItem::leaveEvent(QEvent *event)
{
//    if (m_hoverColumn == HeaderCover && !m_toolTipText.isEmpty()) {
//        qDebug()<<Q_FUNC_INFO<<" hide tool tip "<<m_toolTipText;
//        RockRokrApp::instance()->mainWindow()->hideTip(1500);
//    }
    m_hoverColumn = HeaderUndefined;
    this->update();
}

void RKTableHeaderItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                            QPainter::HighQualityAntialiasing |
                            QPainter::SmoothPixmapTransform);
    qreal rectX = this->rect().x();

//    painter.fillRect(rect(), Qt::GlobalColor::green);

     if (m_displayedColumn.contains(HeaderCover)) {
        const int w = m_displayedColumn.value(HeaderCover);
        QRectF rf(0, 0, w, this->rect().height());
        drawCover(&painter, rf);
        rectX += w;
    }
    if (m_displayedColumn.contains(HeaderTitle)) {
        const int w = m_displayedColumn.value(HeaderTitle);
        QRectF rf(rectX, 0, w, this->rect().height());
        drawTitle(&painter, rf);
        rectX += w;
    }
    if (m_displayedColumn.contains(HeaderArtist)) {
        const int w = m_displayedColumn.value(HeaderArtist);
        QRectF rf(rectX, 0, w, this->rect().height());
        drawArtist(&painter, rf);
        rectX += w;
    }
    if (m_displayedColumn.contains(HeaderAlbum)) {
        const int w = m_displayedColumn.value(HeaderAlbum);
        QRectF rf(rectX, 0, w, this->rect().height());
        drawAlbum(&painter, rf);
        rectX += w;
    }
    if (m_displayedColumn.contains(HeaderDuration)) {
        const int w = m_displayedColumn.value(HeaderDuration);
        QRectF rf(rectX, 0, w, this->rect().height());
        drawDuration(&painter, rf);
        rectX += w;
    }
}

void RKTableHeaderItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calcColumnsWidth();
    this->update();
}

QSize RKTableHeaderItem::sizeHint() const
{
    return QSize(dpi_to_px(1000), dpi_to_px(60));
}

void RKTableHeaderItem::calcColumnsWidth()
{
    int rectWidth = this->rect().width();
    int keySize = m_displayedColumn.size();
    if (m_displayedColumn.contains(HeaderCover)) {
        m_displayedColumn.insert(HeaderCover, m_coverWidth);
        rectWidth -= m_coverWidth;
        --keySize;
    }
    if (m_displayedColumn.contains(HeaderDuration)) {
        QFont f(this->font());
        f.setPixelSize(m_fontSize);
        QFontMetrics fm(f);
        // pixel2point p *96/72
        const int w = fm.width("00:00") *96/72 + MUSIC_LIST_ITEM_DURATION_R_MARGIN;
        m_displayedColumn.insert(HeaderDuration, w);
        rectWidth -= w;
        --keySize;
    }
    if (m_displayedColumn.contains(HeaderTitle)) {
        if (keySize == 1) { //only header title
            m_displayedColumn.insert(HeaderTitle, rectWidth);
        } else if (keySize == 2) {
            const int w = rectWidth *3/5;
            m_displayedColumn.insert(HeaderTitle, w);
            rectWidth -= w;
        } else {
            const int w = rectWidth /2;
            m_displayedColumn.insert(HeaderTitle, w);
            rectWidth -= w;
        }
        --keySize;
    }
    if (m_displayedColumn.contains(HeaderAlbum)) {
        if (keySize == 1) {
            m_displayedColumn.insert(HeaderAlbum, rectWidth);
        } else {
            m_displayedColumn.insert(HeaderAlbum, rectWidth/2);
            rectWidth = rectWidth/2;
        }
        --keySize;
    }
    if (m_displayedColumn.contains(HeaderArtist)) {
        if (keySize == 1) {
            m_displayedColumn.insert(HeaderArtist, rectWidth);
        } else {
            m_displayedColumn.insert(HeaderArtist, rectWidth/2);
            rectWidth = rectWidth/2;
        }
        --keySize;
    }
    emit columnsWidthChanged(m_displayedColumn);
}

void RKTableHeaderItem::drawCover(QPainter *painter, const QRectF &rect)
{
    const bool isHover = m_hoverColumn == HeaderCover;
    SET_BG(painter, m_hoverColumn == HeaderCover, false, rect)

    QPixmap icon;
    if (isHover && !m_coverHoverImage.isNull()) {
        icon = m_coverHoverImage;
    } else {
        icon = m_coverNormalImage;
    }
    if (!icon.isNull()) {
        const qreal minSize = qMin(qMin(rect.width(), rect.height()),
                                   qMin((qreal)icon.width(), (qreal)icon.height()));
        const qreal ratio = (qreal)qMax(icon.width(), icon.height()) / minSize;
        const QSize coverSz((qreal)icon.width()/ratio, (qreal)icon.height()/ratio);
        const QPixmap pixmap = icon.scaled(coverSz, Qt::KeepAspectRatio);
        const qreal xpos = rect.x() + (rect.width() - pixmap.width())/2;
        const qreal ypos = rect.y() + (rect.height() - pixmap.height())/2;
        painter->drawPixmap(xpos, ypos, pixmap);
    }
    if (!m_coverText.isEmpty()) {
        SET_TEXT_PAINTER(painter, isHover, false)
        QFont f = this->font();
        f.setPixelSize(m_fontSize);
        QFontMetrics fm(f);
        const QString str = fm.elidedText(m_coverText, Qt::ElideRight, rect.width());
        qreal textY = (rect.height() - m_fontSize)/2;
        const QRectF tf(rect.x() + dpi_to_px(2), textY, rect.width() - dpi_to_px(4), m_fontSize);
        painter->drawText(tf, m_coverTextAlignment, str);
    }
}

void RKTableHeaderItem::drawTitle(QPainter *painter, const QRectF &rect)
{
    const bool isHover = (m_hoverColumn == HeaderTitle);
    const bool isClicked = (m_clickedColumn == HeaderTitle);

    SET_BG(painter, isHover, isClicked, rect)

    qreal textW = rect.width();
    if (m_enableArrowIcon && (m_arrowMap.value(HeaderTitle) != DirectionUndefined)) {
        DRAW_ARROW_ICON(painter, rect, textW, isHover, HeaderTitle)
    }

    SET_TEXT_PAINTER(painter, isHover, isClicked)
//    QFont f = this->font();
//    f.setPixelSize(m_fontSize);
//    QFontMetrics fm(f);
//    const QString str = fm.elidedText(tr("Title"), Qt::ElideRight, textW - dpi_to_px(4));
//    qreal textY = (rect.height() - m_fontSize)/2;
//    const QRectF tf(rect.x() + dpi_to_px(2), textY, textW - dpi_to_px(4), m_fontSize);
//    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
    DRAW_TEXT(painter, tr("Title"), textW, rect)
}

void RKTableHeaderItem::drawArtist(QPainter *painter, const QRectF &rect)
{
    const bool isHover = (m_hoverColumn == HeaderArtist);
    const bool isClicked = (m_clickedColumn == HeaderArtist);

    SET_BG(painter, isHover, isClicked, rect)

    qreal textW = rect.width();
    if (m_enableArrowIcon && (m_arrowMap.value(HeaderArtist) != DirectionUndefined)) {
        DRAW_ARROW_ICON(painter, rect, textW, isHover, HeaderArtist)
    }

    SET_TEXT_PAINTER(painter, isHover, isClicked)
//    QFont f = this->font();
//    f.setPixelSize(m_fontSize);
//    QFontMetrics fm(f);
//    const QString str = fm.elidedText(tr("Artist"), Qt::ElideRight, textW);
//    qreal textY = (rect.height() - m_fontSize)/2;
//    const QRectF tf(rect.x(), textY, textW, m_fontSize);
//    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
    DRAW_TEXT(painter, tr("Artist"), textW, rect)
}

void RKTableHeaderItem::drawAlbum(QPainter *painter, const QRectF &rect)
{
    const bool isHover = (m_hoverColumn == HeaderAlbum);
    const bool isClicked = (m_clickedColumn == HeaderAlbum);

    SET_BG(painter, isHover, isClicked, rect)

    qreal textW = rect.width();
    if (m_enableArrowIcon && (m_arrowMap.value(HeaderAlbum) != DirectionUndefined)) {
        DRAW_ARROW_ICON(painter, rect, textW, isHover, HeaderAlbum)
    }

    SET_TEXT_PAINTER(painter, isHover, isClicked)
//    QFont f = this->font();
//    f.setPixelSize(m_fontSize);
//    QFontMetrics fm(f);
//    const QString str = fm.elidedText(tr("Album"), Qt::ElideRight, textW);
//    qreal textY = (rect.height() - m_fontSize)/2;
//    const QRectF tf(rect.x(), textY, textW, m_fontSize);
//    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
    DRAW_TEXT(painter, tr("Album"), textW, rect)
}

void RKTableHeaderItem::drawDuration(QPainter *painter, const QRectF &rect)
{
    const bool isHover = (m_hoverColumn == HeaderDuration);

    SET_BG(painter, isHover, false, rect)

    qreal textW = rect.width();

    SET_TEXT_PAINTER(painter, isHover, false)
    QFont f = this->font();
    f.setPixelSize(m_fontSize);
    QFontMetrics fm(f);
    const QString str = fm.elidedText(tr("Duration"), Qt::ElideRight, textW);
    qreal textY = (rect.height() - m_fontSize)/2;
    const QRectF tf(rect.x()+dpi_to_px(2), textY, textW - dpi_to_px(4), m_fontSize);
    painter->drawText(tf, Qt::AlignLeft | Qt::AlignVCenter, str);
}

RKTableHeaderItem::HeaderColumn RKTableHeaderItem::getFocusedColumn(QMouseEvent *event)
{
    const int my = event->x();
    int starX = 0;
    int endX = 0;
    if (m_displayedColumn.contains(HeaderCover)) {
        endX += m_displayedColumn.value(HeaderCover);
        if (my > starX && my < endX) {
            return HeaderCover;
        }
        starX += m_displayedColumn.value(HeaderCover);
    }
    if (m_displayedColumn.contains(HeaderTitle)) {
        endX += m_displayedColumn.value(HeaderTitle);
        if (my > starX && my < endX) {
            return HeaderTitle;
        }
        starX += m_displayedColumn.value(HeaderTitle);
    }
    if (m_displayedColumn.contains(HeaderArtist)) {
        endX += m_displayedColumn.value(HeaderArtist);
        if (my > starX && my < endX) {
            return HeaderArtist;
        }
        starX += m_displayedColumn.value(HeaderArtist);
    }
    if (m_displayedColumn.contains(HeaderAlbum)) {
        endX += m_displayedColumn.value(HeaderAlbum);
        if (my > starX && my < endX) {
            return HeaderAlbum;
        }
        starX += m_displayedColumn.value(HeaderAlbum);
    }
    if (m_displayedColumn.contains(HeaderDuration)) {
        endX += m_displayedColumn.value(HeaderDuration);
        if (my > starX && my < endX) {
            return HeaderDuration;
        }
        starX += m_displayedColumn.value(HeaderDuration);
    }
    return HeaderUndefined;
}

void RKTableHeaderItem::setRadius(int radius)
{
    m_radius = radius;
    this->update();
}

void RKTableHeaderItem::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
    this->update();
}

void RKTableHeaderItem::setHoverBorderColor(const QColor &hoverBorderColor)
{
    m_hoverBorderColor = hoverBorderColor;
    this->update();
}

void RKTableHeaderItem::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    this->update();
}

void RKTableHeaderItem::setHoverBGColor(const QColor &hoverBGColor)
{
    m_hoverBGColor = hoverBGColor;
    this->update();
}

void RKTableHeaderItem::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
    this->update();
}

void RKTableHeaderItem::setHoverTextColor(const QColor &hoverTextColor)
{
    m_hoverTextColor = hoverTextColor;
    this->update();
}

void RKTableHeaderItem::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
    calcColumnsWidth();
    this->update();
}

void RKTableHeaderItem::setSetlectedBorderColor(const QColor &selectedBorderColor)
{
    m_selectedBorderColor = selectedBorderColor;
}

void RKTableHeaderItem::setSelectedTextColor(const QColor &selectedTextColor)
{
    m_selectedTextColor = selectedTextColor;
}

void RKTableHeaderItem::setSelectedBGColor(const QColor &selectedBGColor)
{
    m_selectedBGColor = selectedBGColor;
}

//QString RKTableHeaderItem::text() const
//{
//    return m_text;
//}

//void RKTableHeaderItem::setText(const QString &text)
//{
//    m_text = text;
//    this->update();
//}

bool RKTableHeaderItem::enableArrowIcon() const
{
    return m_enableArrowIcon;
}

void RKTableHeaderItem::setEnableArrowIcon(bool enableArrowIcon)
{
    m_enableArrowIcon = enableArrowIcon;
}

void RKTableHeaderItem::setCoverCloumnText(const QString &text, Qt::Alignment flag)
{
    m_coverText = text;
    this->update();
}

void RKTableHeaderItem::setCoverColumnIcon(const QPixmap &normalIcon, const QPixmap &hoverIcon)
{
    m_coverNormalImage = normalIcon;
    m_coverHoverImage = hoverIcon;
    this->update();
}

void RKTableHeaderItem::setCoverColumnWidth(int width)
{
    m_coverWidth = width;
    calcColumnsWidth();
    this->update();
}

void RKTableHeaderItem::setCoverColumnToolTip(const QString &tip)
{
    m_toolTipText = tip;
}

QColor RKTableHeaderItem::selectedBorderColor() const
{
    return m_selectedBorderColor;
}

QColor RKTableHeaderItem::selectedTextColor() const
{
    return m_selectedTextColor;
}

QColor RKTableHeaderItem::selectedBGColor() const
{
    return m_selectedBGColor;
}

//bool RKTableHeaderItem::eventFilter(QObject *watched, QEvent *event)
//{
//    switch (event->type()) {
//    case QEvent::Enter: {
//        auto w = qobject_cast<QWidget *>(watched);
//        qDebug()<<Q_FUNC_INFO<<" QEvent::Enter  m_hoverColumn "<<m_hoverColumn;
//        break;
//    }
//    case QEvent::Leave: {
//        auto w = qobject_cast<QWidget *>(watched);
//        qDebug()<<Q_FUNC_INFO<<"  QEvent::Leave  m_hoverColumn "<<m_hoverColumn;
//        break;
//    }
//    default:
//        break;
//    }
//    return QObject::eventFilter(watched, event);
//}

bool RKTableHeaderItem::enableClick() const
{
    return m_enableClick;
}

void RKTableHeaderItem::setEnableClick(bool enableClick)
{
    m_enableClick = enableClick;
}

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

