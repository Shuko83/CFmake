#include "SwGuiComponentTools.h"

#include "ui_ComponentToolsGui.h"
#include "Tools/SwAddress_ToolBox.h"
#include "ComponentTools/ComponentTreeModel.h"

using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

//---------------------------------------------------
SwGuiComponentTools::SwGuiComponentTools(QWidget *parent, SwComponent_Class * root_component, QList<QString> current_list) : QDialog(parent)
{
	setModal(true); //The window is modal to the application and blocks input to all windows.
	for (int i = 0; i < current_list.count(); i++) 
	{
		if (SwAddress_ToolBox::FindTarget(current_list[i], root_component) != NULL) 
		{
			_comp_paths.push_back(current_list[i]);
		}
	}

	_ui = new Ui::ComponentToolsGui();
	_ui->setupUi(this);
	setWindowTitle("Starlinx configuration - Select component to save");

	// Création du TableWidget qui contiendra les properties que l'on veut sauvegarder
	_compoTableWidget = new ComponentTableWidget(this);

	_compoTableWidget->setComponentsSaved(_comp_paths);

	_compoTableWidget->setDragEnabled(true);
	_compoTableWidget->setDropIndicatorShown(true);
	_compoTableWidget->viewport()->setAcceptDrops(true);
	_compoTableWidget->setAcceptDrops(true);
	_compoTableWidget->setDragDropMode(QAbstractItemView::DropOnly);
	_ui->verticalLayout_4->addWidget(_compoTableWidget);

	// Ajout du treeView dans la fenêtre de l'AdminSetup
	ComponentTreeModel * model = new ComponentTreeModel(_ui->treeView, root_component);
	_ui->treeView->setModel(model);
	_ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_ui->treeView->setAlternatingRowColors(true);
	_ui->treeView->setDragEnabled(true);
	_ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	_ui->verticalLayout_3->addWidget(_ui->treeView);

	_comp_paths.clear();

	connect(_ui->okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(_ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

//---------------------------------------------------
SwGuiComponentTools::~SwGuiComponentTools()
{

	if (_compoTableWidget != 0)
		delete _compoTableWidget;


	disconnect(_ui->okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	disconnect(_ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

//---------------------------------------------------
QList<QString> SwGuiComponentTools::getListComponents()
{
	return _compoTableWidget->getComponentsSaved();
}
