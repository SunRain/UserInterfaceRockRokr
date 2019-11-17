#ifndef RKTABLEHEADERITEM_H
#define RKTABLEHEADERITEM_H

#include <QFrame>
#include <QPixmap>
#include <QMap>
#include <QtGlobal>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKTableHeaderItem : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(QColor hoverBorderColor READ hoverBorderColor WRITE setHoverBorderColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor hoverBGColor READ hoverBGColor WRITE setHoverBGColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor hoverTextColor READ hoverTextColor WRITE setHoverTextColor)
    Q_PROPERTY(QColor selectedBorderColor READ selectedBorderColor WRITE setSetlectedBorderColor)
    Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
    Q_PROPERTY(QColor selectedBGColor READ selectedBGColor WRITE setSelectedBGColor)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)

public:
    enum ArrowIconDirection
    {
        DirectionUp = 0x0,
        DirectionDown,
        DirectionUndefined
    };

    ///
    /// \brief The HeaderColumn enum
    /// Columns render order HeaderCover -> HeaderTitle -> HeaderArtist -> HeaderAlbum -> HeaderDuration
    ///
    enum HeaderColumn
    {
        HeaderCover = 0x1,
        HeaderTitle = 0x2,
        HeaderArtist = 0x4,
        HeaderAlbum = 0x8,
        HeaderDuration = 0x10,
        HeaderUndefined = 0x0
    };
    Q_DECLARE_FLAGS(HeaderColumns, HeaderColumn)
    Q_FLAG(HeaderColumns)

    explicit RKTableHeaderItem(ArrowIconDirection direction = DirectionDown, QWidget *parent = Q_NULLPTR);
    virtual ~RKTableHeaderItem() override;

    void setDisplayedColumns(HeaderColumns colums);

    void setDisplayAllColumns();

    void setDefaultArrowDirection(HeaderColumn column, ArrowIconDirection direction);

    QMap<HeaderColumn, int> displayedColumnsWidth() const;

    int radius() const;

    QColor borderColor() const;

    QColor hoverBorderColor() const;

    QColor backgroundColor() const;

    QColor hoverBGColor() const;

    QColor textColor() const;

    QColor hoverTextColor() const;

    int fontSize() const;

    // QWidget interface
    bool enableClick() const;
    void setEnableClick(bool enableClick);

    bool enableArrowIcon() const;
    void setEnableArrowIcon(bool enableArrowIcon);

//    QString text() const;
//    void setText(const QString &text);

    void setCoverCloumnText(const QString &text,
                           Qt::Alignment flag = Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignHCenter);

    void setCoverColumnIcon(const QPixmap &normalIcon, const QPixmap &hoverIcon);

    void setCoverColumnWidth(int width);

    void setCoverColumnToolTip(const QString &tip);

    QColor selectedBorderColor() const;

    QColor selectedTextColor() const;

    QColor selectedBGColor() const;

//    // QObject interface
//public:
//    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void calcColumnsWidth();
    void drawCover(QPainter *painter, const QRectF &rect);
    void drawTitle(QPainter *painter, const QRectF &rect);
    void drawArtist(QPainter *painter, const QRectF &rect);
    void drawAlbum(QPainter *painter, const QRectF &rect);
    void drawDuration(QPainter *painter, const QRectF &rect);
    HeaderColumn getFocusedColumn(QMouseEvent *event);

Q_SIGNALS:
    void clicked(HeaderColumn column, ArrowIconDirection direction);
    void columnsWidthChanged(const QMap<HeaderColumn, int> &map);

public Q_SLOTS:
    void setRadius(int radius);

    void setBorderColor(const QColor &borderColor);

    void setHoverBorderColor(const QColor &hoverBorderColor);

    void setBackgroundColor(const QColor &backgroundColor);

    void setHoverBGColor(const QColor &hoverBGColor);

    void setTextColor(const QColor &textColor);

    void setHoverTextColor(const QColor &hoverTextColor);

    void setFontSize(int fontSize);

    void setSetlectedBorderColor(const QColor &selectedBorderColor);

    void setSelectedTextColor(const QColor &selectedTextColor);

    void setSelectedBGColor(const QColor &selectedBGColor);

private:
    int m_radius;
    int m_fontSize;
//    bool m_hoverState;
    HeaderColumn m_hoverColumn;
    HeaderColumn m_clickedColumn;
    bool m_enableClick;
    bool m_enableArrowIcon;
//    HeaderColum m_HeaderColums;
    QMap<HeaderColumn, int> m_displayedColumn; //displayed colum with its width;
    QMap<HeaderColumn, ArrowIconDirection> m_arrowMap;
//    QString m_text;
    int m_coverWidth;
    QString m_coverText;
    QString m_toolTipText;
    Qt::Alignment m_coverTextAlignment;

//    ArrowIconDirection m_arrowDirection;
    QColor m_borderColor;
    QColor m_hoverBorderColor;
    QColor m_backgroundColor;
    QColor m_hoverBGColor;
    QColor m_textColor;
    QColor m_hoverTextColor;
    QPixmap m_arrowUpLightNormalImage;
    QPixmap m_arrowUpLightHoverImage;
    QPixmap m_arrowUpLightPressImage;
    QPixmap m_arrowDownLightNormalImage;
    QPixmap m_arrowDownLightHoverImage;
    QPixmap m_arrowDownLightPressImage;
    QPixmap m_coverNormalImage;
    QPixmap m_coverHoverImage;
    QColor m_selectedBorderColor;
    QColor m_selectedTextColor;
    QColor m_selectedBGColor;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


Q_DECLARE_OPERATORS_FOR_FLAGS(PhoenixPlayer::UserInterface::RockRokr::RKTableHeaderItem::HeaderColumns)

#endif // RKTABLEHEADERITEM_H
