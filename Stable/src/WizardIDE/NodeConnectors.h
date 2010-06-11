/*!
 \file NodeConnectors.h
 \brief Component noeud SwFactoryEntry
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _NodeConnectors_H
#define _NodeConnectors_H

/*
  * INCLUDES LOCAUX
  */
#include "SwSimpleComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class SwFactoryEntry
	\brief Component noeud SwFactoryEntry
*/
class NodeConnectors : public SwSimpleComponent_Class {
	Q_OBJECT
    Q_PROPERTY(QString ConnectorName READ ConnectorName WRITE setConnectorName)
    Q_PROPERTY(QString Type READ Type WRITE setType)
    Q_PROPERTY(ReceiveDataType ReceiveData READ ReceiveData WRITE setReceiveData)
    Q_ENUMS(ReceiveDataType)
public:
    /*! \brief Property InterfaceName */
    QString _connector_name;
    void setConnectorName(QString connector_name) {_connector_name=connector_name;}
    QString ConnectorName() const {return _connector_name;}
    /*! \brief Property DefaultValue */
    QString _type;
    void setType(QString type) {_type=type;}
    QString Type() const {return _type;}
    /*! \brief Property Author */
    enum ReceiveDataType {yes,no};
    enum ReceiveDataType _receivedData;
    void setReceiveData(enum ReceiveDataType receivedData) {_receivedData=receivedData;}
    enum ReceiveDataType ReceiveData() const {return _receivedData;}
public:
    /*! \brief Constructeur */
    NodeConnectors();
    /*! \brief Destructeur */
    virtual ~NodeConnectors();
    /*! label noeud*/
    virtual QString GetNodeLabel();
    /*! \brief label visu noeud*/
    QString GetNodeLabelVisu();
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
