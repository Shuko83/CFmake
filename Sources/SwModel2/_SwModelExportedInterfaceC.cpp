/*!
 \file _SwModelExportedInterfaceC.cpp
 \brief  Class implementant une entite exportable interface consommée
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#include "_SwModelExportedInterfaceC.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"
#include "SwAddress_ToolBox.h"


//-----------------------------------------------------------------------
_SwModelExportedInterfaceC::_SwModelExportedInterfaceC() :_SwModelExportedEntity()
{
	_type = Ent_InterfaceC;
	_internal_provider = NULL;
	_external_consumer = NULL;
	_handle = NULL;
	//_host_consumer=NULL;
	_interface_created = false;
}

//-----------------------------------------------------------------------
_SwModelExportedInterfaceC::~_SwModelExportedInterfaceC()
{

}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::SpecificBuild()
{
	_internal_provider = dynamic_cast<SwInterfaces_Provider_Class *>(_model_host->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	//Creation du producteur interne
	_internal_provider->RegisterProvidedInterfaceWithType(_exported_name + "_", _itype, _handle);
	_internal_provider->SetInterfaceUnavailable(_exported_name + "_");

}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::SpecificDestroy()
{
	//recuperation des consumer existants
	QString name;
	ISwInterfaces_Consumer * consumer = _internal_provider->GetFirstConsumer(_exported_name + "_", &name);
	while ( consumer != NULL )
	{
		_lastPaths.push_back(SwAddress_ToolBox::BuildRelativePath(_model_host, consumer->GetHostComponent()));
		_lastInterfaces.push_back(name);
		consumer = _internal_provider->GetNextConsumer(&name);
	}
	//Destruction du producteur interne
	_internal_provider->UnregisterProvidedInterface(_exported_name + "_");
	//Fin
	_internal_provider = NULL;
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::SpecificBind()
{
	//_host_consumer=dynamic_cast<ISwInterfaces_Consumer *>(_host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));    
	_external_consumer = dynamic_cast<SwInterfaces_Consumer_Class *>(_model->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
	//if (_host_consumer==NULL)
	//    return;
	//_host_consumer->AttachInterfacesServices_Listener(this);
	//Si l'interface existe deja on la cree
	//if (!_host_consumer->GetInterfaceType(_name).isEmpty()) {
	OnAddInterface(_host_consumer, _name);
	//}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::SpecificUnbind()
{
	//if (_host_consumer==NULL)
	//    return;
	//Si l'interface a été crée on la detruit
	//if (_interface_created) {
	OnRemoveInterface(_host_consumer, _name);
	//}
	//_host_consumer->DetachInterfacesServices_Listener(this);
	//_host_consumer=NULL;
	_external_consumer = NULL;
	_handle = NULL;
}

//-----------------------------------------------------------------------
QStringList _SwModelExportedInterfaceC::getConsumerPaths()
{
	return _lastPaths;
}

//-----------------------------------------------------------------------
QStringList _SwModelExportedInterfaceC::getInterfaceNames()
{
	return _lastInterfaces;
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::connectInterfaceTo(QStringList & paths, QStringList & names)
{
	for ( int i = 0; i < paths.count(); i++ )
	{
		SwComponent_Class * consumer_host = SwAddress_ToolBox::FindTarget(paths[i], _model_host);
		if ( consumer_host == NULL )
		{
			return;
		}
		SwInterfaces_Consumer_Class * consumer = dynamic_cast<SwInterfaces_Consumer_Class *>(consumer_host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
		consumer->AttachProvider(_internal_provider, names[i], _exported_name + "_");
	}
}
//-------------------------------------------------------------------------
//Interface ISwInterfaces_ServicesListener
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::OnAddInterface(ISwInterfaces_Service * source, QString interface_name)
{
	if ( source == (ISwInterfaces_Service *) _host_consumer && _name == interface_name )
	{
		//Sur creation de l'interface a propager on crée les deux interfaces locales
		//Connexion a l'hote
		//_host_consumer->AttachProvider(_internal_provider,_name,_exported_name);
		//Creation du consommateur externe
		_external_consumer->RegisterConsumedInterfaceWithType(_exported_name, _itype, &_handle);
		//Observation du consommateur externe
		_external_consumer->AttachInterfacesConsumerObserver(this);
		//Flag d'info
		_interface_created = true;

	}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::OnRemoveInterface(ISwInterfaces_Service * source, QString interface_name)
{
	if ( source == (ISwInterfaces_Service *) _host_consumer && _name == interface_name )
	{
		//Desobservation du consommateur externe
		_external_consumer->DetachInterfacesConsumerObserver(this);
		//Destruction du consommateur externe
		_external_consumer->UnregisterConsumedInterface(_exported_name);
		//Flag d'info
		_interface_created = false;
	}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::OnConnectInterface(ISwInterfaces_Service * source, QString interface_name, ISwInterfaces_Service * remote_source, QString remote_interface_name)
{
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::OnDisconnectInterface(ISwInterfaces_Service * source, QString interface_name, ISwInterfaces_Service * remote_source, QString remote_interface_name)
{

}
//-------------------------------------------------------------------------
//Interface ISwInterfaces_ConsumerObserver
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::BeforeInterfaceAvailabilityChange(QString interface_name, SwComponent_Class * provider_host)
{
	if ( interface_name == _exported_name &&  _handle != NULL )
	{
		//Rendre l'interface non disponible
		_internal_provider->SetInterfaceUnavailable(_exported_name + "_");
	}
}

//-----------------------------------------------------------------------
void _SwModelExportedInterfaceC::AfterInterfaceAvailabilityChange(QString interface_name, SwComponent_Class * provider_host)
{
	if ( interface_name == _exported_name &&  _handle != NULL )
	{
		//Rendre l'interface non disponible
		_internal_provider->SetInterfaceAvailable(_exported_name + "_", _handle);
	}
}
