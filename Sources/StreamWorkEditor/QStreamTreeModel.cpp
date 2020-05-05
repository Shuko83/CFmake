/**
@file QStreamTreeModel.cpp
@brief Modele arbre du stream pour les trees views
@author F.Bighelli
*/

#include <SwApplication.h>
#include "QStreamTreeModel.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;


//-----------------------------------------------------------------------
QStreamTreeModel::QStreamTreeModel(QObject * parent) :QAbstractItemModel(parent)
{
	_controler = 0;
	_rootComponent = 0;
}

//-----------------------------------------------------------------------
void QStreamTreeModel::setStreamControler(StreamControler * controler)
{
	if ( _controler != controler )
	{
		beginResetModel();
		if ( _controler != 0 )
		{
			_controler->removeSelectionObserver(this);
			recursiveDisconnectToControler(_rootComponent);
			_controler = 0;
			_rootComponent = 0;
		}
		_controler = controler;
		if ( _controler != 0 )
		{
			_rootComponent = _controler->getRootItem();
			recursiveConnectToControler(_rootComponent);
			_controler->addSelectionObserver(this);
		}
		endResetModel();
	}
}
//-------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------

//-----------------------------------------------------------------------
Qt::ItemFlags QStreamTreeModel::flags(const QModelIndex & index) const
{
	return Qt::ItemFlags(Qt::ItemIsEnabled);
}

//-----------------------------------------------------------------------
int QStreamTreeModel::columnCount(const QModelIndex & parent) const
{
	return 4;
}

//-----------------------------------------------------------------------
int QStreamTreeModel::rowCount(const QModelIndex & parent) const
{
	if ( !parent.isValid() )
		return 1;
	SwComponent_Class *cparent = (SwComponent_Class *) parent.internalPointer();
	return cparent->GetNbChildren();
}

//-----------------------------------------------------------------------
QVariant QStreamTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ( role == Qt::DisplayRole )
	{
		switch ( section )
		{
			case 0:
				return QVariant(QString("Component"));
				break;
			case 1:
				return QVariant(QString("Description"));
				break;
			case 2:
				return QVariant(QString("Factory"));
				break;
			case 3:
				return QVariant(QString("Factory Description"));
				break;
			default:
				break;
		}
	}
	return QVariant();
}

//-----------------------------------------------------------------------
QVariant QStreamTreeModel::data(const QModelIndex & index, int role) const
{
	if ( !index.isValid() )
		return QVariant();
	SwComponent_Class* component = (SwComponent_Class *) index.internalPointer();
	if ( role == Qt::DisplayRole && index.column() == 0 )
	{
		return QVariant(component->GetName());
	}
	if ( role == Qt::DisplayRole && index.column() == 1 )
	{
		return QVariant(component->GetDescription());
	}
	if ( role == Qt::DisplayRole && index.column() == 2 )
	{
		return QVariant(component->GetFactoryComponentName());
	}
	if ( role == Qt::DisplayRole && index.column() == 3 )
	{
		return QVariant(SW_APP->ComponentsBank().GetComponentDescription(component->GetFactoryName(), component->GetFactoryComponentName()));
	}
	if ( role == Qt::DecorationRole && index.column() == 0 )
	{
		return QVariant(SW_APP->ComponentsBank().GetComponentIcon(component->GetFactoryName(), component->GetFactoryComponentName()));
	}
	if ( role == Qt::ForegroundRole && _selection.indexOf(component) >= 0 )
	{
		return QVariant(QColor(255, 0, 0));
	}

	return QVariant();
}

//-----------------------------------------------------------------------
QModelIndex QStreamTreeModel::index(int row, int column, const QModelIndex & parent) const
{
	if ( _rootComponent == 0 )
		return QModelIndex();
	if ( !parent.isValid() )
		return createIndex(row, column, (void *) _rootComponent);

	SwComponent_Class * cparent = (SwComponent_Class *) parent.internalPointer();
	return createIndex(row, column, (void *) cparent->GetChild(row));

}

//-----------------------------------------------------------------------
QModelIndex QStreamTreeModel::parent(const QModelIndex & index) const
{
	if ( !index.isValid() )
		return QModelIndex();
	SwComponent_Class * cindex = (SwComponent_Class *) index.internalPointer();
	if ( cindex == _rootComponent )
	{
		return QModelIndex();
	}
	SwComponent_Class * cparent = cindex->GetParent();
	if ( cparent == _rootComponent )
	{
		return createIndex(0, 0, (void *) _rootComponent);
	}
	SwComponent_Class * cparentparent = cparent->GetParent();
	int iindex = cparentparent->GetChildIndex(cparent);
	return createIndex(iindex, 0, (void *) cparent);
}
//-------------------------------------------------------------
// Interface evenement du stream
//------------------------------------------------------------

