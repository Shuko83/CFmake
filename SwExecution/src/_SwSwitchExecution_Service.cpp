/*!
\file _SwSwitchExecution_Service.cpp
\date dec 2008
\brief service pour l'execution
\author  HLG
\version 1.0
 */
#include "Tools/SwAddress_ToolBox.h"
#include "Tools/SwTime_ToolBox.h"
#include <QMessageBox>

#include "_SwSwitchExecution_Service.h"
#include "_SwSwitchExecutionSetup.h"
#include "ISwExecutable_Service.h"
#include "_SwSwitchExecutor.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

#define CL_XML_NODE "executable"
#define CL_XML_ATT_PATH "path"

/* Constructeur */
_SwSwitchExecution_Service::_SwSwitchExecution_Service(SwComponent_Class * host,_SwSwitchExecutionList * switchExecutionList):QThread() {
    _host=host;
    _is_first_execute=false;
    _must_be_stopped=true;
    _persistentStopNeeded=false;
    _is_stopped=true;
    _exe_timer=NULL;
    _switchExecutionList = switchExecutionList;
    _clockProvider=0;

}
/* Destructeur */
_SwSwitchExecution_Service::~_SwSwitchExecution_Service() {

}
/* Edit execution list */
void _SwSwitchExecution_Service::Edit() {
    _SwSwitchExecutionSetup * esetup;
    SwComponent_Class * root_comp;

    root_comp=_host;
    while (root_comp->GetParent()!=NULL) root_comp=root_comp->GetParent();

    esetup=new _SwSwitchExecutionSetup(NULL,root_comp,&_exe_paths,_switchExecutionList);
    if (esetup->exec()==QDialog::Accepted) {
        esetup->SaveExePathInCurrentList();
        _exe_paths=*esetup->GetEditedList();
        _exe_modes=*esetup->GetEditedListModes();
        ResolveLinks();
    }
}
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwSwitchExecution_Service::Liberate(){

}  
//---------------------------------------------------------------------
// Interface ISwPersistence
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees
\param[in] elt neoud parent
\param[in] finalizer_manager manager de finalisation
*/
void _SwSwitchExecution_Service::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager){
    // parcours des listes
    int index = 0;
    for(QDomElement elt_list = elt.firstChildElement("List"); !elt_list.isNull(); elt_list = elt_list.nextSiblingElement("List")) {
        if (elt_list.hasAttribute("Name")) {
            // ajout de la liste
            _switchExecutionList->AddListName(index, elt_list.attribute("Name"));
            // parcours des module appartenant à la liste
            QList<QString> exeList;
            QList<_SwExecutionMode> modeList;
            for(QDomElement elt_module = elt_list.firstChildElement("Module"); !elt_module.isNull(); elt_module = elt_module.nextSiblingElement("Module")) {
                // creation de la liste d'executables
                if(elt_module.hasAttribute("Path")) {
                    exeList.push_back(elt_module.attribute("Path"));
                    if(elt_module.hasAttribute("Mode")) {
                        QString mode=elt_module.attribute("Mode");
                        if (mode=="Normal") {
                            modeList.push_back(Normal_mode);
                        } else if (mode=="Replay") {
                            modeList.push_back(Replay_mode);
                        } else {
                            modeList.push_back(Both_mode);
                        }
                    } else {
                        modeList.push_back(Both_mode);
                    }
                }
            }
            _switchExecutionList->AddExeList(elt_list.attribute("Name"), exeList,modeList);
                   
        }
        index++;
    }
    
    // mise à jour de l'exe path dans _SwSwitchExecution_Service
    // ainsi que de la propriété du _SwSwitchExecutor
    if(_switchExecutionList->_exeListName.size()!=0) {
        _switchExecutionList->_list_name.FromInt(0);
        QVariant _list_name_qvariant;
        _list_name_qvariant.setValue(_switchExecutionList->_list_name);
        _switchExecutionList->_list_name_property->SetValue(_list_name_qvariant);
        ExeListMap::iterator it = _switchExecutionList->_exeListMap.find(_switchExecutionList->_list_name.ToString());
        ModeListMap::iterator itm = _switchExecutionList->_modeListMap.find(_switchExecutionList->_list_name.ToString());
        if(it != _switchExecutionList->_exeListMap.end() && itm != _switchExecutionList->_modeListMap.end()) {
            _exe_paths.clear();
            _exe_modes.clear();
            for(int i=0; i<it.value().size(); i++) {
                _exe_paths.push_back(it.value()[i]);
                _exe_modes.push_back(itm.value()[i]);
            }
        }
    }
}

