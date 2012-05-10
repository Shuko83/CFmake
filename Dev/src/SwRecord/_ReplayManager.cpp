/**
 * @file _ReplayManager.cpp
 * @brief _ReplayManager
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#include <SwApplication.h>
#include <SwMacros.h>
#include "_ReplayManager.h"
#include <SwAddress_ToolBox.h>
#include "SwRecordConstantes.h"
#include "ISwSupportReplay.h"
#include <QMessageBox>
#include "_ReplayWidget.h"
#include "_SwExecutor.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

//-------------------------------------------------------------------------
_ReplayManager::_ReplayManager(_SwServiceRecording * serviceRecord):SwAssistedComponent() 
{
    _serviceRecord=serviceRecord;

    _repository=SwFileDescriptor(SwFileDescriptor::DirectorySelect,QString(),QString());
    _enableReplay=false;
    _executorPath="";
    _executor=0;
	_activator = "";

	//_widget = new _ReplayWidget();
	//addReplayManagerListener(_widget);
	//_widget->show();
}

//-------------------------------------------------------------------------
_ReplayManager::~_ReplayManager() 
{

}

//-------------------------------------------------------------------------
void _ReplayManager::initializeComponent() throw(SwException) 
{

	provideInterface<ISwReplayControler>("ReplayManager",(ISwReplayControler *)this);
	createPropertiesForThisObject(QString(),true);
}

//-------------------------------------------------------------------------
void _ReplayManager::loadReplay(QString directoryName) 
{
    _repository.setFileName(directoryName);
}

//-------------------------------------------------------------------------
QString _ReplayManager::getConfigurationFile()
{
    return _repository.getFileName()+QDir::separator()+CG_RECORD_CONFIGURATION_FILE;
}

//-------------------------------------------------------------------------
void _ReplayManager::startReplay(ISwExecution_Service * exeService)
{
    //Chargement du fichier d'enregistrement
    QDir d=QDir(_repository.getFileName());
    if (!d.exists())
        return;

	//Enregistrement du replay manager au pres des record points 
	QList<ISwRecordPoint *> points=_serviceRecord->getRecordPoints();
	foreach(ISwRecordPoint * rpoint,points) 
	{
		ISwReplayListener * rListener = dynamic_cast<ISwReplayListener*>(rpoint);
		addReplayManagerListener(rListener);
	}

	//Active the activatoir
	SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_activator,this);
	if(component)
	{
		component->setActive(true);
	}

    //Creation du fichier d'enregistrement
    QFile *fileReader=new QFile(d.absolutePath()+QDir::separator()+CG_RECORD_FILE);
    if (!fileReader->open(QIODevice::ReadOnly | QIODevice::Text))
	{
        delete fileReader;
        fileReader=0;
        _enableReplay=false;
		QMessageBox::critical(NULL,"Replay Error","Unable to load the record file.");
        return;
    }
    QXmlStreamReader * reader=new QXmlStreamReader(fileReader);
    _timeLine.reset();
    if(!_timeLine.loadRecordFile(reader,_serviceRecord,_repository.getFileName()))
		QMessageBox::critical(NULL,"Replay Error","Unable to load the record file (File corrupt).");
    delete reader;
    fileReader->close();
    delete fileReader;
    fileReader=0;
   //Definition de la clock a l'enregistrement
    _executor=exeService;
    _executor->SetClockProvider(&_timeLine); 
    ISwSupportReplay * replaySupport=dynamic_cast<ISwSupportReplay *>(_executor->GetHostComponent());
    if (replaySupport!=0) 
	{
        replaySupport->setReplayMode(true);
    }
    //Execution
    _executor->StartExecution();
}

//-------------------------------------------------------------------------
void _ReplayManager::startReplay()
{
	ISwExecution_Service * executor=0;

	if(_timeLine.getState() == STATE_PAUSED)
	{
		_timeLine.setPauseState(false);
		return;
	}

	if (_timeLine.getState() != STATE_STOPPED)
		return;

	
	//Recuperation de l'executor
	SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_executorPath,this);
	if (component!=0) 
	{
		executor=dynamic_cast<ISwExecution_Service *>(component->QueryService(CG_SW_SERVICE_EXECUTION));
	}
	if (executor==0)
		return;

	startReplay(executor);
}

//-------------------------------------------------------------------------
void _ReplayManager::stopReplay()
{
    if (_executor!=0) 
	{
        if (!_executor->IsExecutionStopped()) 
		{
            _executor->AskForStopExecution();
            _executor->StopExecution();
            _timeLine.queryStopTime();
        }
		else
		{
			//Done to notify listener
			_timeLine.queryStopTime();
		}

		SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_activator,this);
		if(component)
		{
			component->setActive(false);
		}

        _executor->SetClockProvider(0);

        ISwSupportReplay * replaySupport=dynamic_cast<ISwSupportReplay *>(_executor->GetHostComponent());
        if (replaySupport!=0) 
		{
            replaySupport->setReplayMode(false);
        }
    }
}

//-------------------------------------------------------------------------
void _ReplayManager::addReplayManagerListener(ISwReplayListener * listener) 
{
	if (listener!=0)
		_listeners.push_back(listener);
	_timeLine.addReplayManagerListener(listener);
}

//-------------------------------------------------------------------------
void _ReplayManager::removeReplayManagerListener(ISwReplayListener * listener)
{
	if (listener!=0)
		_listeners.removeOne(listener);

    _timeLine.removeReplayManagerListener(listener);
}

//-------------------------------------------------------------------------
SwFileDescriptor _ReplayManager::getRecordDirectory() const 
{

    return _repository;
}

//-------------------------------------------------------------------------
void _ReplayManager::loadReplay(const SwFileDescriptor & val)
{
    _repository=val;
}

//-------------------------------------------------------------------------
bool _ReplayManager::getEnableReplay() const
{
    return _enableReplay;
}

//-------------------------------------------------------------------------
void _ReplayManager::setEnableReplay(bool val)
{
    ISwExecution_Service * executor=0;
    if (_enableReplay==val)
        return;
    if (val) {
        //Recuperation de l'executor
        SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_executorPath,this);
        if (component!=0) 
		{
            executor=dynamic_cast<ISwExecution_Service *>(component->QueryService(CG_SW_SERVICE_EXECUTION));
        }
        if (executor==0)
            return;
    }
    _enableReplay=val;
    if (_enableReplay) 
	{
        startReplay(executor);
    } 
	else 
	{
        stopReplay();
    }
}

//-------------------------------------------------------------------------
QString _ReplayManager::getExecutor() const
{
    return _executorPath;
}

//-------------------------------------------------------------------------
void _ReplayManager::setExecutor(QString val)
{
    _executorPath=val;

	SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_executorPath,this);
	if (component!=0) 
	{
		ISwProperties * propServ =dynamic_cast<ISwProperties*>(component->QueryService(CG_SW_SERVICE_PROPERTIES));
		if(propServ)
		{
			ISwProperty * prop = propServ->GetProperty("Execution.Time Step (in ms)");
			if(prop)
			{
				_timerStep = prop->GetValue().toUInt();
			}
		}
	}
}


//-------------------------------------------------------------------------
void _ReplayManager::setTimerStep( int val )
{
	SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_executorPath,this);
	if (component!=0) 
	{
		ISwProperties * propServ =dynamic_cast<ISwProperties*>(component->QueryService(CG_SW_SERVICE_PROPERTIES));
		if(propServ)
		{
			ISwProperty * prop = propServ->GetProperty("Execution.Time Step (in ms)");
			if(prop)
			{
				_timerStep = (uint)val;
				prop->SetValue((uint)_timerStep);
			}
		}
	}
}


//-------------------------------------------------------------------------
void _ReplayManager::setActivator( QString val )
{
	_activator = val; 
}

//-------------------------------------------------------------------------
void _ReplayManager::pauseReplay()
{
	_timeLine.setPauseState(true);
}

//-------------------------------------------------------------------------
void _ReplayManager::setSpeed( int val )
{
	setTimerStep(val);
}

//-----------------------------------------------------------------------
void _ReplayManager::setJumpTime( int val )
{
	_timeLine.setJumpTime(val);
}

//-----------------------------------------------------------------------
int _ReplayManager::getJumpTime() const
{
	return _timeLine.getJumpTime();
}
