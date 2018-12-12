/*!
 \file _SwConfigurationExportedExecution.cpp
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwConfigurationExportedExecution.h"



/*! \brief Constructeur */
_SwConfigurationExportedExecution::_SwConfigurationExportedExecution():_SwConfigurationExportedEntity() {
	_type=Ent_Execution;
    _internal_execution=NULL;
}
/*! \brief Destructeur */
_SwConfigurationExportedExecution::~_SwConfigurationExportedExecution() {

}
/*! \brief Specific bind */
void _SwConfigurationExportedExecution::SpecificBind(){
    _internal_execution=dynamic_cast<ISwExecution_Service *>(_host->QueryService(CG_SW_SERVICE_EXECUTION));
    if (_internal_execution==NULL)
        return;
    //Enregistrement des services
    //_model->RegisterService(this);
}
/*! \brief Defait la liaison */
void _SwConfigurationExportedExecution::SpecificUnbind(){
    if (_internal_execution==NULL)
        return;
    //Desenregistrement des services
    //_model->UnregisterService(this->GetServiceName());
    _internal_execution=NULL;
}

      
//---------------------------------------------------------------------
// Interface ISwExecution_Service
//---------------------------------------------------------------------

void _SwConfigurationExportedExecution::Liberate()
{
    
}      
		
/*! \brief Demarrage de l'execution */
void _SwConfigurationExportedExecution::StartExecution(){
    _internal_execution->StartExecution();
}              
/*! \brief demande d'arret de l'execution*/
void _SwConfigurationExportedExecution::AskForStopExecution(){
    _internal_execution->AskForStopExecution();
}              
/*! \brief demande d'arret de l'execution*/
bool _SwConfigurationExportedExecution::IsExecutionStopped(){
    return _internal_execution->IsExecutionStopped();
}              
/*! \brief Arret de l'execution */
void _SwConfigurationExportedExecution::StopExecution(){
    _internal_execution->StopExecution();
}              
/*! \brief definition du fournisseur de temps */
void _SwConfigurationExportedExecution::SetClockProvider(ISwClockProvider * provider){
    _internal_execution->SetClockProvider(provider);
}             

//---------------------------------------------------------------------
// Interface ISwPersistant
//---------------------------------------------------------------------

/*! \brief methode permettant de charger des donnees
\param[in] elt neoud parent
\param[in] finalizer_manager manager de finalisation
*/
void _SwConfigurationExportedExecution::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager){
    _internal_execution->Load(elt,finalizer_manager);
}     
/*! \brief methode permettant de sauver des donnees
\param[in] elt neoud parent
\param[in] doc document parent
*/
void _SwConfigurationExportedExecution::Save(QXmlStreamWriter &writer){
    _internal_execution->Save(writer);
}     	
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------

/*! \brief acces a son composant hote */
SwComponent_Class * _SwConfigurationExportedExecution::GetHostComponent(){
    return _internal_execution->GetHostComponent();
}     