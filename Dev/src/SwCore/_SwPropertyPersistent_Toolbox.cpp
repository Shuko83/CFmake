/*!
\file _SwPropertyPersistent_Toolbox.cpp
\date 18/04/2006
\brief boite a outil pour l'enregistrement des propriétés
\author  Big
\version 1.0
 */

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
/*
 * INCLUDES LOCAUX
 */
#include "_SwPropertyPersistent_Toolbox.h"
#include "SwException.h"
#include "SwMacros.h"
#include "SwEnum.h"
#include "SwFileDescriptor.h"
#include "SwIconDescriptor.h"
#include "SwIpV4Address.h"
#include "SwUUID.h"
#include "ISwProperty.h"
#include "_SwPropertyImpl_Class.h"
#include "SwApplication.h"
#include "SwBuffer_Toolbox.h"
using namespace StreamWork::SwCore;

#define CL_XML_NODE "property"
#define CL_XML_ATT_NAME "pname"
#define CL_XML_ATT_POINT_X "x"
#define CL_XML_ATT_POINT_Y "y"
#define CL_XML_ATT_SIZE_WIDTH "width"
#define CL_XML_ATT_SIZE_HEIGHT "height"
#define CL_XML_ATT_ENUM "enum"
#define CL_XML_ATT_ENUM_STRING "enumString"
#define CL_XML_ATT_FD "fdesc" //ATTENTION IMPACT SUR LES RECORD
#define CL_XML_ATT_ID "idesc" //ATTENTION IMPACT SUR LES RECORD
#define CL_XML_ATT_IPV4 "ipv4"
#define CL_XML_ATT_UUID_H "idh"
#define CL_XML_ATT_UUID_L "idl"

