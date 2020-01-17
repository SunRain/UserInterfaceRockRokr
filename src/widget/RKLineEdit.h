#ifndef RKLINEEDIT_H
#define RKLINEEDIT_H

#include <QLineEdit>
#include <QPropertyAnimation>

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class RKLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)

public:
    explicit RKLineEdit(QWidget *parent = Q_NULLPTR);
    virtual ~RKLineEdit() override;

    QColor lineColor() const;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void lineColorChanged(const QColor &lineColor);

public Q_SLOTS:
    void setLineColor(const QColor &lineColor);

private:
    QPropertyAnimation *m_animation     = Q_NULLPTR;
    float               m_currentValue  = 0;
    QColor              m_lineColor;
};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // RKLINEEDIT_H
