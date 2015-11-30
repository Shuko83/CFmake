/*!
 \file SwServicesManager_Class.h
 \brief Implementation of the Class SwServicesManager_Class
 \version 1.0
 \date 23-août-2006 16:04:57
 \author F.Bighelli
 */
#include <QtGlobal>
#include "SwMacros.h"
#include "SwServicesManager_Class.h"
#include <QCoreApplication>
#include "qmessagebox.h"

using namespace StreamWork::SwCore;


//---------------------------------------------------------------------------------
SwServicesManager_Class::SwServicesManager_Class()
{
	_services.clear();
	_ordered_services.clear();
}

//---------------------------------------------------------------------------------
SwServicesManager_Class::~SwServicesManager_Class()
{
	//LiberateAllServices(); fait par le composant
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::LiberateAllServices()
{
	QMap<QString, ISwService *>::iterator it;
	ISwService * service;

	for ( it = _services.begin(); it != _services.end(); it++ )
	{
		service = it.value();
		//qDebug("------------------ >Destroy service: %s",service->GetServiceName().toLatin1().data());
		service->Liberate();
	}
	_services.clear();
	_ordered_services.clear();
}

//---------------------------------------------------------------------------------
QList<QString> SwServicesManager_Class::GetServicesList()
{
	return _ordered_services;
}

//---------------------------------------------------------------------------------
ISwService * SwServicesManager_Class::QueryService( QString service_name )
{
	QMap<QString, ISwService *>::iterator it;

	it = _services.find( service_name );
	if ( it != _services.end() )
	{
		return it.value();
	}

	// le service n'a pas été trouvé!!
	raiseQueryError( service_name );

	return NULL;
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::RegisterService( ISwService * service ) throw(SwException)
{
	QMap<QString, ISwService *>::iterator it;

	Q_ASSERT( service != NULL );
	it = _services.find( service->GetServiceName() );
	if ( it != _services.end() )
	{
		//Verru pour desactiver la popup relou pour le service de check licence avec streamworkEditor
		if ( (service->GetServiceName() == "CheckLicense" || service->GetServiceName() == "ProductKeyService") && QCoreApplication::applicationName() == "StreamWorkEditor2" )
			return;
		QString msg = QString( "A service with %1 already registered" ).arg( service->GetServiceName() );
		QMessageBox::critical(NULL,"SwCore", msg );
		return;
	}
	_services.insert( service->GetServiceName(), service );
	_ordered_services.push_back( service->GetServiceName() );
	service->InitializeService();
	for ( int i = 0; i < _servicesObservers.count(); i++ )
	{
		_servicesObservers[i]->OnRegisterService( service );
	}
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::UnregisterService( QString service_name ) throw(SwException)
{
	QMap<QString, ISwService *>::iterator it;
	QList<QString>::iterator itd;

	it = _services.find( service_name );
	if ( it == _services.end() )
	{
		QString msg = QString( "Try to unregister unknown service %1" ).arg( service_name );
		LAUNCH_SWEXCEPTION( "SwCore", msg );
	}
	//qDebug("------- > UnregisterService: %s",it.value()->GetServiceName().toLatin1().data());
	for ( int i = 0; i < _servicesObservers.count(); i++ )
	{
		_servicesObservers[i]->OnUnregisterService( it.value() );
	}
	it.value()->Liberate();
	_services.erase( it );
	itd = _ordered_services.begin();
	while ( itd != _ordered_services.end() )
	{
		if ( (*itd) == service_name )
		{
			_ordered_services.erase( itd );
			return;
		}
		itd++;
	}

}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::AddServicesManagerObserver( ISwServicesManager_Listener * observer )
{
	if ( _servicesObservers.indexOf( observer ) >= 0 )
		return;

	_servicesObservers.push_back( observer );
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::RemoveServicesManagerObserver( ISwServicesManager_Listener * observer )
{
	int index = _servicesObservers.indexOf( observer );
	if ( index >= 0 )
		_servicesObservers.removeAt( index );
}

