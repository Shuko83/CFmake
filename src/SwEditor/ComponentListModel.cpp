/**
@file ComponentListModel.cpp
@brief Qt Model pour la liste des composants d'un plugin
@author F.Bighelli
 */

#include "ComponentListModel.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
ComponentListModel::ComponentListModel( QObject * parent):QAbstractListModel(parent) 
{
	_plugin=0;

	_allPlugins = SW_APP->ComponentsBank().GetAllPlugins();

	QMapIterator<QString, StreamWork::SwCore::SwPluginFactory_Class *> it(*_allPlugins);
	while (it.hasNext()) 
	{
		it.next();
		_componentPlugin.insert(it.value()->GetPluginName(),it.value()->GetComponentsList().toList());

	}
}
/** @brief rowCount */
int ComponentListModel::rowCount ( const QModelIndex & parent) const{
    return _map.count();
} 
/** @brief index */
QModelIndex ComponentListModel::index ( int row, int column, const QModelIndex & parent  ) const{
    return createIndex(row,column);
}
/** @brief data */
QVariant ComponentListModel::data ( const QModelIndex & index, int role ) const {
    if (role==Qt::DisplayRole) {
        return QVariant(_map[index.row()].componentName); 
    }
    if (role==Qt::DecorationRole) {
        return QVariant(_map[index.row()].icon); 
    }
    if (role==Qt::ToolTipRole) {
        return QVariant(_map[index.row()].desc); 
    }
	if (role==Qt::UserRole) {
		return QVariant(_map[index.row()].pluginName); 
	}
    return QVariant();
}
/*! \brief Flags du modele*/
Qt::ItemFlags ComponentListModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsDragEnabled |  QAbstractListModel::flags(index);
}
/*! \brief list mimeTypes */
QStringList ComponentListModel::mimeTypes() const{
     QStringList types;
     types << "application/mod_list";
     types << "application/mod";
     return types;
}
/*! \brief acces a la mimeData correspondantes */
QMimeData * ComponentListModel::mimeData(const QModelIndexList &indexes) const{
     QMimeData *mimeData = new QMimeData();
     QString text;

     if (indexes.count()==1) {
         text=data(indexes.front(), Qt::DisplayRole).toString();
         mimeData->setData("application/mod",text.toLatin1());
         return mimeData;
     }

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             text=text+data(index, Qt::DisplayRole).toString();
             text+= ";";
         }
     }

     //mimeData->setText(text);
     mimeData->setData("application/mod_list",text.toLatin1());
     return mimeData;
}

/** @brief sur changement du plugin activé */
void ComponentListModel::onSelectedPluginChanged(const QModelIndex & index) 
{
	_map.clear();
    if (index.isValid())
	{
		beginResetModel();

		_plugin=static_cast<SwPluginFactory_Class *>(index.internalPointer());
		if(_plugin)
		{
			int i =0;
			QList<QString> sortedList =  _plugin->GetComponentsList().toList();
			qSort(sortedList);
			foreach(QString componentName,sortedList)
			{
				InfoCompo tmp;
				tmp.componentName = componentName;
				tmp.icon = _plugin->GetComponentIcon(componentName);
				tmp.desc = _plugin->GetComponentDescription(componentName);
				tmp.pluginName = _plugin->GetPluginName();
				_map.insert(i,tmp);
				i++;
			}
		}
        //_cList=_plugin->GetComponentsList().toList();
        //_cList.sort();
        endResetModel();
    } else {
        clear();
    }
}

//-------------------------------------------------------------------------
void ComponentListModel::clear()
{
	beginResetModel();
	_plugin=0;
	_map.clear();
	endResetModel();
}

//-------------------------------------------------------------------------
void ComponentListModel::manageList(QStringList listComponent) 
{
	_map.clear();

	if (!listComponent.isEmpty()) 
	{
		beginResetModel();
		int indice=0;
 		foreach(QString componentName,listComponent)
 		{
 			QMapIterator<QString, QStringList> i(_componentPlugin);
 			while (i.hasNext()) 
 			{
 				i.next();
 				if(i.value().contains(componentName))
 				{
 					InfoCompo tmp;
 					tmp.componentName = componentName;
 					tmp.icon =_allPlugins->value(i.key())->GetComponentIcon(componentName);
 					tmp.desc = _allPlugins->value(i.key())->GetComponentDescription(componentName);
 					tmp.pluginName = _allPlugins->value(i.key())->GetPluginName();
 					_map.insert(indice,tmp);
 					indice++;
 				}
 			}
 		}
		endResetModel();
	} 
	else 
	{
		clear();
	}
}