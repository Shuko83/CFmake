/*!
\file _SwConsumedInterfaceContainer_Class.cpp
\date 12/04/2006
\brief conteneur de stockage des informations relatives a une interface consommée
\author  Big
\version 1.0
*/
#include <QtGlobal>
#include "SwApplication.h"
#include "_SwConsumedInterfaceContainer_Class.h"

using namespace StreamWork::SwCore;



//-----------------------------------------------------------------------
_SwConsumedInterfaceContainer_Class::_SwConsumedInterfaceContainer_Class(SwInterfaces_Consumer_Class * parent)
{
	_parent = parent;
	_name = QString();
	QString _provider_name = QString();
	QString _type = QString();
	_isAvailable = false;
	_hhandle = NULL;
	_provider_parent = NULL;
	_historical_index = Q_UINT64_C(0);
	_potential_provider_name = QString();
	_potential_provider_path = QString();
}

//-----------------------------------------------------------------------
_SwConsumedInterfaceContainer_Class::~_SwConsumedInterfaceContainer_Class()
{

}

//-----------------------------------------------------------------------
void _SwConsumedInterfaceContainer_Class::Define(QString name, QString type)
{
	_name = name;
	_type = type;
}

//-----------------------------------------------------------------------
QString _SwConsumedInterfaceContainer_Class::GetType()
{
	return _type;
}

//-----------------------------------------------------------------------
QString _SwConsumedInterfaceContainer_Class::GetName()
{
	return _name;
}

//-----------------------------------------------------------------------
void _SwConsumedInterfaceContainer_Class::SetInterfaceHandle(void ** handle)
{
	_hhandle = handle;
}

//-----------------------------------------------------------------------
void _SwConsumedInterfaceContainer_Class::SetHandle(void * handle)
{
	*_hhandle = handle;
	if ( handle == NULL ) _isAvailable = false;
	else _isAvailable = true;
}

//-----------------------------------------------------------------------
bool _SwConsumedInterfaceContainer_Class::IsAvailable()
{
	return _isAvailable;
}

//-----------------------------------------------------------------------
quint64 _SwConsumedInterfaceContainer_Class::GetHistoricalIndex()
{
	return _historical_index;
}

//-----------------------------------------------------------------------
ISwInterfaces_Provider * _SwConsumedInterfaceContainer_Class::GetProvider()
{
	return _provider_parent;
}

//-----------------------------------------------------------------------
QString _SwConsumedInterfaceContainer_Class::GetProvidedInterfaceName()
{
	return _provider_name;
}

//-----------------------------------------------------------------------
void _SwConsumedInterfaceContainer_Class::RegisterProviderInterface(ISwInterfaces_Provider * provider, QString pinterface) throw(SwException)
{
	_provider_parent = provider;
	_provider_name = pinterface;
	//Enregistrement du temps de l'action de regsitration
	_historical_index = SW_APP->GetHistoricCpt();
	provider->AddConsumer(_parent, _name, pinterface);
}

//-----------------------------------------------------------------------
void _SwConsumedInterfaceContainer_Class::UnregisterProviderInterface()
{
	_provider_parent->RemoveConsumer(_parent, _name, _provider_name);
	_provider_parent = NULL;
	_provider_name = QString();
}

//-----------------------------------------------------------------------
void _SwConsumedInterfaceContainer_Class::DefinePotentialProviderInterfaceDescription(QString ppotential_interface, QString ppotential_provider_path)
{
	_potential_provider_name = ppotential_interface;
	_potential_provider_path = ppotential_provider_path;
}

//-----------------------------------------------------------------------
QString _SwConsumedInterfaceContainer_Class::GetPotentialProviderInterfaceName()
{
	return _potential_provider_name;
}

//-----------------------------------------------------------------------
QString _SwConsumedInterfaceContainer_Class::GetPotentialProviderPath()
{
	return _potential_provider_path;
}

