#ifndef ROCKROKRVIEW_H
#define ROCKROKRVIEW_H

#include <QFrame>

class LeftBar;
class RKTitleBar;
class PlayBar;
class RockRokrView : public QFrame
{
    Q_OBJECT
public:
    explicit RockRokrView(QWidget *parent = nullptr);
    virtual ~RockRokrView() override;

private:
    LeftBar             *m_leftbar;
    RKTitleBar          *m_titlebar;
    PlayBar             *m_playbar;
};

#endif // ROCKROKRVIEW_H
