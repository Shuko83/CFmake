/*!
\file SwPropertyPersistentToolbox.cpp
\date 18/04/2006
\brief boite a outil pour l'enregistrement des propriétés
\author  Big
\version 1.0
 */


/*
 * INCLUDES LOCAUX
 */
#include "Tools/SwPropertyPersistentToolbox.h"
#include "Tools/Exception/SwException.h"
#include "Main/SwMacros.h"
#include "Types/SwEnum.h"
#include "Types/SwIntegerEnum.h"
#include "Types/SwInteger.h"
#include "Types/SwDouble.h"
#include "Types/SwString.h"
#include "Types/SwFileDescriptor.h"
#include "Types/SwIconDescriptor.h"
#include "Types/SwIpV4Address.h"
#include "Types/SwUUID.h"
#include "Properties/ISwProperty.h"
#include "Properties/_SwPropertyImpl_Class.h"
#include "Main/SwApplication.h"
#include "Tools/SwBuffer_Toolbox.h"
#include "Component/Services/ISwSnapShotPropertiesService.h"

/*
 * INCLUDES GLOBAUX
 */
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QDomText>
#include <QDomCDATASection>
#include <QMetaType>
#include <QDomImplementation>
#include <QByteArray>
#include <QSizePolicy>
#include <QKeySequence>
#include "QsLog.h"
using namespace StreamWork::SwCore;

#define CL_XML_NODE "property"
#define CL_XML_ATT_PREFIX "prefix"
#define CL_XML_ATT_NAME "pname"
#define CL_XML_ATT_POINT_X "x"
#define CL_XML_ATT_POINT_Y "y"
#define CL_XML_ATT_SIZE_WIDTH "width"
#define CL_XML_ATT_SIZE_HEIGHT "height"
#define CL_XML_ATT_ENUM "enum"
#define CL_XML_ATT_ENUM_STRING "enumString"
#define CL_XML_ATT_INTEGER "SwInteger"
#define CL_XML_ATT_STRING "SwString"
#define CL_XML_ATT_ENUM_INTEGER "SwIntegerEnum"
#define CL_XML_ATT_DOUBLE "SwDouble"
#define CL_XML_ATT_FD "fdesc" //ATTENTION IMPACT SUR LES RECORD
#define CL_XML_ATT_ID "idesc" //ATTENTION IMPACT SUR LES RECORD
#define CL_XML_ATT_IPV4 "ipv4"
#define CL_XML_ATT_UUID_H "idh"
#define CL_XML_ATT_UUID_L "idl"


QS_CATEGORY(SwCore_Save)
//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::LoadProperty(QDomElement & property_node,ISwProperties * properties) 
{
	_SwPropertyImpl_Class * propertyClass;
	ISwProperty *property;

    if (property_node.nodeName()!=QString(CL_XML_NODE) && !property_node.hasAttribute(CL_XML_ATT_NAME)) 
        return;

    propertyClass = dynamic_cast<_SwPropertyImpl_Class *>(properties->GetProperty(property_node.attribute(CL_XML_ATT_NAME).toLatin1().data()));
	property = properties->GetProperty(property_node.attribute(CL_XML_ATT_NAME).toLatin1().data());

    if (property == NULL || propertyClass == NULL || propertyClass->GetComplexeTypeAdapters()!=NULL)
        return;

	setProperty(property_node, property);
}

//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::LoadProperty(QDomElement & property_node, ISwProperty * inProperty)
{
	if (property_node.nodeName()!= QString(CL_XML_NODE) && !property_node.hasAttribute(CL_XML_ATT_NAME) && !property_node.hasAttribute(CL_XML_ATT_PREFIX)) 
		return;
	
	if (inProperty==NULL)
		return;

	setProperty(property_node, inProperty);
}

//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::SaveProperty(QXmlStreamWriter& writer, QString name, ISwProperties * properties)
{
    SavePropertyExtended(writer, name, properties, false);
}


//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::SaveProperty( QDomElement & parent_property_node, QDomDocument &doc, 
	QString propCustomName, ISwProperty * inProperty, QString prefix, QVariant overWriteValue )
{
	QVariant var;
	QDomElement elt;


	if (QDomImplementation::invalidDataPolicy()!=QDomImplementation::ReturnNullNode) 
	{
		QDomImplementation::setInvalidDataPolicy(QDomImplementation::ReturnNullNode);
	}
	
	elt = doc.createElement(CL_XML_NODE);
	elt.setAttribute(CL_XML_ATT_PREFIX, prefix);
	elt.setAttribute(CL_XML_ATT_NAME, propCustomName);

	//Si la propriété existe
	if (inProperty)
		var = inProperty->GetValue();

	if (overWriteValue.isValid())
		var = overWriteValue;

	// Récupératin de la valeur de la property
	createProperty(parent_property_node, doc, inProperty, elt, var);
}

