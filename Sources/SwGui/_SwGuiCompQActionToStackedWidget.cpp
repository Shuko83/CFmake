/*!
 \file _SwGuiCompQActionToStackedWidget.cpp
 \brief Implementation of the Class _SwGuiCompQActionToStackedWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompQActionToStackedWidget.h"
#include "_SwActionStackedWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_ACTION_INTERFACE_NAME "Action_%1"



/*****************************************************************************/
_SwGuiCompQActionToStackedWidget::_SwGuiCompQActionToStackedWidget()
	: SwComponent_Class()
	, _provider_service(nullptr)
	, _consumer_service(nullptr)
	, _properties_service(nullptr)
	, _tmp_handle_StackedWidget(nullptr)
	, _tmp_handle_action(nullptr)
{
}

/*****************************************************************************/
_SwGuiCompQActionToStackedWidget::~_SwGuiCompQActionToStackedWidget()
{
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());

    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
}

/*****************************************************************************/
void _SwGuiCompQActionToStackedWidget::InitializeResources() throw(SwException) 
{
    //Creation des service
    _consumer_service		= new SwInterfaces_Consumer_Class(this) ;
    _provider_service		= new SwInterfaces_Provider_Class(this) ;
    _properties_service		= new SwProperties_Class(this);

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

	_consumer_service->RegisterConsumedInterface<ISwStackedWidget_Controler>("StackedWidget_Controler",&_tmp_handle_StackedWidget);

    if (SW_APP->IsVerbose()) 
		SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwFrameWidget done\n"));

}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------

/*****************************************************************************/
void _SwGuiCompQActionToStackedWidget::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{
	if (_tmp_handle_StackedWidget!=NULL) 
	{
		_tmp_handle_StackedWidget->unRegisterStackedWidgetObserver(this);
		int t_size = _actions.size();
		for(int j = 0; j < t_size; j ++)
		{
			QString interface_name =QString(CL_ACTION_INTERFACE_NAME).arg(j);
			_provider_service->UnregisterProvidedInterface(interface_name);
			_actions.remove(interface_name);
		}
	}



}

/*****************************************************************************/
void _SwGuiCompQActionToStackedWidget::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{
	if (_tmp_handle_StackedWidget) 
	{
		_tmp_handle_StackedWidget->registerStackedWidgetObserver(this);
		notify();
	}
}

/*****************************************************************************/
void _SwGuiCompQActionToStackedWidget::notify()
{
	if(!_tmp_handle_StackedWidget)
	{
		int t_size = _actions.size();

		for(int j = 0; j < t_size; j ++)
		{
			QString interface_name =QString(CL_ACTION_INTERFACE_NAME).arg(j);
			_provider_service->UnregisterProvidedInterface(interface_name);
			_actions.remove(interface_name);
		}
	}
	else
	{
		QMap<QString,QAction *>::iterator action_it;
		int t_size = _actions.size();

		for(int j = _tmp_handle_StackedWidget->getNbPage(); j < t_size; j ++)
		{
			QString interface_name =QString(CL_ACTION_INTERFACE_NAME).arg(j);
			_provider_service->UnregisterProvidedInterface(interface_name);
			_actions.remove(interface_name);
		}

		for(int i = 0; i < _tmp_handle_StackedWidget->getNbPage(); i++)
		{
			QString interface_name = QString(CL_ACTION_INTERFACE_NAME).arg(i);

			action_it=_actions.find(interface_name);
			if (action_it==_actions.end()) 
			{
				_SwActionStackedWidget *handle_action = new _SwActionStackedWidget(_tmp_handle_StackedWidget->getWidgetName(i).replace(" ","_"),i);
				handle_action->GetAction().setIcon(_tmp_handle_StackedWidget->getWidgetIcon(i));

				QObject::connect(handle_action, &_SwActionStackedWidget::callback, this, &_SwGuiCompQActionToStackedWidget::switchStackedWidget);
				_provider_service->RegisterProvidedInterface<QAction>(interface_name,&handle_action->GetAction());
				_actions.insert(interface_name,&handle_action->GetAction());
			}
			else
			{
				_actions[interface_name]->setText(_tmp_handle_StackedWidget->getWidgetName(i).replace(" ","_"));
				_actions[interface_name]->setIcon(_tmp_handle_StackedWidget->getWidgetIcon(i));
			}

		}
		
	}

}

/*****************************************************************************/
void _SwGuiCompQActionToStackedWidget::switchStackedWidget( int index )
{
	_tmp_handle_StackedWidget->setCurrentIndex(index);
}

