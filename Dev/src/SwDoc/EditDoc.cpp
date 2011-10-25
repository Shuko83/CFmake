#include "EditDoc.h"
#include <QDebug>

#include <QPushButton>


/**
 * Include locaux
 */
#include "ManageSQL.h"
#include "WidgetItemDelegate.h"
#include "..\SwCore\SwPluginFactory_Class.h"
#include "..\SwCore\SwApplication.h"
#include "..\SwCore\SwMacros.h"

using namespace StreamWork::SwCore;


//-------------------------------------------------------------------------
EditDoc::EditDoc(QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);

	//Init variables
	_shortcut = new QShortcut(QKeySequence(QKeySequence::Delete),ui.listWidget);
	_component = NULL;
	ui.pb_addKeyword->setDisabled(true);

	//On ajoute la toolbar pour le RichText
	ui.gridLayout_2->addWidget(ui.TE_desc->createToolBar());

	//Connection des slots
	connect(ui.pb_addKeyword		,SIGNAL(clicked())							,this, SLOT(addKeyword()));
	connect(ui.TE_desc				,SIGNAL(textChanged())						,this, SLOT(managePBApply()));
	connect(ui.PB_apply				,SIGNAL(clicked())							,this, SLOT(updateBDD()));
	connect(_shortcut				,SIGNAL(activated ())						,this, SLOT(deleteKeyword()));
	connect(ui.LE_addKeyword		,SIGNAL(textChanged ( const QString &  ))	,this, SLOT(manageTextKeyword(const QString &) ));
	connect(ui.LE_addKeyword		,SIGNAL(returnPressed ())					,this, SLOT(addKeyword()));

	//Disable ihm if the connection to the database is not active
	if(!ManageSQL::getInstance()->isOpen())
		this->setEnabled(false);
}

//-------------------------------------------------------------------------
EditDoc::~EditDoc()
{

}

//-------------------------------------------------------------------------
void EditDoc::onSelectedComponentChanged( const QModelIndex&test )
{
	//Sur changement de selection de la listView Component
	clearIhm();

	//Si l'index est valid on récupère les infos et on affiche dans l'ihm
	if(test.isValid())
	{
		// On renseigne avec les valeur des variables recupérer dans le plugin
		QString componentName = test.data(Qt::DisplayRole).toString();
		QString pluginName = test.data(Qt::UserRole).toString();
		QString desc = test.data(Qt::ToolTipRole).toString();
		QIcon icon = qvariant_cast<QIcon>(test.data(Qt::DecorationRole));
		
		buildIhm(componentName,pluginName,desc,icon);
	}
}

//-------------------------------------------------------------------------
void EditDoc::updateBDD()
{
	//Si le composant est setter, on ecrase la desc
	if(_component)
	{
		_component->setDesc(ui.TE_desc->toHtml());
		_component->save();
	}
}

//-------------------------------------------------------------------------
void EditDoc::addKeyword()
{
	if(_component)
	{
		//Rechercher si le keyword exist pas
		TKeyword *  keyword= findKeyword(ui.LE_addKeyword->text());
		if(!keyword)
		{
			//Si il existe pas on l'insert
			keyword = new TKeyword();
			QString text = ui.LE_addKeyword->text().trimmed();
			keyword->setName(text);
			keyword->setUse(1);
			keyword->save();
		}

		//On verifie que il y a pas deja une assoc entre le composant et le mot clé
		TKeyComp * keycomp = findAssoc(keyword->pk().toInt(),_component->pk().toInt());
		if(!keycomp)
		{
			//Si y'a pas d'association on en créé une et on incrémente le compteur du mot clé
			keycomp = new TKeyComp();
			keycomp->setIdComp(_component->pk().toInt());
			keycomp->setIdKey(keyword->pk().toInt());
			keyword->setUse(keyword->getUse()+1);
			keycomp->save();

			addItemToList(keyword,keycomp);

			ui.LE_addKeyword->setSelection(0,ui.LE_addKeyword->text().length());
			ui.pb_addKeyword->setDisabled(true);
			ui.LE_addKeyword->setFocus(Qt::TabFocusReason);
		}
	}
}

