#ifndef TRACKINFODIALOG_H
#define TRACKINFODIALOG_H

#include <ddialog.h>

#include <AudioMetaObject.h>

class QBoxLayout;

namespace PhoenixPlayer {
namespace UserInterface {
namespace RockRokr {

class TrackInfoDialog : public DTK_WIDGET_NAMESPACE::DAbstractDialog
{
    Q_OBJECT
public:
    explicit TrackInfoDialog(const PhoenixPlayer::AudioMetaObject &obj, QWidget *parent = Q_NULLPTR);
    ~TrackInfoDialog() override;

protected:
    void createInfoLabel(QBoxLayout *parent, const QString &key, const QString &vale);


};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer


#endif // TRACKINFODIALOG_H
