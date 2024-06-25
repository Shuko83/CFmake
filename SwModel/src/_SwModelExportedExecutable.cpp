/*!
 \file _SwModelExportedExecutable.cpp
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwModelExportedExecutable.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"


/*! \brief Constructeur */
_SwModelExportedExecutable::_SwModelExportedExecutable():_SwModelExportedEntity() {
	_type=Ent_Executable;
    _internal_executable=NULL;
    _isActive = true;
}
/*! \brief Destructeur */
_SwModelExportedExecutable::~_SwModelExportedExecutable() {

}
/*! \brief Specific bind */
void _SwModelExportedExecutable::SpecificBind(){
	if (_internal_executable)
		return;

    _internal_executable=dynamic_cast<ISwExecutable_Service *>(_host->QueryService(CG_SW_SERVICE_EXECUTABLE));
    if (_internal_executable==NULL)
        return;
    //Enregistrement des services
    _model->RegisterService(this);
}
/*! \brief Defait la liaison */
void _SwModelExportedExecutable::SpecificUnbind(){
    if (_internal_executable==NULL)
        return;
    //Desenregistrement des services
    _model->UnregisterService(this->GetServiceName());
    _internal_executable=NULL;
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwModelExportedExecutable::Liberate() {

}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwModelExportedExecutable::GetHostComponent(){
	return _model;
}              
//---------------------------------------------------------------------
// Interface ISwExecutable_Service
//---------------------------------------------------------------------
/*! \brief Initialisation */
void _SwModelExportedExecutable::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException){
    _internal_executable->Initialize(start_time,executor);
}             
/*! \brief Demarrage */
void _SwModelExportedExecutable::Start(double current_time) throw (SwException){
    _internal_executable->Start(current_time);
}              
/*! \brief Execution */
void _SwModelExportedExecutable::Execute(double current_time,bool is_first_call) throw (SwException){
    _internal_executable->Execute(current_time,is_first_call);
}              
/*! \brief Execution */
void _SwModelExportedExecutable::Stop(double current_time){
    _internal_executable->Stop(current_time);
}              
//---------------------------------------------------------------------
// Interface ISwSupportReplay
//---------------------------------------------------------------------
/*@brief setter replay mode */
void _SwModelExportedExecutable::setReplayMode(bool replayMode) {
    ISwSupportReplay * sreplay=dynamic_cast<ISwSupportReplay *>(_host);
    if (sreplay==0) {
        sreplay=dynamic_cast<ISwSupportReplay *>(_internal_executable);
    }
    if (sreplay!=0) {
        sreplay->setReplayMode(replayMode);
    }
}
/*@brief getter replay mode */
bool _SwModelExportedExecutable::getReplayMode() {
    ISwSupportReplay * sreplay=dynamic_cast<ISwSupportReplay *>(_host);
    if (sreplay==0) {
        sreplay=dynamic_cast<ISwSupportReplay *>(_internal_executable);
    }
    if (sreplay!=0) {
        return sreplay->getReplayMode();
    }
    return false;
}

//---------------------------------------------------------------------
// Interface ISwActivable
//---------------------------------------------------------------------
/*! \brief Permet de changer la valeur d'activation
\param[in] value d'activation
*/
void _SwModelExportedExecutable::setActive(bool value)
{
    _model->setActive(value);
}
/*! \brief Permet de lire la valeur d'activation
\param[out] valeur d'activation
*/
bool _SwModelExportedExecutable::isActive()
{
    return _model->isActive();
}

