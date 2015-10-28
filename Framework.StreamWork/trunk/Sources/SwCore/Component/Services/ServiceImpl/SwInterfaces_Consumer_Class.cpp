/*!
\file SwInterfaces_Consumer_Class.cpp
\date 12/04/2006
\brief implementation du service consommateur d'interfaces
\author  Big
\version 1.0
*/
#include "SwInterfaces_Consumer_Class.h"
#include "_SwConsumedInterfaceContainer_Class.h"
#include "SwAddress_ToolBox.h"
#include "SwApplication.h"
#include <QDebug>


using namespace StreamWork::SwCore;


#define CL_SW_XML_LISTIDX_NODE    "ListIDX"
#define CL_SW_XML_INTERFACE_NODE    "interface"
#define CL_SW_XML_INTERFACE_NODE_ATT_NAME    "name"
#define CL_SW_XML_INTERFACE_NODE_ATT_PNAME    "provider_interface"
#define CL_SW_XML_INTERFACE_NODE_ATT_IDX     "idx"
#define CL_SW_XML_INTERFACE_NODE_ATT_PPATH    "provider_path"


//-----------------------------------------------------------------------
SwInterfaces_Consumer_Class::SwInterfaces_Consumer_Class(SwComponent_Class * host)
{
	_host_component = host;
	_finalize_interfaces.clear();
	_interfaces.clear();
	_current_interface = _interfaces.end();
}

//-----------------------------------------------------------------------
SwInterfaces_Consumer_Class::~SwInterfaces_Consumer_Class()
{
	_finalize_interfaces.clear();
}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::RegisterConsumedInterfaceWithType(QString cinterface_name, QString cinterface_type, void ** handle_handle_interface)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
	_SwConsumedInterfaceContainer_Class * interface_container;

	it = _interfaces.find(cinterface_name);
	if ( it != _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to register consumed interface %1 because an other consumed interface have same name").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	interface_container = new _SwConsumedInterfaceContainer_Class(this);
	interface_container->Define(cinterface_name, cinterface_type);
	interface_container->SetInterfaceHandle(handle_handle_interface);
	_interfaces.insert(cinterface_name, interface_container);
	//Signal ajout d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnAddInterface(this, cinterface_name);
	}

}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::UnregisterConsumedInterface(QString cinterface_name)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(cinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to unregister consumed interface %1 because it's undefined").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	DetachProvider(cinterface_name);
	delete it.value();
	_interfaces.erase(it);
	//Signal ajout d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnRemoveInterface(this, cinterface_name);
	}
}

