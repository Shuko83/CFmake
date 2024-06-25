/*!
 \file _SwExportStreamTreeModel.cpp
 \brief Modele pour la selection de l'export model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
/*
  * INCLUDES LOCAUX
  */
#include "Main/SwApplication.h"
#include <QMessageBox>
#include <QMimeData>
#include "Tools/SwAddress_ToolBox.h"
#include "_SwExportStreamTreeModel.h"
#include "_SwModelHost_Class.h"
#include "Component/Services/ISwProperties.h"
#include "Properties/ISwProperty.h"
#include "Component/Services/ISwInterfaces_Provider.h"
#include "Component/Services/ISwInterfaces_Consumer.h"
#include "Component/Services/ISwPins_Manager.h"
#include "ISwExecutable_Service.h"
#include "Component/Services/ISwServiceOwnerConfigurable.h"
#include "Component/Interfaces/ISwActivable.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*! \brief Constructeur */
_SwExportStreamTreeModel::_SwExportStreamTreeModel(QObject * parent,SwComponent_Class * root_component):
												   QAbstractItemModel(parent){
    _root_component=root_component;
    _root_item=new _Item(NULL,_root_component,IT_Host,_root_component->GetName());
    BuildItems(_root_component,_root_item);
    _font_registered.setStrikeOut(true);
}
/*! \brief Destructeur*/
_SwExportStreamTreeModel::~_SwExportStreamTreeModel() {
    delete _root_item;
}
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwExportStreamTreeModel::flags ( const QModelIndex & index ) const {
    _Item * item;

    item=(_Item *)index.internalPointer();
    if (index.column()==0 && item->_type!=IT_Host) {
        return Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    }
    return Qt::ItemFlags();
}
/*! \brief list mimeTypes */
QStringList _SwExportStreamTreeModel::mimeTypes() const{
     QStringList types;
     types << "application/model_entity";
     return types;
}
/*! \brief acces a la mimeData correspondantes */
QMimeData * _SwExportStreamTreeModel::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mimeData = new QMimeData();
    QString text;
    _Item * item;

	for (QModelIndex index : indexes) {
        if (index.isValid()) {
            item=(_Item *)index.internalPointer();
            text=text+SwAddress_ToolBox::BuildAbsolutePath(item->_host);
            text+="@";
            switch (item->_type) {
                case IT_Property:
                    text+="PR";
                    break;
                case IT_Interface_C:
                    text+="IC";
                    break;
                case IT_Interface_P:
                    text+="IP";
                    break;
                case IT_Pin:
                    text+="PI";
                    break;
                case IT_Executable:
                    text+="EX";
                    break;
                case IT_OwnerConfigurable:
                    text+="CO";
                    break;  
                case IT_Activable:
                    text+="AC";
                    break;                                       
                default:
                    break;
            }
            text+="@";
            text+=item->_name;
            text+= ";";
        }
    }

    //mimeData->setText(text);
    mimeData->setData("application/model_entity",text.toLatin1());
    return mimeData;
}