/*! \brief methode permettant de sauver des donnees
\param[in] elt neoud parent
\param[in] doc document parent
*/
void _SwSwitchExecution_Service::Save(QXmlStreamWriter& writer)
{
	ExeListMap::iterator it = _switchExecutionList->_exeListMap.begin();
	ModeListMap::iterator itm = _switchExecutionList->_modeListMap.begin();
	while (it != _switchExecutionList->_exeListMap.end() && itm != _switchExecutionList->_modeListMap.end())
	{
		writer.writeStartElement("List");
		writer.writeAttribute("Name", it.key());
		QList<QString> pathList;
		pathList = it.value();
		QList<_SwExecutionMode> mode = itm.value();
		for (int i = 0; i < pathList.count(); ++i)
		{
			writer.writeStartElement("Module");
			writer.writeAttribute("Path", pathList[i]);
			switch (mode[i])
			{
				case Normal_mode:
					writer.writeAttribute("Mode", "Normal");
					break;
				case Replay_mode:
					writer.writeAttribute("Mode", "Replay");
					break;
				default:
					break;
			}
			writer.writeEndElement();
		}
		writer.writeEndElement();
		++it;
		++itm;
	}
}

//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwSwitchExecution_Service::GetHostComponent() {
    return _host;
}
//---------------------------------------------------------------------
// Gestion de l'execution des composants selectionnés
//---------------------------------------------------------------------
/*! \brief Resolution des liens */
void _SwSwitchExecution_Service::ResolveLinks() {    
    _SwSwitchExecutor *_exeHost=dynamic_cast<_SwSwitchExecutor *>(_host);
    _SwExecutionMode mode=Normal_mode;
    if (_exeHost->getReplayMode()) {
        mode=Replay_mode;
    }
    _exe_servs.clear();
    for (int i=0;i<_exe_paths.count();i++) {
        if(_exe_modes[i]==mode || _exe_modes[i]==Both_mode) {
            SwComponent_Class *comp=SwAddress_ToolBox::FindTarget(_exe_paths[i],_host);
            if (comp!=NULL) {
                ISwExecutable_Service * service=dynamic_cast<ISwExecutable_Service *>(comp->QueryService(CG_SW_SERVICE_EXECUTABLE));
                if (service!=NULL) {
                    _exe_servs.push_back(service);
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
}
/*! \brief Acces a la liste des services executables */
QList<ISwExecutable_Service *> * _SwSwitchExecution_Service::GetExecutablesList() {
    return & _exe_servs;
}
/*! \brief Initialisation de tous les composants */
void _SwSwitchExecution_Service::InitializeAll(){
    ResolveLinks();
    double t=_clockProvider!=0?_clockProvider->queryInitTime():SwTime_ToolBox::GetTime();
    ISwExecutable_Service * service = 0;
    for (int i=0;i<_exe_servs.count();i++) 
    {  
        service = _exe_servs[i];
        if (service->isActive())
        {
            service->Initialize(t,this);   
            service->setRunning(true);
        }
    }
}
/*! \brief Demarrage de tous les composants */
void _SwSwitchExecution_Service::StartAll(){
    double t=_clockProvider!=0?_clockProvider->queryStartTime():SwTime_ToolBox::GetTime();
    ISwExecutable_Service * service = 0;
    for (int i=0;i<_exe_servs.count();i++) 
    {    
        service = _exe_servs[i];
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
void _SwSwitchExecution_Service::ExecuteAll(){
    bool stopNeeded=false;
    if (_clockProvider!=0 && _clockProvider->queryPause())
        return;
    //Cas ou le clock provider indiquait que le stop est necessaire
    //mais qu'il etait en pause (voir l'affectation du _persistentStopNeeded
    //juste en dessous apres la boucle d'execution)
    if (_persistentStopNeeded) {
        AskForStopExecution();
        StopExecution();
        return;
    }
    double t=_clockProvider!=0?_clockProvider->queryExecuteTime(&stopNeeded):SwTime_ToolBox::GetTime();
    ISwExecutable_Service * service = 0;
    for (int i=0;i<_exe_servs.count();i++) 
    {  
         
        service = _exe_servs[i];
        if (service->isActive())
        {
            if (!service->isRunning())
            {
                service->Initialize(t,this); 
                service->setRunning(true);
                service->Start(t); 
            }  
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
void _SwSwitchExecution_Service::StopAll(){
    double t=_clockProvider!=0?_clockProvider->queryStopTime():SwTime_ToolBox::GetTime();
    ISwExecutable_Service * service = 0;
    for (int i=0;i<_exe_servs.count();i++) 
    {  
        service = _exe_servs[i];
        if (service->isRunning())
        {
            service->Stop(t);
            service->setRunning(false);
        }
    }
    _exe_servs.clear();
    _clockProvider=0;
}
//---------------------------------------------------------------------
// Algorithmes d'execution 
//---------------------------------------------------------------------
/*! \brief Run periodique */
void _SwSwitchExecution_Service::RunPeriodic() {
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
void _SwSwitchExecution_Service::RunPeriodicWithOverload() {
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
void _SwSwitchExecution_Service::Prepare(int time_step,bool overload,_SwSwitchExecutionType exe_type) {
    if (!_is_stopped)
        return;
    _time_step=time_step;
    _overload=overload;
    _exe_type=exe_type;
}
/*! \brief Demarrage de l'execution */
void _SwSwitchExecution_Service::StartExecution() {
    if (_exe_type==SlaveType) {
        QMessageBox::warning(NULL,"Fail to start","Unable to start executor in slave mode");
        return;
    }
    InitializeAll();
    StartAll();
    _is_stopped=false;
    _must_be_stopped=false;
    switch (_exe_type) {
        case TimerType:
            _exe_timer=new QTimer();
            _exe_timer->setInterval(_time_step);
            _exe_timer->connect(_exe_timer,SIGNAL(timeout()),this,SLOT(ExecuteAll()));
            _exe_timer->start();
            break;
        case ThreadType:
            start();
            break;
        case ExternalType:
            break;
        default:
            break;
    }
}
/*! \brief execution */
void _SwSwitchExecution_Service::Execution() {
    if (_overload) {
        RunPeriodicWithOverload();        
    } else {
        RunPeriodic();
    }
}
/*! \brief demande d'arret de l'execution*/
void _SwSwitchExecution_Service::AskForStopExecution() {
    _must_be_stopped=true;
    switch (_exe_type) {
        case TimerType:
            _exe_timer->stop();
            delete _exe_timer;
            _exe_timer=NULL;
            _is_stopped=true;
            break;
        case ThreadType:
            break;
        case ExternalType:
            break;
        default:
            break;
    }
}
/*! \brief demande d'arret de l'execution*/
bool _SwSwitchExecution_Service::IsExecutionStopped() {
    return _is_stopped;
}
/*! \brief Arret de l'execution */
void _SwSwitchExecution_Service::StopExecution() {
    switch (_exe_type) {
        case TimerType:
            break;
        case ThreadType:
            wait();
            _is_stopped=true;
            break;
        case ExternalType:
            _is_stopped=true;
            break;
        default:
            break;
    }
    StopAll();
}
/*! \brief definition du fournisseur de temps */
void _SwSwitchExecution_Service::SetClockProvider(ISwClockProvider * provider) {
    _clockProvider=provider;
}
//---------------------------------------------------------------------
// QThread
//---------------------------------------------------------------------
/*! \brief run du thread */
void _SwSwitchExecution_Service::run() {
    Execution();
}