//-----------------------------------------------------------------------
bool SwInterfaces_Consumer_Class::IsInterfaceAvailable(QString cinterface_name)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(cinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFailed to know if consumed interface %1 is available because it's undefined").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	return it.value()->IsAvailable();
}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::AttachInterfacesConsumerObserver(ISwInterfaces_ConsumerObserver * observer)
{
	QSet<ISwInterfaces_ConsumerObserver *>::const_iterator it;

	it = _observers.find(observer);
	if ( it != _observers.end() )
	{
		QString msg = QString("In component %2\nFailed to attach an already attached consumer observer").arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	_observers.insert(observer);
}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::DetachInterfacesConsumerObserver(ISwInterfaces_ConsumerObserver * observer)
{
	QSet<ISwInterfaces_ConsumerObserver *>::iterator it;

	it = _observers.find(observer);
	if ( it == _observers.end() )
	{
		QString msg = QString("In component %2\nFailed to detach an undefined attached consumer observer").arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	_observers.erase(it);

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_Consumer
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::AttachProvider(ISwInterfaces_Provider * provider, QString cinterface_name, QString pinterface_name)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(cinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFail to attach to undefined consumed interface %1 ").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//check du type des deux interfaces
	if ( it.value()->GetType() != provider->GetInterfaceType(pinterface_name) )
	{
		QString msg = QString("In component %2\nFail to attach un consumed interface %1 to an provided interface %3\nbecause they don't have same type %4 and %5").arg(cinterface_name).arg(_host_component->GetName()).arg(pinterface_name).arg(it.value()->GetType()).arg(provider->GetInterfaceType(pinterface_name));
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	it.value()->RegisterProviderInterface(provider, pinterface_name);
	//Signal connection d'interface
	for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
	{
		(*its)->OnConnectInterface(this, cinterface_name, provider, pinterface_name);
	}

}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::DetachProvider(QString cinterface_name)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
	ISwInterfaces_Service * provider;
	QString pinterface_name;

	it = _interfaces.find(cinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFail to detach undefined consumed interface %1 ").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Elle-t-elle connecté
	if ( it.value()->GetProvider() != NULL )
	{
		provider = it.value()->GetProvider();
		pinterface_name = it.value()->GetProvidedInterfaceName();
		it.value()->UnregisterProviderInterface();

		//Signal connection d'interface
		for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
		{
			(*its)->OnDisconnectInterface(this, cinterface_name, provider, pinterface_name);
		}
	}
}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::ProvideInterface(QString cinterface_name, void * handle_interface)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
	QSet<ISwInterfaces_ConsumerObserver *>::iterator ito;

	it = _interfaces.find(cinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFail to provide undefined consumed interface %1 ").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	it.value()->SetHandle(NULL);
	//Signal avant changement
	for ( ito = _observers.begin(); ito != _observers.end(); ito++ )
	{
		(*ito)->BeforeInterfaceAvailabilityChange(cinterface_name, it.value()->GetProvider()->GetHostComponent());
	}
	//changement
	it.value()->SetHandle(handle_interface);
	//Signal apres changement
	for ( ito = _observers.begin(); ito != _observers.end(); ito++ )
	{
		(*ito)->AfterInterfaceAvailabilityChange(cinterface_name, it.value()->GetProvider()->GetHostComponent());
	}
}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::UnprovideInterface(QString cinterface_name)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
	QSet<ISwInterfaces_ConsumerObserver *>::iterator ito;

	it = _interfaces.find(cinterface_name);
	if ( it == _interfaces.end() )
	{
		QString msg = QString("In component %2\nFail to unprovide undefined consumed interface %1 ").arg(cinterface_name).arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Signal avant changement
	for ( ito = _observers.begin(); ito != _observers.end(); ito++ )
	{
		(*ito)->BeforeInterfaceAvailabilityChange(cinterface_name, it.value()->GetProvider()->GetHostComponent());
	}
	it.value()->SetHandle(NULL);
	//Signal apres changement
	for ( ito = _observers.begin(); ito != _observers.end(); ito++ )
	{
		(*ito)->AfterInterfaceAvailabilityChange(cinterface_name, it.value()->GetProvider()->GetHostComponent());
	}
}

//-----------------------------------------------------------------------
QString SwInterfaces_Consumer_Class::GetFirstInterface(QString * nametype, ISwInterfaces_Provider ** pt_provider, QString * pt_pinterface_name)
{
	QString name;

	_current_interface = _interfaces.begin();
	if ( _current_interface == _interfaces.end() )
	{
		return QString();
	}
	if ( nametype != NULL ) *nametype = _current_interface.value()->GetType();
	if ( pt_provider != NULL ) *pt_provider = _current_interface.value()->GetProvider();
	if ( pt_pinterface_name != NULL ) *pt_pinterface_name = _current_interface.value()->GetProvidedInterfaceName();
	name = _current_interface.key();
	_current_interface++;
	return name;
}

//-----------------------------------------------------------------------
QString SwInterfaces_Consumer_Class::GetNextInterface(QString * nametype, ISwInterfaces_Provider ** pt_provider, QString * pt_pinterface_name)
{
	QString name;

	if ( _current_interface == _interfaces.end() )
	{
		return QString();
	}
	if ( nametype != NULL ) *nametype = _current_interface.value()->GetType();
	if ( pt_provider != NULL ) *pt_provider = _current_interface.value()->GetProvider();
	if ( pt_pinterface_name != NULL ) *pt_pinterface_name = _current_interface.value()->GetProvidedInterfaceName();
	name = _current_interface.key();
	_current_interface++;
	return name;

}

//-----------------------------------------------------------------------
QString SwInterfaces_Consumer_Class::GetInterfaceType(QString interface_name)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;

	it = _interfaces.find(interface_name);
	if ( it == _interfaces.end() )
		return QString();
	return it.value()->GetType();
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_Service
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::AttachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener)
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
void SwInterfaces_Consumer_Class::DetachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener)
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
void SwInterfaces_Consumer_Class::Liberate()
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;

	for ( it = _interfaces.begin(); it != _interfaces.end(); it++ )
	{
		DetachProvider(it.value()->GetName());
		delete it.value();
		//Signal suppression d'interface
		for ( QSet<ISwInterfaces_ServicesListener *>::const_iterator its = _services_observers.begin(); its != _services_observers.end(); its++ )
		{
			(*its)->OnRemoveInterface(this, it.key());
		}
	}
	_interfaces.clear();
	_observers.clear();
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
SwComponent_Class * SwInterfaces_Consumer_Class::GetHostComponent()
{
	return _host_component;
}
//---------------------------------------------------------------------
// Interface ISwpersistent
//---------------------------------------------------------------------            
#define CL_SW_XML_LISTIDX_NODE    "ListIDX"
#define CL_SW_XML_INTERFACE_NODE    "interface"
#define CL_SW_XML_INTERFACE_NODE_ATT_NAME    "name"
#define CL_SW_XML_INTERFACE_NODE_ATT_PNAME    "provider_interface"
#define CL_SW_XML_INTERFACE_NODE_ATT_IDX     "idx"
#define CL_SW_XML_INTERFACE_NODE_ATT_PPATH    "provider_path"


//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::Load(QDomElement &elt, ISwFinalizerManager & finalizer_manager)
{
	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
	QMap<quint64, _SwConsumedInterfaceContainer_Class *>::iterator itidx;
	quint64 index;
	bool result;
	QString name;

	//RAZ de la table de finalization (load multiples)
	_finalize_interfaces.clear();

	//lire d'abord les IDX
	QDomElement list = elt.firstChildElement(CL_SW_XML_LISTIDX_NODE);
	QMap<QString, quint64> _listIndex;
	if ( !list.isNull() )
	{
		for ( QDomElement interface_node = list.firstChildElement(QString(CL_SW_XML_INTERFACE_NODE));
			 !interface_node.isNull();
			 interface_node = interface_node.nextSiblingElement(QString(CL_SW_XML_INTERFACE_NODE)) )
		{
			index = interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX).toULongLong(&result);
			if ( result == false )
			{
				QString msg = QString("In component %1\nFail to load index of consumed interface description because it's not a valid index %2").arg(_host_component->GetName()).arg(interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX));
				LAUNCH_SWEXCEPTION("SwCore", msg)
			}

			_listIndex.insert(interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_NAME), index);
		}
	}

	//recuperation des interfaces
	for ( QDomElement interface_node = elt.firstChildElement(QString(CL_SW_XML_INTERFACE_NODE));
		 !interface_node.isNull();
		 interface_node = interface_node.nextSiblingElement(QString(CL_SW_XML_INTERFACE_NODE)) )
	{
		if ( interface_node.hasAttribute(CL_SW_XML_INTERFACE_NODE_ATT_NAME) &&
			interface_node.hasAttribute(CL_SW_XML_INTERFACE_NODE_ATT_PNAME) &&
			/* interface_node.hasAttribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX) &&*/
			interface_node.hasAttribute(CL_SW_XML_INTERFACE_NODE_ATT_PPATH) )
		{
			name = interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_NAME);

			//On recupere l'index
			index = interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX).toULongLong(&result);
			if ( result == false && !_listIndex.contains(name) )
			{
				QString msg = QString("In component %1\nFail to load index of consumed interface description because it's not a valid index %2").arg(_host_component->GetName()).arg(interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX));
				LAUNCH_SWEXCEPTION("SwCore", msg)
			}
			if ( !result )
				index = _listIndex.value(name);

			//On verifie que l'index n'est pas deja utilise (ne doit JAMAIS ARRIVE)
			itidx = _finalize_interfaces.find(index);
			if ( itidx != _finalize_interfaces.end() )
			{
				QString msg = QString("In component %1\nFail to load consumed interfaces description because two index are equal to %2").arg(_host_component->GetName()).arg(index);
				LAUNCH_SWEXCEPTION("SwCore", msg)
			}

			//recherche de l'interface correspondante
			it = _interfaces.find(name);

			//Si elle existe
			if ( it != _interfaces.end() )
			{
				it.value()->DefinePotentialProviderInterfaceDescription(interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_PNAME), interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_PPATH));

				//Enregistrement en interne de l'interface
				_finalize_interfaces.insert(index, it.value());

				//Enregistrement au niveau du finalizer manager
				finalizer_manager.RegisterFinalization(index, this);
			}
			else
			{
				_SwConsumedInterfaceContainer_Class * fake = new _SwConsumedInterfaceContainer_Class(0);
				fake->Define(interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_NAME), QString());
				fake->DefinePotentialProviderInterfaceDescription(interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_PNAME), interface_node.attribute(CL_SW_XML_INTERFACE_NODE_ATT_PPATH));
				_finalize_interfaces.insert(index, fake);

				//Enregistrement au niveau du finalizer manager
				finalizer_manager.RegisterFinalization(index, this);
			}
		}
	}
}