/*! \brief methode permettant de charger une propriété
\param[in] property_node noeud propriete potentiel
\param[in] liste des propriétés
*/
void _SwPropertyPersistent_Toolbox::LoadProperty(QDomElement & property_node,ISwProperties * properties) {
    _SwPropertyImpl_Class * property;
    QVariant var;
    QVariant tmp;
    QDomNode node;
    QPoint p;
    QSize s;
    QRect r;
    QString working_string;
    QByteArray working_array;

    if (property_node.nodeName()!=QString(CL_XML_NODE) && !property_node.hasAttribute(CL_XML_ATT_NAME)) 
        return;
    property=dynamic_cast<_SwPropertyImpl_Class *>(properties->GetProperty(property_node.attribute(CL_XML_ATT_NAME).toLatin1().data()));
    if (property==NULL || property->GetComplexeTypeAdapters()!=NULL)
        return;
    //----------------------------------------------------------
    // Gestion des types convertible en QString (et inversement)
    //----------------------------------------------------------
    tmp=QVariant(QString(""));
    var=property->GetValue();
    //Si l'operation de string vers le type est alors alors
    if (var.canConvert(QVariant::String) && tmp.canConvert(var.type()) && var.type() != QVariant::Color) {
        node=property_node.firstChild();
        //On va chercher le text
        while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
        if (node.isText()) {
            tmp=node.nodeValue();
            if (tmp.convert(var.type())) {
                property->SetValue(tmp);
                return;
            }
        } else {
            //Sinon on va chercher une CDATASection
            node=property_node.firstChild(); 
            //On va chercher la CDATASection
            while (!node.isNull() && !node.isCDATASection()) node=property_node.nextSibling();
            if (node.isCDATASection()) {
                tmp=node.nodeValue();
                if (tmp.convert(var.type())) {
                    property->SetValue(tmp);   
                    return;
                }
            }
       }
       property->SetValue(QVariant(QString("")));   
       return;
    }
    //----------------------------------------------------------
    // Gestion des autres types
    //----------------------------------------------------------
    //----------------------------------------------------------
    // Gestion des autres types Qt
    //----------------------------------------------------------
    switch (var.type()) {
        case QVariant::Point:
            if (property_node.hasAttribute(CL_XML_ATT_POINT_X) && property_node.hasAttribute(CL_XML_ATT_POINT_Y)) {
                p.setX(property_node.attribute(CL_XML_ATT_POINT_X).toInt());
                p.setY(property_node.attribute(CL_XML_ATT_POINT_Y).toInt());
                property->SetValue(p);
            }
            break;
        case QVariant::Size:
            if (property_node.hasAttribute(CL_XML_ATT_SIZE_WIDTH) && property_node.hasAttribute(CL_XML_ATT_SIZE_HEIGHT)) {
                s.setWidth(property_node.attribute(CL_XML_ATT_SIZE_WIDTH).toInt());
                s.setHeight(property_node.attribute(CL_XML_ATT_SIZE_HEIGHT).toInt());
                property->SetValue(s);
            }
            break;
        case QVariant::Rect:
            if (property_node.hasAttribute(CL_XML_ATT_POINT_X) && property_node.hasAttribute(CL_XML_ATT_POINT_Y) &&
                property_node.hasAttribute(CL_XML_ATT_SIZE_WIDTH) && property_node.hasAttribute(CL_XML_ATT_SIZE_HEIGHT)) {
                r.setX(property_node.attribute(CL_XML_ATT_POINT_X).toInt());
                r.setY(property_node.attribute(CL_XML_ATT_POINT_Y).toInt());
                r.setWidth(property_node.attribute(CL_XML_ATT_SIZE_WIDTH).toInt());
                r.setHeight(property_node.attribute(CL_XML_ATT_SIZE_HEIGHT).toInt());
                property->SetValue(r);
            }
            break;
        case QVariant::ByteArray:
            node=property_node.firstChild();
            //On va chercher le text
            while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
            if (node.isText()) {
                QByteArray ba=SwBuffer_Toolbox::ConvertStringIntoByteArrayInto(node.nodeValue());
                property->SetValue(QVariant(ba)); 
            }
            break;
        case QVariant::SizePolicy:
            node=property_node.firstChild();
            //On va chercher le text
            while (!node.isNull() && !node.isText()) node=property_node.nextSibling();
            if (node.isText()) {
                QSizePolicy val_to_get;
                if (SwBuffer_Toolbox::ConvertFromString(node.nodeValue(),val_to_get)) {
                    tmp.setValue(val_to_get);
                    property->SetValue(tmp);
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
                QVariant var = text;
                QColor color = qvariant_cast<QColor>(var);
                if (alpha.length ())
                {
                    bool ok = false;
                    color.setAlpha (alpha.toInt (&ok, 16));
                }

                tmp.setValue (color);
                property->SetValue(tmp);
            }
            break;
        default:
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
        property->SetValue(tmp);
    }
    //Type SwFileDescriptor
    if (var.userType()==qMetaTypeId<SwFileDescriptor>() && property_node.hasAttribute(CL_XML_ATT_FD)) {
        SwFileDescriptor fd=var.value<SwFileDescriptor>();
        fd.setFileName(property_node.attribute(CL_XML_ATT_FD));
        tmp.setValue(fd);
        property->SetValue(tmp);
    }
    //Type SwIconDescriptor
    if (var.userType()==qMetaTypeId<SwIconDescriptor>() && property_node.hasAttribute(CL_XML_ATT_ID)) {
        SwIconDescriptor idesc=var.value<SwIconDescriptor>();
        idesc.setPath(property_node.attribute(CL_XML_ATT_ID));
        tmp.setValue(idesc);
        property->SetValue(tmp);
    }
    //Type SwIpV4Address
    if (var.userType()==qMetaTypeId<SwIpV4Address>() && property_node.hasAttribute(CL_XML_ATT_IPV4)) {
        SwIpV4Address value=var.value<SwIpV4Address>();
        value.FromString(property_node.attribute(CL_XML_ATT_IPV4));
        tmp.setValue(value);
        property->SetValue(tmp);
    }
    //Type SwUUID
    if (var.userType()==qMetaTypeId<SwUUID>() && property_node.hasAttribute(CL_XML_ATT_UUID_H) && property_node.hasAttribute(CL_XML_ATT_UUID_L))  {
        SwUUID value=var.value<SwUUID>();
        value.mostSigBits=property_node.attribute(CL_XML_ATT_UUID_H).toLongLong();
        value.leastSigBits=property_node.attribute(CL_XML_ATT_UUID_L).toLongLong();
        tmp.setValue(value);
        property->SetValue(tmp);
    }
        


}
/*! \brief methode permettant de sauver une propriété 
\param[in] parent_property_node noeud parent
\param[in] doc document parent
\param[in] name nom de la propriétés
\param[in] liste des propriétés
*/
void _SwPropertyPersistent_Toolbox::SaveProperty(QDomElement & parent_property_node,QDomDocument &doc,QString name,ISwProperties * properties) {
    SavePropertyExtended(parent_property_node,doc,name,properties,false);
}
/*! \brief methode permettant de sauver une propriété meme si elle a changer
\param[in] parent_property_node noeud parent
\param[in] doc document parent
\param[in] name nom de la propriétés
\param[in] liste des propriétés
\param[in] force la sauvegarde meme si la propriété n'a pas changer
*/
void _SwPropertyPersistent_Toolbox::SavePropertyExtended(QDomElement & parent_property_node,QDomDocument &doc,QString name,ISwProperties * properties,bool forceSave) {

    _SwPropertyImpl_Class * property;
    QVariant var,tmp;
    QDomElement elt;
    QDomText  text_node;
    QDomCDATASection cdata_node;
    QPoint p;
    QSize s;
    QRect r;
    QByteArray working_array;
    bool save_done=false;

    if (QDomImplementation::invalidDataPolicy()!=QDomImplementation::ReturnNullNode) {
        QDomImplementation::setInvalidDataPolicy(QDomImplementation::ReturnNullNode);
    }

    property=dynamic_cast<_SwPropertyImpl_Class *>(properties->GetProperty(name));
    //Si la propriété n'existe pas et qu'elle n'est pas editable (on ne peut ecrire dedans) ou qu'elle n'a pas changer (valeur usine)
    //ou que c'est un type complexe
    if (property==NULL || property->GetComplexeTypeAdapters()!=NULL || !property->IsEditable() || (!forceSave && !property->HasChanged()) )
        return; //Fin
    var=property->GetValue();
    elt=doc.createElement(CL_XML_NODE);
    elt.setAttribute(CL_XML_ATT_NAME,QString(property->GetRealName()));

    //----------------------------------------------------------
    // Gestion des types convertible en QString (et inversement)
    //----------------------------------------------------------
    tmp=QVariant(QString(""));
    //Si la conversion de l'objet en string et inversement est possible, on le sauvegarde en string
    if (var.canConvert(QVariant::String) && tmp.canConvert(var.type()) && var.type () != QVariant::Color) {
        text_node=doc.createTextNode(var.toString());
        if (!text_node.isNull()) {
            elt.appendChild(text_node);
        } else {
            cdata_node=doc.createCDATASection(var.toString());
            if (!cdata_node.isNull()) {
                elt.appendChild(cdata_node);
            } else {
                //Si le save n'a pas reussi alors log
                SW_APP->Logger().Log(LogLvl_Warning,"Unable to save property %s of type %s of component %s because of invalid characters\n",name,var.typeName(),properties->GetHostComponent()->GetName().toLatin1().data());    
            }
        }
        save_done=true;
    } 
    //----------------------------------------------------------
    // Gestion des autres types Qt
    //----------------------------------------------------------
    if (!save_done) {
        switch (var.type()) {
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
                r=var.toRect();
                elt.setAttribute(CL_XML_ATT_POINT_X,r.x());
                elt.setAttribute(CL_XML_ATT_POINT_Y,r.y());
                elt.setAttribute(CL_XML_ATT_SIZE_WIDTH,r.width());
                elt.setAttribute(CL_XML_ATT_SIZE_HEIGHT,r.height());
                save_done=true;
                break;
            case QVariant::ByteArray:
                text_node=doc.createTextNode(SwBuffer_Toolbox::ConvertByteArrayIntoString(var.toByteArray()));
                if (!text_node.isNull()) {
                    elt.appendChild(text_node);
                    save_done=true;
                }
                break;
            case QVariant::SizePolicy:
                text_node=doc.createTextNode(SwBuffer_Toolbox::ConvertIntoString(var.value<QSizePolicy>()));
                if (!text_node.isNull()) {
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
                    if (!text_node.isNull()) {
                        elt.appendChild(text_node);
                        save_done=true;
                    }
                }
                break;
            default:
                break;
        }
    }
    //----------------------------------------------------------
    // Gestion des types user
    //----------------------------------------------------------
    if (!save_done) {
        //Type SwEnum
        if (var.userType()==qMetaTypeId<SwEnum>()) {
            SwEnum enum_value=var.value<SwEnum>();
            elt.setAttribute(CL_XML_ATT_ENUM,QString("%1").arg(enum_value.ToInt()));
            elt.setAttribute(CL_XML_ATT_ENUM_STRING,QString("%1").arg(enum_value.ToString()));
            save_done=true;
        }
        //Type SwFileDescriptor
        if (var.userType()==qMetaTypeId<SwFileDescriptor>()) {
            SwFileDescriptor fd=var.value<SwFileDescriptor>();
            elt.setAttribute(CL_XML_ATT_FD,fd.getFileName());
            save_done=true;
        }
        //Type SwIconDescriptor
        if (var.userType()==qMetaTypeId<SwIconDescriptor>()) {
            SwIconDescriptor idesc=var.value<SwIconDescriptor>();
            elt.setAttribute(CL_XML_ATT_ID,idesc.ToString());
            save_done=true;
        }
        //Type SwIpV4Address
        if (var.userType()==qMetaTypeId<SwIpV4Address>()) {
            SwIpV4Address value=var.value<SwIpV4Address>();
            elt.setAttribute(CL_XML_ATT_IPV4,value.ToString());
            save_done=true;
        }
        //Type SwIpV4Address
        if (var.userType()==qMetaTypeId<SwUUID>()) {
            SwUUID value=var.value<SwUUID>();
            elt.setAttribute(CL_XML_ATT_UUID_H,value.mostSigBits);
            elt.setAttribute(CL_XML_ATT_UUID_L,value.leastSigBits);
            save_done=true;
        }
    }    
    //Si la propriete n'a pas ete enregistrer
    if (!save_done) {
        //Si le save n'a pas reussi alors log
        SW_APP->Logger().Log(LogLvl_Warning,"Unable to save type %s for property %s of component %s\n",var.typeName(),name,properties->GetHostComponent()->GetName().toLatin1().data());    
    } else {
        //Si le save a reussi, enregistrement de l'element
        parent_property_node.appendChild(elt);
    }
}

