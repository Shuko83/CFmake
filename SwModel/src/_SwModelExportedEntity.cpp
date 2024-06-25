/*!
 \file _SwModelExportedEntity.cpp
 \brief Class implementant une entite exportable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "Tools/SwAddress_ToolBox.h"
#include "_SwModelExportedEntity.h"
#include "_SwModelExportedProperty.h"
#include "_SwModelExportedInterfaceC.h"
#include "_SwModelExportedInterfaceP.h"
#include "_SwModelExportedPin.h"
#include "_SwModelExportedExecutable.h"
#include "_SwModelExportedOwnerConfigurable.h"
#include "_SwModelExportedActivable.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"

/*! \brief Constructeur */
_SwModelExportedEntity::_SwModelExportedEntity() {
	_host=NULL;
	_model_host=NULL;
	_model=NULL;
    _type=Ent_Undefined;
}
/*! \brief Destructeur */
_SwModelExportedEntity::~_SwModelExportedEntity() {

}
/*! \brief Effectue la liaison */
void _SwModelExportedEntity::Bind(SwComponent_Class * model){
    SwComponent_Class * root;

    _model=model;
    if (_host_path.isEmpty()) {
        SpecificBind();
        return;
    }
    root=_model_host;
    while (root->GetParent()!=NULL) root=root->GetParent();
    _host=SwAddress_ToolBox::FindTarget(_host_path,root);
    if (_host!=NULL) {
        SpecificBind();   
        _host->OnDestroy.iconnect(*this,&_SwModelExportedEntity::Unbind);
    }
}
/*! \brief Defait la liaison */
void _SwModelExportedEntity::Unbind() {
    if (_host_path.isEmpty()) {
        SpecificUnbind();
        return;
    }
    if (_host!=NULL) {
        SpecificUnbind();
        if (_host->_getReferencesNb()>0)
            _host->OnDestroy.idisconnect(*this,&_SwModelExportedEntity::Unbind);
        _host=NULL;
    }
}
/*! \brief Specific bind */
void _SwModelExportedEntity::SpecificBind() {
}
/*! \brief Defait la liaison */
void _SwModelExportedEntity::SpecificUnbind() {

}
/*! \brief Effectue la construction */
void _SwModelExportedEntity::Build(_SwModelHost_Class * model_host) {
    _model_host=model_host;
    SpecificBuild();

}
/*! \brief Effectue la destruction */
void _SwModelExportedEntity::Destroy() {
    SpecificDestroy();

}
/*! \brief Effectue la construction */
void _SwModelExportedEntity::SpecificBuild() {

}
/*! \brief Effectue la destruction */
void _SwModelExportedEntity::SpecificDestroy() {

}

/*! \brief Creation d'une entity*/
_SwModelExportedEntity * _SwModelExportedEntity::NewEntity(ExportedEntityType type) {
    switch (type) {
        case Ent_Property:
            return new _SwModelExportedProperty;

        case Ent_InterfaceC:
            return new _SwModelExportedInterfaceC;

        case Ent_InterfaceP:
            return new _SwModelExportedInterfaceP;
        case Ent_Pin:
            return new _SwModelExportedPin;

        case Ent_Executable:
            return new _SwModelExportedExecutable;

        case Ent_OwnerConfigurable:
            return new _SwModelExportedOwnerConfigurable;   
                  
        case Ent_Activable:
            return new _SwModelExportedActivable;                  
        default:
            break;
    }
    return NULL;
}

