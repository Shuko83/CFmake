/*!
\file _SwExecution_Service.cpp
\date 12/04/2006
\brief service pour l'execution
\author  Big
\version 1.0
 */
#include <SwAddress_ToolBox.h> 
#include <SwTime_ToolBox.h>
#include <QMessageBox>

#include "_SwExecution_Service.h"
#include "_SwExecutionSetup.h"
#include "ISwExecutable_Service.h"
#include "_SwExecutor.h"
using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

#define CL_XML_NODE "executable"
#define CL_XML_ATT_PATH "path"
#define CL_XML_ATT_MODE "mode"

/* Constructeur */
_SwExecution_Service::_SwExecution_Service(SwComponent_Class * host):QThread() {
    _host=host;
    _is_first_execute=false;
    _must_be_stopped=true;
    _is_stopped=true;
    _persistentStopNeeded=false;
    _exe_timer=NULL;
    _clockProvider=0;
	_time_step_changed = false;

}
/* Destructeur */
_SwExecution_Service::~_SwExecution_Service() {

}
/* Edit execution list */
void _SwExecution_Service::Edit() {
    _SwExecutionSetup * esetup;
    SwComponent_Class * root_comp;

    root_comp=_host;
    while (root_comp->GetParent()!=NULL) root_comp=root_comp->GetParent();

    esetup=new _SwExecutionSetup(NULL,root_comp,&_exe_paths,&_exe_modes);
    if (esetup->exec()==QDialog::Accepted) {
        _exe_paths=*esetup->GetEditedList();
        _exe_modes=*esetup->GetEditedListMode();
		ResolveLinks();
    }
}
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwExecution_Service::Liberate(){
	for (SwComponent_Class * parent : _observedComponents.keys())
	{
		parent->OnRemoveChild.idisconnect(*this, &_SwExecution_Service::onExecutedComponentRemoved);
		for (SwComponent_Class* comp : _observedComponents.values(parent))
		{
			comp->RemoveServicesManagerObserver(this);
		}
	}
	_observedComponents.clear();
}  
//---------------------------------------------------------------------
// Interface ISwPersistence
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees
\param[in] elt neoud parent
\param[in] finalizer_manager manager de finalisation
*/
void _SwExecution_Service::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager){
    for(QDomElement elt_path = elt.firstChildElement(CL_XML_NODE); !elt_path.isNull(); elt_path = elt_path.nextSiblingElement(CL_XML_NODE))
    {
        if (elt_path.hasAttribute(CL_XML_ATT_PATH)) {
             _exe_paths.push_back(elt_path.attribute(CL_XML_ATT_PATH));       
            if(elt_path.hasAttribute(CL_XML_ATT_MODE)) {
                QString mode=elt_path.attribute(CL_XML_ATT_MODE);
                if (mode=="Normal") {
                    _exe_modes.push_back(Normal_mode);
                } else if (mode=="Replay") {
                    _exe_modes.push_back(Replay_mode);
                } else {
                    _exe_modes.push_back(Both_mode);
                }
            } else {
                _exe_modes.push_back(Both_mode);
            }
        }
    }
	ResolveLinks();
}
/*! \brief methode permettant de sauver des donnees
\param[in] elt neoud parent
\param[in] doc document parent
*/
void _SwExecution_Service::Save(QDomElement & elt,QDomDocument &doc) {
    for (int i=0;i<_exe_paths.count();i++) {
        QDomElement p_elt=doc.createElement(CL_XML_NODE);
        p_elt.setAttribute(CL_XML_ATT_PATH,QString(_exe_paths[i]));
        switch (_exe_modes[i]) {
            case Normal_mode:
                p_elt.setAttribute(CL_XML_ATT_MODE,"Normal");
                break;
            case Replay_mode:
                p_elt.setAttribute(CL_XML_ATT_MODE,"Replay");
                break;
            default:
                break;
        }

        elt.appendChild(p_elt);
    }
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwExecution_Service::GetHostComponent() {
    return _host;
}
//---------------------------------------------------------------------
// Gestion de l'execution des composants selectionnés
//---------------------------------------------------------------------
/*! \brief Resolution des liens */
void _SwExecution_Service::ResolveLinks() {
	for (SwComponent_Class * parent : _observedComponents.keys())
	{
		parent->OnRemoveChild.idisconnect(*this, &_SwExecution_Service::onExecutedComponentRemoved);
		for (SwComponent_Class* comp : _observedComponents.values(parent))
		{
			comp->RemoveServicesManagerObserver(this);
		}
	}
	_observedComponents.clear();

	QSet<ISwExecutable_Service *> old_exe_servs = _exe_servs;
	_exe_servs.clear();

    _SwExecutor *_exeHost=dynamic_cast<_SwExecutor *>(_host);
    _SwExecutionMode mode=Normal_mode;
    if (_exeHost->getReplayMode()) {
        mode=Replay_mode;
    }
    for (int i=0;i<_exe_paths.count();i++) {
        if(_exe_modes[i]==mode || _exe_modes[i]==Both_mode) {
            SwComponent_Class *comp=SwAddress_ToolBox::FindTarget(_exe_paths[i],_host);
			if (comp != NULL && _exeHost != comp) {
				if (!_observedComponents.contains(comp->GetParent()))
				{
					comp->GetParent()->OnRemoveChild.iconnect(*this, &_SwExecution_Service::onExecutedComponentRemoved);
				}
				comp->AddServicesManagerObserver(this);

				_observedComponents.insert(comp->GetParent(), comp);

                ISwExecutable_Service * service=dynamic_cast<ISwExecutable_Service *>(comp->QueryService(CG_SW_SERVICE_EXECUTABLE));
				if (service != NULL)
				{
                    _exe_servs.insert(service);
				}

                ISwSupportReplay * sreplay=dynamic_cast<ISwSupportReplay *>(comp);
                if (sreplay==0 && service!=NULL) {
                    sreplay=dynamic_cast<ISwSupportReplay *>(service);
                }               
                if (sreplay!=0) {
                    sreplay->setReplayMode(_exeHost->getReplayMode());
                }
            }
        }
    }

	// Arret des composant supprimés de la liste
	double t = _clockProvider != 0 ? _clockProvider->queryStopTime() : SwTime_ToolBox::GetTime();
	old_exe_servs.subtract(_exe_servs);

	for (ISwExecutable_Service* service : old_exe_servs) {
		if (service->isRunning())
		{
			service->Stop(t);
			service->setRunning(false);
		}
	}
}
/*! \brief Acces a la liste des services executables */
const QSet<ISwExecutable_Service *>& _SwExecution_Service::GetExecutablesList() const {
    return _exe_servs;
}
/*! \brief Initialisation de tous les composants */
void _SwExecution_Service::InitializeAll(){
    double t=_clockProvider!=0?_clockProvider->queryInitTime():SwTime_ToolBox::GetTime();
	for (ISwExecutable_Service* service : _exe_servs)
	{
        if (service->isActive())
        {
            service->Initialize(t,this);   
            service->setRunning(true);
        }
    }
}
/*! \brief Demarrage de tous les composants */
void _SwExecution_Service::StartAll(){
    double t=_clockProvider!=0?_clockProvider->queryStartTime():SwTime_ToolBox::GetTime();
	for (ISwExecutable_Service* service : _exe_servs)
	{
        if (service->isActive())
        {
            if (!service->isRunning())
            {
                service->Initialize(t,this); 
                service->setRunning(true);
            }
            service->Start(t);   
            
        }
        else
        {
            if (service->isRunning())
            {
                service->Stop(t); 
                service->setRunning(false);
            }
        }
    }
    _is_first_execute=true;
    _persistentStopNeeded=false;

}
/*! \brief Execution de tous les composants */
void _SwExecution_Service::ExecuteAll(){
    bool stopNeeded=false;
    if (_clockProvider!=0 && _clockProvider->queryPause())
        return;

	if(_time_step_changed)
	{
		_exe_timer->setInterval(_time_step);
		_time_step_changed = false;
	}
	
    //Cas ou le clock provider indiquait que le stop est necessaire
    //mais qu'il etait en pause (voir l'affectation du _persistentStopNeeded
    //juste en dessous apres la boucle d'execution)
    if (_persistentStopNeeded) {
        AskForStopExecution();
        StopExecution();
        return;
    }
    double t=_clockProvider!=0?_clockProvider->queryExecuteTime(&stopNeeded):SwTime_ToolBox::GetTime();
	for (ISwExecutable_Service* service : _exe_servs)
	{
        if (service->isActive())
        {
            if (!service->isRunning())
            {
                service->Initialize(t,this); 
                service->setRunning(true);
                service->Start(t); 
				service->Execute(t,true);
            }  
			else
				service->Execute(t,_is_first_execute);
            
        }
        else
        {
            if (service->isRunning())
            {
                service->Stop(t); 
                service->setRunning(false);
            }
        } 
    }
    _is_first_execute=false;
    //Cas ou le clock provider indique que le stop est necessaire
    //mais qu'il est en pause
    if (_clockProvider!=0 && _clockProvider->queryPause() && stopNeeded) {
        //On indique au service d'execution de retarder l'arret au moment ou
        //l'utlisateur reprendra l'execution (voir juste au desus le
        // if (_persistentStopNeeded) )
        _persistentStopNeeded=true;
        return;
    }
    if (stopNeeded) {
        AskForStopExecution();
        StopExecution();
    }
}
/*! \brief Arret de tous les composants */
void _SwExecution_Service::StopAll(){
    double t=_clockProvider!=0?_clockProvider->queryStopTime():SwTime_ToolBox::GetTime();
	for (ISwExecutable_Service* service : _exe_servs)
	{
        if (service->isRunning())
        {
            service->Stop(t);
            service->setRunning(false);
        }
    }
}
//---------------------------------------------------------------------
// Algorithmes d'execution 
//---------------------------------------------------------------------
/*! \brief Run periodique */
void _SwExecution_Service::RunPeriodic() {
    double t_start;
    double t_stop;
    long remainding_delay;
    while (!_must_be_stopped) {
        t_start=SwTime_ToolBox::GetTime();
        ExecuteAll();
        t_stop=SwTime_ToolBox::GetTime();
        remainding_delay=_time_step-(long)(1000*(t_stop-t_start));
        if (remainding_delay>0.0) {
            SwTime_ToolBox::InternalSleep((unsigned long)remainding_delay);        
        } else
            SwTime_ToolBox::InternalSleep(1); //Si surcharge alors attente de 1 ms
    }    
    _is_stopped=true;
}
/*! \brief Run periodique avec gestion de la surcharge*/
void _SwExecution_Service::RunPeriodicWithOverload() {
    double t_start;
    double t_stop;
    long remainding_delay;
    long real_delay;
    long multiplier;
    
    while (!_must_be_stopped) {
        t_start=SwTime_ToolBox::GetTime();
        ExecuteAll();
        t_stop=SwTime_ToolBox::GetTime();
        real_delay=(long)(1000*(t_stop-t_start));
        multiplier=(real_delay/_time_step)+1;        
        remainding_delay=multiplier*_time_step-real_delay;
        if (remainding_delay>0.0) {
            SwTime_ToolBox::InternalSleep((unsigned long)remainding_delay);
        } else
            SwTime_ToolBox::InternalSleep(1); //Si surcharge alors attente de 1 ms
    }    
    _is_stopped=true;
}
//---------------------------------------------------------------------
// Pilotage de l'execution
//---------------------------------------------------------------------
/* Preparation pour l'execution*/
void _SwExecution_Service::Prepare(int time_step,bool overload,_SwExecutionType exe_type) 
{
    if (!_is_stopped && time_step != _time_step )
	{
		_time_step_changed = true;
		_time_step=time_step;
	}
	else
	{
		_time_step=time_step;
		_overload=overload;
		_exe_type=exe_type;
	}
}
/*! \brief Demarrage de l'execution */
void _SwExecution_Service::StartExecution() {
    if (_exe_type==Slave_exe) {
        QMessageBox::warning(NULL,"Fail to start","Unable to start executor in slave mode");
        return;
    }
    InitializeAll();
    StartAll();
    _is_stopped=false;
    _must_be_stopped=false;
    switch (_exe_type) {
        case Timer_exe:
            _exe_timer=new QTimer();
            _exe_timer->setInterval(_time_step);
            _exe_timer->connect(_exe_timer,SIGNAL(timeout()),this,SLOT(ExecuteAll()));
            _exe_timer->start();
            break;
        case Thread_exe:
            start();
            break;
        case External_exe:
            break;
        default:
            break;
    }
}
/*! \brief execution */
void _SwExecution_Service::Execution() {
    if (_overload) {
        RunPeriodicWithOverload();        
    } else {
        RunPeriodic();
    }
}
/*! \brief demande d'arret de l'execution*/
void _SwExecution_Service::AskForStopExecution() {
    _must_be_stopped=true;
    switch (_exe_type) {
        case Timer_exe:
			if(_exe_timer)
			{
				_exe_timer->stop();
				delete _exe_timer;
				_exe_timer=NULL;
			}
            _is_stopped=true;
            break;
        case Thread_exe:
            break;
        case External_exe:
            break;
        default:
            break;
    }
}
/*! \brief demande d'arret de l'execution*/
bool _SwExecution_Service::IsExecutionStopped() {
    return _is_stopped;
}
/*! \brief Arret de l'execution */
void _SwExecution_Service::StopExecution() {
    switch (_exe_type) {
        case Timer_exe:
            break;
        case Thread_exe:
            wait();
            _is_stopped=true;
            break;
        case External_exe:
            _is_stopped=true;
            break;
        default:
            break;
    }
    StopAll();
}
/*! \brief definition du fournisseur de temps */
void _SwExecution_Service::SetClockProvider(ISwClockProvider * provider) {
    _clockProvider=provider;
}

//---------------------------------------------------------------------
// QThread
//---------------------------------------------------------------------
/*! \brief run du thread */
void _SwExecution_Service::run() {
    Execution();
}

//---------------------------------------------------------------------------------
void _SwExecution_Service::OnRegisterService(ISwService * service)
{
	ISwExecutable_Service * exe_serv = dynamic_cast<ISwExecutable_Service *>(service);
	if (exe_serv && exe_serv->GetServiceName() == CG_SW_SERVICE_EXECUTABLE)
		_exe_servs.insert(exe_serv);
}

//---------------------------------------------------------------------------------
void _SwExecution_Service::OnUnregisterService(ISwService * service)
{
	ISwExecutable_Service * exe_serv = dynamic_cast<ISwExecutable_Service *>(service);
	if (exe_serv)
		_exe_servs.remove(exe_serv);
}

//---------------------------------------------------------------------------------
void _SwExecution_Service::onExecutedComponentRemoved(StreamWork::SwCore::SwComponent_Class * parent, StreamWork::SwCore::SwComponent_Class *child)
{
	ISwExecutable_Service * service = dynamic_cast<ISwExecutable_Service *>(child->QueryService(CG_SW_SERVICE_EXECUTABLE));
	if (service)
		_exe_servs.remove(service);

	child->RemoveServicesManagerObserver(this);

	_observedComponents.remove(parent, child);
	if (_observedComponents.values(parent).isEmpty())
	{
		parent->OnRemoveChild.idisconnect(*this, &_SwExecution_Service::onExecutedComponentRemoved);
		_observedComponents.remove(parent);
	}
}
