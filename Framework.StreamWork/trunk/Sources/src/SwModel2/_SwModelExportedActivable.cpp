/*!
 \file _SwModelExportedActivable.cpp
 \brief Class implementant une entite exportable activable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwModelExportedActivable.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"


/*! \brief Constructeur */
_SwModelExportedActivable::_SwModelExportedActivable():_SwModelExportedEntity() {
	_type = Ent_Activable;
    _internal_activable=NULL;
    _isActive = true;
}
/*! \brief Destructeur */
_SwModelExportedActivable::~_SwModelExportedActivable() {

}
/*! \brief Specific bind */
void _SwModelExportedActivable::SpecificBind(){
    _internal_activable=dynamic_cast<ISwActivable *>(_host);
    if (_internal_activable==NULL)
        return;
    //Enregistrement des services
    //_model->RegisterService(this);
    _SwModel_Class * model =  dynamic_cast<_SwModel_Class *> (_model);
    model->setActivationDelegate(_internal_activable);
}
/*! \brief Defait la liaison */
void _SwModelExportedActivable::SpecificUnbind(){
    if (_internal_activable==NULL)
        return;
    //Desenregistrement des services
    _SwModel_Class * model =  dynamic_cast<_SwModel_Class *> (_model);
    model->setActivationDelegate(0) ;
    //_model->UnregisterService(this->GetServiceName());
    _internal_activable=0;
}
   

//---------------------------------------------------------------------
// Interface ISwActivable
//---------------------------------------------------------------------
/*! \brief Permet de changer la valeur d'activation
\param[in] value d'activation
*/
void _SwModelExportedActivable::setActive(bool value)
{
    _isActive = value;
}
/*! \brief Permet de lire la valeur d'activation
\param[out] valeur d'activation
*/
bool _SwModelExportedActivable::isActive()
{
    return _isActive;
}