//-----------------------------------------------------------------------
void SwInterfaces_Consumer_Class::Save(QDomElement &elt, QDomDocument &doc)
{
	QDomElement interface_node;
	QString path;

	QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
	QMap<QString, quint64> _listIndex;
	//pour toutes les interfaces connectés
	for ( it = _interfaces.begin(); it != _interfaces.end(); it++ )
	{
		//Si l'interface est connecté
		if ( it.value()->GetProvider() != NULL )
		{
			//Creation du neoud
			interface_node = doc.createElement(CL_SW_XML_INTERFACE_NODE);

			//Enregistrement du nom
			interface_node.setAttribute(CL_SW_XML_INTERFACE_NODE_ATT_NAME, it.key());

			//Enregistrement du nom de l'interface distante
			interface_node.setAttribute(CL_SW_XML_INTERFACE_NODE_ATT_PNAME, it.value()->GetProvidedInterfaceName());

			//Enregistrement de l'index pour la reconstruction
			//interface_node.setAttribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX,it.value()->GetHistoricalIndex());

			//Enregistrement du path pour l'acces au parent
			try
			{
				//On tente un lien relatif
				path = SwAddress_ToolBox::BuildRelativePath(_host_component, it.value()->GetProvider()->GetHostComponent());
			}
			catch ( SwException & )
			{
				//Impossible (pas le meme parent)
				//On recupere un lien universel
				path = SwAddress_ToolBox::BuildUniversalPath(it.value()->GetProvider()->GetHostComponent());
			}

			interface_node.setAttribute(CL_SW_XML_INTERFACE_NODE_ATT_PPATH, path);

			_listIndex.insert(it.key(), it.value()->GetHistoricalIndex());
			//Ajout du neoud interface
			elt.appendChild(interface_node);
		}
	}
	//On append la list des IDX
	if ( !_listIndex.isEmpty() )
	{
		QMap<QString, quint64>::const_iterator itbegin = _listIndex.constBegin();
		QMap<QString, quint64>::const_iterator itend = _listIndex.constEnd();

		QDomElement list_node;
		list_node = doc.createElement(CL_SW_XML_LISTIDX_NODE);
		for ( ; itbegin != itend; ++itbegin )
		{

			//Creation du neoud
			interface_node = doc.createElement(CL_SW_XML_INTERFACE_NODE);

			//Enregistrement du nom
			interface_node.setAttribute(CL_SW_XML_INTERFACE_NODE_ATT_NAME, itbegin.key());
			interface_node.setAttribute(CL_SW_XML_INTERFACE_NODE_ATT_IDX, itbegin.value());
			list_node.appendChild(interface_node);
		}
		elt.appendChild(doc.createComment("Begin : Ignore on merge"));
		elt.appendChild(list_node);
		elt.appendChild(doc.createComment("End : Ignore on merge"));
	}

}
//---------------------------------------------------------------------
// Interface ISwFinalizer
//---------------------------------------------------------------------            

