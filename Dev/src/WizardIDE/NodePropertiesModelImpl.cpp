/*!
 \file _NodePropertiesModelImpl.h
 \brief Definit un modele a partir d'une liste de propriétés d'un composant noeud
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#include <QStringList>
#include <QColor>
/*
  * INCLUDES LOCAUX
  */
#include "NodePropertiesModelImpl.h"


using namespace StreamWork::SwCore;

/*! \brief constructeur */
NodePropertiesModelImpl::NodePropertiesModelImpl(QObject * parent):QAbstractItemModel(parent) {
    _root_item=NULL;
    _properties=NULL;
}
/*! \brief destructeur */
NodePropertiesModelImpl::NodePropertiesModelImpl() {
    //Si l'ancienne collection de propriétés est definie
    if (_properties!=NULL) {
        //Desenregistrement des signaux
        _properties->GetOnBeforePropertiesChange().idisconnect(*this,&NodePropertiesModelImpl::OnBeforeChange);
        _properties->GetOnAfterPropertiesChange().idisconnect(*this,&NodePropertiesModelImpl::OnAfterChange);
        //Destruction des items
        delete _root_item;
    }
}
/*! \brief creation d'un item */
void  NodePropertiesModelImpl::CreateItem(ISwProperty * property) {
    PropertyItem * item;
    PropertyItem * c_item;
    QStringList liste=property->GetRealName().split(".");
    int liste_size=liste.count();
    QMap<QString,PropertyItem *>::iterator it;

    item=_root_item;
    for (int i=0;i<liste_size;i++) {
        it=item->_childrens.find(liste[i]);
        if (it==item->_childrens.end()) {
            if (i==liste_size-1) 
                c_item=new PropertyItem(this,liste[i],property);
            else 
                c_item=new PropertyItem(this,liste[i]);
            c_item->_parent=item;
            item->_childrens.insert(liste[i],c_item);
        } else {
            c_item=it.value();    
        }
        item=c_item;
    }
}
/*! \brief destruction d'un item */
void NodePropertiesModelImpl::DestroyItem(ISwProperty * property) {
    PropertyItem * item;
    PropertyItem * c_item;
    PropertyItem * d_item;
    QStringList liste=property->GetRealName().split(".");
    int liste_size=liste.count();
    QMap<QString,PropertyItem *>::iterator it;

    item=_root_item;
    for (int i=0;i<liste_size;i++) {
        it=item->_childrens.find(liste[i]);
        if (it!=item->_childrens.end()) {
            c_item=it.value();  
            if (i==liste_size-1) {
                //Item final
                d_item=c_item;
                c_item=c_item->_parent;
                delete d_item;
                c_item->_childrens.erase(it);
                while (c_item!=_root_item && c_item->_property==NULL && c_item->_childrens.count()==0) {
                    d_item=c_item;
                    c_item=c_item->_parent;
                    it=c_item->_childrens.find(d_item->_label);
                    delete d_item;
                    c_item->_childrens.erase(it);        
                }
                return;
            }
        } 
        item=c_item;
    }
}
/*! \brief definit les propriétés */
void NodePropertiesModelImpl::SetProperties(ISwProperties * properties,QString root_name) {
    QList<ISwProperty *> properties_list;

    if (_properties!=properties) {
        //Signal avant changement
        
        //Si l'ancienne collection de propriétés est definie
        if (_properties!=NULL) {
            //Desenregistrement des signaux
            _properties->GetOnBeforePropertiesChange().idisconnect(*this,&NodePropertiesModelImpl::OnBeforeChange);
            _properties->GetOnAfterPropertiesChange().idisconnect(*this,&NodePropertiesModelImpl::OnAfterChange);
            _properties->GetOnCreateProperty().idisconnect(*this,&NodePropertiesModelImpl::OnCreateProperty);
            _properties->GetOnDestroyProperty().idisconnect(*this,&NodePropertiesModelImpl::OnDestroyProperty);
            //Destruction des items
            delete _root_item;
        }
        //Enregistrement de la nouvelle collection de propriétés
        _properties=properties;
        if (_properties!=NULL) {
            //Enregistrement des signaux
            _properties->GetOnBeforePropertiesChange().iconnect(*this,&NodePropertiesModelImpl::OnBeforeChange);
            _properties->GetOnAfterPropertiesChange().iconnect(*this,&NodePropertiesModelImpl::OnAfterChange);
            _properties->GetOnCreateProperty().iconnect(*this,&NodePropertiesModelImpl::OnCreateProperty);
            _properties->GetOnDestroyProperty().iconnect(*this,&NodePropertiesModelImpl::OnDestroyProperty);
            //Creation des items
            _root_item=new PropertyItem(this,root_name);
            properties_list=_properties->GetProperties();
            for (int i=0;i<properties_list.count();i++) {
                if (properties_list[i]->IsVisible())
                    CreateItem(properties_list[i]);
            }
        }
        //Signal apres changement
        reset();
    }
}
/*! \brief callback avant changement */
void NodePropertiesModelImpl::OnBeforeChange(ISwProperties * properties) {
    //A Revoir pour la destrcution
    //emit layoutAboutToBeChanged();
    //modelAboutToBeReset();
    reset();
}
/*! \brief callback apres changement */
void NodePropertiesModelImpl::OnAfterChange(ISwProperties * properties) {
    //emit layoutChanged();
    //modelReset();
}
/*! \brief callback sur creation de propriétés */
void NodePropertiesModelImpl::OnCreateProperty(ISwProperties * properties,ISwProperty * property) {
    CreateItem(property);    
}
/*! \brief ccallback sur destruction de propriétés*/
void NodePropertiesModelImpl::OnDestroyProperty(ISwProperties * properties,ISwProperty * property) {
    DestroyItem(property);
}
//-------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags NodePropertiesModelImpl::flags ( const QModelIndex & index ) const {
    PropertyItem *item;

    if (_properties==NULL || !index.isValid()) 
        return Qt::ItemFlags(Qt::ItemIsEnabled);
    item=(PropertyItem *)index.internalPointer();
    if (item->_property==NULL) {
        return Qt::ItemFlags(Qt::ItemIsEnabled);       
    }
    if (index.column()==1 && item->_property->IsEditable())
        return Qt::ItemFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable| Qt::ItemIsSelectable);

    return Qt::ItemFlags(Qt::ItemIsEnabled);
}
/*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
int NodePropertiesModelImpl::columnCount ( const QModelIndex & parent) const{
    return 2;
}			
    /*! \brief Renvoie le nombre de ligne pour un parent donné */
