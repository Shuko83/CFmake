#pragma once

#include <functional>

#include "SwMacros.h"
#include "ISwServicesManager_Listener.h"
#include "SwApplication.h"

using namespace StreamWork::SwCore;

template <typename SERVICE_TYPE, typename YOUR_CLASS>
class SwServiceManager_Helper : public StreamWork::SwCore::ISwServicesManager_Listener
{
public:
	SwServiceManager_Helper();
	~SwServiceManager_Helper();

	SERVICE_TYPE * getService();
	void setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)());						// ATTENTION: ne peut être appelée qu'une seule fois
	void setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(QString serviceName));   // ATTENTION: ne peut être appelée qu'une seule fois
	void setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(QString serviceName,bool isEnable));   // ATTENTION: ne peut être appelée qu'une seule fois

private:
	QString _serviceName;
	SERVICE_TYPE * _service;
	std::function<void(QString serviceName, bool isEnabled)> _callback;

	void registerService(QString serviceName);
	void notify(StreamWork::SwCore::ISwService * service, bool isEnabled);

	virtual void OnRegisterService(StreamWork::SwCore::ISwService * service);
	virtual void OnUnregisterService(StreamWork::SwCore::ISwService * service);
};

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::SwServiceManager_Helper()
{

}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::~SwServiceManager_Helper()
{
	SW_APP->RemoveServicesManagerObserver(this);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
SERVICE_TYPE * SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::getService()
{
	return _service;
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::notify(StreamWork::SwCore::ISwService * service,bool isEnabled)
{
	_service = dynamic_cast<SERVICE_TYPE *> (service);
	_callback(_serviceName,isEnabled);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)())
{
	_callback = ([=](QString serviceName2, bool isEnabled)->void { (thisPointer->*callback)(); });
	registerService(serviceName);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(QString ))
{
	_callback = ([=](QString serviceName2, bool isEnabled)->void { (thisPointer->*callback)(serviceName2); });
	registerService(serviceName);
}


//-----------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(QString , bool ))
{
	_callback = ([=](QString serviceName2, bool isEnabled)->void { (thisPointer->*callback)(serviceName2,isEnabled); });
	registerService(serviceName);
}


//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::registerService(QString serviceName)
{
	_serviceName = serviceName;
	SW_APP->AddServicesManagerObserver(this);
	StreamWork::SwCore::ISwService * service = SW_APP->QueryService(serviceName);
	_service = dynamic_cast<SERVICE_TYPE *> (service);

	if (_service)
		notify(service,true);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::OnUnregisterService(StreamWork::SwCore::ISwService * service)
{
	if (service->GetServiceName() == _serviceName)
		notify(service,false);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE, typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE, YOUR_CLASS>::OnRegisterService(StreamWork::SwCore::ISwService * service)
{
	if (service->GetServiceName() == _serviceName)
		notify(service,true);
}

