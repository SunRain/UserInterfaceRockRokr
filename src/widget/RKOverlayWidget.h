#ifndef RKOVERLAYWIDGET_H
#define RKOVERLAYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QVBoxLayout>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKOverlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RKOverlayWidget(QWidget *parent = Q_NULLPTR);
    virtual ~RKOverlayWidget() override;

    void setBackgroundPixmap(const QPixmap &pixmap);

    /*!
     * \brief addContent This will set content's parent to RKOverlayWidget
     * \param content
     * \param flag AlignmentFlag for how content added to parent widget.
     * Not parent layout is QVBoxLayout.
     */
    void addContent(QWidget *content, Qt::AlignmentFlag flag = Qt::AlignCenter);

signals:
    void mousePressOutsideContent();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget         *m_backgroundWidget = Q_NULLPTR;
    QWidget         *m_content = Q_NULLPTR;
    QVBoxLayout     *m_contentLayout = Q_NULLPTR;
    QPixmap         m_bgPixmap;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKOVERLAYWIDGET_H
