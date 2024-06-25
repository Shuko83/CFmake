/*!
 \file _SwConfigurationExportedEntity.cpp
 \brief Class implementant une entite exportable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "Tools/SwAddress_ToolBox.h"
#include "_SwConfigurationExportedEntity.h"
#include "_SwConfigurationExportedProperty.h"
#include "_SwConfigurationExportedPerspective.h"
#include "_SwConfigurationExportedExecution.h"
#include "_SwConfigurationExportedOwnerConfigurable.h"


/*! \brief Constructeur */
_SwConfigurationExportedEntity::_SwConfigurationExportedEntity() {
	_host=NULL;
	_model_host=NULL;
	_model=NULL;
    _type=Ent_Undefined;
}
/*! \brief Destructeur */
_SwConfigurationExportedEntity::~_SwConfigurationExportedEntity() {

}
/*! \brief Effectue la liaison */
void _SwConfigurationExportedEntity::Bind(SwComponent_Class * model_host,SwComponent_Class * model){
    SwComponent_Class * root;

    _model_host=model_host;
    _model=model;
    root=_model_host;
    while (root->GetParent()!=NULL) root=root->GetParent();
    _host=SwAddress_ToolBox::FindTarget(_host_path,root);
    if (_host!=NULL) {
        SpecificBind();   
        _host->OnDestroy.iconnect(*this,&_SwConfigurationExportedEntity::Unbind);
    }
}
/*! \brief Defait la liaison */
void _SwConfigurationExportedEntity::Unbind() {
    if (_host!=NULL) {
        SpecificUnbind();
        if (_host->_getReferencesNb()>0)
            _host->OnDestroy.idisconnect(*this,&_SwConfigurationExportedEntity::Unbind);
        _host=NULL;
    }
}
/*! \brief Specific bind */
void _SwConfigurationExportedEntity::SpecificBind() {
}
/*! \brief Defait la liaison */
void _SwConfigurationExportedEntity::SpecificUnbind() {

}
/*! \brief Creation d'une entity*/
_SwConfigurationExportedEntity * _SwConfigurationExportedEntity::NewEntity(ExportedEntityType type) {
    switch (type) {
        case Ent_Property:
            return new _SwConfigurationExportedProperty;
        case Ent_Perspective:
            return new _SwConfigurationExportedPerspective;
        case Ent_Execution:
            return new _SwConfigurationExportedExecution;
        case Ent_OwnerConfigurable:
            return new _SwConfigurationExportedOwnerConfigurable;
        default:
            break;
    }
    return NULL;
}


//--------------------------------------------------------------------
QDebug operator<<( QDebug dbg, const _SwConfigurationExportedEntity &entity )
{
	QString tmpOutput;

	tmpOutput.append("Type : ");
	switch (entity._type)
	{
		case _SwConfigurationExportedEntity::Ent_Property:
				tmpOutput.append( "Property" );
				break;
		case _SwConfigurationExportedEntity::Ent_Perspective:
			tmpOutput.append( "Perspective" );
			break;
		case _SwConfigurationExportedEntity::Ent_Execution:
			tmpOutput.append( "Execution" );
			break;
		case _SwConfigurationExportedEntity::Ent_OwnerConfigurable:
			tmpOutput.append( "OwnerConfigurable" ) ;
			break;
		default:
			break;
	}
	tmpOutput.append( " - Name : " + entity._name );
	tmpOutput.append( " - Exported_name : " + entity._exported_name );
	tmpOutput.append( " - Host_path : " + entity._host_path );

	dbg.nospace()<< QString( "SwConfigurationExportedEntity "+tmpOutput);
	return dbg.maybeSpace();
}
