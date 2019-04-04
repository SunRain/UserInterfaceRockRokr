#ifndef RKSLIDER_H
#define RKSLIDER_H

#include <QSlider>

class RKBaseSlider : public QSlider
{
    Q_OBJECT
public:
    explicit RKBaseSlider(QWidget *parent = nullptr);
    explicit RKBaseSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

    // QObject interface
public:
    bool event(QEvent *event) override;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected:
    int m_value;
    bool m_mousePress;
};


class QLabel;
class RKSlider : public RKBaseSlider
{
    Q_OBJECT
public:
    explicit RKSlider(QWidget *parent = nullptr);
    explicit RKSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    virtual ~RKSlider() override;

    inline bool isMoving() const { return m_isMoving;}

    inline void setMoving(bool m) { m_isMoving = m;}

signals:
    void sliderReleasedAt(int pos);

public slots:
    void setValue(int value);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

protected:
    QPoint limitLableGeometry(int x, int y, int z);

private:
    bool m_isMoving;
    QLabel *m_textLabel;


};

#endif // RKSLIDER_H