/*! \brief Renvoie le nombre de colonne pour parent donné */
int _SwExportStreamTreeModel::columnCount ( const QModelIndex & parent ) const{
    return 1;
}
/*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwExportStreamTreeModel::rowCount ( const QModelIndex & parent ) const {
    _Item * item;

    if (!parent.isValid())
        return 1;

    item=(_Item *)parent.internalPointer();
    return item->_childs.count();
}
/*! \brief Renvoie les données d'entete */
QVariant _SwExportStreamTreeModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Entity"));
                break;
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant _SwExportStreamTreeModel::data ( const QModelIndex & index, int role ) const{
    _Item * item;
     if (!index.isValid())
         return QVariant();

    item=(_Item *)index.internalPointer();

    if (role == Qt::DisplayRole) {
        return QVariant(item->_name);
    }
    if (role == Qt::DecorationRole) {
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
            case IT_Host:
                return QVariant(SW_APP->ComponentsBank().GetComponentIcon(item->_host->GetFactoryName(), item->_host->GetFactoryComponentName()));
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex _SwExportStreamTreeModel::index ( int row, int column, const QModelIndex & parent  ) const{
    _Item * pitem;

    if (!parent.isValid())
        return createIndex(row,column,(void *)_root_item);

    pitem=(_Item *)parent.internalPointer();
    return createIndex(row,column,(void *)pitem->_childs[row]);
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwExportStreamTreeModel::parent ( const QModelIndex & index ) const{
    _Item * item;
    _Item * pitem;
    int index_parent;

    if (!index.isValid())
        return QModelIndex();
    item=(_Item *)index.internalPointer();
    if (item->_parent==NULL)
        return QModelIndex();
    //Calcul de l'indice du parent
    item=item->_parent;
    pitem=item->_parent;
    //Si le parent du parent est null, c'est le composant racine
    if (pitem==NULL) {
        return createIndex(0,0,(void *)_root_item);
    } else {
        //Sinon il faut calculer l'index
        index_parent=item->_parent->_childs.indexOf(item);
        if (index_parent==-1)
            return QModelIndex();
    }
    return createIndex(index_parent,0,(void *)item);
}
//-------------------------------------------------------------
// Definition de l'item
//------------------------------------------------------------
/*! \brief Constructeur */
_SwExportStreamTreeModel::_Item::_Item(_Item * parent,SwComponent_Class * host,_T_ItemType type,QString name) {
    _parent=parent;
    _host=host;
    _type=type;
    _name=name;
    if (_parent!=NULL) {
        _parent->_childs.push_back(this);
    }
}
/*! \brief Destructeur */
_SwExportStreamTreeModel::_Item::~_Item() {
	for (auto child : _childs) {
        delete child;
    }
    _childs.clear();
}
/*! \brief Construction des items */
void _SwExportStreamTreeModel::BuildItems(SwComponent_Class * comp,_Item * item) {
    //Creation du noeud executable si necessaire
    if (dynamic_cast<ISwActivable *>(comp)!= 0) 
    {
        new _Item(item,comp,IT_Activable,"Activable interface");
    }   
    //Creation du neoud executable si necessaire
    if (dynamic_cast<ISwExecutable_Service *>(comp->QueryService(CG_SW_SERVICE_EXECUTABLE))!=NULL) {
        new _Item(item,comp,IT_Executable,"Executable interface");
    }
    //Creation du neoud executable si necessaire
    if (dynamic_cast<ISwServiceOwnerConfigurable *>(comp->QueryService(CG_SW_SERVICE_OWNER_CONFIGURABLE))!=NULL) {
        new _Item(item,comp,IT_OwnerConfigurable,"OwnerConfigurable interface");
    }
 
    //Creation des neoud propriétés
    ISwProperties *  properties_handle=dynamic_cast<ISwProperties *>(comp->QueryService(CG_SW_SERVICE_PROPERTIES));
    if (properties_handle!=NULL) {
        QList<ISwProperty *> properties=properties_handle->GetProperties();
        for(int i=0;i<properties.count();i++) {
            if (!properties[i]->IsControlled() && !properties[i]->IsSubProperty()) {
                new _Item(item,comp,IT_Property,properties[i]->GetRealName());
            }
        }
    }
    //Creation des noeud interface consomme
    ISwInterfaces_Provider *provider_handle=dynamic_cast<ISwInterfaces_Provider *>(comp->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (provider_handle!=NULL) {
        QString interface_name;
        interface_name=provider_handle->GetFirstInterface();
        while (!interface_name.isEmpty()) {
            new _Item(item,comp,IT_Interface_P,interface_name);
            interface_name=provider_handle->GetNextInterface();
        }
    }
    //Creation des noeud interface produites
    ISwInterfaces_Consumer *consumer_handle=dynamic_cast<ISwInterfaces_Consumer *>(comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (consumer_handle!=NULL) {
        QString interface_name;
        interface_name=consumer_handle->GetFirstInterface(NULL,NULL,NULL);
        while (!interface_name.isEmpty()) {
            new _Item(item,comp,IT_Interface_C,interface_name);
            interface_name=consumer_handle->GetNextInterface(NULL,NULL,NULL);
        }
    }
    //Creation des noeud pins
    ISwPins_Manager *pins_manager_handle=dynamic_cast<ISwPins_Manager *>(comp->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (pins_manager_handle!=NULL) {
        QList<SwPin *> pins=pins_manager_handle->GetPinList();
        for(int i=0;i<pins.count();i++) {
            new _Item(item,comp,IT_Pin,pins[i]->GetName());
        }
    }
    //Creation des noeuds composants enfants
    SwComponent_Class * comp_child;
    comp_child=comp->GetFirstChild();
    while (comp_child!=NULL) {
        if (dynamic_cast<_SwModelHost_Class *>(comp)==NULL)
            BuildItems(comp_child,new _Item(item,comp_child,IT_Host,comp_child->GetName()));
        comp_child=comp->GetNextChild();
    }
}
