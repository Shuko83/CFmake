/**
 * file : SwPropertiesModelImpl.h
 * brief : Definit un modele a partir d'une liste de propriétés d'un composant
 *			(pour l'affichage dans un QTreeView)
 * date : 02-octobre-2006 16:04:34
 * author : F.Bighelli - modified by CGD (12/09/2013)
*/

#include <QStringList>
#include <QColor>
#include <SwApplication.h>
#include <SwMacros.h>
#include <QPainter>
#include <qdebug>

#include <SwEnum.h>
#include <SwIntegerEnum.h>
#include <SwInteger.h>
#include <SwString.h>
#include <SwDouble.h>
#include <SwIpv4Address.h>
#include <SwUUID.h>
#include <SwFileDescriptor.h>

#include "QDynamicStyledItemDelegate.h"


/*
* INCLUDES LOCAUX
*/
#include "SwPropertiesModelImpl.h"
#include "SwIconDescriptor.h"
#include "ISwSnapShotPropertiesService.h"

using namespace StreamWork::SwCore;

//-------------------------------------------------------------------------
SwPropertiesModelImpl::SwPropertiesModelImpl(QObject * parent):QAbstractItemModel(parent) 
{
    _root_item=NULL;
    _properties=NULL;
    _action_item=NULL;
    _change_in_progress=false;
}


//-------------------------------------------------------------------------
SwPropertiesModelImpl::~SwPropertiesModelImpl() 
{
    //Si l'ancienne collection de propriétés est definie
    if (_properties != 0) 
	{
		//Desenregistrement des signaux
        _properties->GetOnBeforePropertiesChange().idisconnect(*this,&SwPropertiesModelImpl::OnBeforeChange);
        _properties->GetOnAfterPropertiesChange().idisconnect(*this,&SwPropertiesModelImpl::OnAfterChange);
        _properties->GetOnCreateProperty().idisconnect(*this,&SwPropertiesModelImpl::OnCreateProperty);
        _properties->GetOnDestroyProperty().idisconnect(*this,&SwPropertiesModelImpl::OnDestroyProperty);

		//Destruction des items
		delete _root_item;
    }
	
    _map_properties_to_item.clear();

}

//-------------------------------------------------------------------------
void  SwPropertiesModelImpl::CreateItem(ISwProperty * property, QString customConstructedPropertyName) 
{
    PropertyItem * item =0;
    PropertyItem * c_item =0;
	QStringList liste;
	QString propertyName = property->GetRealName(); 

	if(customConstructedPropertyName != "")
		propertyName = customConstructedPropertyName;

	liste = propertyName.split(".");

    if (liste.count()<=1) 
		liste=propertyName.split("_");
    
    int liste_size=liste.count();
    QMap<QString,PropertyItem *>::iterator it;
    int insert_index;
    QMap<SwCore::ISwProperties *,PropertyItem *>::iterator itp;

    itp=_map_properties_to_item.find(property->GetHostingService());
    if (itp==_map_properties_to_item.end())
        return;
    item=itp.value();
    for (int i=0;i<liste_size;i++) {
        it=item->_childrens.find(liste[i]);
        if (it==item->_childrens.end()) {
            if (i==liste_size-1) 
                c_item=new PropertyItem(this,liste[i],property);
            else 
                c_item=new PropertyItem(this,liste[i]);
            c_item->_parent=item;
            item->_childrens.insert(liste[i],c_item);
            item->_showChildrens.push_back(c_item);
            if (_change_in_progress) {
                insert_index=item->_showChildrens.count()-1;
                beginInsertRows(parent(createIndex(insert_index,0,(void *)c_item)),insert_index,insert_index);
                endInsertRows();
            }
        } else {
            c_item=it.value();    
        }
        item=c_item;
    }
}

