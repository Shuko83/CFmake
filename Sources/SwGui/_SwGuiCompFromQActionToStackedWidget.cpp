/*!
 \file _SwGuiCompFromQActionToStackedWidget.cpp
 \brief Implementation of the Class _SwGuiCompFromQActionToStackedWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiCompFromQActionToStackedWidget.h"
#include "_SwActionStackedWidget.h"
#include <QDebug>
using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_FROM_ACTION_INTERFACE_NAME "Action_%1"
#define CL_FROM_ACTION_INTERFACE_NAME_EMPTY_NUMBER "Action_"



/*****************************************************************************/
_SwGuiCompFromQActionToStackedWidget::_SwGuiCompFromQActionToStackedWidget()
	: SwComponent_Class()
	, _provider_service(nullptr)
	, _consumer_service(nullptr)
	, _properties_service(nullptr)
	, _tmp_handle_StackedWidget(nullptr)
	, _tmp_handle_action(nullptr)
	, _typeOfConnection(TOGGLED)
{
}

/*****************************************************************************/
_SwGuiCompFromQActionToStackedWidget::~_SwGuiCompFromQActionToStackedWidget()
{
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());

    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

	_actions.clear();

}

/*****************************************************************************/
void _SwGuiCompFromQActionToStackedWidget::InitializeResources() throw(SwException) 
{
    //Creation des service
    _consumer_service		= new SwInterfaces_Consumer_Class(this) ;
    _provider_service		= new SwInterfaces_Provider_Class(this) ;
    _properties_service		= new SwProperties_Class(this);

	_actions.clear();

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

	//Publication des Q_Properties
	_properties_service->CreatePropertiesForQObject(this,"",true);
	
	_consumer_service->RegisterConsumedInterface<ISwStackedWidget_Controler>("StackedWidget_Controler",&_tmp_handle_StackedWidget);

    if (SW_APP->IsVerbose()) 
		SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiCompFromQActionToStackedWidget done\n"));

}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------

/*****************************************************************************/
void _SwGuiCompFromQActionToStackedWidget::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{
	if (_tmp_handle_StackedWidget!=NULL && interface_name =="StackedWidget_Controler" ) 
	{
		_tmp_handle_StackedWidget->unRegisterStackedWidgetObserver(this);

		//On va plus avoir l'interface d'entrée donc on supprimer les actions 
		int t_size = _actions.size();
		for(int j = 0; j < t_size; j ++)
		{
			QString interfacename =QString(CL_FROM_ACTION_INTERFACE_NAME).arg(j);
			_consumer_service->UnregisterConsumedInterface(interfacename);
			_actions.remove(interfacename);
		}
	}

	if (_tmp_handle_action!=NULL)
	{
		// on parcours la map pour savoir si l'interface_name est contenu dans la clé
		// et si c'est le cas on disconnect l'action
		QMap<QString,QAction*>::iterator it;

		it = _actions.find(interface_name);

		if(it != _actions.end())
		{
			if(it.value() && _tmp_handle_action)
			{
				QAction * tmpAction = (it.value());
				if(_typeOfConnection == TOGGLED)
					disconnect(tmpAction,SIGNAL( triggered (bool) ),this,SLOT(switchStackedWidget(bool)));
				else
					disconnect(tmpAction,SIGNAL( toggled (bool) ),this,SLOT(switchStackedWidget(bool)));
			}
		}
	}
}

/*****************************************************************************/
void _SwGuiCompFromQActionToStackedWidget::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{
	if (_tmp_handle_StackedWidget!=NULL && interface_name =="StackedWidget_Controler") 
	{
		_tmp_handle_StackedWidget->registerStackedWidgetObserver(this);
		notify();
		return;
	}

	if (_tmp_handle_action!=NULL)
	{
		QMap<QString,QAction*>::iterator it;
		
		it = _actions.find(interface_name);

		if(it != _actions.end())
		{
			it.value() = _tmp_handle_action;

			if(it.value())
			{
				QAction * tmpAction = it.value();
				if(_typeOfConnection == TOGGLED)
					connect(tmpAction,SIGNAL( toggled (bool) ),this,SLOT(switchStackedWidget(bool)));
				else
					connect(tmpAction,SIGNAL( triggered (bool) ),this,SLOT(switchStackedWidget(bool)));
			}
		}
	}
}

/*****************************************************************************/
void _SwGuiCompFromQActionToStackedWidget::notify()
{
	QMap<QString,QAction *>::iterator action_it;
	int t_size = _actions.size();

	for(int j = _tmp_handle_StackedWidget->getNbPage(); j < t_size; j ++)
	{
		QString interface_name =QString(CL_FROM_ACTION_INTERFACE_NAME).arg(j);
		_consumer_service->UnregisterConsumedInterface(interface_name);
		_actions.remove(interface_name);
	}


	for(int i = 0; i < _tmp_handle_StackedWidget->getNbPage(); i++)
	{
		QString interface_name = QString(CL_FROM_ACTION_INTERFACE_NAME).arg(i);

		action_it=_actions.find(interface_name);
		if (action_it==_actions.end()) 
		{
			_consumer_service->RegisterConsumedInterface<QAction>(interface_name,&_tmp_handle_action);
			_actions.insert(interface_name,_tmp_handle_action);
		}
	}
}

/*****************************************************************************/
void _SwGuiCompFromQActionToStackedWidget::switchStackedWidget( bool val )
{
	QAction * action= qobject_cast<QAction *> (sender());

	if(action)
	{
		for (auto it = _actions.begin(); it != _actions.end(); it++)
		{
			if(it.value() && it.value() == action)
			{
				QString tmpVal = it.key();
				tmpVal.replace(CL_FROM_ACTION_INTERFACE_NAME_EMPTY_NUMBER,"");
				
				_tmp_handle_StackedWidget->setCurrentIndex(tmpVal.toInt());
			}
		}
	}
}