//-----------------------------------------------------------------------
void QStreamTreeModel::childAdded(StreamWork::SwCore::SwComponent_Class * parent,
								  StreamWork::SwCore::SwComponent_Class *child)
{
	QModelIndex qparent;
	if ( parent == _rootComponent )
	{
		qparent = createIndex(0, 0, (void *) _rootComponent);
	}
	else
	{
		SwComponent_Class * parentparent = parent->GetParent();
		int iindex = parentparent->GetChildIndex(parent);
		qparent = createIndex(iindex, 0, (void *) parent);
	}
	int index = parent->GetChildIndex(child);
	beginInsertRows(qparent, index, index);
	recursiveConnectToControler(child);
	endInsertRows();
}

//-----------------------------------------------------------------------
void QStreamTreeModel::childRemoved(StreamWork::SwCore::SwComponent_Class * parent,
									StreamWork::SwCore::SwComponent_Class *child)
{
	QModelIndex qparent;
	if ( parent == _rootComponent )
	{
		qparent = createIndex(0, 0, (void *) _rootComponent);
	}
	else
	{
		SwComponent_Class * parentparent = parent->GetParent();
		int iindex = parentparent->GetChildIndex(parent);
		qparent = createIndex(iindex, 0, (void *) parent);
	}
	int index = parent->GetChildIndex(child);
	beginRemoveRows(qparent, index, index);
	recursiveDisconnectToControler(child);
	endRemoveRows();
}

//-----------------------------------------------------------------------
void QStreamTreeModel::componentNameChanged(StreamWork::SwCore::SwComponent_Class * component)
{
	QModelIndex qparent;
	if ( component == _rootComponent )
	{
		qparent = createIndex(0, 0, (void *) _rootComponent);
	}
	else
	{
		SwComponent_Class * parent = component->GetParent();
		int index = parent->GetChildIndex(component);
		qparent = createIndex(index, 0, (void *) _rootComponent);
	}
	dataChanged(qparent, qparent);
}

//-----------------------------------------------------------------------
void QStreamTreeModel::connectToControler(SwComponent_Class * component)
{
	component->OnAddChild.iconnect(*this, &QStreamTreeModel::childAdded);
	component->OnRemoveChild.iconnect(*this, &QStreamTreeModel::childRemoved);
	component->OnChangeComponentName.iconnect(*this, &QStreamTreeModel::componentNameChanged);
}

//-----------------------------------------------------------------------
void QStreamTreeModel::recursiveConnectToControler(SwComponent_Class * component)
{
	connectToControler(component);
	unsigned long count = component->GetNbChildren();
	for ( unsigned long i = 0; i < count; i++ )
	{
		recursiveConnectToControler(component->GetChild(i));
	}
}

//-----------------------------------------------------------------------
void QStreamTreeModel::disconnectFromControler(SwComponent_Class * component)
{
	component->OnAddChild.idisconnect(*this, &QStreamTreeModel::childAdded);
	component->OnRemoveChild.idisconnect(*this, &QStreamTreeModel::childRemoved);
	component->OnChangeComponentName.idisconnect(*this, &QStreamTreeModel::componentNameChanged);
}

//-----------------------------------------------------------------------
void QStreamTreeModel::recursiveDisconnectToControler(SwComponent_Class * component)
{
	disconnectFromControler(component);
	unsigned long count = component->GetNbChildren();
	for ( unsigned long i = 0; i < count; i++ )
	{
		recursiveDisconnectToControler(component->GetChild(i));
	}
}
//-----------------------------------------------------------------------
void QStreamTreeModel::setSelection(QList<SwComponent_Class *> & sel)
{
	emit layoutAboutToBeChanged();
	_selection = sel;
	emit layoutChanged();
}
//--------------------------------------------------------------------------
// events gui
//--------------------------------------------------------------------------

//-----------------------------------------------------------------------
void QStreamTreeModel::onSelectItem(const QModelIndex & index)
{
	if ( !index.isValid() )
		return;
	SwComponent_Class * cindex = (SwComponent_Class *) index.internalPointer();
	_controler->setSelection(cindex);
}