//-------------------------------------------------------------------------
void EditDoc::deleteKeyword()
{
	//On récupère le widget sender du slot
	WidgetItemDelegate * item = qobject_cast<WidgetItemDelegate*>(sender());
	WidgetItemDelegate *nexItem = NULL;
	if(item)
	{
		//Si on a un item et que celui ci est contenu dans la map
		if(_map.contains(item))
		{
			//Si la suppression en bdd réussit
			if(_map.value(item)->assoc->remove())
			{
				//On récupère la ligne du widget dans la listWidget
				int row = ui.listWidget->row(_map.value(item)->item);

				//Si c'est possible on récupère un pointeur sur le prochain item pour 
				//pouvoir envoyer un évennement d'enterEvent pour l'affichage de la croix
				if(ui.listWidget->item(row+1))
					nexItem= (WidgetItemDelegate*)(ui.listWidget->itemWidget( ui.listWidget->item(row+1)));
				
				//On prend l'item de la list et on le supprimer
				QListWidgetItem * tmp =ui.listWidget->takeItem(row);
				delete tmp;
				
				//On récupère l'id du mot clé de la suppression
				int id_key = _map.value(item)->assoc->getIdKey();
				TKeyword *  keyword= findKeyword(id_key);
				if(keyword)
				{
					//On décrémente l'utilisation du mot clé
					int use = (keyword->getUse()-1<0)?0:keyword->getUse()-1;
					keyword->setUse(use);
					keyword->save();
				}

				//On supprime l'item de la map
				_map.remove(item);
				delete item;
				
			}
		}
	}

	
	//if call by shortcut
	QShortcut * itemShorcut = qobject_cast<QShortcut*>(sender());
	if(itemShorcut)
	{
		//Pour le moment y'a que une simple selection mais on traite tous les items selectionnés
		QList<QListWidgetItem*> deleteItem = ui.listWidget->selectedItems();
		foreach(QListWidgetItem * toDelete ,deleteItem)
		{
			if(toDelete)
			{
				QMap<WidgetItemDelegate*,ItemAssoc*>::iterator it = _map.begin();
				QMap<WidgetItemDelegate*,ItemAssoc*>::iterator itend = _map.end();
				for(it; it != itend; ++it)
				{
					if(it.value()->item == toDelete)
					{
						if(it.value()->assoc->remove())
						{
							int row = ui.listWidget->row(it.value()->item);
							if(ui.listWidget->item(row+1))
								nexItem= (WidgetItemDelegate*)(ui.listWidget->itemWidget( ui.listWidget->item(row+1)));

							QListWidgetItem * tmp = ui.listWidget->takeItem(row);
							delete tmp;

							int id_key = it.value()->assoc->getIdKey();
							TKeyword *  keyword= findKeyword(id_key);
							if(keyword)
							{
								int use = (keyword->getUse()-1<0)?0:keyword->getUse()-1;
								keyword->setUse(use);
								keyword->save();
							}
							it = _map.erase(it);
						}
					}
				}
			}
		}
	}

	// permet d'afficher la croix sur le prochain item quand on a supprimer un 
	if(nexItem)
	{
		QEvent *test= new QEvent(QEvent::Enter);
		test->setAccepted(true);
		nexItem->enterEvent(test);
	}
}

//-------------------------------------------------------------------------
TKeyword* EditDoc::findKeyword( QString keyword )
{
	QList<TKeyword*> listK = ManageSQL::getInstance()->getOrmObject<TKeyword>("name",keyword,QDjangoWhere::Equals);

	if(listK.count() == 1)
	{
		return listK.at(0);
	}

	return NULL;
}

//-------------------------------------------------------------------------
TComponent* EditDoc::findComponent( int id_comp)
{
	QList<TComponent*> listC = ManageSQL::getInstance()->getOrmObject<TComponent>("id",id_comp,QDjangoWhere::Equals);

	if(listC.count() == 1)
	{
		return listC.at(0);
	}

	return NULL;
}
//-------------------------------------------------------------------------
TKeyword* EditDoc::findKeyword( int id_keyword )
{
	QList<TKeyword*> listK = ManageSQL::getInstance()->getOrmObject<TKeyword>("id",id_keyword,QDjangoWhere::Equals);

	if(listK.count() == 1)
	{
		return listK.at(0);
	}

	return NULL;
}