//-------------------------------------------------------------------------
void SwPropertiesModelImpl::DestroyItem(ISwProperty * property, QString customConstructedPropertyName) 
{
    PropertyItem * item=0;
    PropertyItem * c_item=0;
    PropertyItem * d_item=0;
	QStringList liste;
	QString propertyName = property->GetRealName(); 

	if(customConstructedPropertyName != "")
		propertyName = customConstructedPropertyName;

	// CGD => cas particulier, les properties avec le suffix _DoNotDisplay ou _DebugMode n'ont pas été créées, 
	bool takeCareOfDebugProperties = false;
	#if _DEBUG	
		takeCareOfDebugProperties = true;
	#endif

	// Pas besoin de supprimer les properties car elles n'ont pas été ajoutées
	if( (!takeCareOfDebugProperties && propertyName.contains("_DebugMode", Qt::CaseInsensitive))
		|| propertyName.contains("_DoNotDisplay", Qt::CaseInsensitive) )
		return;

	liste = propertyName.split(".");

	if (liste.count()<=1) 
		liste=propertyName.split("_");

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
	for (int i=0;i<liste_size;i++) 
	{
		it=item->_childrens.find(liste[i]);
		if (it!=item->_childrens.end()) 
		{
			c_item=it.value();  
			if (i==liste_size-1) 
			{
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
				remove_index=c_item->_showChildrens.indexOf(d_item);
				its=c_item->_childrens.begin();
				while (its.value()!=d_item) {
					//remove_index++;
					its++;
				}
				if (_change_in_progress) {
					parent_qindex=parent(createIndex(remove_index,0,(void *)its.value()));
					beginRemoveRows(parent_qindex,remove_index,remove_index);
					signalEndRemove=true;
				}
				delete d_item;
				c_item->_childrens.erase(its);
				c_item->_showChildrens.removeAt(remove_index);
				if (signalEndRemove && parent_qindex.isValid()) {
                    
					endRemoveRows();
				}
				return;
			}
		} 
	    Q_ASSERT(c_item!=0);
		item=c_item;
	}
}


