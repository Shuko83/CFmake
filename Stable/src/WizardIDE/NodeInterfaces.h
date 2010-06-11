/*!
 \file NodeInterfaces.h
 \brief Component noeud SwFactoryEntry
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _NodeInterfaces_H
#define _NodeInterfaces_H

/*
  * INCLUDES LOCAUX
  */
#include "SwSimpleComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwFactoryEntry
	\brief Component noeud SwFactoryEntry
*/
class NodeInterfaces : public SwSimpleComponent_Class {
	Q_OBJECT
    Q_PROPERTY(QString InterfaceName READ InterfaceName WRITE setInterfaceName)
    Q_PROPERTY(QString Type READ Type WRITE setType)
    Q_PROPERTY(IsProviderType IsProvider READ IsProvider WRITE setIsProvider)
    Q_ENUMS(IsProviderType)
public:
    /*! \brief Property InterfaceName */
    QString _interface_name;
    void setInterfaceName(QString interface_name) {_interface_name=interface_name;}
    QString InterfaceName() const {return _interface_name;}
    /*! \brief Property DefaultValue */
    QString _type;
    void setType(QString type) {_type=type;}
    QString Type() const {return _type;}
    /*! \brief Property Author */
    enum IsProviderType {yes,no};
    enum IsProviderType _isProvider;
    void setIsProvider(enum IsProviderType isProvider) {_isProvider=isProvider;}
    enum IsProviderType IsProvider() const {return _isProvider;}
public:
    /*! \brief Constructeur */
    NodeInterfaces();
    /*! \brief Destructeur */
    virtual ~NodeInterfaces();
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! \brief label visu noeud*/
    QString GetNodeLabelVisu();
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