//-------------------------------------------------------------------------
TKeyComp* EditDoc::findAssoc( int id_keyword,int id_component )
{
	QDjangoQuerySet<TKeyComp> query;
	query = query.filter(QDjangoWhere("id_keyword", QDjangoWhere::Equals,id_keyword) && QDjangoWhere("id_component", QDjangoWhere::Equals,id_component));
	
	if(query.size() > 1)
	{
		//Securité au cas ou on a plusieurs assoc pour le même key/comp
		for (int i = 1; i < query.size(); ++i) 
		{
			query.at(i)->remove();
		}
	}

	if(query.size() == 1)
	{
		return query.at(0);
	}

	return NULL;
}

//-------------------------------------------------------------------------
void EditDoc::addItemToList(TKeyword * keyword,TKeyComp * keyAssoc)
{
	QListWidgetItem *itemL = new QListWidgetItem();
	itemL->setText(keyword->getName());
	ui.listWidget->addItem(itemL);

	WidgetItemDelegate * wid = new WidgetItemDelegate();
	itemL->setSizeHint(wid->sizeHint());
	wid->setText(keyword->getName());
	ui.listWidget->setItemWidget(itemL,wid);

	//On crée l'élément de la map
	ItemAssoc *mapAssoc = new ItemAssoc;
	mapAssoc->item = itemL;
	mapAssoc->assoc = keyAssoc;
	_map.insert(wid,mapAssoc);

	ui.listWidget->sortItems();

	connect(wid,SIGNAL(deleteAsked()),this,SLOT(deleteKeyword()));
}

//-------------------------------------------------------------------------
void EditDoc::manageTextKeyword(const QString &text) 
{
	ui.pb_addKeyword->setDisabled(false);

	//Coloration de l'item si il est déjà dans la liste
	QMap<WidgetItemDelegate*,ItemAssoc*>::iterator it = _map.begin();
	QMap<WidgetItemDelegate*,ItemAssoc*>::iterator itend = _map.end();
	for(it; it != itend; ++it)
	{
		if(it.key()->getText() == text)
		{
			it.value()->item->setBackgroundColor(QColor(Qt::red));
			ui.listWidget->setCurrentItem(it.value()->item,QItemSelectionModel::SelectCurrent);
			ui.pb_addKeyword->setDisabled(true);
		}
		else
		{
			it.value()->item->setBackgroundColor(QColor(Qt::transparent));
		}
	}
}

//-------------------------------------------------------------------------
void EditDoc::onSelection( QString&componentName,QString&pluginName)
{

	//Sur selection dans la StreamView

	clearIhm();

	if(componentName == "" || pluginName == "")
	{
		setEnabled(false);
	}

	QMap<QString,SwPluginFactory_Class *> *tmpList=SW_APP->ComponentsBank().GetAllPlugins();

	if(tmpList->contains(pluginName))
	{
		SwPluginFactory_Class *fac = tmpList->value(pluginName);
		buildIhm(componentName,pluginName,fac->GetComponentDescription(componentName),fac->GetComponentIcon(componentName));
	}
}

//-------------------------------------------------------------------------
void EditDoc::clearIhm()
{
	ui.TE_desc->reset();
	ui.TE_desc->clear();
	ui.listWidget->clear();
	ui.listWidget->reset();
	ui.L_pluginName->setText("");
	ui.L_componentName->setText("");
	ui.LE_addKeyword->setText("");
	ui.L_pixmap->setPixmap(QPixmap());
	_map.clear();

	_component = NULL;
}

//-------------------------------------------------------------------------
void EditDoc::buildIhm( QString componentName,QString pluginName,QString desc,QIcon icon )
{
	pluginName = pluginName.mid(0,pluginName.lastIndexOf("."))+"::";

	ui.PB_apply->setEnabled(false);
	ui.LE_addKeyword->setText("");
	ui.L_pixmap->setPixmap(icon.pixmap(30,30));
	ui.L_componentName->setText(componentName);
	ui.L_pluginName->setText(pluginName);
	ui.TE_desc->setText(desc);

	if(!ManageSQL::getInstance()->isOpen())
		return;

	// On check si le composant est en bdd 
	// si oui on update les infos.
	QList<TComponent*> listC = ManageSQL::getInstance()->getOrmObject<TComponent>("name",componentName,QDjangoWhere::Equals);

	if(listC.count() == 1)
	{
		_component = listC.at(0);
		if(_component == NULL)
			return;
		//Normal case
		ui.TE_desc->setText(_component->getDesc());

		//get all keyword associated to this component
		QList<TKeyComp*> listKC = ManageSQL::getInstance()->getOrmObject<TKeyComp>("id_component",_component->pk().toString(),QDjangoWhere::Equals);

		foreach(TKeyComp * keyassoc , listKC)
		{
			//QList<TKeyword*> listK = ManageSQL::getInstance()->getOrmObject<TKeyword>("id",item->pk().toString(),QDjangoWhere::Equals);
			TKeyword * keyword = findKeyword(keyassoc->getIdKey());
			if(keyword)
			{
				addItemToList(keyword,keyassoc);
			}
		}
	}

	if(listC.count() == 0)
	{
		//on le creée
		_component = new TComponent();
		_component->setName(componentName);
		_component->setDesc(desc);
		_component->save();
	}
}

