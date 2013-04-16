/*!
 \file NodeInterfaces.cpp
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "NodeInterfaces.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
NodeInterfaces::NodeInterfaces():SwSimpleComponent_Class() {
    _interface_name="NoNamedInterface";
    _type="UndefinedType";
    _isProvider=NodeInterfaces::yes;
}
/*! \brief Destructeur */
NodeInterfaces::~NodeInterfaces() {
}
/*! label noeud*/
QString NodeInterfaces::GetNodeLabel() {
    return QString("Interfaces");
}
/*! \brief label visu noeud*/
QString NodeInterfaces::GetNodeLabelVisu() {
    return GetNodeLabel()+"["+_interface_name+"]";
}
