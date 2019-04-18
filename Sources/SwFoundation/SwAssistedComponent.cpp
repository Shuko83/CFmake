/**
@file SwAssistedComponent.cpp
@brief Implementation avance d'un composant par defaut
@author Big
*/

#include <SwApplication.h>
#include "SwAssistedComponent.h"
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
#include "ISwServiceOwnerConfigurable.h"
#include "ISwServiceOwner.h"
#include "QCoreApplication"

#include <QElapsedTimer>
#include <QFile>

using namespace StreamWork;
using namespace StreamWork::SwFoundation;
using namespace StreamWork::SwCore;


//-------------------------------------------------------------------------
//							SwExecutable_Class
//-------------------------------------------------------------------------


/*!
\class SwExecutable_Class
\brief implementation du service fournisseur de l'execution
*/
class SwExecutable_Class : public ISwExecutable_Service {
private:

	/*! \brief composant hote */
	SwAssistedComponent * _host_component;

public:
	/*! \brief Construsteur */
	SwExecutable_Class(SwAssistedComponent * host);
	/*! \brief Destrusteur */
	virtual ~SwExecutable_Class();

	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------

	/*! \brief Initialisation */
	virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (SwCore::SwException);            
	/*! \brief Demarrage */
	virtual void Start(double current_time) throw (SwCore::SwException);            
	/*! \brief Execution */
	virtual void Execute(double current_time,bool is_first_call) throw (SwCore::SwException);            
	/*! \brief Execution */
	virtual void Stop(double current_time);

	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------
	/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
	void Liberate();

	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/*! \brief acces a son composant hote */
	SwAssistedComponent * GetHostComponent();

	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/*! \brief methode permettant de charger des donnees */
	void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);
	/*! \brief methode permettant de sauver des donnees */
	void Save(QXmlStreamWriter& writer);

	//---------------------------------------------------------------------
	// Interface ISwActivable
	//---------------------------------------------------------------------
	
	/*! \brief Permet de changer la valeur d'activation
    \param[in] value d'activation
    */
    virtual void setActive(bool value);
    /*! \brief Permet de lire la valeur d'activation
    \param[out] valeur d'activation
    */
    virtual bool isActive();
};

//-------------------------------------------------------------------------
SwExecutable_Class::SwExecutable_Class( SwAssistedComponent * host )
{
	_host_component=host;
}

//-------------------------------------------------------------------------
SwExecutable_Class::~SwExecutable_Class()
{
	Liberate();
}

//-------------------------------------------------------------------------
void SwExecutable_Class::Initialize( double start_time,ISwExecution_Service * executor ) throw (SwCore::SwException)
{
	_host_component->Initialize(start_time,executor);
}

//-------------------------------------------------------------------------
void SwExecutable_Class::Start( double current_time ) throw (SwCore::SwException)
{
	_host_component->Start(current_time);
}

//-------------------------------------------------------------------------
void SwExecutable_Class::Execute( double current_time,bool is_first_call ) throw (SwCore::SwException)
{
	_host_component->Execute(current_time,is_first_call);
}

//-------------------------------------------------------------------------
void SwExecutable_Class::Stop( double current_time )
{
	_host_component->Stop(current_time);
}

//-------------------------------------------------------------------------
void SwExecutable_Class::Liberate()
{

}

//-------------------------------------------------------------------------
SwAssistedComponent * SwExecutable_Class::GetHostComponent()
{
	return _host_component;

}

//-------------------------------------------------------------------------
void SwExecutable_Class::Load( QDomElement& elt, ISwFinalizerManager & finalizer_manager )
{

}

//-------------------------------------------------------------------------
void SwExecutable_Class::Save( QXmlStreamWriter& writer )
{

}

//-------------------------------------------------------------------------
void SwExecutable_Class::setActive( bool value )
{
	_host_component->setActive(value);
}

//-------------------------------------------------------------------------
bool SwExecutable_Class::isActive()
{
	return _host_component->isActive();
}





