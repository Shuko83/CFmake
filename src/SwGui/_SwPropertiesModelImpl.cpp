/*!
 \file __SwPropertiesModelImpl.h
 \brief Definit un modele a partir d'une liste de propriétés d'un composant
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#include <QStringList>
#include <QColor>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <SwApplication.h>
#include <SwMacros.h>
#include <QPainter>

/*
  * INCLUDES LOCAUX
  */
#include "SwIconDescriptor.h"
#include "_SwPropertiesModelImpl.h"



using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief constructeur */
_SwPropertiesModelImpl::_SwPropertiesModelImpl(QObject * parent):QAbstractItemModel(parent) {
    _root_item=NULL;
    _properties=NULL;
    _action_item=NULL;
    _change_in_progress=false;
}
/*! \brief destructeur */
_SwPropertiesModelImpl::_SwPropertiesModelImpl() {
    //Si l'ancienne collection de propriétés est definie
    if (_properties!=NULL) {
        //Desenregistrement des signaux
        _properties->GetOnBeforePropertiesChange().idisconnect(*this,&_SwPropertiesModelImpl::OnBeforeChange);
        _properties->GetOnAfterPropertiesChange().idisconnect(*this,&_SwPropertiesModelImpl::OnAfterChange);
        _properties->GetOnCreateProperty().idisconnect(*this,&_SwPropertiesModelImpl::OnCreateProperty);
        _properties->GetOnDestroyProperty().idisconnect(*this,&_SwPropertiesModelImpl::OnDestroyProperty);
        //Destruction des items
        delete _root_item;
    }
    _map_controller_to_item.clear();
    _map_properties_to_item.clear();
}
/*! \brief creation d'un item */
void  _SwPropertiesModelImpl::CreateItem(ISwProperty * property) {
    PropertyItem * item;
    PropertyItem * c_item;
    QStringList liste=property->GetRealName().split(".");
    if (liste.count()<=1) {
        liste=property->GetRealName().split("_");
    }
	int liste_size = liste.count();
    QMap<QString,PropertyItem *>::iterator it;
    QMap<QString,PropertyItem *>::iterator its;
	QMap<QString, PropertyItem *>::iterator iti;
    int insert_index;
    QMap<SwCore::ISwProperties *,PropertyItem *>::iterator itp;

    itp=_map_properties_to_item.find(property->GetHostingService());
    if (itp == _map_properties_to_item.end())
        return;
	item = itp.value();
    for (int i=0;i<liste_size;i++) {
		it = item->_childrens.find(liste[i]);
        if (it==item->_childrens.end()) {
            if (i == liste_size - 1) 
                c_item=new PropertyItem(this,liste[i],property);
            else 
                c_item=new PropertyItem(this,liste[i]);
            c_item->_parent=item;
            iti = item->_childrens.insert(liste[i],c_item);
            if (_change_in_progress) {
                insert_index=0;
                its=item->_childrens.begin();
                while (its!=iti) {
                    insert_index++;
                    its++;
                }
                beginInsertRows(parent(createIndex(insert_index,0,(void *)its.value())),insert_index,insert_index);
                endInsertRows();
            }
            if (property->IsControlled()) {
                OnControlChange(property,property->GetController(),NULL);    
            }
        } else {
            c_item=it.value();    
        }
        item=c_item;
    }
}
/*! \brief destruction d'un item */
void _SwPropertiesModelImpl::DestroyItem(ISwProperty * property) {
    PropertyItem * item;
    PropertyItem * c_item;
    PropertyItem * d_item;
    QStringList liste=property->GetRealName().split(".");
    if (liste.count()<=1) {
        liste=property->GetRealName().split("_");
    }
    int liste_size=liste.count();
    QMap<QString,PropertyItem *>::iterator it;
    QMap<QString,PropertyItem *>::iterator its;
    int remove_index;
    QModelIndex parent_qindex=QModelIndex();
    bool signalEndRemove;

    QMap<SwCore::ISwProperties *,PropertyItem *>::iterator itp;

    itp=_map_properties_to_item.find(property->GetHostingService());
    if (itp==_map_properties_to_item.end())
        return;
    item=itp.value();
    for (int i=0;i<liste_size;i++) {
        it=item->_childrens.find(liste[i]);
        if (it!=item->_childrens.end()) {
            c_item=it.value();  
            if (i==liste_size-1) {
                d_item=c_item;
                if (d_item->_childrens.count()>0) {
                    d_item->_property=NULL;
                    return;
                }
                c_item=c_item->_parent;
                //Change
                while (c_item!=_root_item && c_item->_property==NULL && c_item->_childrens.count()==1) {
                    d_item=c_item;
                    c_item=c_item->_parent;
                }
                signalEndRemove=false;
                remove_index=0;
                its=c_item->_childrens.begin();
                while (its.value()!=d_item) {
                    remove_index++;
                    its++;
                }
                if (_change_in_progress) {
                    parent_qindex=parent(createIndex(remove_index,0,(void *)its.value()));
                    beginRemoveRows(parent_qindex,remove_index,remove_index);
                    signalEndRemove=true;
                }
                delete d_item;
                c_item->_childrens.erase(its);
                if (signalEndRemove && parent_qindex.isValid()) {
                    
                    endRemoveRows();
                }
                return;
            }
        } 
        item=c_item;
    }
}
/*! \brief definit les propriétés */
void _SwPropertiesModelImpl::SetProperties(ISwProperties * properties) {
    QList<ISwProperty *> properties_list;
    QMap<SwCore::ISwProperties *,PropertyItem *>::iterator itp;

    if (_properties!=properties) {
        //Signal avant changement
		beginResetModel();

        //Si l'ancienne collection de propriétés est definie
        if (_properties!=NULL) {
            //Desenregistrement des signaux
            _properties->GetOnBeforePropertiesChange().idisconnect(*this,&_SwPropertiesModelImpl::OnBeforeChange);
            _properties->GetOnAfterPropertiesChange().idisconnect(*this,&_SwPropertiesModelImpl::OnAfterChange);
            _properties->GetOnCreateProperty().idisconnect(*this,&_SwPropertiesModelImpl::OnCreateProperty);
            _properties->GetOnDestroyProperty().idisconnect(*this,&_SwPropertiesModelImpl::OnDestroyProperty);
            itp=_map_properties_to_item.find(_properties);
            if (itp!=_map_properties_to_item.end())
                _map_properties_to_item.erase(itp);
            //Destruction des items
            delete _root_item;
        }
        //Enregistrement de la nouvelle collection de propriétés
        _properties=properties;
        if (_properties!=NULL) {
            //Enregistrement des signaux
            _properties->GetOnBeforePropertiesChange().iconnect(*this,&_SwPropertiesModelImpl::OnBeforeChange);
            _properties->GetOnAfterPropertiesChange().iconnect(*this,&_SwPropertiesModelImpl::OnAfterChange);
            _properties->GetOnCreateProperty().iconnect(*this,&_SwPropertiesModelImpl::OnCreateProperty);
            _properties->GetOnDestroyProperty().iconnect(*this,&_SwPropertiesModelImpl::OnDestroyProperty);
            //Creation des items
            _root_item=new PropertyItem(this, QString());
            _map_properties_to_item.insert(_properties,_root_item);
            properties_list=_properties->GetProperties();
            for (int i=0;i<properties_list.count();i++) {
                if (properties_list[i]->IsVisible())
                    CreateItem(properties_list[i]);
            }

        }
        //Signal apres changement
        endResetModel();
    }
}
/*! \brief callback sur changement de controle */
void _SwPropertiesModelImpl::OnControlChange(ISwProperty * property,ISwController *new_controller,ISwController *old_controller) {
    PropertyItem * item;
    PropertyItem * c_item;
    QStringList liste=property->GetRealName().split(".");
    int liste_size=liste.count();
    QMap<QString,PropertyItem *>::iterator it;
    QMap<QString,PropertyItem *>::iterator its;
    QMap<QString,PropertyItem *>::iterator iti;
    QMap<SwCore::ISwProperties *,PropertyItem *>::iterator itp;
    QMap<SwCore::ISwController *,PropertyItem *>::iterator itc;
    QList<ISwProperty *> properties_list;

    if (old_controller!=NULL) {
        itc=_map_controller_to_item.find(old_controller);
        if (itc!=_map_controller_to_item.end()) {
            item=itc.value();
            beginRemoveRows(parent(createIndex(0,0,(void *)item)),0,0);
            c_item=item->_parent;
            c_item->_childrens.erase(c_item->_childrens.begin());
            delete item;
            endRemoveRows();
        }
    }
    if (new_controller!=NULL) {
        itp=_map_properties_to_item.find(property->GetHostingService());
        if (itp==_map_properties_to_item.end())
            return;
        item=itp.value();
        for (int i=0;i<liste_size;i++) {
            it=item->_childrens.find(liste[i]);
            if (it==item->_childrens.end()) {
                return;
            } else {
                c_item=it.value();    
            }
            item=c_item;
        }
        //Ajout de l'item controlleur
        c_item=new PropertyItem(this,property->GetController());
        c_item->_parent=item;
        //On enregistre la propriété
        //Le nom enregistrer au niveau des enfants garantie que c'ets toujours le premier item
        iti=item->_childrens.insert("!"+c_item->_label,c_item);
        //On enregistre les enfants
        properties_list=c_item->_controller_properties->GetProperties();
        for (int i=0;i<properties_list.count();i++) {
            if (properties_list[i]->IsVisible())
                CreateItem(properties_list[i]);
        }
        //Mise a jour de l'affichage
        beginInsertRows(parent(createIndex(0,0,(void *)c_item)),0,0);
        endInsertRows();
    }
}
/*! \brief callback avant changement */
void _SwPropertiesModelImpl::OnBeforeChange(ISwProperties * properties) {
    //A Revoir pour la destrcution
    //emit layoutAboutToBeChanged();
    //modelAboutToBeReset();
    //reset();
    _change_in_progress=true;
}
/*! \brief callback apres changement */
void _SwPropertiesModelImpl::OnAfterChange(ISwProperties * properties) {
    //emit layoutChanged();
    //modelReset();
    _change_in_progress=false;
}
/*! \brief callback sur creation de propriétés */
void _SwPropertiesModelImpl::OnCreateProperty(ISwProperties * properties,ISwProperty * property) {
    CreateItem(property);    
}
/*! \brief ccallback sur destruction de propriétés*/
void _SwPropertiesModelImpl::OnDestroyProperty(ISwProperties * properties,ISwProperty * property) {
    DestroyItem(property);
}
//-------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwPropertiesModelImpl::flags ( const QModelIndex & index ) const {
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
int _SwPropertiesModelImpl::columnCount ( const QModelIndex & parent) const{
    return 2;
}			
    /*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwPropertiesModelImpl::rowCount ( const QModelIndex & parent ) const{
    PropertyItem *iparent;

    if (_properties==NULL) 
        return 0;
    if (!parent.isValid())
        return 1;
    iparent=(PropertyItem *)parent.internalPointer();
    return iparent->_childrens.count(); 
}
/*! \brief Renvoie les données d'entete */
QVariant _SwPropertiesModelImpl::headerData ( int section, Qt::Orientation orientation, int role) const{
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
QVariant _SwPropertiesModelImpl::data ( const QModelIndex & index, int role) const {
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
            if (item->_controller!=NULL) {
                return QVariant(item->_controller->GetHostComponent()->GetFactoryComponentName());    
            }
            return QVariant();            
        }
    }
    if (role == Qt::ForegroundRole) {
        if (item->_property!=NULL) {
            if (item->_property->IsEditable())
                return QVariant(QColor(Qt::black));
            else
                return QVariant(QColor(Qt::lightGray));
        } else {
            if (item->_controller!=NULL) 
                return QVariant(QColor(Qt::blue));
            return QVariant(QColor(Qt::black));
        }
    }
    if (role == Qt::FontRole) {
        QFont font;
        if (item->_property!=NULL) {
            if (item->_property->IsControllable()) {
                if (item->_property->IsControlled()) {
                    font.setItalic(true);
                } else {
                    font.setBold(true);
                }
                return QVariant(font);
            } else {
                return QVariant(font);
            }
        } else {
            return QVariant(font);
        }

    }
    if (role==Qt::DecorationRole && index.column()==1 && item->_property!=0) {
        if (item->_property->GetValue().type()==QVariant::Color) {
            QPixmap p(18,18);
            p.fill(item->_property->GetValue().value<QColor>());
            QPainter pl(&p);
            pl.drawRect(0,0,17,17);
            return QVariant(QIcon(p));
        } else if (item->_property->GetValue().userType()==qMetaTypeId<SwIconDescriptor>()) {
            return QVariant(item->_property->GetValue().value<SwIconDescriptor>().ToIcon());
        }
    }
    if (role==Qt::DecorationRole && item->_property!=NULL) {
        if (index.column()==0) {
            QIcon ico=item->_property->GetIcon();
            if (ico.isNull()) 
                return QVariant();
            return QVariant(ico);
        }
        if (index.column()==1) {
            QIcon ico=item->_property->GetValueIcon();
            if (ico.isNull()) 
                return QVariant();
            return QVariant(ico);
        }
    }
    //if (role==Qt::DecorationRole && index.column()==0 && item->_property!=NULL && item->_property->IsControllable()) {
    //    if (item->_property->IsControlled())
    //        return QVariant(QIcon(":/SwGui/gear.png"));
    //    else
    //        return QVariant(QIcon(":/SwGui/lightgear.png"));
    //}
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
bool _SwPropertiesModelImpl::setData ( const QModelIndex & index, const QVariant & value, int role) {
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
QModelIndex _SwPropertiesModelImpl::index ( int row, int column, const QModelIndex & parent) const{
    PropertyItem * iparent;
    QMap<QString,PropertyItem *>::const_iterator it;
    int index;

    if (_properties==NULL)
        return QModelIndex();

	if (!parent.isValid())
	{
		iparent = _root_item;
	}
	else
	{
		iparent=(PropertyItem *)parent.internalPointer();
	}

    index=0;
    it=iparent->_childrens.begin();
    while (index!=row) {
        index++;
        it++;
    }
    return createIndex(row,column,(void *)it.value());
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwPropertiesModelImpl::parent ( const QModelIndex & index ) const{
    PropertyItem * iindex;
    PropertyItem * iparent;
    int index_parent;
    QMap<QString,PropertyItem *>::const_iterator it;

    if (_properties==NULL || !index.isValid())
        return QModelIndex();
    iindex=(PropertyItem *)index.internalPointer();
    iparent=iindex->_parent;
    if (iparent==_root_item)
        return QModelIndex();
    //Calcul de l'indice du parent
    index_parent=0;
    it=iparent->_childrens.begin();
    while (it.value()!=iindex) {
        index_parent++;
        it++;
    }
    return createIndex(index_parent,index.column(),(void *)iindex);   
}
//---------------------------------------------------------------------
// PropertyItem
//---------------------------------------------------------------------
/*! \brief constructeur par interface*/
_SwPropertiesModelImpl::PropertyItem::PropertyItem(_SwPropertiesModelImpl * host,QString label,ISwProperty * property) {
    _host=host; 
    _parent=NULL;
    _controller=NULL;
    _controller_properties=NULL;
    _property=property;
    _property->GetOnChangeSignal().iconnect(*this,&_SwPropertiesModelImpl::PropertyItem::OnPropertyChange);
    _property->GetOnControlChangeSignal().iconnect(*host,&_SwPropertiesModelImpl::OnControlChange);
    _label=label;
}
/*! \brief constructeur par controller*/
_SwPropertiesModelImpl::PropertyItem::PropertyItem(_SwPropertiesModelImpl * host,ISwController * controller) {
    _host=host; 
    _parent=NULL;
    _property=NULL;
    _controller=controller;
    _controller_properties=dynamic_cast<ISwProperties *>(_controller->GetHostComponent()->QueryService(CG_SW_SERVICE_PROPERTIES));
    _host->_map_controller_to_item.insert(_controller,this);
    if (_controller_properties!=NULL) {
        _controller_properties->GetOnBeforePropertiesChange().iconnect(*_host,&_SwPropertiesModelImpl::OnBeforeChange);
        _controller_properties->GetOnAfterPropertiesChange().iconnect(*_host,&_SwPropertiesModelImpl::OnAfterChange);
        _controller_properties->GetOnCreateProperty().iconnect(*_host,&_SwPropertiesModelImpl::OnCreateProperty);
        _controller_properties->GetOnDestroyProperty().iconnect(*_host,&_SwPropertiesModelImpl::OnDestroyProperty);
        _host->_map_properties_to_item.insert(_controller_properties,this);
    }
    _label="Controller";    
}
/*! \brief constructeur par nom*/
_SwPropertiesModelImpl::PropertyItem::PropertyItem(_SwPropertiesModelImpl * host,QString label) {
    _host=host; 
    _parent=NULL;
    _controller=NULL;
    _controller_properties=NULL;
    _property=NULL;
    _label=label;
}

/*! \brief Destructeur */
_SwPropertiesModelImpl::PropertyItem::~PropertyItem(){
    QMap<QString,PropertyItem *>::iterator it;
    QMap<ISwController *,PropertyItem *>::iterator itc;
    QMap<ISwProperties *,PropertyItem *>::iterator itp;

    if (_property!=NULL) {
        _property->GetOnChangeSignal().idisconnect(*this,&_SwPropertiesModelImpl::PropertyItem::OnPropertyChange);
        _property->GetOnControlChangeSignal().idisconnect(*_host,&_SwPropertiesModelImpl::OnControlChange);
    }
    if (_controller!=NULL) {
        itc=_host->_map_controller_to_item.find(_controller);
        if (itc!=_host->_map_controller_to_item.end())
            _host->_map_controller_to_item.erase(itc);
        if (_controller_properties!=NULL) {
            _controller_properties->GetOnBeforePropertiesChange().idisconnect(*_host,&_SwPropertiesModelImpl::OnBeforeChange);
            _controller_properties->GetOnAfterPropertiesChange().idisconnect(*_host,&_SwPropertiesModelImpl::OnAfterChange);
            _controller_properties->GetOnCreateProperty().idisconnect(*_host,&_SwPropertiesModelImpl::OnCreateProperty);
            _controller_properties->GetOnDestroyProperty().idisconnect(*_host,&_SwPropertiesModelImpl::OnDestroyProperty);
            itp=_host->_map_properties_to_item.find(_controller_properties);
            if (itp!=_host->_map_properties_to_item.end()) {
                _host->_map_properties_to_item.erase(itp);
            }
        }
    }
    for(it=_childrens.begin();it!=_childrens.end();it++) {
        delete it.value();
    }
    _childrens.clear();
}
/*! \brief Sur changement de la propriété */
void _SwPropertiesModelImpl::PropertyItem::OnPropertyChange(ISwProperty * property){
    QMap<QString,PropertyItem *>::const_iterator it;
    int index;
    QModelIndex mindex;

	index = 0;
	it = _parent->_childrens.begin();
	while (it.value() != this) {
		index++;
		it++;
	}
	mindex = _host->createIndex(0, index, (void *)this);

    _host->dataChanged(mindex,mindex);
}
//-------------------------------------------------------------
// Interface ISwQAbstractItemViewSlots
//------------------------------------------------------------
/*! \brief sur activation */
void _SwPropertiesModelImpl::onActivated ( const QModelIndex & index ) {

}
/*! \brief sur click */
void _SwPropertiesModelImpl::onClicked ( const QModelIndex & index ){

}
/*! \brief sur double click */
void _SwPropertiesModelImpl::onDoubleClicked ( const QModelIndex & index ){

}
/*! \brief sur entree */
void _SwPropertiesModelImpl::onEntered ( const QModelIndex & index ){

}
/*! \brief sur  appuie de la sourie sur l'item */
void _SwPropertiesModelImpl::onPressed ( const QModelIndex & index ){

}
/*! \brief sur entre dans la vue*/
void _SwPropertiesModelImpl::onViewportEntered (){

}
/*! \brief sur evenement sourie quelconque*/
void _SwPropertiesModelImpl::onMouseEvent(QMouseEvent * mouse_event){

}
/*! \brief sur demande de l'aide (F1)*/
void _SwPropertiesModelImpl::onHelp(){
    QMessageBox::information(NULL,"Help","No Help available",QMessageBox::Ok);
}
/*! \brief OnMenuRequest*/
void _SwPropertiesModelImpl::onMenuRequested( const QPoint & globalpos,QModelIndex index){
    //Recuperation de l'item
    PropertyItem *item;

    if (_properties==NULL || !index.isValid()) 
        return;
    item=(PropertyItem *)index.internalPointer();
    if (item->_property==NULL || !item->_property->IsControllable())
        return;
    _action_item=item;
    //La propriété est controllable
    if (item->_property->IsControlled()) {
        //Si elle est controlé, on propose la suppression du controle
        QMenu * menu=new QMenu();
        menu->addAction(QString("Remove Controller"));
        menu->connect(menu,SIGNAL(triggered(QAction *)),this,SLOT(OnDetachController(QAction *)));
        menu->exec(globalpos);
        delete menu;
    } else {
        //Si elle n'est pas controlé, on propose la liste des controleurs valide pour son type
        QVariant val=_action_item->_property->GetValue();
        QList<QString> liste=SW_FACTORIES.GetControllersListForType(val.userType());
        if (liste.count()==0) {
            _action_item=NULL;
            return;
        }
        QMenu * menu=new QMenu();
        QMenu * ctrl_menu=menu->addMenu(QString("Add Controller"));
        ctrl_menu->connect(ctrl_menu,SIGNAL(triggered(QAction *)),this,SLOT(OnAttachController(QAction *)));
        for(int i=0;i<liste.count();i++) {
            ctrl_menu->addAction(liste[i]);
        }
        menu->exec(globalpos);
        delete menu;
    }

}
/*! \brief OnDetachController*/
void _SwPropertiesModelImpl::OnDetachController(QAction * a) {
    _action_item->_property->GetController()->DetachControl();
    _action_item=NULL;
}
/*! \brief OnAttachController*/
void _SwPropertiesModelImpl::OnAttachController(QAction * a) {
    SwComponent_ClassPtr controller;
    ISwController * i_controller;
    SwComponent_Class * host;
    //Creation du controller
    controller=SW_FACTORIES.CreateComponent(a->text());
    if (controller==NULL)
        return;
    //Ok ajout en tant qu'enfant du host de la propriété
    host=_action_item->_property->GetHostingService()->GetHostComponent();
    controller->SetName(host->GetSuggestedNameForChild("_controller_"+QString(_action_item->_property->GetRealName()).replace(".","_")));
    host->AddChild(controller);
    //Liaison a la propriété
    i_controller=dynamic_cast<ISwController *>(controller->QueryService(CG_SW_SERVICE_CONTROLLER));
    i_controller->InitializeControl(_action_item->_property->GetHostingService(),_action_item->_property->GetRealName());
    //Fin
}

