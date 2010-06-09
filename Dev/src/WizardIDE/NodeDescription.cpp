/*!
 \file NodeDescription.cpp
 \brief Component noeud Description
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "NodeDescription.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
NodeDescription::NodeDescription():SwSimpleComponent_Class() {
    _text=(QStringList() << "No" << "Description");
}
/*! \brief Destructeur */
NodeDescription::~NodeDescription() {
}
/*! label noeud*/
QString NodeDescription::GetNodeLabel() {
    return QString("Description");
}
/*! \brief label visu noeud*/
QString NodeDescription::GetNodeLabelVisu() {
    return GetNodeLabel()+"["+_text.join(",")+"]";
}
