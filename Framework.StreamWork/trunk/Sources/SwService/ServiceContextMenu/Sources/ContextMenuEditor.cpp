#include "ContextMenuEditor.h"

#include "ISwServiceContextMenu.h"

#include <QApplication>

static const QString newActionHtml = "<font color=\"LimeGreen \">";
static const QString deletedActionHtml = "<font color=\"OrangeRed \">";
static const QString endHtml = "</font>";

//---------------------------------------------------------------------------------
ContextMenuEditor::ContextMenuEditor() :_editedMenu(nullptr),
										_menuReady(false)
{
	setOwnerServiceAvaibility(true);

	_contextMenuServiceHelper = new SwServiceManager_Helper<IEditableContextMenu>();
	_contextMenuServiceHelper->setService(CG_SW_SERVICE_CONTEXT_MENU, this, &ContextMenuEditor::onContextMenuServiceChange);	

	_mainWindowServicehelper = new SwServiceManager_Helper<StreamWork::Service::ISwServiceMainWindow>();
	_mainWindowServicehelper->setService(CG_SW_SERVICE_MAINWINDOW);
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::AdminSetup()
{
	ContextMenuEditorWidget * _editorWidget = new ContextMenuEditorWidget();
	bool isSubMenuAction = false;

	for (auto line : _editedMenuAsText)
	{	
		QString formattedLine = "";

		if (line.contains(actionPrefix))
		{						
			QString actionName;
			
			if (isSubMenuAction)
				actionName = line.right(line.length() - actionPrefix.length() - subMenuIndent.length());
			else
				actionName = line.right(line.length() - actionPrefix.length());

			bool actionIsNoMoreRegistered = (_contextMenuServiceHelper->getService()->getAction(actionName) == nullptr);
					
			if (actionIsNoMoreRegistered)
				formattedLine += deletedActionHtml;

			formattedLine += line;

			if (actionIsNoMoreRegistered)
				formattedLine += endHtml;
		}
		else if (line.contains(separatorMark))
		{
			formattedLine = line;		
		}
		else if (line.contains(submenuBeginMark))
		{
			formattedLine = line;
			isSubMenuAction = true;
		}
		else if (line.contains(submenuEndMark))
		{
			formattedLine = line;
			isSubMenuAction = false;
		}

		_editorWidget->addLineToTextEdit(formattedLine);
	}
	
	//On parcourt les actions registered pour ajouter les éventuelles nouvelles actions
	QList<QAction*> actions = _contextMenuServiceHelper->getService()->getActions();
	for (auto action : actions)
	{
		QString newLine = actionPrefix + action->text();

		bool isUnsavedAction = !(_editedMenuAsText.contains(newLine) || _editedMenuAsText.contains(subMenuIndent + newLine));

		//Si l'action n'est pas présente dans le menu sauvegardée on la colore en vert dans l'editor
		if (isUnsavedAction)
		{
			newLine = newActionHtml + newLine +endHtml;
			_editorWidget->addLineToTextEdit(newLine);
		}		
	}

	if (_editorWidget->exec() == QDialog::Accepted)
	{		
		_editedMenuAsText = _editorWidget->getLines();
		_menuReady = false;	//Le menu vient d'être édité on doit le recréer
	}

	delete _editorWidget;
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::Load(QDomElement & elt, ISwFinalizerManager & finalizerManager)
{
	QDomElement elt_ent;

	bool subMenuAction = false;	

	for (elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement())
	{
		//ACTION
		if (elt_ent.nodeName() == "Action")
		{
			QString action = elt_ent.attribute("Name");
			QString actionText = "";

			if (subMenuAction)
				actionText = subMenuIndent;

			actionText += actionPrefix + action;

			_editedMenuAsText.append(actionText);
		}
		//SEPARATOR
		else if (elt_ent.nodeName() == "Separator")
		{
			_editedMenuAsText.append(separatorMark);
		}		
		//SUBMENU
		else if (elt_ent.nodeName() == "SubMenuBegin")
		{
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(submenuBeginMark + title);
			subMenuAction = true;
		}
		else if (elt_ent.nodeName() == "SubMenuEnd")
		{
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(submenuEndMark);
			subMenuAction = false;
		}
		//CATEGORY
		/*
		else if (elt_ent.nodeName() == "CategoryBegin")
		{
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(categoryBeginMark + title);
		}
		else if (elt_ent.nodeName() == "CategoryEnd")
		{
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(categoryEndMark);
		}*/
	}
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::Save(QDomElement &elt, QDomDocument &doc)
{
	QDomElement elt_ent;

	for (auto line : _editedMenuAsText)
	{
		//ACTION
		if (line.contains(actionPrefix))
		{
			elt_ent = doc.createElement("Action");
			QString actionName = line.right(line.length() - line.indexOf("#") - 1);
			elt_ent.setAttribute("Name", actionName);		
			elt.appendChild(elt_ent);
		}
		//SEPARATOR
		else if (line.contains(separatorMark))
		{
			elt_ent = doc.createElement("Separator");
			elt.appendChild(elt_ent);
		}		
		//SUBMENU
		else if (line.contains(submenuBeginMark))
		{
			elt_ent = doc.createElement("SubMenuBegin");
			QString titleName = line.right(line.length() - line.indexOf("#") - 1);
			elt_ent.setAttribute("Name", titleName);
			elt.appendChild(elt_ent);
		}
		else if (line.contains(submenuEndMark))
		{
			elt_ent = doc.createElement("SubMenuEnd");
			QString titleName = line.right(line.length() - line.indexOf("#") - 1);			
			elt.appendChild(elt_ent);
		}
		//CATEGORY
		/*else if (line.contains(categoryBeginMark))
		{
			elt_ent = doc.createElement("CategoryBegin");
			QString titleName = line.right(line.length() - line.indexOf("#") - 1);
			elt_ent.setAttribute("Name", titleName);
			elt.appendChild(elt_ent);
		}
		else if (line.contains(categoryEndMark))
		{
			elt_ent = doc.createElement("CategoryEnd");
			QString titleName = line.right(line.length() - line.indexOf("#") - 1);
			elt.appendChild(elt_ent);
		}*/

	}
}

//---------------------------------------------------------------------------------
QMenu* ContextMenuEditor::getMenu()
{
	if (_editedMenuAsText.count() == 0)
		return nullptr;	

	if (!_menuReady)
	{
		_editedMenu = new QMenu(_mainWindowServicehelper->getService()->getMainWindow());

		//Submenu data		
		bool submenuEnabled = false;		
		QMenu *currentSubMenu;

		for (auto menuElementString : _editedMenuAsText)
		{		
			//ACTION
			if (menuElementString.contains(actionPrefix))
			{

				if (submenuEnabled)
					menuElementString = menuElementString.right(menuElementString.length() - actionPrefix.length() - subMenuIndent.length());
				else
					menuElementString = menuElementString.right(menuElementString.length() - actionPrefix.length());

				QAction * action = _contextMenuServiceHelper->getService()->getAction(menuElementString);			

				if (action)
				{
					if (submenuEnabled)
						currentSubMenu->addAction(action);
					else
						_editedMenu->addAction(action);				
				}
			}
			//SEPARATOR
			else if (menuElementString.contains(separatorMark))
			{
				_editedMenu->addSeparator();
			}
			//SUBMENU
			else if (menuElementString.contains(submenuBeginMark))
			{
				menuElementString = menuElementString.right(menuElementString.length() - submenuBeginMark.length());
				QString subMenuName = menuElementString;

				submenuEnabled = true;
				currentSubMenu = new QMenu(_editedMenu);
				currentSubMenu->setTitle(subMenuName);

				_editedMenu->addMenu(currentSubMenu);
			}
			else if (menuElementString.contains(submenuEndMark))
			{
				submenuEnabled = false;				
			}
			//CATEGORY
			/*else if (menuElementString.contains(categoryBeginMark))
			{
			menuElementString = menuElementString.right(menuElementString.length() - categoryBeginMark.length());
			categoryEnabled = true;
			currentActionGroup = new QActionGroup(_editedMenu);
			categoryName = menuElementString;
			}
			else if (menuElementString.contains(categoryEndMark))
			{
			categoryEnabled = false;
			categoryName = "none";
			}*/
		
		}
		_menuReady = true;
	}
	return _editedMenu;
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::onContextMenuServiceChange(bool available)
{
	_contextMenuServiceHelper->getService()->setMenuEditor(this);
}

