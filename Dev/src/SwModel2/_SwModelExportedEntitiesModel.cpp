/*!
 \file _SwModelExportedEntitiesModel.cpp
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
#include "_SwModelExportedEntitiesModel.h"
#include "_SwModelHost_Class.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelExportedEntitiesModel::_SwModelExportedEntitiesModel(QObject * parent,SwComponent_Class * root_component,QList<_SwModelExportedEntity *> * entities_list):
												   QAbstractItemModel(parent){
    _root_component=root_component;
    _entities_list=entities_list;
    TransformEntitiesToModel();
}
/*! \brief Destructeur*/
_SwModelExportedEntitiesModel::~_SwModelExportedEntitiesModel() {
}
/*! \brief Transformation liste entites en modele */
void _SwModelExportedEntitiesModel::TransformEntitiesToModel() {
    for(int i=0;i<_entities_list->count();i++) {
        switch((*_entities_list)[i]->_type) {
            case Ent_Property:
                _properties.push_back(new _Item(IT_Property,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path,QString()));
                break;
            case Ent_InterfaceC:
                _interfacesC.push_back(new _Item(IT_Interface_C,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,QString(),(*_entities_list)[i]->_itype));
                break;
            case Ent_InterfaceP:
                _interfacesP.push_back(new _Item(IT_Interface_P,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,QString(),(*_entities_list)[i]->_itype));
                break;
            case Ent_Pin:
                _pins.push_back(new _Item(IT_Pin,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,QString(),(*_entities_list)[i]->_itype));
                break;
            case Ent_Executable:
            	_executables.push_back(new _Item(IT_Executable,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path,QString()));
                break;
            case Ent_OwnerConfigurable:
            	_ownerConfigurables.push_back(new _Item(IT_OwnerConfigurable,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path,QString()));
                break;    
            case Ent_Activable:
            	_activables.push_back(new _Item(IT_Activable,(*_entities_list)[i]->_name,(*_entities_list)[i]->_exported_name,(*_entities_list)[i]->_host_path,QString()));
                break;                               
            default:
                break;
        }
    }
}
/*! \brief Transformation modele en liste entites */
void _SwModelExportedEntitiesModel::TransformModelToEntities() {
    //Nettoyage de la liste precedente
    for(int i=0;i<_entities_list->count();i++) {
        delete (*_entities_list)[i];
    }
    _entities_list->clear();
    //Reconstruction
    for(int i=0;i<_properties.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_Property);
        entity->_name=_properties[i]->_name;
        entity->_exported_name=_properties[i]->_exported_name;
        entity->_host_path=_properties[i]->_host_path;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_interfacesC.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_InterfaceC);
        entity->_name=_interfacesC[i]->_name;
        entity->_exported_name=_interfacesC[i]->_exported_name;
        entity->_itype=_interfacesC[i]->_itype;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_interfacesP.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_InterfaceP);
        entity->_name=_interfacesP[i]->_name;
        entity->_exported_name=_interfacesP[i]->_exported_name;
        entity->_host_path=_interfacesP[i]->_host_path;
        entity->_itype=_interfacesP[i]->_itype;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_pins.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_Pin);
        entity->_name=_pins[i]->_name;
        entity->_exported_name=_pins[i]->_exported_name;
        entity->_host_path=_pins[i]->_host_path;
        entity->_itype=_pins[i]->_itype;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_executables.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_Executable);
        entity->_name=_executables[i]->_name;
        entity->_exported_name=_executables[i]->_exported_name;
        entity->_host_path=_executables[i]->_host_path;
        _entities_list->push_back(entity);
    }
    for(int i=0;i<_ownerConfigurables.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_OwnerConfigurable);
        entity->_name=_ownerConfigurables[i]->_name;
        entity->_exported_name=_ownerConfigurables[i]->_exported_name;
        entity->_host_path=_ownerConfigurables[i]->_host_path;
        _entities_list->push_back(entity);
    }    
    for(int i=0;i<_activables.count();i++) {
        _SwModelExportedEntity * entity=_SwModelExportedEntity::NewEntity(Ent_Activable);
        entity->_name=_activables[i]->_name;
        entity->_exported_name=_activables[i]->_exported_name;
        entity->_host_path=_activables[i]->_host_path;
        _entities_list->push_back(entity);
    }     
}
/*! \brief Suppression des elements selectionnées */
void _SwModelExportedEntitiesModel::RemoveSelectedItems(QModelIndexList list_to_remove) {
    _Item * item;
    int index;

    //emit layoutAboutToBeChanged();
    for (int i=0;i<list_to_remove.count();i++) {
        item=(_Item *)list_to_remove[i].internalPointer();
        
        if (item!=NULL) {
            switch (item->_type) {
                case IT_Property:
                    index=_properties.indexOf(item);                  
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _properties.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;
                case IT_Interface_C:
                    index=_interfacesC.indexOf(item);
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _interfacesC.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;
                case IT_Interface_P:
                    index=_interfacesP.indexOf(item);
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _interfacesP.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;
                case IT_Pin:
                    index=_pins.indexOf(item);
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _pins.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;
                case IT_Executable:
                    index=_executables.indexOf(item);
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _executables.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;
                case IT_OwnerConfigurable:
                    index=_ownerConfigurables.indexOf(item);
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _ownerConfigurables.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;    
                case IT_Activable:
                    index=_activables.indexOf(item);
                    if (index!=-1) {
                        beginRemoveRows(parent(list_to_remove[i]),index,index);
                        _activables.removeAt(index);
                        delete item;
                        endRemoveRows();
                    }
                    break;                                       
                default:
                    break;
            }
        }
    }
    //emit layoutChanged();
}
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwModelExportedEntitiesModel::flags ( const QModelIndex & index ) const {
    _Item * item;
    if (index.internalPointer()!=NULL) {
        item=(_Item *)index.internalPointer();
        if (index.column()==0 && item->_type!=IT_Executable && item->_type!=IT_Activable) {
            return Qt::ItemFlags(Qt::ItemIsEnabled |Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled);
        } else {
            return Qt::ItemFlags(Qt::ItemIsEnabled |Qt::ItemIsSelectable | Qt::ItemIsDropEnabled);
        }
    }
    return Qt::ItemFlags(Qt::ItemIsEnabled);
}
/*! \brief Renvoie le nombre de colonne pour parent donné */
int _SwModelExportedEntitiesModel::columnCount ( const QModelIndex & parent ) const{
    return 4;
}
/*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwModelExportedEntitiesModel::rowCount ( const QModelIndex & parent ) const {
    if (!parent.isValid())
        return 7;
    if (parent.isValid() && parent.internalPointer()==NULL) {
        switch(parent.row()) {
            case 0:
                return _properties.count();
            case 1:
                return _interfacesC.count();
            case 2:
                return _interfacesP.count();
            case 3:
                return _pins.count();
            case 4:
                return _executables.count();
            case 5:
                return _ownerConfigurables.count();     
            case 6:
                return _activables.count();                                 
            default:
                break;
        }
    }
    return 0;
}
/*! \brief Renvoie les données d'entete */
QVariant _SwModelExportedEntitiesModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Entity"));
            case 1:
                return QVariant(QString("Name on host"));
            case 2:
                return QVariant(QString("Host path"));
            case 3:
                return QVariant(QString("Type"));
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant _SwModelExportedEntitiesModel::data ( const QModelIndex & index, int role ) const{
    _Item * item;

     if (!index.isValid())
         return QVariant();
    item=(_Item *)index.internalPointer();
    if (item==NULL)  {
        if (role == Qt::DisplayRole && index.column()==0) {
            switch(index.row()) {
                case 0:
                    return QVariant(QString("Properties"));
                case 1:
                    return QVariant(QString("Consumed interfaces"));
                case 2:
                    return QVariant(QString("Provided interfaces"));
                case 3:
                    return QVariant(QString("Pins"));
                case 4:
                    return QVariant(QString("Executable"));
                case 5:
                    return QVariant(QString("OwnerConfigurables"));     
                case 6:
                    return QVariant(QString("Activable"));                  
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
            if (index.column()==3)
                return QVariant(item->_itype);
        }
        if (role == Qt::DecorationRole && index.column()==0) {
            switch (item->_type) {
                case IT_Property:
                    return QVariant(QIcon(":/SwModel/property.png"));
                case IT_Interface_C:
                    return QVariant(QIcon(":/SwModel/interface_in.png"));
                case IT_Interface_P:
                    return QVariant(QIcon(":/SwModel/interface_out.png"));
                case IT_Pin:
                    return QVariant(QIcon(":/SwModel/connector.png"));
                case IT_Executable:
                    return QVariant(QIcon(":/SwModel/executor.png"));
                case IT_OwnerConfigurable:
                    return QVariant(QIcon(":/SwModel/connector.png"));  
                case IT_Activable:
                    return QVariant(QIcon(":/SwModel/executor.png"));                  
                default:
                    break;
            }
        }
    }
    return QVariant();
}
/*! \brief Edition*/
bool _SwModelExportedEntitiesModel::setData ( const QModelIndex & index, const QVariant & value, int role) {
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
            case IT_Interface_C:
                for(int i=0;i<_interfacesC.count();i++) {
                    if (_interfacesC[i]!=item && _interfacesC[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other consumed interface has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_Interface_P:
                for(int i=0;i<_interfacesP.count();i++) {
                    if (_interfacesP[i]!=item && _interfacesP[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other provided interface has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_Pin:
                for(int i=0;i<_pins.count();i++) {
                    if (_pins[i]!=item && _pins[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other pin has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_Executable:
                for(int i=0;i<_executables.count();i++) {
                    if (_executables[i]!=item && _executables[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other executable has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;
            case IT_OwnerConfigurable:
                for(int i=0;i<_ownerConfigurables.count();i++) {
                    if (_ownerConfigurables[i]!=item && _ownerConfigurables[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other executable has this exported name");
                        return false;
                    }
                }
                item->_exported_name=value.toString();
                emit dataChanged(index,index);
                return true;   
            case IT_Activable:
                for(int i=0;i<_activables.count();i++) {
                    if (_activables[i]!=item && _activables[i]->_exported_name==value.toString()) {
                        QMessageBox::warning(0,"Export name edition","This name can't be used because an other activable has this exported name");
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
QModelIndex _SwModelExportedEntitiesModel::index ( int row, int column, const QModelIndex & parent  ) const{
    if (!parent.isValid())
        return createIndex(row,column,(void *)NULL);
    switch(parent.row()) {
        case 0:
            return createIndex(row,column,(void *)_properties[row]);
        case 1:
            return createIndex(row,column,(void *)_interfacesC[row]);
        case 2:
            return createIndex(row,column,(void *)_interfacesP[row]);
        case 3:
            return createIndex(row,column,(void *)_pins[row]);
        case 4:
            return createIndex(row,column,(void *)_executables[row]);
        case 5:
            return createIndex(row,column,(void *)_ownerConfigurables[row]);        
        case 6:
            return createIndex(row,column,(void *)_activables[row]);                    
        default:
            break;
    }
    return QModelIndex();
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwModelExportedEntitiesModel::parent ( const QModelIndex & index ) const{
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
        case IT_Interface_C:
            return createIndex(1,index.column(),(void *)NULL);
        case IT_Interface_P:
            return createIndex(2,index.column(),(void *)NULL);
        case IT_Pin:
            return createIndex(3,index.column(),(void *)NULL);
        case IT_Executable:
            return createIndex(4,index.column(),(void *)NULL);
        case IT_OwnerConfigurable:
            return createIndex(5,index.column(),(void *)NULL);            
        case IT_Activable:
            return createIndex(6,index.column(),(void *)NULL);               
        default:
            break;
    }
    return QModelIndex();
}
/*! \brief Action de drop */
bool _SwModelExportedEntitiesModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent ) {
    if (data->hasFormat("application/model_entity")) {
        emit layoutAboutToBeChanged();
        SwComponent_Class * host;
        QString drop_data=QString(data->data("application/model_entity").data());
        QStringList newItems=drop_data.split(";",QString::SkipEmptyParts);
        for(int i=0;i<newItems.count();i++) {
            QStringList item_parts=newItems[i].split("@",QString::SkipEmptyParts);
            QString source=item_parts[0];source.replace('.','_');
            QString type=item_parts[1];
            QString name=item_parts[2];
            QString exported_name=name+"_exported_from_"+source; //QString("%1").arg((uint)floor(SwTime_ToolBox::GetTime()));
            if (type==QString("PR")) {
                _properties.push_back(new _Item(IT_Property,name,exported_name,item_parts[0],QString()));
            }
            if (type==QString("IC")) {
                host=SwAddress_ToolBox::FindTarget(item_parts[0],_root_component);
                ISwInterfaces_Consumer * host_consumer=dynamic_cast<ISwInterfaces_Consumer *>(host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
                _interfacesC.push_back(new _Item(IT_Interface_C,name,exported_name,QString(),host_consumer->GetInterfaceType(name)));
            }
            if (type==QString("IP")) {
                host=SwAddress_ToolBox::FindTarget(item_parts[0],_root_component);
                ISwInterfaces_Provider * host_provider=dynamic_cast<ISwInterfaces_Provider *>(host->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
                _interfacesP.push_back(new _Item(IT_Interface_P,name,exported_name,QString(),host_provider->GetInterfaceType(name)));
            }
            if (type==QString("PI")) {
                host=SwAddress_ToolBox::FindTarget(item_parts[0],_root_component);
                ISwPins_Manager * host_pin_manager=dynamic_cast<ISwPins_Manager *>(host->QueryService(CG_SW_SERVICE_PINS_MANAGER));  
                SwPin * host_pin =host_pin_manager->GetPinByName(name);
                _pins.push_back(new _Item(IT_Pin,name,exported_name,QString(),host_pin->GetType()));
            }
            if (type==QString("EX") && _executables.count()==0) {
                _executables.push_back(new _Item(IT_Executable,"Executable interface","Executable interface",item_parts[0],QString()));
            }
            if (type==QString("CO") && _ownerConfigurables.count()==0) {
                _ownerConfigurables.push_back(new _Item(IT_OwnerConfigurable,"OwnerConfigurable interface","OwnerConfigurable interface",item_parts[0],QString()));
            }     
            if (type==QString("AC") && _activables.count()==0) {
                _activables.push_back(new _Item(IT_Activable,"Activable interface","Activable interface",item_parts[0],QString()));
            }                   
        }
        emit layoutChanged();
        return true;
    }
    return false;
}

