/*!
\file _SwExecutionSetup.cpp
\date 12/04/2006
\brief Ihm de configuration de l'execution
\author  Big
\version 1.0
*/

#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTreeView>
#include <QTableView>
#include <QVBoxLayout>
#include <SwAddress_ToolBox.h>
#include "_SwExecutorStreamTreeModel.h"
#include "_SwExecutorListEditor.h"
#include "_SwExecutionSetup.h"

//-----------------------------------------------------------------------
_SwExecutionSetup::_SwExecutionSetup(QWidget *parent, SwComponent_Class * root_component, QList<QString> * current_list, QList<_SwExecutionMode> * current_mode_list)
	: QDialog(parent), ui(new Ui::_SwExecutionSetupWidget)
{
	ui->setupUi(this);

	_root_component = root_component;
	for (int i = 0; i < (*current_list).count(); i++) 
	{
		if (SwAddress_ToolBox::FindTarget((*current_list)[i], root_component) != NULL) 
		{
			_exe_paths.push_back((*current_list)[i]);
			_exe_modes.push_back((*current_mode_list)[i]);
		}
	}

	ui->listView->setRoot_component ( _root_component );
	ui->listView->setPath_list ( &_exe_paths );
	ui->listView->setMode_list ( &_exe_modes );

	_SwExecutorStreamTreeModel *model = new _SwExecutorStreamTreeModel ( this, _root_component );
	ui->treeView->setModel ( model );
	ui->treeView->setDragEnabled ( true );
	ui->treeView->setSelectionMode ( QAbstractItemView::ExtendedSelection );
	ui->treeView->expandAll ();
	ui->treeView->resizeColumnToContents ( 0 );

	ui->listView->connect ( ui->listView, SIGNAL ( OnExeAdded ( const QStringList ) ), model, SLOT ( OnExeAdded ( const QStringList ) ) );
	ui->listView->connect ( ui->listView, SIGNAL ( OnExeRemoved ( const QStringList ) ), model, SLOT ( OnExeRemoved ( const QStringList ) ) );
	ui->listView->UpdateList ();



}

//-----------------------------------------------------------------------
_SwExecutionSetup::~_SwExecutionSetup()
{

}
//-----------------------------------------------------------------------
const QList<QString> * _SwExecutionSetup::GetEditedList() 
{
	return &_exe_paths;
}

//-----------------------------------------------------------------------
const QList<_SwExecutionMode> * _SwExecutionSetup::GetEditedListMode()
{
	return &_exe_modes;
}
