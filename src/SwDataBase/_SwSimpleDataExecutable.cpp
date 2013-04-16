/*!
 \file _SwSimpleDataExecutable.cpp
 \brief Implementation of the Class _SwSimpleDataExecutable permettant de tester l'executeur l'executeur et l'echange de données
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QSet>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwSimpleDataExecutable.h"
#include "SwDataBasicBuffer_Class.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*! \brief Constructeur */
_SwSimpleDataExecutable::_SwSimpleDataExecutable(): SwComponent_Class(){
}
/*! \brief Destructeur */
_SwSimpleDataExecutable::~_SwSimpleDataExecutable(){
    //Desenregistrement des services
    this->UnregisterService(this->GetServiceName());
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwSimpleDataExecutable::InitializeResources() throw(SwException) {
    //Enregistrement des services
    this->RegisterService(this);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwSimpleDataExecutable done\n"));

}
 //----------------------------------------------------
// Interface ISwExecutable_Service
//----------------------------------------------------
/*! \brief Initialisation */
void _SwSimpleDataExecutable::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {
    SW_DEBUG(QString("%1->Initialize at (Absolute)%2").arg(GetName()).arg(start_time));
    _start_time=start_time;
}           
/*! \brief Demarrage */
void _SwSimpleDataExecutable::Start(double current_time) throw (SwException){
    SW_DEBUG(QString("%1->Start at %2").arg(GetName()).arg(current_time-_start_time));
}                      
/*! \brief Execution */
void _SwSimpleDataExecutable::Execute(double current_time,bool is_first_call) throw (SwException){
    SW_DEBUG(QString("%1->Execute(%3) at %2").arg(GetName()).arg(current_time-_start_time).arg(is_first_call));
    //Test a revoir pour les swData en references counting
}                       
/*! \brief Execution */
void _SwSimpleDataExecutable::Stop(double current_time){
    SW_DEBUG(QString("%1->Stop at %2").arg(GetName()).arg(current_time-_start_time));
}                       
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwSimpleDataExecutable::Liberate(){
	//Rien a faire
}             
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwSimpleDataExecutable::GetHostComponent(){
	return this;
}                      
