/*!
 \file NodeConnectors.cpp
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "NodeConnectors.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
NodeConnectors::NodeConnectors():SwSimpleComponent_Class() {
    _connector_name="NoNamedConnector";
    _type="UndefinedType";
    _receivedData=NodeConnectors::yes;
}
/*! \brief Destructeur */
NodeConnectors::~NodeConnectors() {
}
/*! \brief label noeud*/
QString NodeConnectors::GetNodeLabel() {
    return QString("Connectors");
}
/*! \brief label visu noeud*/
QString NodeConnectors::GetNodeLabelVisu() {
    return GetNodeLabel()+"["+_connector_name+"]";
}