//-------------------------------------------------------------------------
//							SwOwnerConfigurable_Class
//-------------------------------------------------------------------------

/*!
\class SwOwnerConfigurable_Class
\brief implementation du service de owner configurable
*/
class SwOwnerConfigurable_Class : public ISwServiceOwnerConfigurable {
private:

	/*! \brief composant hote */
	SwAssistedComponent * _host_component;

public:
	/*! \brief Construsteur */
	SwOwnerConfigurable_Class(SwAssistedComponent * host);
	/*! \brief Destrusteur */
	virtual ~SwOwnerConfigurable_Class();

	//---------------------------------------------------------------------
	// Interface ISwPersistentConfigurable
	//---------------------------------------------------------------------
	
	/**
	 * @brief    : methode permettant de charger des donnees de configuration
	 * @param	 : QDomElement & -  Noeud parent
	 */
	virtual void LoadConfiguration(QDomElement &);

	/**
	 * @brief    : methode permettant de sauver des donnees de configuration
	 * @param	 : QXmlStreamWriter & - Stream de sortie XML
	 */
	virtual void SaveConfiguration(QXmlStreamWriter &);

	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------
	/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
	void Liberate();

};

//-------------------------------------------------------------------------
SwOwnerConfigurable_Class::SwOwnerConfigurable_Class( SwAssistedComponent * host )
{
	_host_component = host;
}

//-------------------------------------------------------------------------
SwOwnerConfigurable_Class::~SwOwnerConfigurable_Class()
{
	Liberate();
}

//-------------------------------------------------------------------------
void SwOwnerConfigurable_Class::LoadConfiguration( QDomElement & elm)
{
	_host_component->LoadConfiguration(elm);
}

//-------------------------------------------------------------------------
void SwOwnerConfigurable_Class::SaveConfiguration( QXmlStreamWriter &writer)
{
	_host_component->SaveConfiguration(writer);
}


//-------------------------------------------------------------------------
void SwOwnerConfigurable_Class::Liberate()
{

}





//-------------------------------------------------------------------------
//							SwOwner_Class
//-------------------------------------------------------------------------

/*!
\class SwOwner_Class
\brief implementation du service de owner configurable
*/
class SwOwner_Class : public ISwServiceOwner, public ISwFinalizer {
private:

	/*! \brief composant hote */
	SwAssistedComponent * _host_component;
	quint64				  _historyIndex;

public:
	/*! \brief Construsteur */
	SwOwner_Class(SwAssistedComponent * host);
	/*! \brief Destrusteur */
	virtual ~SwOwner_Class();

	//---------------------------------------------------------------------
	// Interface ISwPersistent
	//---------------------------------------------------------------------
	
	/**
	 * @brief    : methode permettant de charger des donnees
	 * @param	 : QDomElement & elt - Noeud parent
	 * @param	 : ISwFinalizerManager & finalizer_manager -  manager de finalisation
	 */
	virtual void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);

	/**
	 * @brief    : retourne l'index "historique" enregistrer à la fermeture du stream
	 * @return   : l'index history
	 */
	quint64 getHistoryIndex();

	virtual bool Finalize( quint64 historic_index );

	 /**
	  * @brief    :  methode permettant de sauver des donnees
	  * @param	 : QDomElement & elt - Noeud parent
	  * @param	 : QDomDocument & doc - Document parent
	  */
	 void Save(QXmlStreamWriter& writer);

	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------
	/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
	void Liberate();
};

//-------------------------------------------------------------------------
SwOwner_Class::SwOwner_Class( SwAssistedComponent * host ) : _host_component(host), _historyIndex(0)
{
}

//-------------------------------------------------------------------------
SwOwner_Class::~SwOwner_Class()
{
	Liberate();
}

//-------------------------------------------------------------------------
quint64 SwOwner_Class::getHistoryIndex()
{
	return _historyIndex;
}

