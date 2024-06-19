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
#include <QElapsedTimer>
#include <QFile>

#define CL_DEFAULT_TIME_STEP 50
#define CL_EXE_TIMER 0
#define CL_EXE_THREAD 1
#define CL_EXE_MAINENTRY 2
#define CL_EXE_FSLAVE 3
#define CL_EXE_PSLAVE 4
#define CL_EXE_TIMER_MAINENTRY 5

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;


//-----------------------------------------------------------------------
_SwExecutor::_SwExecutor() : SwComponent_Class(), _exe_service(this)
{

	_defaultActivated_property = 0;
	_executor = 0;
	_time_step = CL_DEFAULT_TIME_STEP;
	_overload.AddKey(0, "No");
	_overload.AddKey(1, "Yes");
	_overload.FromInt(0);
	_executable_entry.AddKey(CL_EXE_TIMER, "Timer");
	_executable_entry.AddKey(CL_EXE_THREAD, "Thread");
	_executable_entry.AddKey(CL_EXE_MAINENTRY, "MainEntry");
	_executable_entry.AddKey(CL_EXE_FSLAVE, "Full Slave");
	_executable_entry.AddKey(CL_EXE_PSLAVE, "Partial Slave");
	_executable_entry.AddKey(CL_EXE_TIMER_MAINENTRY, "Timer/MainEntry");
	_executable_entry.FromInt(0);
	_priority.AddKey(QThread::IdlePriority, "IdlePriority");
	_priority.AddKey(QThread::LowestPriority, "LowestPriority");
	_priority.AddKey(QThread::LowPriority, "LowPriority");
	_priority.AddKey(QThread::NormalPriority, "NormalPriority");
	_priority.AddKey(QThread::HighPriority, "HighPriority");
	_priority.AddKey(QThread::HighestPriority, "HighestPriority");
	_priority.AddKey(QThread::TimeCriticalPriority, "TimeCriticalPriority");
	_priority.AddKey(QThread::InheritPriority, "InheritPriority");
	_priority.FromInt((int) _exe_service.priority());
	_defaultActivated = true;
	_replayMode = false;
}