//-----------------------------------------------------------------------
bool SwInterfaces_Consumer_Class::Finalize(quint64 historic_index)
{
	QMap<quint64, _SwConsumedInterfaceContainer_Class *>::iterator itidx;
	SwComponent_Class * host_provider;
	ISwInterfaces_Provider * provider;

	itidx = _finalize_interfaces.find(historic_index);
	if ( itidx != _finalize_interfaces.end() )
	{
		_SwConsumedInterfaceContainer_Class * container = itidx.value();
		//Si c'est un fake on cherche le vrai container
		if ( container->GetType().isEmpty() )
		{
			QMap<QString, _SwConsumedInterfaceContainer_Class *>::iterator it;
			it = _interfaces.find(container->GetName());
			if ( it != _interfaces.end() )
			{
				it.value()->DefinePotentialProviderInterfaceDescription(container->GetPotentialProviderInterfaceName(), container->GetPotentialProviderPath());
				container = it.value();
			}
			else
			{
				container = 0;
			}
		}
		if ( container != 0 )
		{
			//On a trouve l'interface concerné
			//Si elle est deja connecté, on la deconnecte (permet appel multiple du load)
			if ( container->GetProvider() != NULL )
			{
				DetachProvider(container->GetName());
			}
			//Acces au fournisseur
			host_provider = SwAddress_ToolBox::FindTarget(container->GetPotentialProviderPath(), _host_component);
			//S'il est defini et qu'il est producteur d'interfaces et qu'il produit l'interface qui nous interesse au bon type
			if ( host_provider != NULL &&
				(provider = dynamic_cast<ISwInterfaces_Provider *>(host_provider->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER))) != NULL &&
				provider->InterfaceExist(container->GetPotentialProviderInterfaceName()) )
			{
				//On attache l'interface
				AttachProvider(provider, container->GetName(), container->GetPotentialProviderInterfaceName());
				//Ok la finalization a reussi on libere l'index
				_finalize_interfaces.erase(itidx);
				//Et on renvoie ok
				return true;
			}
			else
			{
				SW_APP->Logger().Log(LogLvl_Debug, QString("Unable to connect %1 of %2").arg(container->GetName()).arg(_host_component->GetName()));
			}
		}
		else
		{
			return true;
		}
		//Fin sans finalisation possible
	}
	return false;
}
