/*!
 \file _SwSwitchExecutor.h
 \brief Implementation of the Class _SwSwitchExecutor realisant la fonction d'executeur de plusiseurs listes de modules
 \version 1.0
 \date dec 2008
 \author HLG
*/
#include <QSet>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwSwitchExecutor.h"
#include <QMessageBox>
#include <SwTime_ToolBox.h>

#define CL_DEFAULT_TIME_STEP 50
#define CL_EXE_TIMER 0
#define CL_EXE_THREAD 1
#define CL_EXE_MAINENTRY 2
#define CL_EXE_FSLAVE 3
#define CL_EXE_PSLAVE 4

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwSwitchExecutor::_SwSwitchExecutor(): SwComponent_Class(),_exe_service(this,&this->_switchExecutionList){

    _defaultActivated_property = 0;
    _executor = 0;
    _time_step=CL_DEFAULT_TIME_STEP;
    _overload.AddKey(0,"No");
    _overload.AddKey(1,"Yes");
    _overload.FromInt(0);
    _executable_entry.AddKey(CL_EXE_TIMER,"Timer");
    _executable_entry.AddKey(CL_EXE_THREAD,"Thread");
    _executable_entry.AddKey(CL_EXE_MAINENTRY,"MainEntry");
    _executable_entry.AddKey(CL_EXE_FSLAVE,"Full Slave");
    _executable_entry.AddKey(CL_EXE_PSLAVE,"Partial Slave");
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
    
    _provider_service = 0;
    _listeners.clear();
    _replayMode=false;
}
/*! \brief Destructeur */
_SwSwitchExecutor::~_SwSwitchExecutor(){
    
    _listeners.clear();
    //Desenregistrement des services
    this->UnregisterService(GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());    
    this->UnregisterService(_properties_service->GetServiceName());
    this->UnregisterService(_exe_service.GetServiceName());
    //Destruction des services
    delete _properties_service;
    delete _provider_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwSwitchExecutor::InitializeResources() throw(SwException) {
    //Creation des service
    _properties_service=new SwProperties_Class(this);
    _provider_service   = new SwInterfaces_Provider_Class(this) ;
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_provider_service);
    this->RegisterService(&_exe_service);
    this->RegisterService(this);


    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
    _provider_service->RegisterProvidedInterface<ISwSwitchExecutorListProvider>("IExecutableList", this);


    //Gestion du pas temporel
    _time_step_property=_properties_service->CreateProperty<uint>("Execution.Time Step (in ms)");
    if (_time_step_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.TimeStep property\n"));
    }
    _time_step_property->SetDescription("Define time step in ms");  
    _time_step_property->SetValue(QVariant(_time_step));
    _time_step_property->GetOnChangeSignal().iconnect(*this,&_SwSwitchExecutor::OnPropertyChange);

    //Overload
    _overload_property=_properties_service->CreateProperty<SwEnum>("Execution.Overload");
    if (_overload_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.Overload property\n"));
    }
    _overload_property->SetDescription("Enable overload");  
    QVariant _overload_variant;
    _overload_variant.setValue(_overload);
    _overload_property->SetValue(_overload_variant);
    _overload_property->GetOnChangeSignal().iconnect(*this,&_SwSwitchExecutor::OnPropertyChange);

    //list name
    _switchExecutionList._list_name_property=_properties_service->CreateProperty<SwEnum>("Execution.Executable list name");
    if (_switchExecutionList._list_name_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.exe_list_name property\n"));
    }
    _switchExecutionList._list_name_property->SetDescription("Exe list name");  
    QVariant _list_name_variant;
    _list_name_variant.setValue(_switchExecutionList._list_name);
    _switchExecutionList._list_name_property->SetValue(_list_name_variant);
    _switchExecutionList._list_name_property->GetOnChangeSignal().iconnect(*this,&_SwSwitchExecutor::OnPropertyChange);

    //Thread priority
    _priority_property=_properties_service->CreateProperty<SwEnum>("Execution.Priority");
    if (_priority_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.Priority property\n"));
    }
    _priority_property->SetDescription("Define thread priority (cf Doc QThread)");  
    QVariant _priority_variant;
    _priority_variant.setValue(_priority);
    _priority_property->SetValue(_priority_variant);
    _priority_property->GetOnChangeSignal().iconnect(*this,&_SwSwitchExecutor::OnPropertyChange);


    //Est l'entree de l'executable
    _executable_entry_property=_properties_service->CreateProperty<SwEnum>("Execution.Execution Type");
    if (_executable_entry_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Execution.Execution Type property\n"));
    }
    _executable_entry_property->SetDescription("Main Thread on timer (QTimer), Threaded, Main entry (ISwExecutor),\nFull Slave (time inherit from parent executor),Partial Slave (time is system time)");  
    QVariant _executable_entry_variant;
    _executable_entry_variant.setValue(_executable_entry);
    _executable_entry_property->SetValue(_executable_entry_variant);
    _executable_entry_property->GetOnChangeSignal().iconnect(*this,&_SwSwitchExecutor::OnPropertyChange);

    //Gestion de l activation par defaut 
    _defaultActivated_property=_properties_service->CreateProperty<bool>("Activation.Default");
    if (_defaultActivated_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register Activation.Default property\n"));
    }
    _defaultActivated_property->SetDescription("Component is Activated/Disactivated by default");  
    _defaultActivated_property->SetValue(QVariant(_defaultActivated));
    _defaultActivated_property->GetOnChangeSignal().iconnect(*this,&_SwSwitchExecutor::OnPropertyChange);

    //Configuration par defaut
    _exe_service.Prepare(_time_step,_overload.ToInt()!=0,TimerType);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwSwitchExecutor done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwSwitchExecutor::OnPropertyChange(ISwProperty * property) {
    if (property==_switchExecutionList._list_name_property) {
        _switchExecutionList._list_name=_switchExecutionList._list_name_property->GetValue().value<SwEnum>();
        if(_switchExecutionList._exeListName.size()!=0) {
            ExeListMap::iterator it = _switchExecutionList._exeListMap.find(_switchExecutionList._list_name.ToString());
            ModeListMap::iterator itm = _switchExecutionList._modeListMap.find(_switchExecutionList._list_name.ToString());
            if(it != _switchExecutionList._exeListMap.end() && itm!=_switchExecutionList._modeListMap.end()) {
                _exe_service._exe_paths = it.value();
                _exe_service._exe_modes = itm.value();
                _exe_service.ResolveLinks();
            }
        }
        notifyListNameChanged();
        return;
    }
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
            default:
                break;
        }
    }
    switch (_executable_entry.ToInt()) {
        case CL_EXE_TIMER:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,TimerType);
            break;
        case CL_EXE_THREAD:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,ThreadType);
            break;
        case CL_EXE_MAINENTRY:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,ExternalType);
            break;
        case CL_EXE_FSLAVE:
        case CL_EXE_PSLAVE:
            _exe_service.Prepare(_time_step,_overload.ToInt()!=0,SlaveType);
            break;
        default:
            break;
    }         
    // gestion de l'activation par defaut
    if ( property == _defaultActivated_property)
    {
        _defaultActivated = _defaultActivated_property->GetValue().value<bool>();
        setActive(_defaultActivated);
    }
}
//----------------------------------------------------
// Interface ISwExecutor
//----------------------------------------------------
/*! \brief Execute le stream
\return le resultat de l'operation */
int _SwSwitchExecutor::StreamExecute() {
    _exe_service.StartExecution();
    _exe_service.Execution();
    _exe_service.StopExecution();
    return 0;
}
//----------------------------------------------------
// Interface ISwExecutable
//----------------------------------------------------
/*! \brief Initialisation */
void _SwSwitchExecutor::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {

    if (_executable_entry.ToInt()<CL_EXE_FSLAVE && !_replayMode)
        return;


    _exe_service.ResolveLinks();
    _exe_list =_exe_service.GetExecutablesList();
    _executor = executor;

    // parcours de tous les executables (independants de l'activation)
    for (int i=0;i<_exe_list->count();i++) 
    {
        // on vérifie si l'activation de l'executable a changé
        ISwExecutable_Service * executable = (*_exe_list)[i];
        if (executable->isActive())
        {
            // on initialise le composant
            if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                executable->Initialize(start_time,executor); 
            else 
                executable->Initialize(SwTime_ToolBox::GetTime(),executor); 
            executable->setRunning(true);
        }
    }

}
/*! \brief Demarrage */
void _SwSwitchExecutor::Start(double current_time) throw (SwException){

    if (_exe_list==0)
        return;

    // parcours de tous les executables (independants de l'activation)
    for (int i=0;i<_exe_list->count();i++) 
    {
        // on vérifie l'activation de l'executable
        ISwExecutable_Service * executable = (*_exe_list)[i];
        if (executable->isActive())
        {
            if (!executable->isRunning())
            {
                // on initialise le composant
                if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                    executable->Initialize(current_time,_executor); 
                else 
                    executable->Initialize(SwTime_ToolBox::GetTime(),_executor); 
                executable->setRunning(true);
            }
            
            // on start le composant
            if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                    executable->Start(current_time); 
            else 
                executable->Start(SwTime_ToolBox::GetTime()); 
        }
        else
        {
            if (executable->isRunning())
            {
                // on arrete le composant
                if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                        executable->Stop(current_time); 
                else 
                    executable->Stop(SwTime_ToolBox::GetTime());
                executable->setRunning(false);
            }
        }
    }
}            
/*! \brief Execution */
void _SwSwitchExecutor::Execute(double current_time,bool is_first_call) throw (SwException){

    if (_exe_list==0)
        return;

    // parcours de tous les executables (independants de l'activation)
    for (int i=0;i<_exe_list->count();i++) 
    {
        // on vérifie l'activation de l'executable
        ISwExecutable_Service * executable = (*_exe_list)[i];
        if (executable->isActive())
        {
            if (!executable->isRunning())
            {
                // on initialise et start le composant
                if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                {
                    executable->Initialize(current_time,_executor); 
                    executable->setRunning(true);
                    executable->Start(current_time); 
                }
                else 
                {
                    executable->Initialize(SwTime_ToolBox::GetTime(),_executor);
                    executable->setRunning(true);
                    executable->Start(SwTime_ToolBox::GetTime()); 
                }
            }
            
            // on execute le composant
            if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                    executable->Execute(current_time,is_first_call); 
            else 
                executable->Execute(SwTime_ToolBox::GetTime(),is_first_call); 
        }
        else
        {
            if (executable->isRunning())
            {
                // on arrete le composant
                if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                        executable->Stop(current_time); 
                else 
                    executable->Stop(SwTime_ToolBox::GetTime());
                executable->setRunning(false);
            }
        }
    }
}            
/*! \brief Execution */
void _SwSwitchExecutor::Stop(double current_time){

    if (_exe_list==NULL)
        return;
    
    for (int i=0;i<_exe_list->count();i++) 
    {  
        ISwExecutable_Service * executable = (*_exe_list)[i];
        if (executable->isRunning())
        {
            if (_executable_entry.ToInt()==CL_EXE_FSLAVE || _replayMode) 
                (*_exe_list)[i]->Stop(current_time);
            else
                (*_exe_list)[i]->Stop(SwTime_ToolBox::GetTime());
            (*_exe_list)[i]->setRunning(false);
        }

    }
    _exe_list->clear();
    //_active_exe_list->clear();
    //_active_exe_list= 0
    _exe_list=0;
    _executor = 0;
}            
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwSwitchExecutor::Liberate(){

}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwSwitchExecutor::GetHostComponent(){
    return this;
}          
//----------------------------------------------------
// Interface ISwAdminSetup
//----------------------------------------------------
/*! \brief Permet d'acceder aux preferences d'administration*/
void _SwSwitchExecutor::AdminSetup() {
    _exe_service.Edit();    
}