//-------------------------------------------------------------------------
void SwOwner_Class::Load( QDomElement & elt,ISwFinalizerManager & finalizer_manager )
{
	//On recupere l'index
	bool result = false;
	_historyIndex=elt.attribute("historyIndex").toULongLong(&result);
	if (!result)
	{
		qDebug() << "Fail to load historyIndex on SwOwnerClass :" << _host_component->GetName();
	}
	SW_APP->SetHistoricCpt(_historyIndex);

	finalizer_manager.RegisterFinalization(_historyIndex, this);

	_host_component->Load(elt, finalizer_manager);
}

//-------------------------------------------------------------------------
void SwOwner_Class::Save(QXmlStreamWriter& writer)
{
	writer.writeAttribute("historyIndex", QString::number(_historyIndex));
	_host_component->Save(writer);
}


//-------------------------------------------------------------------------
void SwOwner_Class::Liberate()
{

}

//---------------------------------------------------------------------------------
bool SwOwner_Class::Finalize( quint64 historic_index )
{
	if (_historyIndex == 0)
		_historyIndex = SW_APP->GetHistoricCpt();

	ISwFinalizer * iFinalizer = dynamic_cast<ISwFinalizer*> (_host_component);
	
	if(iFinalizer)
		iFinalizer->Finalize(historic_index);

	return true;
}



//-------------------------------------------------------------------------
//							SwAssistedComponent
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
SwAssistedComponent::SwAssistedComponent():SwComponent_Class() 
{    
	_provider_service			= NULL;
	_consumer_service			= NULL;
	_properties_service			= NULL;
	_shortcuts_service			= NULL;
	_owner_service				= NULL;
	_ownerConf_service			= NULL;
	_executable_service			= NULL;
	_pins_service				= NULL;
	
	_disable_service			= false;

	_isExecutable				= false;
	_isConsumer					= true;
	_isProvider					= true;
	_isProperty					= true;
	_isPin						= false;
	_isOwnerConf				= false;
	_isOwner					= false;
	_isInitialized				= false;

	_doCheckTimer = qApp->arguments().contains("-checktime", Qt::CaseInsensitive);
}

//-------------------------------------------------------------------------
SwAssistedComponent::~SwAssistedComponent() 
{    
	_disable_service=true;

	//Clear Consummed Interface
	for ( QString name : _mapIConsummed.keys() )
		unconsummeInterface(name);

	for ( auto name : _mapIConsummedWithCallBack.keys() )
		unconsummeInterface(name);

	//Clear provided interface
	for (QString providedInterfaceName : _listIProvided)
		unprovideInterface(providedInterfaceName);

	//clear registrer service

	//Clear pin
	QList<SwPin*> tmpList3 = _listPin;
	for (SwPin *pin : tmpList3)
		unregisterPin(pin);

	//Desenregistrement des services
	if(_isConsumer && _consumer_service)
	{
		this->UnregisterService(_consumer_service->GetServiceName());
		delete _consumer_service;
		_consumer_service	= NULL;
	}

	if(_isProvider && _provider_service)
	{
		this->UnregisterService(_provider_service->GetServiceName());
		delete _provider_service;
		_provider_service	= NULL;
	}

	if(_isPin && _pins_service)
	{
		this->UnregisterService(_pins_service->GetServiceName());
		delete _pins_service;
		_pins_service	= NULL;
	}

	if(_isProperty && _properties_service)
	{
		this->UnregisterService(_properties_service->GetServiceName());
		delete _properties_service;
		_properties_service	= NULL;
	}

	if(_isExecutable && _executable_service)
	{
		this->UnregisterService(_executable_service->GetServiceName());
		delete _executable_service;
		_executable_service	= NULL;
	}

	if(_isOwnerConf && _ownerConf_service)
	{
		this->UnregisterService(_ownerConf_service->GetServiceName());
		delete _ownerConf_service;
		_ownerConf_service	= NULL;
	}

	if(_isOwner && _owner_service)
	{

		this->UnregisterService(_owner_service->GetServiceName());
		delete _owner_service;
		_owner_service	= NULL;
	}

	//Unregister all Shortcuts
	if (_shortcuts_service && !_mapShortcutNameWithCategory.isEmpty())
	{
		auto itBegin = _mapShortcutNameWithCategory.constBegin();
		auto itEnd = _mapShortcutNameWithCategory.constEnd();

		for (; itBegin != itEnd; itBegin++)
		{
			_shortcuts_service->unregisterCommand(itBegin.value(), itBegin.key(), this);
		}
	}
	_shortcuts_service = NULL;

}