int NodePropertiesModelImpl::rowCount ( const QModelIndex & parent ) const{
    PropertyItem *iparent;

    if (_properties==NULL) 
        return 0;
    if (!parent.isValid())
        return _root_item->_childrens.count(); 
    iparent=(PropertyItem *)parent.internalPointer();
    return iparent->_childrens.count(); 
}
/*! \brief Renvoie les données d'entete */
QVariant NodePropertiesModelImpl::headerData ( int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("property"));
                break;
            case 1:
                return QVariant(QString("value"));
                break;
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant NodePropertiesModelImpl::data ( const QModelIndex & index, int role) const {
    PropertyItem *item;

    if (_properties==NULL || !index.isValid()) 
        return QVariant();
    item=(PropertyItem *)index.internalPointer();
    if (role == Qt::DisplayRole && index.column()==0) {
        return QVariant(item->_label);
    }
    if (role == Qt::DisplayRole && index.column()==1) {
        if (item->_property!=NULL) {
            return QVariant(item->_property->GetValue());
        } else {
            return QVariant();            
        }
    }
    if (role == Qt::BackgroundRole) {
        if (item->_property!=NULL) {
            return QVariant(QColor(Qt::white));
        } else {
            return QVariant(QColor(Qt::lightGray));
        }
    }
    if (role == Qt::ForegroundRole) {
        if (item->_property!=NULL) {
            if (item->_property->IsEditable())
                return QVariant(QColor(Qt::black));
            else
                return QVariant(QColor(Qt::darkGray));
        } else {
            return QVariant(QColor(Qt::black));
        }
    }
    if (role == Qt::ToolTipRole ) {
        if (item->_property!=NULL) {
            QString desc=item->_property->GetDescription();
            if (!desc.isEmpty())
                return QVariant(desc);
        }
    }
    return QVariant();
}
/*! \brief Permet de definir la valeur d'un item*/
bool NodePropertiesModelImpl::setData ( const QModelIndex & index, const QVariant & value, int role) {
    PropertyItem *item;
    QVariant ivalue,tmpvalue;

    if (_properties==NULL || !index.isValid() || role!=Qt::EditRole) 
        return false;
    item=(PropertyItem *)index.internalPointer();
    if (item->_property==NULL)
        return false;
    ivalue=item->_property->GetValue();
    if (ivalue.userType()!=value.userType()) {
        tmpvalue=value;
        if (!tmpvalue.convert((QVariant::Type)ivalue.userType()))
            return false;
    } else {
        tmpvalue=value;    
    }
    item->_property->SetValue(tmpvalue);
    return true;
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex NodePropertiesModelImpl::index ( int row, int column, const QModelIndex & parent) const{
    PropertyItem * iparent;
    QMap<QString,PropertyItem *>::const_iterator it;
    int index;

    if (_properties==NULL)
        return QModelIndex();
    if (!parent.isValid()) 
        iparent=_root_item;
    else 
        iparent=(PropertyItem *)parent.internalPointer();
    index=0;
    it=iparent->_childrens.begin();
    while (index!=row) {
        index++;
        it++;
    }
    return createIndex(row,column,(void *)it.value());
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex NodePropertiesModelImpl::parent ( const QModelIndex & index ) const{
    PropertyItem * iindex;
    PropertyItem * iparent;
    int index_parent;
    PropertyItem * current_item;
    QMap<QString,PropertyItem *>::const_iterator it;

    if (_properties==NULL || !index.isValid())
        return QModelIndex();
    iindex=(PropertyItem *)index.internalPointer();
    iparent=iindex->_parent;
    if (iparent==_root_item)
        return QModelIndex();
    //Calcul de l'indice du parent
    iindex=iparent;
    iparent=iindex->_parent;
    index_parent=0;
    it=iparent->_childrens.begin();
    while (it.value()!=iindex) {
        index_parent++;
        it++;
    }
    return createIndex(index_parent,0,(void *)iindex);   
}
//---------------------------------------------------------------------
// PropertyItem
//---------------------------------------------------------------------
/*! \brief constructeur par interface*/
NodePropertiesModelImpl::PropertyItem::PropertyItem(NodePropertiesModelImpl * host,QString label,ISwProperty * property) {
    _host=host; 
    _parent=NULL;
    _property=property;
    _property->GetOnChangeSignal().iconnect(*this,&NodePropertiesModelImpl::PropertyItem::OnPropertyChange);
    _label=label;
}
/*! \brief constructeur par nom*/
NodePropertiesModelImpl::PropertyItem::PropertyItem(NodePropertiesModelImpl * host,QString label) {
    _host=host; 
    _parent=NULL;
    _property=NULL;
    _label=label;
}
/*! \brief Destructeur */
NodePropertiesModelImpl::PropertyItem::~PropertyItem(){
    QMap<QString,PropertyItem *>::iterator it;

    if (_property!=NULL)
        _property->GetOnChangeSignal().idisconnect(*this,&NodePropertiesModelImpl::PropertyItem::OnPropertyChange);
    for(it=_childrens.begin();it!=_childrens.end();it++) {
        delete it.value();
    }
    _childrens.clear();
}
/*! \brief Sur changement de la propriété */
void NodePropertiesModelImpl::PropertyItem::OnPropertyChange(ISwProperty * property){
    QMap<QString,PropertyItem *>::const_iterator it;
    int index;
    QModelIndex mindex;

    if (_parent==NULL) 
        mindex=_host->createIndex(0,1,(void *)_host->_root_item);
    else {
        index=0;
        it=_parent->_childrens.begin();
        while (it.value()!=this) {
            index++;
            it++;
        }
        mindex=_host->createIndex(0,index,(void *)this);
    }
    _host->dataChanged(mindex,mindex);
}
