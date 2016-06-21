#include "ContextMenuEditor.h"

#include "ISwServiceContextMenu.h"

#include "TitleWidgetAction.h"

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
	_editorWidget = new ContextMenuEditorWidget();

	connect(_editorWidget, &ContextMenuEditorWidget::okPushed, this, &ContextMenuEditor::onOkPushed);
	connect(_editorWidget, &ContextMenuEditorWidget::cancelPushed, this, &ContextMenuEditor::onCancelPushed);
	connect(_editorWidget, &ContextMenuEditorWidget::applyPushed, this, &ContextMenuEditor::onApplyPushed);

	int subMenuLevel = 0;

	for (auto line : _editedMenuAsText)
	{	
		QString formattedLine = "";

		//ACTION
		if (line.contains(actionPrefix))
		{						
			QString actionName;			
			actionName = line.right(line.length() - actionPrefix.length());

			bool actionIsNoMoreRegistered = (_contextMenuServiceHelper->getService()->getAction(actionName) == nullptr);
					
			if (actionIsNoMoreRegistered)
				formattedLine += deletedActionHtml;

			formattedLine += subMenuIndent.repeated(subMenuLevel);

			formattedLine += line;

			if (actionIsNoMoreRegistered)
				formattedLine += endHtml;
		}
		//SEPARATOR
		else if (line.contains(separatorMark))
		{
			formattedLine = line;		
		}
		//SUBMENU
		else if (line.contains(submenuBeginMark))
		{
			subMenuLevel++;
			formattedLine += subMenuIndent.repeated(subMenuLevel);
			formattedLine += line;
		}
		else if (line.contains(submenuEndMark))
		{
			formattedLine += subMenuIndent.repeated(subMenuLevel);
			formattedLine += line;
			subMenuLevel--;
		}
		//TITLE
		else if (line.contains(titleMark))
		{			
			formattedLine = line;			
		}

		_editorWidget->addLineToTextEdit(formattedLine);
	}
	
	//On parcourt les actions registered pour ajouter les éventuelles nouvelles actions
	QList<QAction*> actions = _contextMenuServiceHelper->getService()->getActions();
	for (auto action : actions)
	{
		//QString newLine = actionPrefix + action->text();
		QString newLine = actionPrefix + _contextMenuServiceHelper->getService()->getActionNonHumanName(action);

		bool isUnsavedAction =	 !(_editedMenuAsText.contains(newLine)	//maniere plus elegante de faire?
								 ||_editedMenuAsText.contains(subMenuIndent + newLine)
								 ||_editedMenuAsText.contains(subMenuIndent.repeated(2) + newLine)
								 ||_editedMenuAsText.contains(subMenuIndent.repeated(3) + newLine)
								 ||_editedMenuAsText.contains(subMenuIndent.repeated(4) + newLine)
								 ||_editedMenuAsText.contains(subMenuIndent.repeated(5) + newLine)
								 );

		//Si l'action n'est pas présente dans le menu sauvegardée on la colore en vert dans l'editor
		if (isUnsavedAction)
		{
			newLine = newActionHtml + newLine +endHtml;
			_editorWidget->addLineToTextEdit(newLine);
		}		
	}

	_editorWidget->show();
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::Load(QDomElement & elt, ISwFinalizerManager & finalizerManager)
{
	QDomElement elt_ent;
	
	int subMenuLevel = 0;

	for (elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement())
	{
		//ACTION
		if (elt_ent.nodeName() == "Action")
		{
			QString action = elt_ent.attribute("Name");
			QString actionText = "";
		
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
			subMenuLevel++;
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(submenuBeginMark + title);
		}
		else if (elt_ent.nodeName() == "SubMenuEnd")
		{
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(submenuEndMark);
			subMenuLevel--;
		}
		//TITLE		
		else if (elt_ent.nodeName() == "Title")
		{
			QString title = elt_ent.attribute("Name");
			_editedMenuAsText.append(titleMark + title);
		}

	}

	/*qDebug() << "After Load";
	for (auto line : _editedMenuAsText)
		qDebug() << line;*/
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
		//TITLE
		else if (line.contains(titleMark))
		{
			elt_ent = doc.createElement("Title");
			QString titleName = line.right(line.length() - line.indexOf("#") - 1);
			elt_ent.setAttribute("Name", titleName);
			elt.appendChild(elt_ent);
		}
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
		int subMenuLevel = 0;
		
		QList<QMenu*> currentSubMenus;

		for (auto menuElementString : _editedMenuAsText)
		{		
			//ACTION
			if (menuElementString.contains(actionPrefix))
			{
				menuElementString = menuElementString.right(menuElementString.length() - actionPrefix.length());

				QAction * action = _contextMenuServiceHelper->getService()->getAction(menuElementString);			

				if (action)
				{
					if (subMenuLevel != 0)
						currentSubMenus.last()->addAction(action);
					else
						_editedMenu->addAction(action);
				}
				else
				{
					qDebug() << "Action " << menuElementString << " not found in contextMenuService";
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
			
				subMenuLevel++;

				QMenu* newSubMenu;
				if (subMenuLevel == 1)
					newSubMenu = new QMenu(_editedMenu);
				else
					newSubMenu = new QMenu(currentSubMenus.last());

				newSubMenu->setTitle(subMenuName);				

				if (subMenuLevel == 1)
					_editedMenu->addMenu(newSubMenu);
				else
					currentSubMenus.last()->addMenu(newSubMenu);

				currentSubMenus.append(newSubMenu);
			}
			else if (menuElementString.contains(submenuEndMark))
			{				
				subMenuLevel--;
				currentSubMenus.removeLast();
			}
			//TITLE
			else if (menuElementString.contains(titleMark))
			{
				menuElementString = menuElementString.right(menuElementString.length() - titleMark.length());
				TitleWidgetAction * newTitle;

				//A refactoriser
				if (subMenuLevel == 0)
					newTitle = new TitleWidgetAction(_editedMenu, menuElementString);
				else
					newTitle = new TitleWidgetAction(currentSubMenus.last(), menuElementString);
				
				if (subMenuLevel == 0)
					_editedMenu->addAction(newTitle);
				else
					currentSubMenus.last()->addAction(newTitle);
			}
		
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

//---------------------------------------------------------------------------------
void ContextMenuEditor::onOkPushed()
{
	onApplyPushed();

	delete _editorWidget;
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::onCancelPushed()
{
	delete _editorWidget;
}

//---------------------------------------------------------------------------------
void ContextMenuEditor::onApplyPushed()
{
	QStringList formattedLines = _editorWidget->getLines();
	QStringList unformattedLines;

	for (auto formattedLine : formattedLines)
	{		
		QString unformattedLine = formattedLine.replace(subMenuIndent, "");
		unformattedLines.append(unformattedLine);
	}

	_editedMenuAsText = unformattedLines;

	_menuReady = false;	//Le menu vient d'être édité on doit le recréer
}