//-------------------------------------------------------------------------
void EditDoc::managePBApply()
{
	ui.PB_apply->setEnabled(true);
}


//-------------------------------------------------------------------------
bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
	return s1.toLower() < s2.toLower();
}

//-------------------------------------------------------------------------
QStringList  EditDoc::getSearchStringList()
{

	_tmpKeywords.clear();
	_tmpComponents.clear();
	_tmpKeywords = ManageSQL::getInstance()->getOrmObject<TKeyword>();
	_tmpComponents = ManageSQL::getInstance()->getOrmObject<TComponent>();

	QStringList wordList;

	foreach(TKeyword*keyword, _tmpKeywords)
	{
		wordList << keyword->getName();
	}

	foreach(TComponent*cp, _tmpComponents)
	{
		wordList << cp->getName();
	}

	qSort(wordList.begin(), wordList.end(), caseInsensitiveLessThan);

	return wordList;
}


QStringList EditDoc::getListOfComponentName(const QStringList& text)
{
	QList<TComponent*> findedComp = findComponentFromKeyword(text);
	
	QStringList listComp;
	foreach(TComponent *comp , findedComp)
	{
		listComp << comp->getName();
	}

	return listComp ;

}

//-------------------------------------------------------------------------
QList<TComponent*> EditDoc::findComponentFromKeyword( QStringList keywordList )
{
	
	//Get all id of keyword
	QList<int> idKeywordList;
	foreach(TKeyword*keyword, _tmpKeywords)
	{
		if(keywordList.contains(keyword->getName(),Qt::CaseInsensitive))
		{
			idKeywordList << keyword->pk().toInt();
		}
	}
	//on cherche les id des composants qui ont le keyword
	QDjangoQuerySet<TKeyComp> query;
	QDjangoQuerySet<TKeyComp> filteredQuery;

	//Map key : componentId, QList<keyword_id>
	QMap<int, QList<int>> mapC;
	foreach(int idkeyword, idKeywordList)
	{
		QList<TKeyComp*> listC = ManageSQL::getInstance()->getOrmObject<TKeyComp>("id_keyword",idkeyword,QDjangoWhere::Equals);
		QList<int> _componentID;
		foreach(TKeyComp * keycomp, listC)
		{
			if(mapC.contains(keycomp->getIdComp()))
			{
				mapC[keycomp->getIdComp()].append(idkeyword);
			}
			else
			{
				QList<int> tmpList;
				tmpList << idkeyword;
				mapC.insert(keycomp->getIdComp(),tmpList);
			}
		}
	}

	//Si le nom d'un composant est un mot clé
	QList<int> idComposantList;;
	foreach(TComponent*comp, _tmpComponents)
	{
		foreach(QString keyword, keywordList)
		{
			if(comp->getName().contains(keyword,Qt::CaseInsensitive))
			{
				if(mapC.contains(comp->pk().toInt()))
				{
					mapC[comp->pk().toInt()].append(-5); // -5 arbitraire pour dire je match avec le component name
				}
				else
				{
					QList<int> tmpList;
					tmpList << -5;
					mapC.insert(comp->pk().toInt(),tmpList);
				}
			}
		}
	}

	//On vérifie que tout les composants sont dans toute les listes
	QMap<int, QList<int>>::iterator it = mapC.begin();
	QMap<int, QList<int>>::iterator itend = mapC.end();

	QList<TComponent*> returnList;
	for(it ; it != itend; )
	{
		if(keywordList.count() != it->count()  )
			it = mapC.erase(it);
		else
			it++;
	}

	it = mapC.begin();
	for(it ; it != itend ; ++it)
	{
		returnList<< findComponent(it.key());
	}

	return returnList;
}
