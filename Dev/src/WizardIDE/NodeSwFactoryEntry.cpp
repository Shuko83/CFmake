/*!
 \file NodeSwFactoryEntry.cpp
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include <QMessageBox>
#include "NodeSwFactoryEntry.h"
#include "NodeDescription.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
NodeSwFactoryEntry::NodeSwFactoryEntry():SwSimpleComponent_Class() {
    _name="NameOfPlugin";
    _author="AuthorOfPlugin";
    _path="C:\\tmp";
    _isOwnPluginInDistribution=NodeSwFactoryEntry::no;
}
/*! \brief Destructeur */
NodeSwFactoryEntry::~NodeSwFactoryEntry() {
}
/*! label noeud*/
QString NodeSwFactoryEntry::GetNodeLabel() {
    return QString("SwFactoryEntry");
}
/*! \brief label visu noeud*/
QString NodeSwFactoryEntry::GetNodeLabelVisu() {
    return GetNodeLabel()+"["+_name+"]";
}
/*! slot ajout composants*/
void NodeSwFactoryEntry::Action_Add_Components() {
    SwComponent_ClassPtr child;

    child=SW_FACTORIES.CreateComponent(QString("NodeComponents"));
    child->SetName(GetSuggestedNameForChild(child->GetName()));
    AddChild(child);
}
/*! slot ajout description*/
void NodeSwFactoryEntry::Action_Add_Description() {
    SwComponent_ClassPtr child;

    child=GetFirstChild();
    while (child!=NULL) {
        if (dynamic_cast<NodeDescription *>((SwComponent_Class *)child)!=NULL) {
            QMessageBox::warning(NULL,"Wizard Alert","Only one description can be added");
            return;
        }
        child=GetNextChild();
    }

    child=SW_FACTORIES.CreateComponent(QString("NodeDescription"));
    child->SetName(GetSuggestedNameForChild(child->GetName()));
    AddChild(child);
}
