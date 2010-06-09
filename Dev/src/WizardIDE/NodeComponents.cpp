/*!
 \file NodeComponents.cpp
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "NodeComponents.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
NodeComponents::NodeComponents():SwSimpleComponent_Class() {
    _name="NoNamedComponent";
    _description="NoDescription";
}
/*! \brief Destructeur */
NodeComponents::~NodeComponents() {
}
/*! label noeud*/
QString NodeComponents::GetNodeLabel() {
    return QString("Components");
}
/*! \brief label visu noeud*/
QString NodeComponents::GetNodeLabelVisu() {
    return GetNodeLabel()+"["+_name+"]";
}
/*! slot ajout Interfaces*/
void NodeComponents::Action_Add_Interfaces() {
    SwComponent_ClassPtr child;

    child=SW_FACTORIES.CreateComponent(QString("NodeInterfaces"));
    child->SetName(GetSuggestedNameForChild(child->GetName()));
    AddChild(child);
}
/*! slot ajout Properties*/
void NodeComponents::Action_Add_Properties() {
    SwComponent_ClassPtr child;

    child=SW_FACTORIES.CreateComponent(QString("NodeProperties"));
    child->SetName(GetSuggestedNameForChild(child->GetName()));
    AddChild(child);
}
/*! slot ajout Connectors*/
void NodeComponents::Action_Add_Connectors() {
    SwComponent_ClassPtr child;

    child=SW_FACTORIES.CreateComponent(QString("NodeConnectors"));
    child->SetName(GetSuggestedNameForChild(child->GetName()));
    AddChild(child);
}
