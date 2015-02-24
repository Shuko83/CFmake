#ifndef EDITDOC_H
#define EDITDOC_H


#include <QWidget>
#include "SwDocConstantes.h"
#include <QListView>
#include <QListWidgetItem>

class TKeyComp;
class TComponent;
class TKeyword;
class WidgetItemDelegate;
namespace Ui {
	class EditDocUi;
}

static int SizeMinWord = 2;

class BUILD_SWDOC EditDoc : public QWidget
{
	Q_OBJECT

public:
	EditDoc(QWidget *parent = NULL);
	~EditDoc();

	bool eventFilter(QObject *obj, QEvent *event);


public slots:

	void clearIhm();
	void onSelectedComponentChanged( const QModelIndex&);
	void updateBDD();
	void deleteKeyword();
	void addKeyword();
	void managePBApply();
	void changeIcon();


	/**
	 * @brief    : Method apellé lors de la selection d'un composant sur la StreamView
	 * @param	 : QString & - Nom du composant
	 * @param	 : QString & - Nom du plugin
	 */
	void onSelection(QString&,QString&);


	void manageTextKeyword(const QString &text);

	/**
	 * @brief    : Find a keyword in database and return a pointer of the keyword 
	 * @return   : TKeyword* a pointer of the keyword or NULL
	 * @param	 : QString keyword - the keyword
	 */
	TKeyword* findKeyword(QString keyword);

	/**
	 * @brief    : Find a keyword in database and return a pointer of the keyword 
	 * @return   : TKeyword* a pointer of the keyword or NULL
	 * @param	 : int id_keyword - the keyword id
	 */
	TKeyword* findKeyword(int id_keyword);

	/**
	 * @brief    : Find a component in the database by the id 
	 * @return   : TComponent* a pointer of the component or NULL
	 * @param	 : int id_comp - id of the component
	 */
	TComponent* findComponent( int id_comp);


	/**
	 * @brief    : find a associsation in database between keyword and component and renturna pointer of the TKeyComp
	 * @return   : TKeyComp* - pointer of a TKeyComp or NULL
	 * @param	 : int id_keyword - id of the keyword
	 * @param	 : int id_component -  id of the component
	 */
	TKeyComp* findAssoc(int id_keyword,int id_component);


	/**
	 * @brief    : Find all components that contains the keyword in association
	 * @return   : QList<TComponent*> - list of component
	 * @param	 : QStringList keywordList - all keyword to match
	 */
	QList<TComponent*> findComponentFromKeyword(QStringList keywordList);

	/**
	 * @brief    : Add keyword / keyassoc to listWidget
	 * @param	 : TKeyword * keyword - 
	 * @param	 : TKeyComp * keyAssoc - 
	 */
	void addItemToList(TKeyword * keyword,TKeyComp * keyAssoc);

	QStringList  getSearchStringList();

	QStringList getListOfComponentName(const QStringList & text);

private:
	

	void buildIhm(QString componentName,QString pluginName,QString desc,QIcon icon);
	//QListView* _listView;

	Ui::EditDocUi * ui;

	typedef struct{QListWidgetItem* item;TKeyComp* assoc;}ItemAssoc;
	QMap<WidgetItemDelegate*,ItemAssoc*> _map;

	TComponent* _component;
	QShortcut *_shortcut;


	QList<TKeyword*> _tmpKeywords;
	QList<TComponent*> _tmpComponents;


};



#endif