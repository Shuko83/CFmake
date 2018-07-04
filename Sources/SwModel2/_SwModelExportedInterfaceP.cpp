/*!
 \file _SwModelExportedInterfaceP.cpp
 \brief  Class implementant une entite exportable interface produite
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#include "_SwModelExportedInterfaceP.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"
#include "SwAddress_ToolBox.h"


//-----------------------------------------------------------------------
_SwModelExportedInterfaceP::_SwModelExportedInterfaceP() :_SwModelExportedEntity()
{
	_type = Ent_InterfaceP;
	_internal_consumer = NULL;
	_external_provider = NULL;
	_handle = NULL;
	//_host_provider=NULL;
	_interface_created = false;
}

//-----------------------------------------------------------------------
_SwModelExportedInterfaceP::~_SwModelExportedInterfaceP()
{

}

//-----------------------------------------------------------------------
QString _SwModelExportedInterfaceP::getProviderPath()
{
	return _lastPath;
}

//-----------------------------------------------------------------------
QString _SwModelExportedInterfaceP::getInterfaceName()
{
	return _lastInterface;
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::connectInterfaceTo(QString path, QString name)
{
	SwComponent_Class * provider_host = SwAddress_ToolBox::FindTarget(path, _model_host);
	if ( provider_host == NULL )
	{
		return;
	}
	SwInterfaces_Provider_Class * lprovider = dynamic_cast<SwInterfaces_Provider_Class *>(provider_host->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	_internal_consumer->AttachProvider(lprovider, _exported_name, name);
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::SpecificBuild()
{
	_internal_consumer = dynamic_cast<SwInterfaces_Consumer_Class *>(_model_host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
	//Creation du consommateur interne
	_internal_consumer->RegisterConsumedInterfaceWithType(_exported_name, _itype, &_handle);
	//Observation du consommateur interne
	_internal_consumer->AttachInterfacesConsumerObserver(this);

}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::SpecificDestroy()
{
	//Desobservation du consommateur interne
	_internal_consumer->DetachInterfacesConsumerObserver(this);
	//Destruction du consommateur interne
	_internal_consumer->UnregisterConsumedInterface(_exported_name);
	//Fin
	_internal_consumer = NULL;
}


//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::SpecificBind()
{
	//_host_provider=dynamic_cast<ISwInterfaces_Provider *>(_host->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));    
	_external_provider = dynamic_cast<SwInterfaces_Provider_Class *>(_model->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	//if (_host_provider==NULL)
	//    return;
	//_host_provider->AttachInterfacesServices_Listener(this);
	//Si l'interface existe deja on la cree
	//if (!_host_provider->GetInterfaceType(_name).isEmpty()) {
	OnAddInterface(NULL, _name);
	//}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::SpecificUnbind()
{
	//if (_host_provider==NULL)
	//    return;
	//Si l'interface a été crée on la detruit
	//if (_interface_created) {
	OnRemoveInterface(NULL, _name);
	//}
	//_host_provider->DetachInterfacesServices_Listener(this);
	//_host_provider=NULL;
	_external_provider = NULL;
	_handle = NULL;
}
//-------------------------------------------------------------------------
//Interface ISwInterfaces_ServicesListener
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::OnAddInterface(ISwInterfaces_Service * source, QString interface_name)
{
	if (/*source==(ISwInterfaces_Service *)_host_provider &&*/ _name == interface_name )
	{
		//Sur creation de l'interface a propager on crée les deux interfaces locales
		//Creation du producteur externe
		_external_provider->RegisterProvidedInterfaceWithType(_exported_name, _itype, _handle);
		_external_provider->SetInterfaceUnavailable(_exported_name);
		if ( _handle != 0 )
		{
			_external_provider->SetInterfaceAvailable(_exported_name, _handle);
		}
		//Connexion a l'hote
		//_internal_consumer->AttachProvider(_host_provider,_exported_name,_name);
		//Flag d'info
		_interface_created = true;

	}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::OnRemoveInterface(ISwInterfaces_Service * source, QString interface_name)
{
	if (/*source==(ISwInterfaces_Service *)_host_provider &&*/ _name == interface_name )
	{
		//Destruction du producteur externe
		if ( _handle != 0 )
		{
			_external_provider->SetInterfaceUnavailable(_exported_name);
		}
		_external_provider->UnregisterProvidedInterface(_exported_name);
		//Flag d'info
		_interface_created = false;
	}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::OnConnectInterface(ISwInterfaces_Service * source, QString interface_name, ISwInterfaces_Service * remote_source, QString remote_interface_name)
{
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::OnDisconnectInterface(ISwInterfaces_Service * source, QString interface_name, ISwInterfaces_Service * remote_source, QString remote_interface_name)
{

}
//-------------------------------------------------------------------------
//Interface ISwInterfaces_ConsumerObserver
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::BeforeInterfaceAvailabilityChange(QString interface_name, SwComponent_Class * provider_host)
{
	if ( interface_name == _exported_name &&  _handle != NULL )
	{
		_lastPath = QString();
		_lastInterface = QString();
		//Rendre l'interface non disponible
		if ( _external_provider != 0 )
			_external_provider->SetInterfaceUnavailable(_exported_name);
	}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceP::AfterInterfaceAvailabilityChange(QString interface_name, SwComponent_Class * provider_host)
{

	if ( interface_name == _exported_name &&  _handle != NULL )
	{
		_lastPath = SwAddress_ToolBox::BuildRelativePath(_model_host, provider_host);
		_host_path = SwAddress_ToolBox::BuildAbsolutePath(provider_host);
		ISwInterfaces_Provider * _provider = NULL;
		QString provider_name;
		QString result = _internal_consumer->GetFirstInterface(NULL, &_provider, &provider_name);
		while ( !result.isEmpty() && result != interface_name )
		{
			_provider = NULL;
			result = _internal_consumer->GetNextInterface(NULL, &_provider, &provider_name);
		}
		if ( _provider != NULL )
		{
			_lastInterface = provider_name;
		}
		//Rendre l'interface non disponible
		if ( _external_provider != 0 )
			_external_provider->SetInterfaceAvailable(_exported_name, _handle);
	}
}
