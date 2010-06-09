/*!
 \file _SwExecutor.cpp
 \brief Implementation of the Class _SwExecutor realisant la fonction d'executeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QSet>
#include <QApplication>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwExecutor.h"
#include <QMessageBox>
#include <SwTime_ToolBox.h>

#define CL_DEFAULT_TIME_STEP 50
#define CL_EXE_TIMER 0
#define CL_EXE_THREAD 1
#define CL_EXE_MAINENTRY 2
#define CL_EXE_FSLAVE 3
#define CL_EXE_PSLAVE 4
#define CL_EXE_TIMER_MAINENTRY 5

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwExecutor::_SwExecutor(): SwComponent_Class(),_exe_service(this){

    _time_step=CL_DEFAULT_TIME_STEP;
    _overload.AddKey(0,"No");
    _overload.AddKey(1,"Yes");
    _overload.FromInt(0);
    _executable_entry.AddKey(CL_EXE_TIMER,"Timer");
    _executable_entry.AddKey(CL_EXE_THREAD,"Thread");
    _executable_entry.AddKey(CL_EXE_MAINENTRY,"MainEntry");
    _executable_entry.AddKey(CL_EXE_FSLAVE,"Full Slave");
    _executable_entry.AddKey(CL_EXE_PSLAVE,"Partial Slave");
    _executable_entry.AddKey(CL_EXE_TIMER_MAINENTRY,"Timer/MainEntry");
    _executable_entry.FromInt(0);
    _priority.AddKey(QThread::IdlePriority,"IdlePriority");
    _priority.AddKey(QThread::LowestPriority,"LowestPriority");
    _priority.AddKey(QThread::LowPriority,"LowPriority");
    _priority.AddKey(QThread::NormalPriority,"NormalPriority");
    _priority.AddKey(QThread::HighPriority,"HighPriority");
    _priority.AddKey(QThread::HighestPriority,"HighestPriority");
    _priority.AddKey(QThread::TimeCriticalPriority,"TimeCriticalPriority");
    _priority.AddKey(QThread::InheritPriority,"InheritPriority");
    _priority.FromInt((int)_exe_service.priority());
    _exe_list=NULL;
    _replayMode=false;
}
/*! \brief Destructeur */
_SwExecutor::~_SwExecutor(){
    switch (_executable_entry.ToInt()) {
        case CL_EXE_MAINENTRY:
        case CL_EXE_FSLAVE:
        case CL_EXE_PSLAVE:
        case CL_EXE_TIMER_MAINENTRY:
            SW_APP->RegisterExecutor(0);
            break;
        default:
            break;
    }
    //Desenregistrement des services
    this->UnregisterService(GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    this->UnregisterService(_exe_service.GetServiceName());
    //Destruction des services
    delete _properties_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwExecutor::InitializeResources() throw(SwException) {
    //Creation des service
    _properties_service=new SwProperties_Class(this);
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(&_exe_service);
    this->RegisterService(this);

    //Gestion du pas temporel
    _time_step_property=_properties_service->CreateProperty<uint>("Execution.Time Step (in ms)");
    if (_time_step_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.TimeStep property\n"));
    }
    _time_step_property->SetDescription("Define time step in ms");  
    _time_step_property->SetValue(QVariant(_time_step));
    _time_step_property->GetOnChangeSignal().iconnect(*this,&_SwExecutor::OnPropertyChange);

    //Overload
    _overload_property=_properties_service->CreateProperty<SwEnum>("Execution.Overload");
    if (_overload_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.Overload property\n"));
    }
    _overload_property->SetDescription("Enable overload");  
    QVariant _overload_variant;
    _overload_variant.setValue(_overload);
    _overload_property->SetValue(_overload_variant);
    _overload_property->GetOnChangeSignal().iconnect(*this,&_SwExecutor::OnPropertyChange);

    //Thread priority
    _priority_property=_properties_service->CreateProperty<SwEnum>("Execution.Priority");
    if (_priority_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.Priority property\n"));
    }
    _priority_property->SetDescription("Define thread priority (cf Doc QThread)");  
    QVariant _priority_variant;
    _priority_variant.setValue(_priority);
    _priority_property->SetValue(_priority_variant);
    _priority_property->GetOnChangeSignal().iconnect(*this,&_SwExecutor::OnPropertyChange);


    //Est l'entree de l'executable
    _executable_entry_property=_properties_service->CreateProperty<SwEnum>("Execution.Execution Type");
    if (_executable_entry_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.Execution Type property\n"));
    }
    _executable_entry_property->SetDescription("Main Thread on timer (QTimer), Threaded, Main entry (ISwExecutor),\nFull Slave (time inherit from parent executor),Partial Slave (time is system time)");  
    QVariant _executable_entry_variant;
    _executable_entry_variant.setValue(_executable_entry);
    _executable_entry_property->SetValue(_executable_entry_variant);
    _executable_entry_property->GetOnChangeSignal().iconnect(*this,&_SwExecutor::OnPropertyChange);

    //Configuration par defaut
    _exe_service.Prepare(_time_step,_overload.ToInt()!=0,Timer_exe);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwExecutor done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwExecutor::OnPropertyChange(ISwProperty * property) {
    if (property==_priority_property) {
        SwEnum new_priority=_priority_property->GetValue().value<SwEnum>();
        _exe_service.setPriority((QThread::Priority)new_priority.ToInt());
        return;
    }
    if (property==_time_step_property) {
        _time_step=_time_step_property->GetValue().toUInt();
    }
    if (property==_overload_property) {
       _overload=_overload_property->GetValue().value<SwEnum>();
    }
    if (property==_executable_entry_property) {
        SwEnum new_executable_entry=_executable_entry_property->GetValue().value<SwEnum>();
        if (new_executable_entry.ToInt()!=_executable_entry.ToInt() && _executable_entry.ToInt()==CL_EXE_MAINENTRY) {
            SW_APP->RegisterExecutor(NULL);
        }
        _executable_entry=new_executable_entry;
        switch (_executable_entry.ToInt()) {
            case CL_EXE_TIMER:
                if (!_time_step_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_time_step_property->GetName(),true);
                if (_priority_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_priority_property->GetName(),false);
                break;
            case CL_EXE_THREAD:
                if (!_time_step_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_time_step_property->GetName(),true);
                if (!_overload_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_overload_property->GetName(),true);
                if (!_priority_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_priority_property->GetName(),true);
                break;
            case CL_EXE_MAINENTRY:
                if (!_time_step_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_time_step_property->GetName(),true);
                if (!_overload_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_overload_property->GetName(),true);
                if (_priority_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_priority_property->GetName(),false);
                SW_APP->RegisterExecutor(this);
                break;
            case CL_EXE_FSLAVE:
            case CL_EXE_PSLAVE:
                if (_time_step_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_time_step_property->GetName(),false);
                if (_overload_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_overload_property->GetName(),false);
                if (_priority_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_priority_property->GetName(),false);
                break;
            case CL_EXE_TIMER_MAINENTRY:
                if (!_time_step_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_time_step_property->GetName(),true);
                if (_priority_property->IsEditable()) 
                    _properties_service->ChangePropertyEdition(_priority_property->GetName(),false);
                SW_APP->RegisterExecutor(this);
                break;
            default:
                break;
        }
    }
    switch (_executable_entry.ToInt()) {
        case CL_EXE_TIMER:
        case CL_EXE_TIMER_MAINENTRY:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,Timer_exe);
            break;
        case CL_EXE_THREAD:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,Thread_exe);
            break;
        case CL_EXE_MAINENTRY:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,External_exe);
            break;
        case CL_EXE_FSLAVE:
        case CL_EXE_PSLAVE:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,Slave_exe);
            break;
        default:
            break;
    }
}
//----------------------------------------------------
// Interface ISwExecutor
//----------------------------------------------------
/*! \brief Execute le stream
\return le resultat de l'operation */
int _SwExecutor::StreamExecute() {
    _exe_service.StartExecution();
    if (_executable_entry.ToInt()==CL_EXE_TIMER_MAINENTRY) {
        QApplication * gui_app=dynamic_cast<QApplication *>(qApp);
        gui_app->connect(gui_app, SIGNAL(lastWindowClosed()),this, SLOT(onQuit()));
    } else {
        _exe_service.Execution();
        _exe_service.StopExecution();
    }
    return 0;
}
//----------------------------------------------------
// Interface ISwExecutable
//----------------------------------------------------
/*! \brief Initialisation */
void _SwExecutor::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {
    if (_executable_entry.ToInt()<CL_EXE_FSLAVE && !_replayMode)
        return;
    _exe_service.ResolveLinks();
    _exe_list=_exe_service.GetExecutablesList();
    for (int i=0;i<_exe_list->count();i++) {  
        if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
            (*_exe_list)[i]->Initialize(start_time,executor); 
        else 
            (*_exe_list)[i]->Initialize(SwTime_ToolBox::GetTime(),executor); 
        (*_exe_list)[i]->setRunning(true);
    }

}
/*! \brief Demarrage */
void _SwExecutor::Start(double current_time) throw (SwException){
    if (_exe_list==NULL)
        return;
    for (int i=0;i<_exe_list->count();i++) {  
        if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
            (*_exe_list)[i]->Start(current_time);
        else 
            (*_exe_list)[i]->Start(SwTime_ToolBox::GetTime()); 
    }
}            
/*! \brief Execution */
void _SwExecutor::Execute(double current_time,bool is_first_call) throw (SwException){
    if (_exe_list==NULL)
        return;
    for (int i=0;i<_exe_list->count();i++) {  
        if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
            (*_exe_list)[i]->Execute(current_time,is_first_call);
        else 
            (*_exe_list)[i]->Execute(SwTime_ToolBox::GetTime(),is_first_call); 
    }
}            
/*! \brief Execution */
void _SwExecutor::Stop(double current_time){
    if (_exe_list==NULL)
        return;
    for (int i=0;i<_exe_list->count();i++) {  
        if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
            (*_exe_list)[i]->Stop(current_time);
        else
            (*_exe_list)[i]->Stop(SwTime_ToolBox::GetTime());
        (*_exe_list)[i]->setRunning(false);

    }
    _exe_list->clear();
    _exe_list=NULL;
}            
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwExecutor::Liberate(){

}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwExecutor::GetHostComponent(){
    return this;
}          
//----------------------------------------------------
// Interface ISwAdminSetup
//----------------------------------------------------
/*! \brief Permet d'acceder aux preferences d'administration*/
void _SwExecutor::AdminSetup() {
    _exe_service.Edit();    
}
/*! \brief sur fin*/
void _SwExecutor::onQuit() {
    _exe_service.AskForStopExecution();
    while (!_exe_service.IsExecutionStopped()) {
        SwTime_ToolBox::InternalSleep(10);    
    }
    _exe_service.StopExecution();
}
//----------------------------------------------------
// Interface ISwSupportReplay
//----------------------------------------------------
/*@brief setter replay mode */
void _SwExecutor::setReplayMode(bool replayMode) {
    _replayMode=replayMode;
}
/*@brief getter replay mode */
bool _SwExecutor::getReplayMode(){
    return _replayMode;
}