//-------------------------------------------------------------------------
void SwAssistedComponent::InitializeResources() throw(SwException) 
{  
	QElapsedTimer *timer = nullptr;
	if ( _doCheckTimer )
	{
		timer = new QElapsedTimer();
		timer->start();
	}
	_isInitialized = true;
	//Creation des services
	if(_isConsumer)
	{
		_consumer_service		= new SwInterfaces_Consumer_Class(this) ;
		this->RegisterService(_consumer_service);
		_consumer_service->AttachInterfacesConsumerObserver(this);
	}
	
	if(_isProvider)
	{
		_provider_service		= new SwInterfaces_Provider_Class(this) ;
		this->RegisterService(_provider_service);
	}

	if(_isProperty)
	{
		_properties_service		= new SwProperties_Class(this);
		this->RegisterService(_properties_service);
	}

	if(_isPin)
	{
		_pins_service			= new SwPins_Manager_Class(this) ;
		this->RegisterService(_pins_service);
	}

	if(_isExecutable)
	{
		_executable_service		= new SwExecutable_Class(this);
		this->RegisterService(_executable_service);
	}

	if(_isOwnerConf)
	{
		_ownerConf_service= new SwOwnerConfigurable_Class(this);
		this->RegisterService(_ownerConf_service);
	}

	if(_isOwner)
	{
		_owner_service= new SwOwner_Class(this);
		this->RegisterService(_owner_service);
	}

	_shortcuts_service		= dynamic_cast <ISwServiceShortcuts *>(SW_APP->QueryService(CG_SW_SERVICE_SHORTCUTS));

	initializeComponent();

	if ( _doCheckTimer && timer )
	{
		QFile debugFile("log.csv");
		debugFile.open(QIODevice::Append);
		debugFile.write(QString(GetFactoryComponentName() + ";" + QString::number(timer->elapsed()) + "\n").toLatin1());
		debugFile.close();
		delete timer;
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setActive(bool active) 
{    
	if (isActive()==active)
		return;

	SwComponent_Class::setActive(active);

	eventActivationChanged();
}


//-------------------------------------------------------------------------
void SwAssistedComponent::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{    
	QElapsedTimer *timer = nullptr;
	if ( _doCheckTimer )
	{
		timer = new QElapsedTimer();
		timer->start();
	}

	if ( !_disable_service )
		eventBeforeInterfaceAvailability(interface_name, provider_host);

	if ( _doCheckTimer && timer )
	{
		QFile debugFile("log.csv");
		debugFile.open(QIODevice::Append);
		debugFile.write(QString(GetFactoryComponentName() + ";;;" + QString::number(timer->elapsed()) + "\n").toLatin1());
		debugFile.close();
		delete timer;
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host)
{    
	QElapsedTimer *timer = nullptr;
	if ( _doCheckTimer )
	{
		timer = new QElapsedTimer();
		timer->start();
	}


	if ( !_disable_service )
		eventAfterInterfaceAvailability(interface_name, provider_host);

	if ( _doCheckTimer && timer )
	{
		QFile debugFile("log.csv");
		debugFile.open(QIODevice::Append);
		debugFile.write(QString(GetFactoryComponentName() + ";;;;" + QString::number(timer->elapsed()) + "\n").toLatin1());
		debugFile.close();
		delete timer;
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::OnReceiveData(SwPin * src,SwData_Class * data)
{    
	if (!_disable_service)
		eventReceiveData(src,data);
}           

//-------------------------------------------------------------------------
void SwAssistedComponent::initializeComponent() throw(SwException) 
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host)
{
	if(_mapIConsummed.contains(interface_name))
	{
		void ** phandle = _mapIConsummed.value(interface_name);
		if( (*phandle) != NULL)
		{
			interfaceUnavailable(interface_name);
		}
	}
	if (_mapIConsummedWithCallBack.contains(interface_name))
	{
		_mapIConsummedWithCallBack[interface_name](BEFORE_POINTER_ASSIGNEMENT);
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host)
{
	if(_mapIConsummed.contains(interface_name))
	{
		void ** phandle = _mapIConsummed.value(interface_name);
		if( (*phandle) != NULL)
		{
			interfaceAvailable(interface_name);
		}
	}
	if (_mapIConsummedWithCallBack.contains(interface_name))
	{
		_mapIConsummedWithCallBack[interface_name](AFTER_POINTER_ASSIGNEMENT);
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::eventReceiveData(SwPin * src,SwData_Class * data)
{

}

//-------------------------------------------------------------------------
ISwInterfaces_Provider & SwAssistedComponent::getIProviderService() 
{    
	return *_provider_service;
}

//-------------------------------------------------------------------------
ISwInterfaces_Consumer & SwAssistedComponent::getIConsumerService() 
{    
	return *_consumer_service;
}

//-------------------------------------------------------------------------
ISwProperties & SwAssistedComponent::getPropertiesService() 
{    
	return * _properties_service;
}

//-------------------------------------------------------------------------
ISwPins_Manager & SwAssistedComponent::getPinsService() 
{    
	return * _pins_service;
}

//-------------------------------------------------------------------------
ISwServiceOwnerConfigurable & SwAssistedComponent::getOwnerConfigurableService()
{
	return * _ownerConf_service;
}

//-------------------------------------------------------------------------
ISwExecutable_Service& SwAssistedComponent::getExecutableService()
{
	return * _executable_service;
}

//-------------------------------------------------------------------------
ISwServiceOwner& StreamWork::SwFoundation::SwAssistedComponent::getOwnerService()
{
	return *_owner_service;
}

//-------------------------------------------------------------------------
ISwProperty* StreamWork::SwFoundation::SwAssistedComponent::getISwProperty( QString name )
{
	if(_isProperty)
	{
		if(_properties_service)
			return _properties_service->GetProperty(name);
	}

	return NULL;
}

//-----------------------------------------------------------------------
void SwAssistedComponent::OnRegisterService( ISwService * service )
{
	auto name = service->GetServiceName();
	if ( _mapServiceWithCallBack.contains( name ) )
		_mapServiceWithCallBack[name]( service );
}

//-----------------------------------------------------------------------
void SwAssistedComponent::OnUnregisterService( ISwService * service )
{
	auto name = service->GetServiceName();
	if ( _mapServiceWithCallBack.contains( name ) )
		_mapServiceWithCallBack[name]( service );
}

//-------------------------------------------------------------------------
void SwAssistedComponent::processCommand( QString name )
{
	QHash<QString, std::function<void()> >::iterator it = _mapShortcutWithCallBack.find(name);

	if (it != _mapShortcutWithCallBack.end())
	{
		std::function<void()> shortcutFunction = *it;
		shortcutFunction();
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::interfaceAvailable( QString interfaceName )
{
#ifdef  _DEBUG
	//qDebugAA
	qDebug() << "Interface : "  << interfaceName << " is now available";
#endif
}

//-------------------------------------------------------------------------
void SwAssistedComponent::interfaceUnavailable( QString interfaceName )
{
#ifdef  _DEBUG
	//qDebugAA
	qDebug() << "Interface : "  << interfaceName << " is now unavailable";
#endif
}

//-------------------------------------------------------------------------
void SwAssistedComponent::eventActivationChanged()
{
	if(isActive())
	{
		activation();
	}
	else
	{
		deactivation();
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setProvidedInterfaceAvaibility( QString pinterface_name, bool avaibility )
{
	if(!_isProvider)
		return;

	if(avaibility)
	{
		getIProviderService().SetInterfaceAvailable(pinterface_name);
	}
	else
	{
		getIProviderService().SetInterfaceUnavailable(pinterface_name);
	}
}

//-------------------------------------------------------------------------
void SwAssistedComponent::unprovideInterface( QString pinterface_name )
{
	if(!_isProvider)
		return;

	getIProviderService().UnregisterProvidedInterface(pinterface_name);
	_listIProvided.removeOne(pinterface_name);
}

//-------------------------------------------------------------------------
void SwAssistedComponent::unconsummeInterface( QString pinterface_name )
{
	if(!_isConsumer)
		return;

	if(_mapIConsummed.contains(pinterface_name))
	{
		getIConsumerService().UnregisterConsumedInterface(pinterface_name);

		void ** handle_interface = _mapIConsummed.value(pinterface_name);
		delete handle_interface;
		//*handle_interface = NULL;
		_mapIConsummed.remove(pinterface_name);
	}

	if(_mapIConsummedWithCallBack.contains(pinterface_name))
	{
		getIConsumerService().UnregisterConsumedInterface(pinterface_name);
		_mapIConsummedWithCallBack.remove(pinterface_name);
	}
}

//-------------------------------------------------------------------------
SwPin* SwAssistedComponent::registerPin( QString name,QString data_type,bool isListener/*=false*/ )
{
	Q_ASSERT(_isPin);

	SwPin *tmp = new SwPin(&(getPinsService()),name,data_type);

	_listPin.append(tmp);

	if(isListener)
		tmp->RegisterListener(this);

	getPinsService().RegisterPin(tmp);

	return tmp;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::unregisterPin( SwPin * pin )
{
	Q_ASSERT(_isPin);

	if(pin)
	{
		_listPin.removeOne(pin);
		pin->UnregisterListener(this);
		getPinsService().UnregisterPin(pin);
		delete pin;
		pin = NULL;
	}
}


//-------------------------------------------------------------------------
void SwAssistedComponent::activation()
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::deactivation()
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::createPropertiesForThisObject(QString prefix/*=QString()*/,bool disable_objectName/*=false*/ )
{
	if(_isProperty)
		createPropertiesForQObject(this,prefix,disable_objectName);
}

//-------------------------------------------------------------------------
void SwAssistedComponent::createPropertiesForQObject( QObject *obj,QString prefix/*=QString()*/,bool disable_objectName/*=false*/ )
{
	if(_isProperty)
		getPropertiesService().CreatePropertiesForQObject(obj,prefix,disable_objectName);
}

//-------------------------------------------------------------------------
SwComponent_Class * SwAssistedComponent::GetHostComponent()
{
	return this;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::Liberate()
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::Initialize( double start_time,ISwExecution_Service * executor ) throw (SwException)
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::Start( double current_time ) throw (SwException)
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::Execute( double current_time,bool is_first_call ) throw (SwException)
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::Stop( double current_time )
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::setExecutableServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isExecutable = val;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setConsumerServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isConsumer = val;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setProviderServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isProvider = val;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setPropertyServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isProperty = val;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setPinServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isPin = val;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setOwnerConfigurableServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isOwnerConf = val;
}

//-------------------------------------------------------------------------
void SwAssistedComponent::setOwnerServiceAvaibility( bool val )
{
	Q_ASSERT(!_isInitialized);
	_isOwner = val; 
}

//-------------------------------------------------------------------------
void SwAssistedComponent::LoadConfiguration( QDomElement &elm )
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::SaveConfiguration( QXmlStreamWriter &writer )
{

}


//-------------------------------------------------------------------------
void SwAssistedComponent::Load( QDomElement & elt,ISwFinalizerManager & finalizer_manager )
{

}

//-------------------------------------------------------------------------
void SwAssistedComponent::Save(QXmlStreamWriter& writer)
{

}

//-------------------------------------------------------------------------
quint64 SwAssistedComponent::getHistoryIndex()
{
	Q_ASSERT(_isOwner != false);
	Q_ASSERT(_owner_service != NULL);
	
	return _owner_service->getHistoryIndex();
}

