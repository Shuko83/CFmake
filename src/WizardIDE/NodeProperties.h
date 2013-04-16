/*!
 \file NodeProperties.h
 \brief Component noeud SwFactoryEntry
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _NodeProperties_H
#define _NodeProperties_H

/*
  * INCLUDES LOCAUX
  */
#include "SwSimpleComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwFactoryEntry
	\brief Component noeud SwFactoryEntry
*/
class NodeProperties : public SwSimpleComponent_Class {
	Q_OBJECT
    Q_PROPERTY(QString PropertyName READ PropertyName WRITE setPropertyName)
    Q_PROPERTY(QString DefaultValue READ DefaultValue WRITE setDefaultValue)
    Q_PROPERTY(PossibleType Type READ Type WRITE setType)
    Q_ENUMS(PossibleType)
public:
    /*! \brief Property PropertyName */
    QString _property_name;
    void setPropertyName(QString property_name) {_property_name=property_name;}
    QString PropertyName() const {return _property_name;}
    /*! \brief Property DefaultValue */
    QString _default_value;
    void setDefaultValue(QString default_value) {_default_value=default_value;}
    QString DefaultValue() const {return _default_value;}
    /*! \brief Property Author */
    enum PossibleType {Integer,UnsignedInteger,Boolean,Double,SwEnum,String};
    enum PossibleType _type;
    void setType(enum PossibleType type) {_type=type;}
    enum PossibleType Type() const {return _type;}
public:
    /*! \brief Constructeur */
    NodeProperties();
    /*! \brief Destructeur */
    virtual ~NodeProperties();
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! \brief label visu noeud*/
    QString GetNodeLabelVisu();
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
