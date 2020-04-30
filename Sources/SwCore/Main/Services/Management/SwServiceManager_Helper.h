#pragma once

#include <functional>

#include "SwMacros.h"
#include "ISwServicesManager_Listener.h"
#include "SwApplication.h"

template <typename SERVICE_TYPE>
class SwServiceManager_Helper : public StreamWork::SwCore::ISwServicesManager_Listener
{
public:
    SwServiceManager_Helper(QString serviceName);
	~SwServiceManager_Helper();

	SERVICE_TYPE * getService() const;

	template <typename YOUR_CLASS>
	void setCallback(YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(bool available));
	
    void setCallback(std::function<void(bool)> callback);

private:
	const QString _serviceName;
	SERVICE_TYPE * _service;
	std::function<void(bool)> _callback;

	// Inline pour eviter le warning C4505 remonté par visual incorrectement sur les methodes virtual dans des classes template
	virtual void OnRegisterService(StreamWork::SwCore::ISwService * service) override
	{
		if (service->GetServiceName() == _serviceName)
		{
			_service = dynamic_cast<SERVICE_TYPE *>(service);

			if (_callback)
				_callback(true);
		}
	}

	// Inline pour eviter le warning C4505 remonté par visual incorrectement sur les methodes virtual dans des classes template
	virtual void OnUnregisterService(StreamWork::SwCore::ISwService * service) override
	{
		if (service->GetServiceName() == _serviceName)
		{
			if (_callback)
				_callback(false);

			_service = nullptr;
		}
	}

};

//---------------------------------------------------------------------------------
template <typename SERVICE_TYPE>
SwServiceManager_Helper<SERVICE_TYPE>::SwServiceManager_Helper(QString serviceName)
    : _serviceName(serviceName)
    , _service(dynamic_cast<SERVICE_TYPE *> (SW_APP->QueryService(serviceName)))
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
SERVICE_TYPE * SwServiceManager_Helper<SERVICE_TYPE>::getService() const
{
	return _service;
}

//-----------------------------------------------------------------------
template <typename SERVICE_TYPE>
template <typename YOUR_CLASS>
void SwServiceManager_Helper<SERVICE_TYPE>::setCallback(YOUR_CLASS* thisPointer, void (YOUR_CLASS::*callback)(bool))
{
	if (_service && _callback)
		_callback(false);

	_callback = ([callback,thisPointer](bool available)->void { (thisPointer->*callback)(available); });

	if (_service && _callback)
		_callback(true);
}

//-----------------------------------------------------------------------
template <typename SERVICE_TYPE>
void SwServiceManager_Helper<SERVICE_TYPE>::setCallback(std::function<void(bool)> callback)
{
    if (_service && _callback)
        _callback(false);

    _callback = callback;

	if (_service && _callback)
        _callback(true);
}
