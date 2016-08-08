#pragma once

/*
* Includes locaux
*/
#include "ContextMenuEditorWidget.h"

/*
* Includes globaux
*/
#include "SwServiceManager_Helper.h"
#include "SwAssistedComponent.h"
#include "ISwAdminSetup.h"
#include "ISwPersistent.h"
#include "ISwServiceMainWindow.h"
#include "IMenuEditor.h"
#include "IEditableContextMenu.h"
#include "ContextMenuWidget.h"


class ContextMenuEditor : public StreamWork::SwFoundation::SwAssistedComponent,
						  public ISwAdminSetup,
						  public IMenuEditor
{
public:
	ContextMenuEditor();
	virtual ~ContextMenuEditor() = default;

	//Interface de ISwAdminSetup
	void AdminSetup() override;

	//Interface de SwAssistedComponent : public ISwPersistent
	void Load(QDomElement&, ISwFinalizerManager&) override;
	void Save(QDomElement&, QDomDocument&) override;

	//Interface de IMenuEditor
	ContextMenuWidget* getMenu() override;

private:
	ContextMenuWidget* _editedMenu;
	QStringList  _editedMenuAsText; //contient le texte a parser pour créer le QMenu (sans le formattage d'affichage de l'editor)
	bool _menuReady;
	ContextMenuEditorWidget * _editorWidget;

	SwServiceManager_Helper<IEditableContextMenu> * _contextMenuServiceHelper;
	SwServiceManager_Helper<StreamWork::Service::ISwServiceMainWindow> *_mainWindowServicehelper;

	void onContextMenuServiceChange(bool available);	

	void onOkPushed();
	void onCancelPushed();
	void onApplyPushed();
};