/*!
 \file ComponentTreeModel.cpp
 \brief Modele pour la selection de composant
 \version 1.0
 \date 2015
 \author EPO
 */
/*
  * INCLUDES LOCAUX
  */
#include <SwApplication.h>
#include <QMessageBox>
#include <QMimeData>
#include <SwAddress_ToolBox.h>
#include "ComponentTreeModel.h"

using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

//---------------------------------------------------------------------------------
ComponentTreeModel::ComponentTreeModel(QObject * parent, SwComponent_Class * root_component) :
QAbstractItemModel(parent), _root_component(root_component)
{
	_root_item = new _Item(NULL, _root_component, _root_component->GetName());
	buildItems(_root_component, _root_item);
}

//---------------------------------------------------------------------------------
ComponentTreeModel::~ComponentTreeModel()
{
	delete _root_item;
}

//---------------------------------------------------------------------------------
Qt::ItemFlags ComponentTreeModel::flags(const QModelIndex & index) const
{
	if ( index.column() == 0 )
		return Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

	return Qt::ItemFlags();
}

//---------------------------------------------------------------------------------
QStringList ComponentTreeModel::mimeTypes() const
{
	QStringList types;
	types << "application/component";
	return types;
}

//---------------------------------------------------------------------------------
QMimeData * ComponentTreeModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QString text;
	_Item * item;

	for (QModelIndex index : indexes)
	{
		if ( index.isValid() )
		{
			item = (_Item *) index.internalPointer();
			text = SwAddress_ToolBox::BuildAbsolutePath(item->_host);
		}
	}

	mimeData->setData("application/component", text.toLatin1());
	return mimeData;
}

//---------------------------------------------------------------------------------
int ComponentTreeModel::columnCount(const QModelIndex & parent) const
{
	return 1;
}

//---------------------------------------------------------------------------------
int ComponentTreeModel::rowCount(const QModelIndex & parent) const
{
	_Item * item;

	if ( !parent.isValid() )
		return 1;

	item = (_Item *) parent.internalPointer();
	return item->_childs.count();
}

//---------------------------------------------------------------------------------
QVariant ComponentTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ( role == Qt::DisplayRole )
	{
		switch ( section )
		{
			case 0:
				return QVariant(QString("Components"));
				break;
			default:
				break;
		}
	}
	return QVariant();
}

//---------------------------------------------------------------------------------
QVariant ComponentTreeModel::data(const QModelIndex & index, int role) const
{
	_Item * item;
	if ( !index.isValid() )
		return QVariant();

	item = (_Item *) index.internalPointer();

	if ( role == Qt::DisplayRole )
		return QVariant(item->_name);

	return QVariant();
}

//---------------------------------------------------------------------------------
QModelIndex ComponentTreeModel::index(int row, int column, const QModelIndex & parent) const
{
	_Item * pitem;

	if ( !parent.isValid() )
		return createIndex(row, column, (void *) _root_item);

	pitem = (_Item *) parent.internalPointer();
	return createIndex(row, column, (void *) pitem->_childs[row]);
}

//---------------------------------------------------------------------------------
QModelIndex ComponentTreeModel::parent(const QModelIndex & index) const
{
	_Item * item;
	_Item * pitem;
	int index_parent;

	if ( !index.isValid() )
		return QModelIndex();

	item = (_Item *) index.internalPointer();

	if ( item->_parent == NULL )
		return QModelIndex();

	//Calcul de l'indice du parent
	item = item->_parent;
	pitem = item->_parent;

	//Si le parent du parent est null, c'est le composant racine
	if ( pitem == NULL )
		return createIndex(0, 0, (void *) _root_item);
	else
	{
		//Sinon il faut calculer l'index
		index_parent = item->_parent->_childs.indexOf(item);

		if ( index_parent == -1 )
			return QModelIndex();
	}
	return createIndex(index_parent, 0, (void *) item);
}

//---------------------------------------------------------------------------------
ComponentTreeModel::_Item::_Item(_Item * parent, SwComponent_Class * host, QString name)
{
	_parent = parent;
	_host = host;
	_name = name;
	if ( _parent != NULL )
		_parent->_childs.push_back(this);
}

//---------------------------------------------------------------------------------
ComponentTreeModel::_Item::~_Item()
{
	_Item * child;

	for (auto child : _childs)
		delete child;

	_childs.clear();
}

//---------------------------------------------------------------------------------
void ComponentTreeModel::buildItems(SwComponent_Class * comp, _Item * item)
{
	//Creation des noeuds composants enfants
	SwComponent_Class * comp_child;
	comp_child = comp->GetFirstChild();
	while ( comp_child != NULL )
	{
		buildItems(comp_child, new _Item(item, comp_child, comp_child->GetName()));
		comp_child = comp->GetNextChild();
	}
}
