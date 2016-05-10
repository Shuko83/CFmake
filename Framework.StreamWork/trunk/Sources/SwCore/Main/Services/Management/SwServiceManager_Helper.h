#pragma once

#include <functional>

#include "SwMacros.h"
#include "ISwServicesManager_Listener.h"
#include "SwApplication.h"

using namespace StreamWork::SwCore;

template <typename SERVICE_TYPE>
class SwServiceManager_Helper : public StreamWork::SwCore::ISwServicesManager_Listener
{
public:
	SwServiceManager_Helper();
	~SwServiceManager_Helper();

	SERVICE_TYPE * getService();

	template <typename YOUR_CLASS>
	void setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(bool available));	// ATTENTION: ne peut être appelée qu'une seule fois
	
	void setService(QString serviceName);																			// ATTENTION: ne peut être appelée qu'une seule fois

private:
	QString _serviceName;
	SERVICE_TYPE * _service;
	std::function<void(bool)> _callback;

	virtual void OnRegisterService(StreamWork::SwCore::ISwService * service);
	virtual void OnUnregisterService(StreamWork::SwCore::ISwService * service);
};

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE>
SwServiceManager_Helper<SERVICE_TYPE>::SwServiceManager_Helper() : _service(nullptr)
{
	SW_APP->AddServicesManagerObserver(this);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE>
SwServiceManager_Helper<SERVICE_TYPE>::~SwServiceManager_Helper()
{
	SW_APP->RemoveServicesManagerObserver(this);

	if (_service && _callback)
		_callback(false);
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE>
SERVICE_TYPE * SwServiceManager_Helper<SERVICE_TYPE>::getService()
{
	return _service;
}

//-----------------------------------------------------------------------
template <typename SERVICE_TYPE>
template <typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE>::setService(QString serviceName, YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(bool))
{
	_serviceName = serviceName;
	_callback = ([=](bool available)->void { (thisPointer->*callback)(available); });
	_service = dynamic_cast<SERVICE_TYPE *> (SW_APP->QueryService(serviceName));

	if (_service)
		_callback(true);
}

//-----------------------------------------------------------------------
template <typename SERVICE_TYPE>
void SwServiceManager_Helper<SERVICE_TYPE>::setService(QString serviceName)
{
	_serviceName = serviceName;
	_service = dynamic_cast<SERVICE_TYPE *> (SW_APP->QueryService(serviceName));
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE>
void SwServiceManager_Helper<SERVICE_TYPE>::OnUnregisterService(StreamWork::SwCore::ISwService * service)
{
	if (service->GetServiceName() == _serviceName)
	{
		if (_callback)
			_callback(false);

		_service = nullptr;
	}
}

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE>
void SwServiceManager_Helper<SERVICE_TYPE>::OnRegisterService(StreamWork::SwCore::ISwService * service)
{
	if (service->GetServiceName() == _serviceName)
	{
		_service = dynamic_cast<SERVICE_TYPE *>(service);

		if (_callback)
			_callback(true);
	}
}
