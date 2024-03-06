/*!
 \file _SwGuiCompActionProvider.cpp
 \brief Implementation of the Class _SwGuiCompActionProvider permettant de générer des QAction
 \version 1.0
 \date 01/09/2013 2:00
 \author AAY
*/

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiCompActionProvider.h"
#include <QMessageBox>
#include "SwQActionAdapter.h"

using namespace StreamWork::SwCore;

#define CL_ACTION_INTERFACE_NAME "Action_%1"

//-----------------------------------------------------------------------
_SwGuiCompActionProvider::_SwGuiCompActionProvider()
	: SwComponent_Class()
	, _provider_service(nullptr)
	, _properties_service(nullptr)
	, _actions_nb(0)
{
}

//-----------------------------------------------------------------------
_SwGuiCompActionProvider::~_SwGuiCompActionProvider()
{
    //Desenregistrement des services
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _provider_service;
    delete _properties_service;

}

//-----------------------------------------------------------------------
void _SwGuiCompActionProvider::InitializeResources() throw(SwException) 
{
    
	//Creation des service
	_provider_service	= new SwInterfaces_Provider_Class(this) ;
	_properties_service	= new SwProperties_Class(this);

	//Enregistrement des services
	this->RegisterService(_properties_service);
	this->RegisterService(_provider_service);

	//Gestion des actions
	_actions_nb_property=_properties_service->CreateProperty<uint>("nb_actions");
	_actions_nb_property->SetDescription("Define how many QAction interfaces this component provide");  
	_actions_nb_property->SetValue(QVariant(_actions_nb));
	_actions_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompActionProvider::OnPropertyChange);

}

//-----------------------------------------------------------------------
void _SwGuiCompActionProvider::OnPropertyChange(ISwProperty * property) 
{
    uint val;
    QString interface_name;

    if (_actions_nb_property==property) 
	{
        val=property->GetValue().toUInt();
        if (val==_actions_nb) return;
        if (val<_actions_nb) 
		{
            for (uint i=val;i<_actions_nb;i++) 
			{
                interface_name=QString(CL_ACTION_INTERFACE_NAME).arg(i);
                _provider_service->UnregisterProvidedInterface(interface_name);
				if(_actions.contains(interface_name))
				{
					_properties_service->DestroyProperty(interface_name+".actionName");
					_properties_service->DestroyProperty(interface_name+".actionIcon");
					//todo remove proper && delete action
					SwQActionAdapter *tmp = _actions.take(interface_name);
					delete tmp;
				}

            }
        } 
		else 
		{
            for (uint i=_actions_nb;i<val;i++) 
			{
                interface_name=QString(CL_ACTION_INTERFACE_NAME).arg(i);
				SwQActionAdapter *tmp = new SwQActionAdapter();
				_properties_service->CreatePropertiesForQObject(tmp,interface_name,true);
				tmp->setActionName(interface_name);
                _actions.insert(interface_name,tmp);
                _provider_service->RegisterProvidedInterface<QAction>(interface_name,&(tmp->GetAction()));
            }
        }
        _actions_nb=val;
    }
}


