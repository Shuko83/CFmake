/*!
 \file NodeProperties.cpp
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "NodeProperties.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
NodeProperties::NodeProperties():SwSimpleComponent_Class() {
    _property_name="NoNamedProperty";
    _default_value="0";
    _type=NodeProperties::Integer;

}
/*! \brief Destructeur */
NodeProperties::~NodeProperties() {
}
/*! label noeud*/
QString NodeProperties::GetNodeLabel() {
    return QString("Properties");
}
/*! \brief label visu noeud*/
QString NodeProperties::GetNodeLabelVisu() {
    return GetNodeLabel()+"["+_property_name+"]";
}
