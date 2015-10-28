/**
@file Component.cpp
@brief Implementation avance d'un composant par defaut
@author Big
*/

#include "Component.h"
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>

using namespace StreamWork::SwCore;

//-----------------------------------------------------------------------
Component::Component() :SwComponent_Class()
{
	_provider_service = NULL;
	_consumer_service = NULL;
	_properties_service = NULL;
	_disable_service = false;
}

//-----------------------------------------------------------------------
Component::~Component()
{
	_disable_service = true;

	//Desenregistrement des services
	this->UnregisterService(_consumer_service->GetServiceName());
	this->UnregisterService(_provider_service->GetServiceName());
	this->UnregisterService(_pins_service->GetServiceName());
	this->UnregisterService(_properties_service->GetServiceName());

	//Destruction des services
	delete _consumer_service;
	delete _provider_service;
	delete _properties_service;
}

//-----------------------------------------------------------------------
void Component::InitializeResources() throw(SwException)
{

	//Creation des service
	_consumer_service = new SwInterfaces_Consumer_Class(this);
	_provider_service = new SwInterfaces_Provider_Class(this);
	_properties_service = new SwProperties_Class(this);
	_pins_service = new SwPins_Manager_Class(this);

	//Enregistrement des services
	this->RegisterService(_properties_service);
	this->RegisterService(_consumer_service);
	this->RegisterService(_pins_service);
	this->RegisterService(_provider_service);

	_consumer_service->AttachInterfacesConsumerObserver(this);

	initializeComponent();

}

//-----------------------------------------------------------------------
void Component::setActive(bool active)
{
	if ( isActive() == active )
		return;
	SwComponent_Class::setActive(active);
	eventActivationChanged();
}

//-----------------------------------------------------------------------
void Component::OnPropertyChange(ISwProperty * property)
{
	if ( !_disable_service )
		eventPropertyChange(property);
}

//-----------------------------------------------------------------------
void Component::BeforeInterfaceAvailabilityChange(QString interface_name, SwComponent_Class * provider_host)
{
	if ( !_disable_service )
		eventBeforeInterfaceAvailability(interface_name, provider_host);
}

//-----------------------------------------------------------------------
void Component::AfterInterfaceAvailabilityChange(QString interface_name, SwComponent_Class * provider_host)
{
	if ( !_disable_service )
		eventAfterInterfaceAvailability(interface_name, provider_host);
}

//-----------------------------------------------------------------------
void Component::OnReceiveData(SwPin * src, SwData_Class * data)
{
	if ( !_disable_service )
		eventReceiveData(src, data);
}

//-----------------------------------------------------------------------
void Component::initializeComponent() throw(SwException)
{

}

//-----------------------------------------------------------------------
void Component::eventPropertyChange(ISwProperty * property)
{

}

//-----------------------------------------------------------------------
void Component::eventBeforeInterfaceAvailability(QString interface_name, SwComponent_Class * provider_host)
{

}

//-----------------------------------------------------------------------
void Component::eventAfterInterfaceAvailability(QString interface_name, SwComponent_Class * provider_host)
{

}
//-----------------------------------------------------------------------
void Component::eventReceiveData(SwPin * src, SwData_Class * data)
{

}

//-----------------------------------------------------------------------
void Component::eventActivationChanged()
{

}

//-----------------------------------------------------------------------
ISwInterfaces_Provider & Component::getIProviderService()
{
	return *_provider_service;
}

//-----------------------------------------------------------------------
ISwInterfaces_Consumer & Component::getIConsumerService()
{
	return *_consumer_service;
}

//-----------------------------------------------------------------------
ISwProperties & Component::getPropertiesService()
{
	return *_properties_service;
}

//-----------------------------------------------------------------------
ISwPins_Manager & Component::getPinsService()
{
	return *_pins_service;
}

//-----------------------------------------------------------------------
void Component::enableListeningChangeForProperty(ISwProperty * property)
{
	property->GetOnChangeSignal().iconnect(*this, &Component::OnPropertyChange);
}

//-----------------------------------------------------------------------
void Component::enableListeningForPin(SwPin * pin)
{
	pin->RegisterListener(this);
}