//-------------------------------------------------------------------------
void SwPropertiesModelImpl::SetProperties(ISwProperties * properties,QString rootName,QString rootLabel) 
{
    QList<ISwProperty *> properties_list;
    QMap<SwCore::ISwProperties *,PropertyItem *>::iterator itp;

    if (_properties!=properties) {
        //Signal avant changement
		beginResetModel();

        //Si l'ancienne collection de propriétés est definie
        if (_properties!=NULL) {
            //Desenregistrement des signaux
            _properties->GetOnBeforePropertiesChange().idisconnect(*this,&SwPropertiesModelImpl::OnBeforeChange);
            _properties->GetOnAfterPropertiesChange().idisconnect(*this,&SwPropertiesModelImpl::OnAfterChange);
            _properties->GetOnCreateProperty().idisconnect(*this,&SwPropertiesModelImpl::OnCreateProperty);
            _properties->GetOnDestroyProperty().idisconnect(*this,&SwPropertiesModelImpl::OnDestroyProperty);
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
            _properties->GetOnBeforePropertiesChange().iconnect(*this,&SwPropertiesModelImpl::OnBeforeChange);
            _properties->GetOnAfterPropertiesChange().iconnect(*this,&SwPropertiesModelImpl::OnAfterChange);
            _properties->GetOnCreateProperty().iconnect(*this,&SwPropertiesModelImpl::OnCreateProperty);
            _properties->GetOnDestroyProperty().iconnect(*this,&SwPropertiesModelImpl::OnDestroyProperty);
            //Creation des items
            _root_item=new PropertyItem(this,rootName);
            _rootLabel=rootLabel;
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

//-------------------------------------------------------------------------
void SwPropertiesModelImpl::OnBeforeChange(ISwProperties * properties) 
{
    //A Revoir pour la destrcution
    //emit layoutAboutToBeChanged();
    //modelAboutToBeReset();
    //reset();
    _change_in_progress=true;
}

//-------------------------------------------------------------------------
void SwPropertiesModelImpl::OnAfterChange(ISwProperties * properties) 
{
    //emit layoutChanged();
    //modelReset();
    _change_in_progress=false;
}

//-------------------------------------------------------------------------
void SwPropertiesModelImpl::OnCreateProperty(ISwProperties * properties,ISwProperty * property) 
{
    CreateItem(property);    
}

//-------------------------------------------------------------------------
void SwPropertiesModelImpl::OnDestroyProperty(ISwProperties * properties,ISwProperty * property) 
{
    DestroyItem(property);
}


//-------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------

//-------------------------------------------------------------------------
Qt::ItemFlags SwPropertiesModelImpl::flags ( const QModelIndex & index ) const 
{
    PropertyItem *item =0;

    if ((_properties == 0 && _map_properties_to_item.size() == 0) || !index.isValid()) 
        return Qt::ItemFlags(Qt::ItemIsEnabled);
    item=(PropertyItem *)index.internalPointer();
    if (item->_property==NULL) {
        return Qt::ItemFlags(Qt::ItemIsEnabled);       
    }
    if (index.column()==1)
	{
		if( item->_property->IsEditable())
			return Qt::ItemFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable| Qt::ItemIsSelectable);
		else
			return Qt::NoItemFlags;
	}
	else
		if(! item->_property->IsEditable())
			return Qt::NoItemFlags;

    return Qt::ItemFlags(Qt::ItemIsEnabled);
}

//-------------------------------------------------------------------------
int SwPropertiesModelImpl::columnCount ( const QModelIndex & parent) const
{
    return 2;
}			

//-------------------------------------------------------------------------
int SwPropertiesModelImpl::rowCount ( const QModelIndex & parent ) const
{
    PropertyItem *iparent;

    if ((_properties == 0 && _map_properties_to_item.size() == 0)) 
        return 0;
    if (!parent.isValid())
        return 1;
    iparent=(PropertyItem *)parent.internalPointer();
    return iparent->_childrens.count(); 
}

//-------------------------------------------------------------------------
QVariant SwPropertiesModelImpl::headerData ( int section, Qt::Orientation orientation, int role) const
{
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

//-------------------------------------------------------------------------
QVariant SwPropertiesModelImpl::data ( const QModelIndex & index, int role) const 
{
    PropertyItem *item =0;

    if ((_properties == 0 && _map_properties_to_item.size() == 0) || !index.isValid()) 
        return QVariant();
    item=(PropertyItem *)index.internalPointer();
    if (item==_root_item) {
        if (role == Qt::DisplayRole && index.column()==0) {
            if (_rootLabel.isEmpty())
                return QVariant(QString("Component"));
            else
                return QVariant(_rootLabel);
        }
        if (role == Qt::DisplayRole && index.column()==1) {
            return QVariant(item->_label);
        }
    }

	//pas d'edition sur la premeire colone
	if (index.column() == 0)
	{
		if (role == Qt::DisplayRole ) {
			return QVariant(item->_label);
		}
	}

	//affichage et eddition sur la deuxieme colone
	if (index.column() == 1)
	{
		if (role == Qt::DisplayRole ) {
			if (item->_property != NULL) {
				QVariant variant = item->_property->GetValue();
				if (isSupportedType(variant))
					return QVariant(displayText(variant));
				return QVariant();
			}
			else {
				return QVariant();
			}
		}
		else if (role == Qt::EditRole ) {
			if (item->_property != NULL) {
				QVariant variant = item->_property->GetValue();
				if (isSupportedType(variant))
					return variant;
				return QVariant();
			}
			else {
				return QVariant();
			}
		}
	}

	if (role == StyledStateRole) {
		if (item->_property != NULL) {
			if (item->_property->HasChanged()) {
				if (item->_property->IsControlled())
					return QVariant(QDynamicStyledItemDelegate::StyledState::Modified_And_Controled);
				else
					return QVariant(QDynamicStyledItemDelegate::StyledState::Modified);
			}
			else
			{
				if (item->_property->IsControlled())
					return QVariant(QDynamicStyledItemDelegate::StyledState::Controled);					
			}
		}
		return QVariant(QDynamicStyledItemDelegate::StyledState::Default);
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
    if (role == Qt::ToolTipRole ) {
        if (item->_property!=NULL) {
            QString desc=item->_property->GetDescription();
            if (!desc.isEmpty())
                return QVariant(desc);
        }
    }
    if (role == Qt::UserRole ) {
        return qVariantFromValue((void *)item->_property);
    }
    return QVariant();
}

//-------------------------------------------------------------------------
bool SwPropertiesModelImpl::setData ( const QModelIndex & index, const QVariant & value, int role) 
{
    PropertyItem *item =0;
    QVariant ivalue,tmpvalue;

    if ((_properties == 0 && _map_properties_to_item.size() == 0) || !index.isValid() || role!=Qt::EditRole) 
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
	
	if(_properties != 0 )
	{
		ISwSnapShotPropertiesService * snapshotService=dynamic_cast<ISwSnapShotPropertiesService *>(_properties->GetHostComponent()->QueryService(CG_SW_SNAPSHOPPROPERTY_SERVICE));
		if (snapshotService!=0 && snapshotService->exist(item->_property->GetRealName())) {
			snapshotService->removeFromSnapShot(item->_property->GetRealName());
		} 
	}
	
    item->_property->SetValue(tmpvalue);
    emit dataChanged(index,index);
    return true;
}

//-------------------------------------------------------------------------
QModelIndex SwPropertiesModelImpl::index ( int row, int column, const QModelIndex & parent) const
{
    PropertyItem * iparent =0 ;
    QMap<QString,PropertyItem *>::const_iterator it;
	
    if ((_properties == 0 && _map_properties_to_item.size() == 0) )
        return QModelIndex();
    if (!parent.isValid()) 
        return createIndex(row,column,(void *)_root_item);

    iparent=(PropertyItem *)parent.internalPointer();
    if (iparent->_showChildrens.size()<=row) {
        return QModelIndex();
    }
    return createIndex(row,column,(void *)iparent->_showChildrens.at(row));
}

//-------------------------------------------------------------------------
QModelIndex SwPropertiesModelImpl::parent ( const QModelIndex & index ) const
{
    PropertyItem * iindex =0;
    PropertyItem * iparent =0;
    int index_parent;
    QMap<QString,PropertyItem *>::const_iterator it;

    if ((_properties == 0 && _map_properties_to_item.size() == 0)  || !index.isValid())
        return QModelIndex();
    iindex=(PropertyItem *)index.internalPointer();
    iparent=iindex->_parent;
    if (iparent==NULL)
        return QModelIndex();
    //Calcul de l'indice du parent
    iindex=iparent;
    iparent=iindex->_parent;
    //Si le parent du parent est null, c'est le composant racine
    if (iparent==NULL) {
        return createIndex(0,index.column(),(void *)iindex);    
    } 
    //Sinon il faut calculer l'index
    index_parent=iparent->_showChildrens.indexOf(iindex);
    return createIndex(index_parent,index.column(),(void *)iindex);   
}

//--------------------------------------------------------------
QString SwPropertiesModelImpl::displayText(const QVariant &value)
{
	QIcon tmpIcon;

	switch (value.type()) {
	case QVariant::Bool:
	case QVariant::ByteArray:
	case QVariant::Char:
	case QVariant::Int:
	case QVariant::LongLong:
	case QVariant::String:
	case QVariant::UInt:
	case QVariant::ULongLong:
		return value.toString();
	case QVariant::Double:
		return QLocale().toString(value.toDouble(), 'f', 4);
	case QVariant::Color:
	{
		QColor color = qvariant_cast<QColor>(value);
		QString text = color.name().toUpper();
		QString alphaS = QString::number(color.alpha(), 16).toUpper();
		if (alphaS.length() == 1)
		{
			alphaS = QString("0") + alphaS;
		}
		text += alphaS;
		return text;
	}
	case QVariant::Font:
	{
		return value.value<QFont>().toString();
	}
	case QVariant::Date:
		return value.toDate().toString(Qt::ISODate);
	case QVariant::DateTime:
		return value.toDateTime().toString(Qt::ISODate);
	case QVariant::Invalid:
		return "<Invalid>";
	case QVariant::Point:
	{
		QPoint point = value.toPoint();
		return QString("(%1,%2)").arg(point.x()).arg(point.y());
	}
	case QVariant::Rect:
	{
		QRect rect = value.toRect();
		return QString("(%1,%2,%3,%4)")
			.arg(rect.x()).arg(rect.y())
			.arg(rect.width()).arg(rect.height());
	}
	case QVariant::Size:
	{
		QSize size = value.toSize();
		return QString("(%1,%2)").arg(size.width()).arg(size.height());
	}
	case QVariant::StringList:
		return value.toStringList().join(",");
	case QVariant::Time:
		return value.toTime().toString(Qt::ISODate);
	case QVariant::UserType:
		if (qMetaTypeId<SwEnum>() == value.userType()) {
			SwEnum venum = value.value<SwEnum>();
			return venum.ToString();
		}
		if (qMetaTypeId<SwIntegerEnum>() == value.userType()) {
			SwIntegerEnum venum = value.value<SwIntegerEnum>();
			return venum.toString();
		}
		if (qMetaTypeId<SwString>() == value.userType()) {
			SwString vstring = value.value<SwString>();
			return vstring.toString();
		}
		if (qMetaTypeId<SwInteger>() == value.userType()) {
			SwInteger vinteger = value.value<SwInteger>();
			return QString::number(vinteger.getValue());
		}
		if (qMetaTypeId<SwDouble>() == value.userType()) {
			SwDouble vdouble = value.value<SwDouble>();
			return QString::number(vdouble.getValue());
		}
		if (qMetaTypeId<SwFileDescriptor>() == value.userType()) {
			return value.value<SwFileDescriptor>().getFileName();
		}
		if (qMetaTypeId<SwIconDescriptor>() == value.userType()) {
			return value.value<SwIconDescriptor>().ToString();
		}
		if (qMetaTypeId<SwIpV4Address>() == value.userType()) {
			return value.value<SwIpV4Address>().ToString();
		}
		if (qMetaTypeId<SwUUID>() == value.userType()) {
			return value.value<SwUUID>().toQString();
		}
	}
	return QString("<%1>").arg(value.typeName());
}

/*! \brief Permet de savoir si le type de l'item est support� */
bool SwPropertiesModelImpl::isSupportedType(QVariant & val)
{
	switch (val.type()) {
	case QVariant::Bool:
	case QVariant::ByteArray:
	case QVariant::Char:
	case QVariant::Color:
	case QVariant::Date:
	case QVariant::DateTime:
	case QVariant::Double:
	case QVariant::Font:
	case QVariant::Int:
	case QVariant::LongLong:
	case QVariant::Point:
	case QVariant::Rect:
	case QVariant::Size:
	case QVariant::String:
	case QVariant::StringList:
	case QVariant::Time:
	case QVariant::UInt:
	case QVariant::ULongLong:
		return true;
	case QVariant::UserType:
		if (qMetaTypeId<SwEnum>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwIntegerEnum>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwInteger>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwString>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwDouble>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwFileDescriptor>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwIconDescriptor>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwIpV4Address>() == val.userType()) {
			return true;
		}
		if (qMetaTypeId<SwUUID>() == val.userType()) {
			return true;
		}
	default:
		break;
	}
	return false;
}

//---------------------------------------------------------------------
// PropertyItem
//---------------------------------------------------------------------

//-------------------------------------------------------------------------
SwPropertiesModelImpl::PropertyItem::PropertyItem(SwPropertiesModelImpl * host,QString label,ISwProperty * property) 
{
    _host=host; 
    _parent=NULL;
    _property=property;
    _property->GetOnChangeSignal().iconnect(*this,&SwPropertiesModelImpl::PropertyItem::OnPropertyChange);
    _label=label;
}

//-------------------------------------------------------------------------
SwPropertiesModelImpl::PropertyItem::PropertyItem(SwPropertiesModelImpl * host,QString label) 
{
    _host=host; 
    _parent=NULL;
    _property=NULL;
    _label=label;
}

//-------------------------------------------------------------------------
SwPropertiesModelImpl::PropertyItem::~PropertyItem()
{
    QMap<QString,PropertyItem *>::iterator it;
    QMap<ISwProperties *,PropertyItem *>::iterator itp;

    if (_property!=NULL) {
        _property->GetOnChangeSignal().idisconnect(*this,&SwPropertiesModelImpl::PropertyItem::OnPropertyChange);
    }
    for(it=_childrens.begin();it!=_childrens.end();it++) {
        delete it.value();
    }
    _childrens.clear();
}

//-------------------------------------------------------------------------
void SwPropertiesModelImpl::PropertyItem::OnPropertyChange(ISwProperty * property)
{
    QMap<QString,PropertyItem *>::const_iterator it;
    int index;
    QModelIndex mindex;

    if (_parent==NULL) 
        mindex=_host->createIndex(0,1,(void *)_host->_root_item);
    else {
        index=_parent->_showChildrens.indexOf(this);
        mindex=_host->createIndex(0,index,(void *)this);
    }
    _host->dataChanged(mindex,mindex);
}
