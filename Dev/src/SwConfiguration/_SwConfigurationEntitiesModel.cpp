/*!
 \file _SwConfigurationEntitiesModel.cpp
 \brief Modele et conteneur des données a exporter
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
/*
  * INCLUDES LOCAUX
  */
#include <math.h>
#include <SwApplication.h>
#include <QMessageBox>
#include <QMimeData>
#include <QStringList>
#include <SwAddress_ToolBox.h>
#include <SwTime_ToolBox.h>
#include "_SwConfigurationEntitiesModel.h"
#include "_SwConfiguration.h"
#include <_SwConfigurationExportedEntity.h>
using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwConfigurationEntitiesModel::_SwConfigurationEntitiesModel(QObject * parent,SwComponent_Class * root_component,QList<_SwConfigurationExportedEntity *> * entities_list):
												   QAbstractItemModel(parent){
    _root_component=root_component;
    _entities_list=entities_list;
	_isLimitedToProperties = false;
    TransformEntitiesToModel();
}
/*! \brief Destructeur*/
_SwConfigurationEntitiesModel::~_SwConfigurationEntitiesModel() {
}
/*! \brief Transformation liste entites en modele */
void _SwConfigurationEntitiesModel::TransformEntitiesToModel() {
    for(int i=0;i<_entities_list->count();i++) {
        switch((*_entities_list)[i]->_type) {
            case _SwConfigurationExportedEntity::Ent_Property:
                _properties.push_back(new _Item(IT_Property,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path));
                break;
            case _SwConfigurationExportedEntity::Ent_Perspective:
                _perspectives.push_back(new _Item(IT_Perspective,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path));
                break;
            case _SwConfigurationExportedEntity::Ent_Execution:
            	_executions.push_back(new _Item(IT_Execution,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path));
                break;
            case _SwConfigurationExportedEntity::Ent_OwnerConfigurable:
                _ownerConfigurables.push_back(new _Item(IT_OwnerConfigurable,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path));
                break;
            default:
                break;
        }
    }
}
/*! \brief Transformation modele en liste entites */
void _SwConfigurationEntitiesModel::TransformModelToEntities() {
    //Nettoyage de la liste precedente
    for(int i=0;i<_entities_list->count();i++) {
        delete (*_entities_list)[i];
    }
    _entities_list->clear();
    //Reconstruction
    for(int i=0;i<_properties.count();i++) {
        _SwConfigurationExportedEntity * entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Property);
        entity->_name=_properties[i]->_name;
        entity->_exported_name=_properties[i]->_exported_name;
        entity->_host_path=_properties[i]->_host_path;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_perspectives.count();i++) {
        _SwConfigurationExportedEntity * entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Perspective);
        entity->_name=_perspectives[i]->_name;
        entity->_exported_name=_perspectives[i]->_exported_name;
        entity->_host_path=_perspectives[i]->_host_path;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_executions.count();i++) {
        _SwConfigurationExportedEntity * entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Execution);
        entity->_name=_executions[i]->_name;
        entity->_exported_name=_executions[i]->_exported_name;
        entity->_host_path=_executions[i]->_host_path;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_ownerConfigurables.count();i++) {
        _SwConfigurationExportedEntity * entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_OwnerConfigurable);
        entity->_name=_ownerConfigurables[i]->_name;
        entity->_exported_name=_ownerConfigurables[i]->_exported_name;
        entity->_host_path=_ownerConfigurables[i]->_host_path;
        _entities_list->push_back(entity);
    }
}
/*! \brief Suppression des elements selectionnées */
void _SwConfigurationEntitiesModel::RemoveSelectedItems(QModelIndexList list_to_remove) {
    _Item * item;
    int index;

    emit layoutAboutToBeChanged();
    for (int i=0;i<list_to_remove.count();i++) {
        item=(_Item *)list_to_remove[i].internalPointer();
        if (item!=NULL) {
            switch (item->_type) {
                case IT_Property:
                    index=_properties.indexOf(item);
                    if (index!=-1) {
                        _properties.removeAt(index);
                        delete item;
                    }
                    break;
                case IT_Perspective:
                    index=_perspectives.indexOf(item);
                    if (index!=-1) {
                        _perspectives.removeAt(index);
                        delete item;
                    }
                    break;
                case IT_Execution:
                    index=_executions.indexOf(item);
                    if (index!=-1) {
                        _executions.removeAt(index);
                        delete item;
                    }
                    break;
                case IT_OwnerConfigurable:
                    index=_ownerConfigurables.indexOf(item);
                    if (index!=-1) {
                        _ownerConfigurables.removeAt(index);
                        delete item;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    emit layoutChanged();
}
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwConfigurationEntitiesModel::flags ( const QModelIndex & index ) const {
    _Item * item;
    if (index.internalPointer()!=NULL) {
        item=(_Item *)index.internalPointer();
        if (index.column()==0 && item->_type!=IT_Execution) {
            return Qt::ItemFlags(Qt::ItemIsEnabled |Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled);
        } else {
            return Qt::ItemFlags(Qt::ItemIsEnabled |Qt::ItemIsSelectable | Qt::ItemIsDropEnabled);
        }
    }
    return Qt::ItemFlags(Qt::ItemIsEnabled);
}
/*! \brief Renvoie le nombre de colonne pour parent donné */
int _SwConfigurationEntitiesModel::columnCount ( const QModelIndex & parent ) const{
    return 3;
}
/*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwConfigurationEntitiesModel::rowCount ( const QModelIndex & parent ) const {
    if (!parent.isValid())
	{
		if(!_isLimitedToProperties)
			return 4;
		else
			return 1;
	}
	
    if (parent.isValid() && parent.internalPointer()==NULL) {
        switch(parent.row()) {
            case 0:
                return _properties.count();
            case 1:
                return _perspectives.count();
            case 2:
                return _executions.count();
            case 3:
                return _ownerConfigurables.count();
            default:
                break;
        }
    }
    return 0;
}
/*! \brief Renvoie les données d'entete */
QVariant _SwConfigurationEntitiesModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Entity"));
            case 1:
                return QVariant(QString("Name on host"));
            case 2:
                return QVariant(QString("Host path"));
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant _SwConfigurationEntitiesModel::data ( const QModelIndex & index, int role ) const{
    _Item * item;

     if (!index.isValid())
         return QVariant();
    item=(_Item *)index.internalPointer();
    if (item==NULL)  
	{
        if (role == Qt::DisplayRole && index.column()==0) 
		{
            switch(index.row()) 
			{
                case 0:
                    return QVariant(QString("Properties"));
                case 1:
                    return QVariant(QString("Perspectives"));
				case 2:
                    return QVariant(QString("Execution"));
				case 3:
                    return QVariant(QString("Configurable"));
                default:
                    break;
            }
        }
        return QVariant();
    } else {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if (index.column()==0)
                return QVariant(item->_exported_name);
            if (index.column()==1)
                return QVariant(item->_name);
            if (index.column()==2)
                return QVariant(item->_host_path);
        }
        if (role == Qt::DecorationRole && index.column()==0) {
            switch (item->_type) {
                case IT_Property:
                    return QVariant(QIcon(":/SwConfiguration/property.png"));
                case IT_Perspective:
                    return QVariant(QIcon(":/SwConfiguration/interface_out.png"));
				case IT_Execution:
                    return QVariant(QIcon(":/SwConfiguration/executor.png"));
				case IT_OwnerConfigurable:
                    return QVariant(QIcon(":/SwConfiguration/configuration.png"));
                default:
                    break;
            }
        }
    }
    return QVariant();
}
/*! \brief Edition*/
bool _SwConfigurationEntitiesModel::setData ( const QModelIndex & index, const QVariant & value, int role) {
    _Item * item;
    if (role == Qt::EditRole && index.internalPointer()!=NULL) {
        if (value.toString().isEmpty()) {
            QMessageBox::warning(0,"Export name edition","Invalid name");
            return false;
        }
        item=(_Item *)index.internalPointer();
        switch (item->_type) {
            case IT_Property:
                for(int i=0;i<_properties.count();i++) {
                    if (_properties[i]!=item && _properties[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other property has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_Perspective:
                for(int i=0;i<_perspectives.count();i++) {
                    if (_perspectives[i]!=item && _perspectives[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other provided interface has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_Execution:
                for(int i=0;i<_executions.count();i++) {
                    if (_executions[i]!=item && _executions[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other execution has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_OwnerConfigurable:
                for(int i=0;i<_ownerConfigurables.count();i++) {
                    if (_ownerConfigurables[i]!=item &&_ownerConfigurables[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other persistant has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            default:
                break;
        }
    }
    return false;
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex _SwConfigurationEntitiesModel::index ( int row, int column, const QModelIndex & parent  ) const{
    if (!parent.isValid())
        return createIndex(row,column,(void *)NULL);
    switch(parent.row()) {
        case 0:
            return createIndex(row,column,(void *)_properties[row]);
        case 1:
            return createIndex(row,column,(void *)_perspectives[row]);
        case 2:
            return createIndex(row,column,(void *)_executions[row]);
        case 3:
            return createIndex(row,column,(void *)_ownerConfigurables[row]);
        default:
            break;
    }
    return QModelIndex();
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwConfigurationEntitiesModel::parent ( const QModelIndex & index ) const{
    _Item * item;

    if (!index.isValid())
        return QModelIndex();
    if (index.internalPointer()==NULL) {
        return QModelIndex();
    }
    item=(_Item *)index.internalPointer();
    switch (item->_type) {
        case IT_Property:
            return createIndex(0,index.column(),(void *)NULL);
        case IT_Perspective:
            return createIndex(1,index.column(),(void *)NULL);
        case IT_Execution:
            return createIndex(2,index.column(),(void *)NULL);
        case IT_OwnerConfigurable:
            return createIndex(3,index.column(),(void *)NULL);
        default:
            break;
    }
    return QModelIndex();
}
/*! \brief Action de drop */
bool _SwConfigurationEntitiesModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent ) {
    if (data->hasFormat("application/model_entity")) {
        emit layoutAboutToBeChanged();
        QString drop_data=QString(data->data("application/model_entity").data());
        QStringList newItems=drop_data.split(";",QString::SkipEmptyParts);
        for(int i=0;i<newItems.count();i++) {
            QStringList item_parts=newItems[i].split("@",QString::SkipEmptyParts);
            QString source=item_parts[0];source.replace('.','_');
            QString type=item_parts[1];
            QString name=item_parts[2];
            QString exported_name=name+"_exported_from_"+source; //QString("%1").arg((uint)floor(SwTime_ToolBox::GetTime()));
            if (type==QString("PR")) {
                _properties.push_back(new _Item(IT_Property,name,exported_name,item_parts[0]));
            }
            if (type==QString("IP")) {
                _perspectives.push_back(new _Item(IT_Perspective,name,exported_name,item_parts[0]));
            }
            if (type==QString("EX") && _executions.count()==0) {
                _executions.push_back(new _Item(IT_Execution,"Executable interface","Executable interface",item_parts[0]));
            }
            if (type==QString("CO") ) {
               _ownerConfigurables.push_back(new _Item(IT_OwnerConfigurable,"Persistant interface","Persistant interface",item_parts[0]));
            }
        }
        emit layoutChanged();
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
void _SwConfigurationEntitiesModel::limitToProperty()
{
	_isLimitedToProperties = true;
}