//-----------------------------------------------------------------------
_SwExecutor::~_SwExecutor()
{
	switch ( _executable_entry.ToInt() )
	{
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


//-----------------------------------------------------------------------
void _SwExecutor::InitializeResources() throw(SwException)
{
	//Creation des service
	_properties_service = new SwProperties_Class(this);
	//Enregistrement des services
	this->RegisterService(_properties_service);
	this->RegisterService(&_exe_service);
	this->RegisterService(this);

	//Gestion du pas temporel
	_time_step_property = _properties_service->CreateProperty<uint>("Execution.Time Step (in ms)");
	if ( _time_step_property == NULL )
	{
		if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register Execution.TimeStep property\n"));
	}
	else
	{
		_time_step_property->SetDescription("Define time step in ms");
		_time_step_property->SetValue(QVariant(_time_step));
		_time_step_property->GetOnChangeSignal().iconnect(*this, &_SwExecutor::OnPropertyChange);
	}

	//Overload
	_overload_property = _properties_service->CreateProperty<SwEnum>("Execution.Overload");
	if ( _overload_property == NULL )
	{
		if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register Execution.Overload property\n"));
	}
	else
	{
		_overload_property->SetDescription("Enable overload");
		QVariant _overload_variant;
		_overload_variant.setValue(_overload);
		_overload_property->SetValue(_overload_variant);
		_overload_property->GetOnChangeSignal().iconnect(*this, &_SwExecutor::OnPropertyChange);
	}	

	//Thread priority
	_priority_property = _properties_service->CreateProperty<SwEnum>("Execution.Priority");
	if ( _priority_property == NULL )
	{
		if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register Execution.Priority property\n"));
	}
	else
	{
		_priority_property->SetDescription("Define thread priority (cf Doc QThread)");
		QVariant _priority_variant;
		_priority_variant.setValue(_priority);
		_priority_property->SetValue(_priority_variant);
		_priority_property->GetOnChangeSignal().iconnect(*this, &_SwExecutor::OnPropertyChange);
	}

	//Est l'entree de l'executable
	_executable_entry_property = _properties_service->CreateProperty<SwEnum>("Execution.Execution Type");
	if ( _executable_entry_property == NULL )
	{
		if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register Execution.Execution Type property\n"));
	}
	else
	{
		_executable_entry_property->SetDescription("Main Thread on timer (QTimer), Threaded, Main entry (ISwExecutor),\nFull Slave (time inherit from parent executor),Partial Slave (time is system time)");
		QVariant _executable_entry_variant;
		_executable_entry_variant.setValue(_executable_entry);
		_executable_entry_property->SetValue(_executable_entry_variant);
		_executable_entry_property->GetOnChangeSignal().iconnect(*this, &_SwExecutor::OnPropertyChange);
	}

	//Gestion de l activation par defaut 
	_defaultActivated_property = _properties_service->CreateProperty<bool>("Activation.Default");
	if ( _defaultActivated_property == NULL )
	{
		if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register Activation.Default property\n"));
	}
	else
	{
		_defaultActivated_property->SetDescription("Component is Activated/Disactivated by default");
		_defaultActivated_property->SetValue(QVariant(_defaultActivated));
		_defaultActivated_property->GetOnChangeSignal().iconnect(*this, &_SwExecutor::OnPropertyChange);
	}

	//Configuration par defaut
	_exe_service.Prepare(_time_step, _overload.ToInt() != 0, Timer_exe);

	if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Info, QString("InitializeResources of SwExecutor done\n"));

}

//-----------------------------------------------------------------------
void _SwExecutor::OnPropertyChange(ISwProperty * property)
{
	if ( property == _priority_property )
	{
		SwEnum new_priority = _priority_property->GetValue().value<SwEnum>();
		_exe_service.setPriority((QThread::Priority)new_priority.ToInt());
		return;
	}
	if ( property == _time_step_property )
	{
		_time_step = _time_step_property->GetValue().toUInt();
	}
	if ( property == _overload_property )
	{
		_overload = _overload_property->GetValue().value<SwEnum>();
	}
	if ( property == _executable_entry_property )
	{
		SwEnum new_executable_entry = _executable_entry_property->GetValue().value<SwEnum>();
		if ( new_executable_entry.ToInt() != _executable_entry.ToInt() && _executable_entry.ToInt() == CL_EXE_MAINENTRY )
		{
			SW_APP->RegisterExecutor(NULL);
		}
		_executable_entry = new_executable_entry;
		switch ( _executable_entry.ToInt() )
		{
			case CL_EXE_TIMER:
				if ( !_time_step_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_time_step_property->GetName(), true);
				if ( _priority_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_priority_property->GetName(), false);
				break;
			case CL_EXE_THREAD:
				if ( !_time_step_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_time_step_property->GetName(), true);
				if ( !_overload_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_overload_property->GetName(), true);
				if ( !_priority_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_priority_property->GetName(), true);
				break;
			case CL_EXE_MAINENTRY:
				if ( !_time_step_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_time_step_property->GetName(), true);
				if ( !_overload_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_overload_property->GetName(), true);
				if ( _priority_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_priority_property->GetName(), false);
				SW_APP->RegisterExecutor(this);
				break;
			case CL_EXE_FSLAVE:
			case CL_EXE_PSLAVE:
				if ( _time_step_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_time_step_property->GetName(), false);
				if ( _overload_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_overload_property->GetName(), false);
				if ( _priority_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_priority_property->GetName(), false);
				break;
			case CL_EXE_TIMER_MAINENTRY:
				if ( !_time_step_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_time_step_property->GetName(), true);
				if ( _priority_property->IsEditable() )
					_properties_service->ChangePropertyEdition(_priority_property->GetName(), false);
				SW_APP->RegisterExecutor(this);
				break;
			default:
				break;
		}
	}
	switch ( _executable_entry.ToInt() )
	{
		case CL_EXE_TIMER:
		case CL_EXE_TIMER_MAINENTRY:
			_exe_service.Prepare(_time_step, _overload.ToInt() != 0, Timer_exe);
			break;
		case CL_EXE_THREAD:
			_exe_service.Prepare(_time_step, _overload.ToInt() != 0, Thread_exe);
			break;
		case CL_EXE_MAINENTRY:
			_exe_service.Prepare(_time_step, _overload.ToInt() != 0, External_exe);
			break;
		case CL_EXE_FSLAVE:
		case CL_EXE_PSLAVE:
			_exe_service.Prepare(_time_step, _overload.ToInt() != 0, Slave_exe);
			break;
		default:
			break;
	}
	// gestion de l'activation par defaut
	if ( property == _defaultActivated_property )
	{
		_defaultActivated = _defaultActivated_property->GetValue().value<bool>();
		setActive(_defaultActivated);
	}
}
//----------------------------------------------------
// Interface ISwExecutor
//----------------------------------------------------

//-----------------------------------------------------------------------
int _SwExecutor::StreamExecute()
{
	_exe_service.StartExecution();
	if ( _executable_entry.ToInt() == CL_EXE_TIMER_MAINENTRY )
	{
		//QApplication * gui_app=dynamic_cast<QApplication *>(qApp);
	}
	else
	{
		_exe_service.Execution();
		_exe_service.StopExecution();
	}
	return 0;
}

//-----------------------------------------------------------------------
int _SwExecutor::StreamStop()
{
	onQuit();
	return 0;
}

//-----------------------------------------------------------------------
void _SwExecutor::Initialize(double start_time, ISwExecution_Service * executor) throw (SwException)
{
	if ( _executable_entry.ToInt() < CL_EXE_FSLAVE && !_replayMode )
		return;

	_executor = executor;

	// parcours de tous les executables (independants de l'activation)
	for (ISwExecutable_Service* executable : _exe_service.GetExecutablesList())
	{
		// on vérifie si l'activation de l'executable a changé
		if ( executable->isActive() )
		{
			// on initialise le composant
			if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
				executable->Initialize(start_time, executor);
			else
				executable->Initialize(SwTime_ToolBox::GetTime(), executor);

			executable->setRunning(true);
		}
	}

}

//-----------------------------------------------------------------------
void _SwExecutor::Start(double current_time) throw (SwException)
{
	// parcours de tous les executables (independants de l'activation)
	for (ISwExecutable_Service* executable : _exe_service.GetExecutablesList())
	{
		// on vérifie l'activation de l'executable
		if ( executable->isActive() )
		{
			if ( !executable->isRunning() )
			{
				// on initialise le composant
				if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
					executable->Initialize(current_time, _executor);
				else
					executable->Initialize(SwTime_ToolBox::GetTime(), _executor);
				executable->setRunning(true);
			}

			// on start le composant
			if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
				executable->Start(current_time);
			else
				executable->Start(SwTime_ToolBox::GetTime());
		}
		else
		{
			if ( executable->isRunning() )
			{
				// on arrete le composant
				if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
					executable->Stop(current_time);
				else
					executable->Stop(SwTime_ToolBox::GetTime());
				executable->setRunning(false);
			}
		}
	}
}

//-----------------------------------------------------------------------
void _SwExecutor::Execute(double current_time, bool is_first_call) throw (SwException)
{
	bool internalFirstCall = is_first_call;
	// parcours de tous les executables (independants de l'activation)
	for (ISwExecutable_Service* executable : _exe_service.GetExecutablesList())
	{
		// on vérifie l'activation de l'executable
		if ( executable->isActive() )
		{
			if ( !executable->isRunning() )
			{
				// on initialise et start le composant
				if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
				{
					executable->Initialize(current_time, _executor);
					executable->setRunning(true);
					executable->Start(current_time);
				}
				else
				{
					executable->Initialize(SwTime_ToolBox::GetTime(), _executor);
					executable->setRunning(true);
					executable->Start(SwTime_ToolBox::GetTime());
				}
				internalFirstCall = true;
			}

			// on execute le composant
			if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
			{
				if (_logTime)
				{
					QElapsedTimer timer;
					timer.start();
					executable->Execute(current_time, internalFirstCall);
					if ( executable->GetHostComponent() )
					{
						QFile debugFile("log.csv");
						debugFile.open(QIODevice::Append);
						debugFile.write(QString(executable->GetHostComponent()->GetFactoryComponentName() + ";;" + QString::number(timer.elapsed()) + "\n").toLatin1());
						debugFile.close();
					}
				}
				else
					executable->Execute(current_time, internalFirstCall);
			}
			else
			{
				if (_logTime)
				{
					QElapsedTimer timer;
					timer.start();
					executable->Execute(SwTime_ToolBox::GetTime(), internalFirstCall);
					if ( executable->GetHostComponent() )
					{
						QFile debugFile("log.csv");
						debugFile.open(QIODevice::Append);
						debugFile.write(QString(executable->GetHostComponent()->GetFactoryComponentName() + ";;" + QString::number(timer.elapsed()) + "\n").toLatin1());
						debugFile.close();
					}
				}
				else
					executable->Execute(SwTime_ToolBox::GetTime(), internalFirstCall);

			}
		}
		else
		{
			if ( executable->isRunning() )
			{
				// on arrete le composant
				if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
					executable->Stop(current_time);
				else
					executable->Stop(SwTime_ToolBox::GetTime());
				executable->setRunning(false);
			}
		}
	}
}

//-----------------------------------------------------------------------
void _SwExecutor::Stop(double current_time)
{
	for (ISwExecutable_Service* executable : _exe_service.GetExecutablesList())
	{
		if ( executable->isRunning() )
		{
			if ( _executable_entry.ToInt() == CL_EXE_FSLAVE || _replayMode )
				executable->Stop(current_time);
			else
				executable->Stop(SwTime_ToolBox::GetTime());
			executable->setRunning(false);
		}
	}
	//_active_exe_list->clear();
	//_active_exe_list= 0
	_executor = 0;
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
void _SwExecutor::Liberate()
{

}

//-----------------------------------------------------------------------
SwComponent_Class * _SwExecutor::GetHostComponent()
{
	return this;
}

//-----------------------------------------------------------------------
void _SwExecutor::AdminSetup()
{
	_exe_service.Edit();
}

//-----------------------------------------------------------------------
void _SwExecutor::onQuit()
{
	_exe_service.AskForStopExecution();
	while ( !_exe_service.IsExecutionStopped() )
	{
		SwTime_ToolBox::InternalSleep(10);
	}
	_exe_service.StopExecution();
}

//-----------------------------------------------------------------------
void _SwExecutor::setReplayMode(bool replayMode)
{
	_replayMode = replayMode;
}

//-----------------------------------------------------------------------
bool _SwExecutor::getReplayMode()
{
	return _replayMode;
}

