#ifndef PLUGINOVERVIEW_H
#define PLUGINOVERVIEW_H

#include <QWidget>
#include "ui_PluginOverview.h"
#include <QEvent>
#include <QFutureWatcher>
#include "ComponentListModel.h"

class  EditDoc;

static const char * ScrollBarVerticalStyle = " \
											 QScrollBar:vertical { \
											 border: 0px; \
											 border-color :#111111; \
											 background: #111111; \
											 width: 10px; \
											 padding: 2px 2px 2px 2px; \
											 } \
											 \
											 QScrollBar::handle:vertical {\
											 color: #EEEEEE;\
											 background: #EEEEEE;\
											 min-height: 20px;\
											 }\
											 QScrollBar::add-line:vertical {\
											 border: 2px solid grey;\
											 background: solid grey;\
											 height: 20px;\
											 subcontrol-position: bottom;\
											 subcontrol-origin: margin;\
											 }\
											 \
											 QScrollBar::sub-line:vertical {\
											 border: 2px solid grey;\
											 background: solid grey;\
											 height: 20px;\
											 subcontrol-position: top;\
											 subcontrol-origin: margin;\
											 }\
											 ";

static const char * ScrollBarHorizontalStyle = " \
											   QScrollBar:horizontal { \
											   border: 0px; \
											   border-color :#111111; \
											   background: #111111; \
											   width: 10px; \
											   padding: 2px 2px 2px 2px; \
											   } \
											   \
											   QScrollBar::handle:horizontal {\
											   color: #EEEEEE;\
											   background: #EEEEEE;\
											   min-height: 20px;\
											   }\
											   QScrollBar::add-line:horizontal {\
											   border: 2px solid grey;\
											   background: solid grey;\
											   height: 20px;\
											   subcontrol-position: right;\
											   subcontrol-origin: margin;\
											   }\
											   \
											   QScrollBar::sub-line:horizontal {\
											   border: 2px solid grey;\
											   background: solid grey;\
											   height: 20px;\
											   subcontrol-position: left;\
											   subcontrol-origin: margin;\
											   }\
											   ";

static const char * DefaultSearchText = "Search components by tag or component name";

class PluginOverview : public QWidget
{
	Q_OBJECT

public:
	PluginOverview(EditDoc* doc,bool isGraphViewHosted,QPalette graphPalette,QWidget *parent = 0);
	~PluginOverview();

	bool eventFilter(QObject *obj, QEvent *event);

public slots:
	/**
	 * @brief    : Slot pour reset la lineEdit de recherche
	 */
	void resetSearchText();
	/**
	 * @brief    : Slot pour la recherche
	 * @param	 : const QString & text - text a rechercher
	 */
	void doSearch(const QString&text);
	
	/**
	 * @brief    : Au cas ou on veut remettre un completer mais pour le moment on le fait pas
	 */
	//void setCompleter();

private:
	Ui::PluginOverview ui;
	EditDoc* _doc;
	QPalette _pal;
	QFutureWatcher<QStringList> _futureWatcher;
	ComponentListModel * _componentModel;

};

#endif // PLUGINOVERVIEW_H
