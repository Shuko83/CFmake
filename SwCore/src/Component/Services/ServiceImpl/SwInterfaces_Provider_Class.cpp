/*!
\file SwInterfaces_Provider_Class.cpp
\date 12/04/2006
\brief implementation du service fournisseur d'interfaces
\author  Big
\version 1.0
*/

#include "Main/Connexion/_SwProvidedInterfaceContainer_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ISwInterfaces_Consumer.h"
#include <QMessageBox>


using namespace StreamWork::SwCore;


//-----------------------------------------------------------------------
SwInterfaces_Provider_Class::SwInterfaces_Provider_Class(SwComponent_Class * host)
{
	_host_component = host;
	_interfaces.clear();
	_current_interface = _interfaces.end();
	_current_interface_container = nullptr;
}

//-----------------------------------------------------------------------
SwInterfaces_Provider_Class::~SwInterfaces_Provider_Class()
{
	Liberate();
}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::RegisterProvidedInterfaceWithType(QString pinterface_name, QString pinterface_type, void * handle_interface) throw(SwException)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::const_iterator it;
	_SwProvidedInterfaceContainer_Class * interface_container;

	it = _interfaces.find(pinterface_name);
	if ( it != _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to register provided interface %1 because an other provided interface have same name").arg(pinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	interface_container = new _SwProvidedInterfaceContainer_Class(this);
	interface_container->Define(pinterface_name, pinterface_type);
	interface_container->SetInterface(handle_interface);
	_interfaces.insert(pinterface_name, interface_container);
	//Signal ajout d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnAddInterface(this, pinterface_name);
	}

}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::UnregisterProvidedInterface(QString pinterface_name) throw(SwException)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to unregister provided interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Si on deplace le pointeur courant si c'est celui a supprimer
	if ( _current_interface == it ) _current_interface++;
	//Si le container courant est celui a supprimer on le remets a zero
	if ( _current_interface_container == it.value() ) _current_interface_container = nullptr;
	//suppression
	delete it.value();
	_interfaces.erase(it);
	//Signal suppression d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnRemoveInterface(this, pinterface_name);
	}

}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::SetInterfaceAvailable(QString pinterface_name, void * new_handle) throw(SwException)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to availabilise of interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		QMessageBox::critical(nullptr, "Critical error", msg);
		return;
	}
	if ( new_handle != nullptr )
		it.value()->SetInterface(new_handle);
	it.value()->ChangeAvailability(true);
}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::SetInterfaceUnavailable(QString pinterface_name) throw(SwException)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to unavailabilise of interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		QMessageBox::critical(nullptr, "Critical error", msg);
		return;
	}
	it.value()->ChangeAvailability(false);
}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::SubstituteInterfaceWithType(QString pinterface_name, QString pinterface_type, void * handle_interface) throw(SwException)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to change interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		QMessageBox::critical(nullptr, "Critical error", msg);
	}
	it.value()->SetInterface(handle_interface);

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_Provider
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::AddConsumer(ISwInterfaces_Consumer * consumer, QString cinterface_name, QString pinterface_name)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to add comsumer to interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		QMessageBox::critical(nullptr, "Critical error", msg);
	}
	it.value()->RegisterConsumedInterface(consumer, cinterface_name);
	//Signal connection d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnConnectInterface(this, pinterface_name, consumer, cinterface_name);
	}
}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::RemoveConsumer(ISwInterfaces_Consumer * consumer, QString cinterface_name, QString pinterface_name)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to remove comsumer to interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		QMessageBox::critical(nullptr, "Critical error", msg);	
		return;
	}
	it.value()->UnregisterConsumedInterface(consumer, cinterface_name);
	//Signal connection d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnDisconnectInterface(this, pinterface_name, consumer, cinterface_name);
	}
}

//-----------------------------------------------------------------------
QString SwInterfaces_Provider_Class::GetInterfaceType(QString pinterface_name)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to get type of interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
		QMessageBox::critical(nullptr, "Critical error", msg);
		return "";
	}
	return it.value()->GetType();
}

//-----------------------------------------------------------------------
bool SwInterfaces_Provider_Class::InterfaceExist(QString pinterface_name)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
		return false;
	else
		return true;
}

//-----------------------------------------------------------------------
QString SwInterfaces_Provider_Class::GetFirstInterface()
{
	QString result;

	_current_interface = _interfaces.begin();
	if ( _current_interface != _interfaces.end() )
	{
		result = _current_interface.key();
		_current_interface++;
	}
	else
	{
		result = QString();
	}
	return result;
}

//-----------------------------------------------------------------------
QString SwInterfaces_Provider_Class::GetNextInterface()
{
	QString result;

	if ( _current_interface != _interfaces.end() )
	{
		result = _current_interface.key();
		_current_interface++;
	}
	else
	{
		result = QString();
	}
	return result;
}

//-----------------------------------------------------------------------
ISwInterfaces_Consumer * SwInterfaces_Provider_Class::GetFirstConsumer(QString pinterface_name, QString * pt_cinterfacename)
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(pinterface_name);
	if ( it == _interfaces.end() )
	{
		_current_interface_container = nullptr;
		return nullptr;
	}
	_current_interface_container = it.value();
	return _current_interface_container->GetFirstConsumer(pt_cinterfacename);
}

//-----------------------------------------------------------------------
ISwInterfaces_Consumer * SwInterfaces_Provider_Class::GetNextConsumer(QString * pt_cinterfacename)
{

	if ( _current_interface_container != nullptr )
		return _current_interface_container->GetNextConsumer(pt_cinterfacename);
	return nullptr;
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_Service
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::AttachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener)
{
	QSet<ISwInterfaces_ServicesListener *>::const_iterator it;

	it = _services_observers.find(listener);
	if ( it != _services_observers.end() )
	{
		QString msg = QString("In component %2\nFailed to attach an already attached interfaces services listener").arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	_services_observers.insert(listener);
}

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::DetachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener)
{
	QSet<ISwInterfaces_ServicesListener *>::iterator it;

	it = _services_observers.find(listener);
	if ( it == _services_observers.end() )
	{
		QString msg = QString("In component %2\nFailed to detach an undefined attached interfaces services listener").arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	_services_observers.erase(it);
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
void SwInterfaces_Provider_Class::Liberate()
{
	QMap<QString, _SwProvidedInterfaceContainer_Class *>::iterator it;

	for ( it = _interfaces.begin(); it != _interfaces.end(); it++ )
	{
		delete it.value();
		//Signal suppression d'interface
		for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
		{
			(*its)->OnRemoveInterface(this, it.key());
		}
	}
	_interfaces.clear();
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
SwComponent_Class * SwInterfaces_Provider_Class::GetHostComponent()
{
	return _host_component;
}
