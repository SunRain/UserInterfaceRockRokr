#ifndef SEARCHRESULTPOPUP_H
#define SEARCHRESULTPOPUP_H

#include <QFrame>
#include <QListView>
#include <QStringListModel>
#include <QListWidget>
#include <QPushButton>

namespace PhoenixPlayer {
    class PluginMetaData;

    namespace DataProvider {
        class TrackSearchProvider;
        class MatchObject;
    }

namespace UserInterface {
namespace RockRokr {

class RKListWidget;
class SearchResultPopup : public QFrame
{
    Q_OBJECT
    friend class BtnFiler;
public:
    explicit SearchResultPopup(QWidget *parent = Q_NULLPTR);
    virtual ~SearchResultPopup();

    void calToResize();

    void setSearchString(const QString &str);

    void setMaximumHeight(int maxh);

    void bindTrackSearchProvider(DataProvider::TrackSearchProvider *provider);

    Q_SLOT void doSearch();

Q_SIGNALS:
    void searchMatched();

protected:
//    void initProvider();

    void searchByAllPluginBtnEntered();
    void searchByAllPluginBtnLeave();

    // QWidget interface
protected:
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    RKListWidget          *m_resultView       = Q_NULLPTR;
//    QStringListModel    *m_resultModel      = Q_NULLPTR;
    QListView           *m_pluginView       = Q_NULLPTR;
    QStringListModel    *m_pluginViewModel  = Q_NULLPTR;
    QPushButton         *m_searchByAllPluginBtn = Q_NULLPTR;

    DataProvider::TrackSearchProvider *m_searchProvider = Q_NULLPTR;

    int m_maxHeight = 0;

    QString                             m_searchStr;
    QList<PluginMetaData>               m_usedPlugins;
    QList<DataProvider::MatchObject>    m_resultObjList;

};

} //namespace RockRokr
} //namespace UserInterface
} //namespace PhoenixPlayer

#endif // SEARCHRESULTPOPUP_H