//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::SavePropertyExtended(QXmlStreamWriter& writer, QString name, ISwProperties * properties, bool forceSave) 
{
	_SwPropertyImpl_Class * property;
	QVariant var;
	ISwSnapShotPropertiesService * snapshotService = dynamic_cast<ISwSnapShotPropertiesService *>(properties->GetHostComponent()->QueryService(CG_SW_SNAPSHOPPROPERTY_SERVICE));

	property = dynamic_cast<_SwPropertyImpl_Class *>(properties->GetProperty(name));
	//Si la propriété n'existe pas et qu'elle n'est pas editable (on ne peut ecrire dedans) ou qu'elle n'a pas changer (valeur usine)
	//ou que c'est un type complexe
	if (property == NULL || property->GetComplexeTypeAdapters() != NULL || /*!property->IsEditable() || */ (!forceSave && !property->HasChanged()))
	{
		if (snapshotService != 0 && snapshotService->exist(name))
		{
			if (!snapshotService->getHasChanged(name))
			{
				return;
			}
			else
			{
				var = snapshotService->getValue(name);
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		if (snapshotService != 0 && snapshotService->exist(name))
		{
			if (!snapshotService->getHasChanged(name))
			{
				return;
			}
			else
			{
				var = snapshotService->getValue(name);
			}
		}
		else
		{
			var = property->GetValue();
		}
	}
	writer.writeStartElement(CL_XML_NODE);
	writer.writeAttribute(CL_XML_ATT_NAME, QString(property->GetRealName()));	
	// création de la property et ajout dans le fichier
	createProperty(writer, property, var);
	writer.writeEndElement();
}


//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::createProperty(QDomElement & parent_property_node, QDomDocument &doc, ISwProperty * inProperty, QDomElement &elt, QVariant var) 
{
	QVariant tmp;
	QDomText  text_node;
	QDomCDATASection cdata_node;
	QPoint p;
	QSize s;
	QRect rr;
	QRectF rrF;
	bool save_done=false;
	QString name = "";

	if(inProperty)
		name = inProperty->GetName();

	//----------------------------------------------------------
	// Gestion des types convertible en QString (et inversement)
	//----------------------------------------------------------
	tmp = QVariant(QString(""));
	//Si la conversion de l'objet en string et inversement est possible, on le sauvegarde en string
	if ((var.canConvert(QVariant::String) && tmp.canConvert(var.type()) && var.type () != QVariant::Color) 
		|| (var.userType() == qMetaTypeId<ulong>()))
	{		
		text_node = doc.createTextNode(var.toString());
		if (!text_node.isNull()) 
		{
			elt.appendChild(text_node);
		} 
		else 
		{
			cdata_node = doc.createCDATASection(var.toString());
			if (!cdata_node.isNull()) 
			{
				elt.appendChild(cdata_node);
			} else 
			{
				//Si le save n'a pas reussi alors log
				if (inProperty->GetHostingService())
				SW_APP->Logger().Log(LogLvl_Warning,"Unable to save property %s of type %s of component %s because of invalid characters\n",
					name,
					var.typeName(),
					inProperty->GetHostingService()->GetHostComponent()->GetName().toLatin1().data());    
				else
					SW_APP->Logger().Log(LogLvl_Warning, "Unable to save property %s of type %s of component %s because of invalid characters\n",
					name,
					var.typeName(),
					inProperty->GetName());
			}
		}
		save_done=true;
	}
	

	//----------------------------------------------------------
	// Gestion des autres types Qt
	//----------------------------------------------------------
	if (!save_done)
	{
		switch (var.type())
		{
			case QVariant::Point:
				p=var.toPoint();
				elt.setAttribute(CL_XML_ATT_POINT_X,p.x());
				elt.setAttribute(CL_XML_ATT_POINT_Y,p.y());
				save_done=true;
				break;
			case QVariant::Size:
				s=var.toSize();
				elt.setAttribute(CL_XML_ATT_SIZE_WIDTH,s.width());
				elt.setAttribute(CL_XML_ATT_SIZE_HEIGHT,s.height());
				save_done=true;
				break;
			case QVariant::Rect:
				rr = var.toRect();
				elt.setAttribute( CL_XML_ATT_POINT_X, rr.x() );
				elt.setAttribute( CL_XML_ATT_POINT_Y, rr.y() );
				elt.setAttribute( CL_XML_ATT_SIZE_WIDTH, rr.width() );
				elt.setAttribute( CL_XML_ATT_SIZE_HEIGHT, rr.height() );
				save_done = true;
				break;
			case QVariant::RectF:
				rrF = var.toRectF();
				elt.setAttribute( CL_XML_ATT_POINT_X, rrF.x() );
				elt.setAttribute( CL_XML_ATT_POINT_Y, rrF.y() );
				elt.setAttribute( CL_XML_ATT_SIZE_WIDTH, rrF.width() );
				elt.setAttribute( CL_XML_ATT_SIZE_HEIGHT, rrF.height() );
				save_done=true;
				break;
			case QVariant::ByteArray:
				text_node=doc.createTextNode(SwBuffer_Toolbox::ConvertByteArrayIntoString(var.toByteArray()));
				if (!text_node.isNull()) 
			{
					elt.appendChild(text_node);
					save_done=true;
				}
				break;
			case QVariant::SizePolicy:
				text_node=doc.createTextNode(SwBuffer_Toolbox::ConvertIntoString(var.value<QSizePolicy>()));
				if (!text_node.isNull()) 
			{
					elt.appendChild(text_node);
					save_done=true;
				}
				break;
			case QVariant::Color:
			{
				QColor color = qvariant_cast<QColor>(var);
				// Couleur sans composante alpha
					if (color.alpha () == 255)
				{
						text_node=doc.createTextNode(color.name ().toUpper ());
				}
				else
				{
					QString text = color.name();
						QString alphaS = QString::number (color.alpha (), 16).toUpper ();
						if (alphaS.length () == 1)
					{
							alphaS = QString ("0") + alphaS;
					}
					text += alphaS;
						text_node=doc.createTextNode(text);
					}
					if (!text_node.isNull()) 
					{
						elt.appendChild(text_node);
						save_done=true;
					}
				}
				break;

			case QVariant::KeySequence:
				text_node = doc.createTextNode(qvariant_cast<QKeySequence>(var).toString());
				if (!text_node.isNull())
			{
					elt.appendChild(text_node);
				save_done = true;
				}
				break;
			default:
				break;
		}
	}

	//----------------------------------------------------------
	// Gestion des types user
	//----------------------------------------------------------
	if (!save_done)
	{
		//Type SwEnum
        if (var.userType()==qMetaTypeId<SwEnum>()) {
            SwEnum enum_value=var.value<SwEnum>();
            elt.setAttribute(CL_XML_ATT_ENUM,QString("%1").arg(enum_value.ToInt()));
            elt.setAttribute(CL_XML_ATT_ENUM_STRING,QString("%1").arg(enum_value.ToString()));
            save_done=true;
		}
		//Type SwIntegerEnum
		if (var.userType()==qMetaTypeId<SwIntegerEnum>()) {
			SwIntegerEnum enum_value=var.value<SwIntegerEnum>();
			elt.setAttribute(CL_XML_ATT_ENUM_INTEGER, enum_value.toInt());
			save_done=true;
		}
		//Type SwInteger
		if (var.userType()==qMetaTypeId<SwInteger>()) {
			SwInteger integer=var.value<SwInteger>();
			elt.setAttribute(CL_XML_ATT_INTEGER,integer.getValue());
			save_done=true;
		}
		//Type SwString
		if (var.userType()==qMetaTypeId<SwString>()) {
			SwString string=var.value<SwString>();
			elt.setAttribute(CL_XML_ATT_STRING,string.toString());
			save_done=true;
		}
		//Type SwDouble
		if (var.userType()==qMetaTypeId<SwDouble>()) {
			SwDouble double_value=var.value<SwDouble>();
			elt.setAttribute(CL_XML_ATT_DOUBLE,double_value.getValue());
			save_done=true;
		}
		//Type SwFileDescriptor
		if (var.userType()==qMetaTypeId<SwFileDescriptor>()) 
		{
			SwFileDescriptor fd=var.value<SwFileDescriptor>();
			elt.setAttribute(CL_XML_ATT_FD,fd.getFileName());
			save_done=true;
		}
		//Type SwIconDescriptor
		if (var.userType()==qMetaTypeId<SwIconDescriptor>()) 
		{
			SwIconDescriptor idesc=var.value<SwIconDescriptor>();
			elt.setAttribute(CL_XML_ATT_ID,idesc.ToString());
			save_done=true;
		}
		//Type SwIpV4Address
		if (var.userType()==qMetaTypeId<SwIpV4Address>()) 
		{
			SwIpV4Address value=var.value<SwIpV4Address>();
			elt.setAttribute(CL_XML_ATT_IPV4,value.ToString());
			save_done=true;
		}
		//Type SwUUID
		if (var.userType()==qMetaTypeId<SwUUID>()) 
		{
			SwUUID value=var.value<SwUUID>();
			elt.setAttribute(CL_XML_ATT_UUID_H,value.mostSigBits);
			elt.setAttribute(CL_XML_ATT_UUID_L,value.leastSigBits);
			save_done=true;
		}
		//Type QEnum
		{
			const QMetaObject * metaEnumMetaObject = QMetaType::metaObjectForType(var.userType());
			if (metaEnumMetaObject)
			{
				QString metaEnumName = QMetaType::typeName(var.userType());
				metaEnumName = metaEnumName.mid(metaEnumName.lastIndexOf(":") + 1);
				QMetaEnum metaEnum = metaEnumMetaObject->enumerator(metaEnumMetaObject->indexOfEnumerator(metaEnumName.toLatin1()));

				if (metaEnum.isValid())
				{
					elt.setAttribute(CL_XML_ATT_ENUM, QString("%1").arg(*(int*)var.data()));
					if (metaEnum.isFlag())
						elt.setAttribute(CL_XML_ATT_ENUM_STRING, QString("%1").arg(QString(metaEnum.valueToKeys(*(int*)var.data()))));
					else
						elt.setAttribute(CL_XML_ATT_ENUM_STRING, QString("%1").arg(QString(metaEnum.valueToKey(*(int*)var.data()))));

					save_done = true;
				}
			}
		}
	}
	//Si la propriete n'a pas ete enregistrée
	if (!save_done)
	{
		//Si le save n'a pas reussi alors log
		if (inProperty->GetHostingService())
		{
			SW_APP->Logger().Log(LogLvl_Warning, "Unable to save type %s for property %s of component %s\n",
								 name,
								 var.typeName(),
								 inProperty->GetHostingService()->GetHostComponent()->GetName().toLatin1().data());

			qDebug() << "Unable to save type " << name << " for property " << var.typeName() << " of component", inProperty->GetHostingService()->GetHostComponent()->GetName().toLatin1().data();
		}
		else
		{
			SW_APP->Logger().Log(LogLvl_Warning, "Unable to save type %s for property %s of component %s\n",
								 name,
								 var.typeName(),
								 inProperty->GetName());

			qDebug() << "Unable to save type " << name << " for property " << var.typeName() << " of component " << inProperty->GetName();
		}

	} else 
	{
		//Si le save a reussi, enregistrement de l'element
		parent_property_node.appendChild(elt);
	}
}

//-------------------------------------------------------------------------
void SwPropertyPersistentToolbox::setProperty(QDomElement & property_node, ISwProperty * inProperty) 
{
	QVariant var;
	QVariant tmp;
	QDomNode node;
	QPoint p;
	QSize s;
	QRect r;
	QRectF rF;
	bool valueSetted = false;

	//----------------------------------------------------------
	// Gestion des types convertible en QString (et inversement)
	//----------------------------------------------------------
	tmp = QVariant(QString(""));
	var = inProperty->GetValue();

	//Si l'operation de string vers le type est alors 
	if  ( (var.canConvert(QVariant::String) && tmp.canConvert(var.type()) && var.type() != QVariant::Color) 
		|| (var.userType() == qMetaTypeId<ulong>()) )
	{
		node=property_node.firstChild();

		//On va chercher le text
		while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
		if (node.isText()) 
		{
			tmp=node.nodeValue();
			if (tmp.convert(var.type())) 
			{
				inProperty->SetValue(tmp, true);
				valueSetted = true;
				return;
			}
		}
		else 
		{
			//Sinon on va chercher une CDATASection
			node=property_node.firstChild(); 

			//On va chercher la CDATASection
			while (!node.isNull() && !node.isCDATASection()) node=property_node.nextSibling();
			if (node.isCDATASection()) 
			{
				tmp=node.nodeValue();
				if (tmp.convert(var.type())) 
				{
					inProperty->SetValue(tmp, true);   
					valueSetted = true;
					return;
				}
			}
		}

		tmp = QVariant(QString(""));
		if (tmp.convert(var.type()))
		{
			inProperty->SetValue(tmp, true);
			valueSetted = true;
			return;
		}
		return;
	}


	//----------------------------------------------------------
	// Gestion des autres types
	//----------------------------------------------------------
	//----------------------------------------------------------
	// Gestion des autres types Qt
	//----------------------------------------------------------
	switch (var.type()) 
	{
	case QVariant::Point:
		if (property_node.hasAttribute(CL_XML_ATT_POINT_X) && property_node.hasAttribute(CL_XML_ATT_POINT_Y)) 
		{
			p.setX(property_node.attribute(CL_XML_ATT_POINT_X).toInt());
			p.setY(property_node.attribute(CL_XML_ATT_POINT_Y).toInt());
			inProperty->SetValue(p, true);
			valueSetted = true;
		}
		break;
	case QVariant::Size:
		if (property_node.hasAttribute(CL_XML_ATT_SIZE_WIDTH) && property_node.hasAttribute(CL_XML_ATT_SIZE_HEIGHT)) 
		{
			s.setWidth(property_node.attribute(CL_XML_ATT_SIZE_WIDTH).toInt());
			s.setHeight(property_node.attribute(CL_XML_ATT_SIZE_HEIGHT).toInt());
			inProperty->SetValue(s, true);
			valueSetted = true;
		}
		break;
	case QVariant::Rect:
		if ( property_node.hasAttribute( CL_XML_ATT_POINT_X ) && property_node.hasAttribute( CL_XML_ATT_POINT_Y ) &&
			 property_node.hasAttribute( CL_XML_ATT_SIZE_WIDTH ) && property_node.hasAttribute( CL_XML_ATT_SIZE_HEIGHT ) )
		{
			r.setX( property_node.attribute( CL_XML_ATT_POINT_X ).toInt() );
			r.setY( property_node.attribute( CL_XML_ATT_POINT_Y ).toInt() );
			r.setWidth( property_node.attribute( CL_XML_ATT_SIZE_WIDTH ).toInt() );
			r.setHeight( property_node.attribute( CL_XML_ATT_SIZE_HEIGHT ).toInt() );
			inProperty->SetValue( r, true );
			valueSetted = true;
		}
		break;
	case QVariant::RectF:
		if (property_node.hasAttribute(CL_XML_ATT_POINT_X) && property_node.hasAttribute(CL_XML_ATT_POINT_Y) &&
			property_node.hasAttribute(CL_XML_ATT_SIZE_WIDTH) && property_node.hasAttribute(CL_XML_ATT_SIZE_HEIGHT)) 
		{
			rF.setX( property_node.attribute( CL_XML_ATT_POINT_X ).toDouble() );
			rF.setY( property_node.attribute( CL_XML_ATT_POINT_Y ).toDouble() );
			rF.setWidth( property_node.attribute( CL_XML_ATT_SIZE_WIDTH ).toDouble() );
			rF.setHeight( property_node.attribute( CL_XML_ATT_SIZE_HEIGHT ).toDouble() );
			inProperty->SetValue( rF, true );
			valueSetted = true;
		}
		break;
	case QVariant::ByteArray:
		node=property_node.firstChild();
		//On va chercher le text
		while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
		if (node.isText()) 
		{
			QByteArray ba=SwBuffer_Toolbox::ConvertStringIntoByteArrayInto(node.nodeValue());
			inProperty->SetValue(QVariant(ba), true);
			valueSetted = true;
		}
		break;
	case QVariant::SizePolicy:
		node=property_node.firstChild();
		//On va chercher le text
		while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
		if (node.isText()) 
		{
			QSizePolicy val_to_get;
			if (SwBuffer_Toolbox::ConvertFromString(node.nodeValue(),val_to_get)) 
			{
				tmp.setValue(val_to_get);
				inProperty->SetValue(tmp, true);
				valueSetted = true;
			}    
		}
		break;
	case QVariant::Color:
		node=property_node.firstChild();
		//On va chercher le text
		while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
		if (node.isText()) 
		{
			QString text = node.nodeValue().toUpper ();
			QString alpha = "";
			if (text.length () == 9)
			{
				alpha = text.right (2);
				text = text.left (7);
			}
			QVariant variant = text;
			QColor color = qvariant_cast<QColor>(variant);
			if (alpha.length ())
			{
				bool ok = false;
				color.setAlpha (alpha.toInt (&ok, 16));
			}

			tmp.setValue (color);
			inProperty->SetValue(tmp, true);
			valueSetted = true;
		}
		break;
	default:
		valueSetted = false;
		break;
	}


	//----------------------------------------------------------
	// Gestion des types user
	//----------------------------------------------------------
	//Type SwEnum
    if (var.userType()==qMetaTypeId<SwEnum>() && property_node.hasAttribute(CL_XML_ATT_ENUM)) {
        int evalue=property_node.attribute(CL_XML_ATT_ENUM).toInt();
        SwEnum enum_value=var.value<SwEnum>();        
        if (property_node.hasAttribute(CL_XML_ATT_ENUM_STRING) && !enum_value.IsFlag()) {
            QString esvalue=property_node.attribute(CL_XML_ATT_ENUM_STRING);
            if (enum_value.GetValues()->count()>0 && enum_value.GetValues()->value(evalue,"")!=esvalue) {
                evalue=enum_value.GetValues()->key(esvalue,enum_value.GetValues()->begin().key());
            }
        }        
        enum_value.FromInt(evalue);
        tmp.setValue(enum_value);
        inProperty->SetValue(tmp, true);
		valueSetted = true;
    }
	//Type SwIntegerEnum
	else if (var.userType()==qMetaTypeId<SwIntegerEnum>() && property_node.hasAttribute(CL_XML_ATT_ENUM_INTEGER)) {
		int evalue=property_node.attribute(CL_XML_ATT_ENUM_INTEGER).toInt();
		SwIntegerEnum enum_value=var.value<SwIntegerEnum>();            
		enum_value.fromInt(evalue);
		tmp.setValue(enum_value);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwInteger
	else if (var.userType()==qMetaTypeId<SwInteger>() && property_node.hasAttribute(CL_XML_ATT_INTEGER)) {
		int evalue=property_node.attribute(CL_XML_ATT_INTEGER).toInt();
		SwInteger int_value=var.value<SwInteger>();        
		int_value.setValue(evalue);
		tmp.setValue(int_value);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwString
	else if (var.userType()==qMetaTypeId<SwString>() && property_node.hasAttribute(CL_XML_ATT_STRING)) {
		QString string=property_node.attribute(CL_XML_ATT_STRING);
		SwString string_value=var.value<SwString>();        
		string_value.fromString(string);
		tmp.setValue(string_value);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwDouble
	else if (var.userType()==qMetaTypeId<SwDouble>() && property_node.hasAttribute(CL_XML_ATT_DOUBLE)) {
		double evalue=property_node.attribute(CL_XML_ATT_DOUBLE).toDouble();
		SwDouble double_value=var.value<SwDouble>();        
		double_value.setValue(evalue);
		tmp.setValue(double_value);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwFileDescriptor
	else if (var.userType()==qMetaTypeId<SwFileDescriptor>() && property_node.hasAttribute(CL_XML_ATT_FD)) 
	{
		SwFileDescriptor fd=var.value<SwFileDescriptor>();
		fd.setFileName(property_node.attribute(CL_XML_ATT_FD));
		tmp.setValue(fd);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwIconDescriptor
	else if (var.userType()==qMetaTypeId<SwIconDescriptor>() && property_node.hasAttribute(CL_XML_ATT_ID)) 
	{
		SwIconDescriptor idesc=var.value<SwIconDescriptor>();
		idesc.setPath(property_node.attribute(CL_XML_ATT_ID));
		tmp.setValue(idesc);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwIpV4Address
	else if (var.userType()==qMetaTypeId<SwIpV4Address>() && property_node.hasAttribute(CL_XML_ATT_IPV4)) 
	{
		SwIpV4Address value=var.value<SwIpV4Address>();
		value.FromString(property_node.attribute(CL_XML_ATT_IPV4));
		tmp.setValue(value);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	//Type SwUUID
	else if (var.userType()==qMetaTypeId<SwUUID>() && property_node.hasAttribute(CL_XML_ATT_UUID_H) && property_node.hasAttribute(CL_XML_ATT_UUID_L))  
	{
		SwUUID value=var.value<SwUUID>();
		value.mostSigBits=property_node.attribute(CL_XML_ATT_UUID_H).toLongLong();
		value.leastSigBits=property_node.attribute(CL_XML_ATT_UUID_L).toLongLong();
		tmp.setValue(value);
		inProperty->SetValue(tmp, true);
		valueSetted = true;
	}
	// Type QEnum
	else if (property_node.hasAttribute(CL_XML_ATT_ENUM))
	{
		const QMetaObject * metaEnumMetaObject = QMetaType::metaObjectForType(var.userType());
		if (metaEnumMetaObject)
		{
			QString metaEnumName = QMetaType::typeName(var.userType());
			metaEnumName = metaEnumName.mid(metaEnumName.lastIndexOf(":") + 1);
			QMetaEnum metaEnum = metaEnumMetaObject->enumerator(metaEnumMetaObject->indexOfEnumerator(metaEnumName.toLatin1()));

			if (metaEnum.isValid())
			{
				int evalue = property_node.attribute(CL_XML_ATT_ENUM).toInt();
				inProperty->SetValue(QVariant(var.userType(), &evalue), true);
				valueSetted = true;
			}
		}
	}

	if (!valueSetted)
		qDebug() << "ERROR in SwPropertyPersistentToolbox::setProperty() : " << inProperty->GetName() << " cannot be setted because QVariant Type is unknown " << var.type();
}

//-----------------------------------------------------------------------
void SwPropertyPersistentToolbox::createProperty(QXmlStreamWriter& writer, ISwProperty * inProperty, QVariant var)
{
	bool save_done = false;
	QString name = "";

	if (inProperty)
		name = inProperty->GetName();

	//----------------------------------------------------------
	// Gestion des types convertible en QString (et inversement)
	//----------------------------------------------------------
	QVariant tmp = QVariant(QString(""));

	//Si la conversion de l'objet en string et inversement est possible, on le sauvegarde en string
	if ((var.canConvert(QVariant::String) && tmp.canConvert(var.type()) && var.type() != QVariant::Color)
		|| (var.userType() == qMetaTypeId<ulong>()))
	{		
		writer.writeCharacters(var.toString());
		save_done = true;		
	}
	
	//----------------------------------------------------------
	// Gestion des autres types Qt
	//----------------------------------------------------------
	if (!save_done)
	{
		switch (var.type())
		{
			case QVariant::Point:
			{
				QPoint p = var.toPoint();
				writer.writeAttribute(CL_XML_ATT_POINT_X, QString::number(p.x()));
				writer.writeAttribute(CL_XML_ATT_POINT_Y, QString::number(p.y()));
				save_done = true;
				break;
			}
			case QVariant::Size:
			{
				QSize s = var.toSize();
				writer.writeAttribute(CL_XML_ATT_SIZE_WIDTH, QString::number(s.width()));
				writer.writeAttribute(CL_XML_ATT_SIZE_HEIGHT, QString::number(s.height()));
				save_done = true;
				break;
			}
			case QVariant::Rect:
			{
				QRect rr = var.toRect();
				writer.writeAttribute(CL_XML_ATT_POINT_X, QString::number(rr.x()));
				writer.writeAttribute(CL_XML_ATT_POINT_Y, QString::number(rr.y()));
				writer.writeAttribute(CL_XML_ATT_SIZE_WIDTH, QString::number(rr.width()));
				writer.writeAttribute(CL_XML_ATT_SIZE_HEIGHT, QString::number(rr.height()));
				save_done = true;
				break;
			}
			case QVariant::RectF:
			{
				QRectF rrF = var.toRectF();
				writer.writeAttribute(CL_XML_ATT_POINT_X, QString::number(rrF.x(), 'g', 12));
				writer.writeAttribute(CL_XML_ATT_POINT_Y, QString::number(rrF.y(), 'g', 12));
				writer.writeAttribute(CL_XML_ATT_SIZE_WIDTH, QString::number(rrF.width(), 'g', 12));
				writer.writeAttribute(CL_XML_ATT_SIZE_HEIGHT, QString::number(rrF.height(), 'g', 12));
				save_done = true;
				break;
			}
			case QVariant::ByteArray:
			{
				QString str = SwBuffer_Toolbox::ConvertByteArrayIntoString(var.toByteArray());
				writer.writeCharacters(str);
				save_done = true;
				break;
			}
			case QVariant::SizePolicy:
			{
				QString str = SwBuffer_Toolbox::ConvertByteArrayIntoString(var.toByteArray());
				writer.writeCharacters(str);
				save_done = true;
				break;
			}
			case QVariant::Color:
			{
				QColor color = qvariant_cast<QColor>(var);
				// Couleur sans composante alpha
				if (color.alpha() == 255)
				{
					writer.writeCharacters(color.name().toUpper());
				}
				else
				{
					QString text = color.name();
					QString alphaS = QString::number(color.alpha(), 16).toUpper();
					if (alphaS.length() == 1)
					{
						alphaS = QString("0") + alphaS;
					}
					text += alphaS;

					writer.writeCharacters(text);
				}
				save_done = true;
				break;
			}
			case QVariant::KeySequence:
		{
				QString keySequence = qvariant_cast<QKeySequence>(var).toString();
				writer.writeCharacters(keySequence);
				save_done = true;
				break;
			}
			default:
				break;
		}
	}

	//----------------------------------------------------------
	// Gestion des types user
	//----------------------------------------------------------
	if (!save_done)
	{
		//Type SwEnum
		if (var.userType() == qMetaTypeId<SwEnum>())
		{
			SwEnum enum_value = var.value<SwEnum>();
			writer.writeAttribute(CL_XML_ATT_ENUM, QString("%1").arg(enum_value.ToInt()));
			writer.writeAttribute(CL_XML_ATT_ENUM_STRING, QString("%1").arg(enum_value.ToString()));
			save_done = true;
		}
		//Type SwIntegerEnum
		if (var.userType() == qMetaTypeId<SwIntegerEnum>())
		{
			SwIntegerEnum enum_value = var.value<SwIntegerEnum>();
			writer.writeAttribute(CL_XML_ATT_ENUM_INTEGER, QString::number(enum_value.toInt()));
			save_done = true;
		}
		//Type SwInteger
		if (var.userType() == qMetaTypeId<SwInteger>())
		{
			SwInteger integer = var.value<SwInteger>();
			writer.writeAttribute(CL_XML_ATT_INTEGER, QString::number(integer.getValue()));
			save_done = true;
		}
		//Type SwString
		if (var.userType() == qMetaTypeId<SwString>())
		{
			SwString string = var.value<SwString>();
			writer.writeAttribute(CL_XML_ATT_STRING, string.toString());
			save_done = true;
		}
		//Type SwDouble
		if (var.userType() == qMetaTypeId<SwDouble>())
		{
			SwDouble double_value = var.value<SwDouble>();
			writer.writeAttribute(CL_XML_ATT_DOUBLE, QString::number(double_value.getValue()));
			save_done = true;
		}
		//Type SwFileDescriptor
		if (var.userType() == qMetaTypeId<SwFileDescriptor>())
		{
			SwFileDescriptor fd = var.value<SwFileDescriptor>();
			writer.writeAttribute(CL_XML_ATT_FD, fd.getFileName());
			save_done = true;
		}
		//Type SwIconDescriptor
		if (var.userType() == qMetaTypeId<SwIconDescriptor>())
		{
			SwIconDescriptor idesc = var.value<SwIconDescriptor>();
			writer.writeAttribute(CL_XML_ATT_ID, idesc.ToString());
			save_done = true;
		}
		//Type SwIpV4Address
		if (var.userType() == qMetaTypeId<SwIpV4Address>())
		{
			SwIpV4Address value = var.value<SwIpV4Address>();
			writer.writeAttribute(CL_XML_ATT_IPV4, value.ToString());
			save_done = true;
		}
		//Type SwUUID
		if (var.userType() == qMetaTypeId<SwUUID>())
		{
			SwUUID value = var.value<SwUUID>();
			writer.writeAttribute(CL_XML_ATT_UUID_H, QString::number(value.mostSigBits));
			writer.writeAttribute(CL_XML_ATT_UUID_L, QString::number(value.leastSigBits));
			save_done = true;
		}
		//Type QEnum
		{
			const QMetaObject * metaEnumMetaObject = QMetaType::metaObjectForType(var.userType());
			if (metaEnumMetaObject)
			{
				QString metaEnumName = QMetaType::typeName(var.userType());
				metaEnumName = metaEnumName.mid(metaEnumName.lastIndexOf(":") + 1);
				QMetaEnum metaEnum = metaEnumMetaObject->enumerator(metaEnumMetaObject->indexOfEnumerator(metaEnumName.toLatin1()));

				if (metaEnum.isValid())
				{
					writer.writeAttribute(CL_XML_ATT_ENUM, QString("%1").arg(*(int*)var.data()));
					if (metaEnum.isFlag())
						writer.writeAttribute(CL_XML_ATT_ENUM_STRING, QString("%1").arg(QString(metaEnum.valueToKeys(*(int*)var.data()))));
					else
						writer.writeAttribute(CL_XML_ATT_ENUM_STRING, QString("%1").arg(QString(metaEnum.valueToKey(*(int*)var.data()))));

					save_done = true;
				}
			}
		}
	}
	//Si la propriete n'a pas ete enregistrée
	if (!save_done)
	{
		//Si le save n'a pas reussi alors log
		if (inProperty->GetHostingService())
		{
			SW_APP->Logger().Log(LogLvl_Warning, "Unable to save type %s for property %s of component %s\n",
								 name,
								 var.typeName(),
								 inProperty->GetHostingService()->GetHostComponent()->GetName().toLatin1().data());

			qDebug() << "Unable to save type " << name << " for property " << var.typeName() << " of component", inProperty->GetHostingService()->GetHostComponent()->GetName().toLatin1().data();
		}
		else
		{
			SW_APP->Logger().Log(LogLvl_Warning, "Unable to save type %s for property %s of component %s\n",
								 name,
								 var.typeName(),
								 inProperty->GetName());

			qDebug() << "Unable to save type " << name << " for property " << var.typeName() << " of component " << inProperty->GetName();
		}

	}
}

//-----------------------------------------------------------------------
QVariant SwPropertyPersistentToolbox::createQVariantFromString(ISwProperty* prop, QString value)
{
	QVariant var = prop->GetValue();
	QVariant tmp;
	


	if ((var.canConvert(QVariant::String) && var.type() != QVariant::Color) || (var.userType() == qMetaTypeId<ulong>()))
	{
		tmp = value;
		tmp.convert(var.type());
	}
	else
	{

		switch (var.type())
		{
			case QVariant::Point:
			{
				auto values = value.remove("(").remove(")").split(",");
				if (values.count() == 2)
				{
					QPoint p;
					p.setX(values[0].toInt());
					p.setY(values[1].toInt());
					tmp = p;
				}
				break;
			}
			case QVariant::Size:
			{
				auto values = value.remove("(").remove(")").split(",");
				if (values.count() == 2)
				{
					QSize s;
					s.setWidth(values[0].toInt());
					s.setHeight(values[1].toInt());
					tmp = s;
				}

				break;
			}
			case QVariant::Rect:
			{
				auto values = value.remove("(").remove(")").split(",");
				if (values.count() == 4)
				{
					QRect r;
					r.setX(values[0].toInt());
					r.setY(values[1].toInt());
					r.setWidth(values[2].toInt());
					r.setHeight(values[3].toInt());
					tmp = r;
				}
				break;
			}
			case QVariant::RectF:
			{
				auto values = value.remove("(").remove(")").split(",");
				if (values.count() == 4)
				{
					QRectF r;
					r.setX(values[0].toDouble());
					r.setY(values[1].toDouble());
					r.setWidth(values[2].toDouble());
					r.setHeight(values[3].toDouble());
					tmp = r;
				}
				break;
			}
			case QVariant::ByteArray:
			{
				tmp = QByteArray::fromHex(value.toLatin1());
				break;
			}
			case QVariant::Color:
			{
				QString text = value;
				QString alpha = "";
				if (text.length() == 9)
				{
					alpha = text.right(2);
					text = text.left(7);
				}
				QVariant variant = text;
				QColor color = qvariant_cast<QColor>(variant);
				if (alpha.length())
				{
					bool ok = false;
					color.setAlpha(alpha.toInt(&ok, 16));
				}

				tmp.setValue(color);
				break;
			}
			default:
				break;
		}
	}
	
	if (var.userType() == qMetaTypeId<SwEnum>())
	{
		SwEnum enum_value = var.value<SwEnum>();
		enum_value.FromString(value);
		tmp.setValue(enum_value);
	}
	else if (var.userType() == qMetaTypeId<SwIntegerEnum>())
	{
		SwIntegerEnum enum_value = var.value<SwIntegerEnum>();
		enum_value.fromString(value);
		tmp.setValue(enum_value);
	}
	//Type SwInteger
	else if (var.userType() == qMetaTypeId<SwInteger>())
	{
		SwInteger int_value = var.value<SwInteger>();
		int_value.setValue(value.toInt());
		tmp.setValue(int_value);
	}
	//Type SwString
	else if (var.userType() == qMetaTypeId<SwString>())
	{
		SwString string_value = var.value<SwString>();
		string_value.fromString(value);
		tmp.setValue(string_value);
	}
	//Type SwDouble
	else if (var.userType() == qMetaTypeId<SwDouble>())
	{
		SwDouble double_value = var.value<SwDouble>();
		double_value.setValue(value.toDouble());
		tmp.setValue(double_value);
	}
	//Type SwFileDescriptor
	else if (var.userType() == qMetaTypeId<SwFileDescriptor>())
	{
		SwFileDescriptor fd = var.value<SwFileDescriptor>();
		fd.setFileName(value);
		tmp.setValue(fd);
	}
	//Type SwIconDescriptor
	else if (var.userType() == qMetaTypeId<SwIconDescriptor>())
	{
		SwIconDescriptor idesc = var.value<SwIconDescriptor>();
		idesc.setPath(value);
		tmp.setValue(idesc);
	}
	//Type SwIpV4Address
	else if (var.userType() == qMetaTypeId<SwIpV4Address>())
	{
		SwIpV4Address ipv4 = var.value<SwIpV4Address>();
		ipv4.FromString(value);
		tmp.setValue(ipv4);
	}
	else
	{
		const QMetaObject * metaEnumMetaObject = QMetaType::metaObjectForType(var.userType());
		if (metaEnumMetaObject)
		{
			QString metaEnumName = QMetaType::typeName(var.userType());
			metaEnumName = metaEnumName.mid(metaEnumName.lastIndexOf(":") + 1);
			QMetaEnum metaEnum = metaEnumMetaObject->enumerator(metaEnumMetaObject->indexOfEnumerator(metaEnumName.toLatin1()));

			if (metaEnum.isValid())
			{
				bool ok = false;
				int enum_value;

				if (metaEnum.isFlag())
					enum_value = metaEnum.keysToValue(value.toLatin1().constData(), &ok);
				else
					enum_value = metaEnum.keyToValue(value.toLatin1().constData(), &ok);

				if (!ok)
				{
					void * default_enum = QMetaType::create(var.userType());

					tmp = QVariant(var.userType(), default_enum);

					QMetaType::destroy(var.userType(), default_enum);
				}
				else
					tmp = QVariant(var.userType(), &enum_value);
			}
		}
	}

	if (!tmp.isValid())
		qCWarning(SwCore_Save) << "Unable to map the property <" + prop->GetRealName() + ">";

	return tmp;
}
