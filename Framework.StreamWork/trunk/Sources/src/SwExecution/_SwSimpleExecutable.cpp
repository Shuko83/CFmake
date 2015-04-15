/*!
 \file _SwSimpleExecutable.cpp
 \brief Implementation of the Class _SwSimpleExecutable permettant de tester l'executeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QSet>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwSimpleExecutable.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*! \brief Constructeur */
_SwSimpleExecutable::_SwSimpleExecutable(): ExecutableComponent(){
}
/*! \brief Destructeur */
_SwSimpleExecutable::~_SwSimpleExecutable(){
}

 //----------------------------------------------------
// Interface ISwExecutable_Service
//----------------------------------------------------
/*! \brief Initialisation */
void _SwSimpleExecutable::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {
    SW_LOG.Log(LogLvl_Info,QString("%1->Initialize at (Absolute)%2\n").arg(GetName()).arg(start_time));
    _start_time=start_time;
}           
/*! \brief Demarrage */
void _SwSimpleExecutable::Start(double current_time) throw (SwException){
    SW_LOG.Log(LogLvl_Info,QString("%1->Start at %2\n").arg(GetName()).arg(current_time-_start_time));
}                      
/*! \brief Execution */
void _SwSimpleExecutable::Execute(double current_time,bool is_first_call) throw (SwException){
    SW_LOG.Log(LogLvl_Info,QString("%1->Execute(%3) at %2\n").arg(GetName()).arg(current_time-_start_time).arg(is_first_call));
}                       
/*! \brief Execution */
void _SwSimpleExecutable::Stop(double current_time){
    SW_LOG.Log(LogLvl_Info,QString("%1->Stop at %2\n").arg(GetName()).arg(current_time-_start_time));
}                       