//----------------------------------------------------
// Interface ISwSwitchExecutorListProvider
//----------------------------------------------------

/*! \brief acces au nom de la liste en execution */
QString _SwSwitchExecutor::GetListName(){
    return _switchExecutionList._list_name.ToString();
} 
          
void _SwSwitchExecutor::AddSwitchExectuorListener (ISwSwitchExecutorListener * listener){

    if(!_listeners.contains(listener) && (listener != 0)) 
    {
        _listeners.append(listener);
    }
        
}
void _SwSwitchExecutor::RemoveSwitchExectuorListener (ISwSwitchExecutorListener * listener){
    
    _listeners.removeOne(listener);
   
} 

void _SwSwitchExecutor::notifyListNameChanged() {
    QListIterator<ISwSwitchExecutorListener *> iterator(_listeners);
    ISwSwitchExecutorListener * listener = 0;

    while(iterator.hasNext()) {
        listener = iterator.next();
        if(listener != 0) {
            listener->OnListModified();
        }
    } 
}
//----------------------------------------------------
// Interface ISwSupportReplay
//----------------------------------------------------
/*@brief setter replay mode */
void _SwSwitchExecutor::setReplayMode(bool replayMode) {
    _replayMode=replayMode;
}
/*@brief getter replay mode */
bool _SwSwitchExecutor::getReplayMode(){
    return _replayMode;
}
