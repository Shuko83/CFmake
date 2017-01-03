/**
@file SwAssistedComponent.h
@brief Implementation avance d'un composant par defaut pour les assistés
	   Ce composant est de base Provider/Consumed d'interface & implemente 
	   l'utilisation des propriétés, pour les autres services, il faut utiliser
	   les accesseurs.
@author AAY
 */

#ifndef _STREAMWORK_SWFOUNDATION_ASSISTED_COMPONENT_H
#define _STREAMWORK_SWFOUNDATION_ASSISTED_COMPONENT_H

#include <SwComponent_Class.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwProperties.h>
#include <ISwInterfaces_ConsumerObserver.h>
#include <ISwPin_Listener.h>
#include <ISwPins_Manager.h>
#include "SwInterfaces_Provider_Class.h"
#include "SwInterfaces_Consumer_Class.h"
#include "SwProperties_Class.h"
#include "SwPins_Manager_Class.h"
#include "ISwShortcut.h"
#include "ISwServiceShortcuts.h"
#include "ISwExecutable_Service.h"
#include "ISwServiceOwnerConfigurable.h"
#include "ISwPersistentConfigurable.h"
#include "ISwPersistent.h"
#include "ISwServiceOwner.h"

#include <functional>

#ifdef SWFOUNDATION_LIB
# define BUILD_SWFOUNDATION Q_DECL_EXPORT
#else
# define BUILD_SWFOUNDATION Q_DECL_IMPORT
#endif

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

//implementation Inline
class SwExecutable_Class;
class SwOwnerConfigurable_Class;
class SwOwner_Class;

namespace StreamWork {

    namespace SwFoundation {

		enum INTERFACE_EVENT {
			AFTER_INTERFACE_AVAILABLE,
			BEFORE_INTERFACE_UNAVAILABLE
		};

        /**
        @class SwAssistedComponent
        @brief Implementation avance d'un composant par defaut pour les assistés
        @ingroup SwCoreGrp
       */
		class BUILD_SWFOUNDATION SwAssistedComponent :
			public SwComponent_Class,
			virtual public ISwInterfaces_ConsumerObserver,
			virtual public ISwPin_Listener,
			virtual private ISwShortcut,
			virtual public ISwPersistent,
			virtual public ISwPersistentConfigurable,
			virtual public ISwServicesManager_Listener
		{
            Q_OBJECT
			Q_PROPERTY(bool SwAssistedComponent_isActive READ isActive WRITE setActive)
        public:

            /**
             * @brief    : Constructor
             */
            SwAssistedComponent();

            /**
             * @brief    : Destructor
             */
            virtual ~SwAssistedComponent();


			/**
			 * Activation des services 
			 */
			void setExecutableServiceAvaibility(bool val);							//_isExecutable = false;
			void setConsumerServiceAvaibility(bool val);							//_isConsumer = true;
			void setProviderServiceAvaibility(bool val);							//_isProvider = true;
			void setPropertyServiceAvaibility(bool val);							//_isProperty = true;
			void setPinServiceAvaibility(bool val);									//_isPin = false;
			void setOwnerConfigurableServiceAvaibility(bool val);					//_isOwnerConf = false;
			void setOwnerServiceAvaibility(bool val);								//_isOwner = false;

			/**
			 * @brief    : Permet de récuperer un pointerur sur une ISwProperty
			 * @return   : ISwProperty* - pointeur sur la propriété Stremwork
			 * @param	 : QString name - Nom de la propriété
			 */
			ISwProperty* getISwProperty(QString name);


            /**
             * @brief    : Initialisation du composant
             * @note	 : A surcharger
             */
            virtual void initializeComponent() throw(SwException);

            /**
             * @brief    : surcharge du setter pour signaler le changement d'activation
             * @param	 : bool active 
             */
            virtual void setActive(bool active);

			/**
			 * @brief    : Callback appelée lors de l'activation du composant
			 */
			virtual void activation();
			
			/**
			 * @brief    : Callback appelée lors de la désactivation du composant
			 */
			virtual void deactivation();

			//----------------------------------------------------
            // Interface ISwPin_Listener
            //----------------------------------------------------
	                   
            /**
             * @brief    : Calback sur reception d'une data
             * @param	 : SwPin * src - Pointeur sur la pin
             * @param	 : SwData_Class * data - Pointeur sur la data ? (a valider)
			 * @warning	 : Si vous gardez une reference sur la donnée reçues au dela de la portée de la methode suivante
			 *			   utiliser un SwRefPtr sur la donnée
			 *			   Si vous souhaitez modifier une donnée recue il faut d'abord en faire une copy
             */
            virtual void eventReceiveData(SwPin * src,SwData_Class * data);
			
			//----------------------------------------------------
			// Interface ISwPersistent
			//----------------------------------------------------

			/**
			* @brief    : Methode permettant de charger des donnees
			* @param	 : QDomElement & elt - Noeud parent
			* @param	 : ISwFinalizerManager & finalizer_manager - Manager de finalisation
			*/
			virtual void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);

			/**
			* @brief    : methode permettant de sauver des donnees
			* @param	 : QDomElement & elt - Noeud parent
			* @param	 : QDomDocument & doc - Document parent
			*/
			virtual void Save(QDomElement & elt,QDomDocument &doc);


			//------------------------------------------------------------------
			// Utilisation des shortcuts (ISwShortcut)
			//------------------------------------------------------------------		

			/**
			* @brief    : Callback d'appel avec en parametre le nom associé a la commande
			* @param	 : QString name - nom de la commande appelée
			*/
			template<typename T> inline void registerShortcut(QString shortcutCategory, QString shortcutName,  void (T::*shortcutCallback)())
			{
				registerInternalShortcut(shortcutCategory, shortcutName, [=](){(static_cast<T*>(this)->*shortcutCallback)(); });
			}

			/**
			* @brief    : Callback d'appel avec en parametre le nom associé a la commande
			* @param	 : QString name - nom de la commande appelée
			*/
			template<typename T, typename MEMBER> inline void registerShortcut(QString shortcutCategory, QString shortcutName, T* ptr, MEMBER shortcutCallback)
			{
				registerInternalShortcut(shortcutCategory, shortcutName, [=](){(ptr->*shortcutCallback)(); });
			}

			//----------------------------------------------------
			// Interface ISwExecutable_Service
			//----------------------------------------------------
			
			/**
			 * @brief    : Initialisation du composant executable
			 * @param	 : double start_time - le temps de début
			 * @param	 : ISwExecution_Service * executor - Pointeur sur le service d'exécution
			 */
			virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);   

			/**
			 * @brief    : Démarage (Premier pas d'execution)
			 * @param	 : double current_time - Temps de début
			 */
			virtual void Start(double current_time) throw (SwException);            

			/**
			 * @brief    : Boucle d'éxecution
			 * @param	 : double current_time - Temps d'éxecution
			 * @param	 : bool is_first_call - Si c'est le premier appel
			 */
			virtual void Execute(double current_time,bool is_first_call) throw (SwException);  

			/**
			 * @brief    : Methode appelé au stop
			 * @param	 : double current_time - Temps d'éxecution
			 */
			virtual void Stop(double current_time);       

			//---------------------------------------------------------------------
			// Interface ISwPersistentConfigurable
			//---------------------------------------------------------------------            

			/**
			 * @brief    : methode permettant de charger des donnees de configuration
			 * @param	 : QDomElement & -  Noeud parent
			 */
			virtual void LoadConfiguration(QDomElement &elm);

			/**
			 * @brief    : methode permettant de sauver des donnees de configuration
			 * @param	 : QDomElement & - Noeud parent
			 * @param	 : QDomDocument & - Document parent
			 */
			virtual void SaveConfiguration(QDomElement &elm,QDomDocument &doc);

			//---------------------------------------------------------------------
			// Interface ISwService
			//---------------------------------------------------------------------            
			
			/**
			 * @brief    : Est appele uniquement par le service manager aupres duquel le service est enregistré
			 *	           lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée
			 */
			virtual void Liberate();  

			//---------------------------------------------------------------------
			// Interface ISwHost
			//---------------------------------------------------------------------

			/**
			 * @brief    : acces a son composant hote
			 * @return   : SwComponent_Class * - pointeur sur le composant host
			 */
			virtual SwComponent_Class * GetHostComponent();        

			//------------------------------------------------------------------
			// Template pour la gestion des interfaces
			//------------------------------------------------------------------

			/**
			@brief Methode simplifiant la consomation d'interface

			@details
			- set automatique du pointeur
			- fini les appels à getInterface avec le nom de l'interface
			- Pratique quand aucun traitmenent specifique a la dispo/indispo de l'interface

			exemple :
			.h :
			ISwWidget * _i_widget;
			constructor :
			_i_widget = 0;
			initializeComponent :
			consumeInterface("ISwWidget", &_i_widget);

			@param interfaceName : QString  => nom de l'interface (utilisé pour le unconsume)
			@param interfaceHandle : T * *  => pointeur sur le pointeur d'interface
			*/
			template<typename T> inline void consumeInterface(QString interfaceName, T ** interfaceHandle)
			{
				registerInterfaceCallback(interfaceName, interfaceHandle, [=](CALLBACK_EVENTS eventType)->void {});
			}

			/**
			@brief Methode simplifiant la consomation d'interface

			@details
					- A chaque interface sa methode de disponibilité
					- set automatique du pointeur
					- notifie avant de changer le pointeur et aprés avoir changé le pointeur
					- fini les appels à getInterface avec le nom de l'interface

				exemple : 
				.h : 
					ISwWidget * _i_widget;
				constructor : 
					_i_widget = 0;
				initializeComponent :
					consumeInterface("ISwWidget", &_i_widget, [this](INTERFACE_EVENT eventType)->void { this->onWidgetChange(eventType); });
				onWidgetChange(INTERFACE_EVENT event) : // Specifique
					if(event == BEFORE_INTERFACE_UNAVAILABLE)
						//do something on old value of _i_widget ( unregister from listener or anything else...)
					if(event == AFTER_INTERFACE_AVAILABLE)
						//do something with new value of _i_widget

			@param interfaceName : QString  => nom de l'interface (utilisé pour le unconsume)
			@param interfaceHandle : T * *  => pointeur sur le pointeur d'interface
			@param callback : >  => methode à appeler lors des évennements de disponibilité d'interface (à utilisé de préférence avec une lambda expression)
			*/
			template<typename T> inline void consumeInterface(QString interfaceName, T ** interfaceHandle, std::function<void(INTERFACE_EVENT)> callback)
			{
				registerInterfaceCallback(interfaceName, interfaceHandle, [=](CALLBACK_EVENTS eventType)->void {
					if (*interfaceHandle)
					{
						if (eventType == AFTER_POINTER_ASSIGNEMENT)
						{
							callback(AFTER_INTERFACE_AVAILABLE);
						}
						else if (eventType == BEFORE_POINTER_ASSIGNEMENT)
						{
							callback(BEFORE_INTERFACE_UNAVAILABLE);
						}
					}
				});
			}

			/**
			@brief Methode simplifiant la consomation d'interface
				- A chaque interface sa methode de disponibilité
				- set automatique du pointeur
				- notifie avant de changer le pointeur et aprés avoir changé le pointeur
				- fini les appels à getInterface avec le nom de l'interface

			@details
			
			.h :
				ISwWidget * _i_widget;
			
			constructor :
				_i_widget = 0;
			
			initializeComponent :
				consumeInterface("ISwWidget", &_i_widget, this, &MaClass::onWidgetChange);
			
			onWidgetChange(INTERFACE_EVENT event) : // Specifique
			if(event == BEFORE_INTERFACE_UNAVAILABLE)
				//do something on old value of _i_widget ( unregister from listener or anything else...)
			if(event == AFTER_INTERFACE_AVAILABLE)
				//do something with new value of _i_widget

			@param interfaceName : QString  => nom de l'interface (utilisé pour le unconsume)
			@param interfaceHandle : T * *  => pointeur sur le pointeur d'interface
			@param thisPointer : U *  => pointeur sur la classe ayant la callback en membre
			@param callback : void (U::*callback)(INTERFACE_EVENT) => pointeur sur la callback en tant que fonction membre
			*/
			template<typename T, typename U> inline void consumeInterface(QString interfaceName, T ** interfaceHandle, U* thisPointer, void (U::*callback)(INTERFACE_EVENT))
			{
				registerInterfaceCallback(interfaceName, interfaceHandle, [=](CALLBACK_EVENTS eventType)->void {
					if (*interfaceHandle)
					{
						if (eventType == AFTER_POINTER_ASSIGNEMENT)
						{
							(thisPointer->*callback)(AFTER_INTERFACE_AVAILABLE);
						}
						else if (eventType == BEFORE_POINTER_ASSIGNEMENT)
						{
							(thisPointer->*callback)(BEFORE_INTERFACE_UNAVAILABLE);
						}
					}
				});
			}

			/**
			@brief Methode simplifiant la consomation d'interface
				- A chaque interface sa methode de disponibilité
				- notifie aprés avoir changé le pointeur
				- fini les appels à getInterface avec le nom de l'interface

			@details
			
			.h :
				ISwWidget * _i_widget;
			
			constructor :
				_i_widget = 0;
			
			initializeComponent :
				consumeInterface("ISwWidget", &_i_widget, this, &MaClass::onWidgetChange);
			
			onWidgetChange() : // Specifique
				//do something with new value of _i_widget

			@param interfaceName : QString  => nom de l'interface (utilisé pour le unconsume)
			@param interfaceHandle : T * *  => pointeur sur le pointeur d'interface
			@param thisPointer : U *  => pointeur sur la classe ayant la callback en membre
			@param callback : void (U::*callback)() => pointeur sur la callback en tant que fonction membre
			*/
			template<typename T, typename U, typename MEMBER> inline void consumeInterface(QString interfaceName, T ** interfaceHandle, U* thisPointer, MEMBER func)
			{
				//passage de this pointer en = au contexte de la lambda expression pour copier l'adresse du pointeur dans la lambda expression car en sortie de la methode courante, la reference sur le pointeur n'est plus valide.
				registerInterfaceCallback(interfaceName, interfaceHandle, [=](CALLBACK_EVENTS eventType)->void {
					if(eventType==AFTER_POINTER_ASSIGNEMENT) 
					{
						(thisPointer->*func)();
					}
				});
			}
			
			/**
			 * @brief    : fourni une interface (sortie)
			 * @param	 : QString pinterface_name - Nom de l'interface
			 * @param	 : T * handle_interface - Pointeur sur l'interface fournite
			 */
			template<typename T> inline void provideInterface(QString pinterface_name,T * handle_interface) 
			{
				if(!_isProvider)
					return;

				getIProviderService().RegisterProvidedInterface<T>(pinterface_name,(T *)handle_interface);
				_listIProvided.append(pinterface_name);
			}

			/**
			 * @brief    : Permet de consommer une interface
			 * @param	 : QString pinterface_name - Nom de l'interface
			 */
			template<typename T> void consummeInterface(QString pinterface_name) 
			{
				if(!_isConsumer)
					return;

				void ** handle_interface = new void *;
				*handle_interface = NULL;
				getIConsumerService().RegisterConsumedInterface<T>(pinterface_name,(T**)handle_interface);
				_mapIConsummed.insert(pinterface_name,handle_interface);
			}

			/**
			 * @brief    : Permet d'obtenir un pointeur sur l'interface consommée
			 * @return   : T* - Pointeur de type <T> sur l'interface consommée
			 * @param	 : QString pinterface_name - Nom de l'interface
			 */
			template<typename T> inline T* getInterface(QString pinterface_name)
			{
				if(!_isConsumer)
					return NULL;

				if(_mapIConsummed.contains(pinterface_name))
				{
					void ** handle_interface = _mapIConsummed.value(pinterface_name);
					return (T*)(*handle_interface) ;
				}
				else
					return NULL;
			}

			/**
			 * @brief    : Permet de supprimer une interface consommée
			 * @param	 : QString pinterface_name - Nom de l'interface
			 */
			virtual void unconsummeInterface(QString pinterface_name);

			/**
			 * @brief    : Supprime la production d'interface
			 * @param	 : QString pinterface_name - Nom de l'interface
			 */
			virtual void unprovideInterface(QString pinterface_name);

			/**
			 * @brief    : Permet de définir la disponibilité de l'interface d'une interface produite
			 * @param	 : QString pinterface_name - Nom de l'interface
			 * @param	 : bool avaibility - True si interface disponible , False sinon
			 */
			virtual void setProvidedInterfaceAvaibility(QString pinterface_name, bool avaibility);

			/********************
			 * Gestion des Pins
			 ********************/

			/**
			 * @brief    : Enregistre un nouveau pin et retourne le pointeur sur le pin
			 * @return   : SwPin* - pointeur sur le pin
			 * @param	 : QString name - Nom du pin
			 * @param	 : QString data_type - Type de donnée
			 * @param	 : bool isListener - True si on s'enregistre en listener
			 */
			virtual SwPin* registerPin(QString name,QString data_type,bool isListener=false);
			
			/**
			 * @brief    : Désenregistre un pin déjà enregistré
			 * @param	 : SwPin * pin - Pointeur sur le pin 
			 */
			virtual void unregisterPin(SwPin * pin);
			
			/********************
			 * Gestion des Propriétés
			 ********************/

			/**
			 * @brief    : Exporte la liste des propriété de l'object courrant
			 * @param	 : QString prefix - Prefix à ajouter dans l'arborescence
			 * @param	 : bool disable_objectName - Permet de désactiver l'objectName comme propriété
			 */
			virtual void createPropertiesForThisObject(QString prefix=QString(),bool disable_objectName=false);

			/**
			 * @brief    : Exporte la liste des propriétés pour un object QT
			 * @param	 : QObject * obj - Pointeur sur l'object
			 * @param	 : QString prefix - Prefix à ajouter dans l'arborescence
			 * @param	 : bool disable_objectName - Permet de désactiver l'objectName comme propriété
			 */
			virtual void createPropertiesForQObject(QObject *obj,QString prefix=QString(),bool disable_objectName=false);
			
			/**
			 * @brief    : Callback appelée lors de la disponibilité de l'interface
			 * @param	 : QString interfaceName - Nom de l'interface
			 */
			virtual void interfaceAvailable(QString interfaceName);

			/**
			 * @brief    : Callback appelée lors de l'indisponibilité de l'interface
			 * @param	 : QString interfaceName - Nom de l'interface
			 */
			virtual void interfaceUnavailable(QString interfaceName);

			
			template<typename U, typename T = ISwService> void RegisterToService( QString name, U* thisPointer, void (U::*callback)(T*) )
			{
				if ( !_allreadyListenerOfService )
				{
					_allreadyListenerOfService = true;
					SW_APP->AddServicesManagerObserver( this );
				}


				_mapServiceWithCallBack.insert( name, [=]( ISwService* service )->void {

					auto castS = dynamic_cast<T*>(service);
					if ( castS )
						(thisPointer->*callback)(castS);
				} );

				StreamWork::SwCore::ISwService * service = SW_APP->QueryService( name );
				if ( service )
					_mapServiceWithCallBack[name]( service );
			}

	
protected:
			/**
             * @brief    : Initialisation des ressources
             * @note	 : Tous les services du composants doivent être déclarés dans cette methodes
             */
            virtual void InitializeResources() throw(SwException);


			//------------------------------------------------------------------
			// Accès aux différents services (Interne)
			//------------------------------------------------------------------

			/**
			 * @brief    : Acces au service fournisseur d'interface
			 * @return   : ISwInterfaces_Provider & - Référence sur le service
			 */
			ISwInterfaces_Provider & getIProviderService();

			/**
			 * @brief    : Acces au service consommateur d'interface
			 * @return   : ISwInterfaces_Consumer & - Référence sur le service
			 */
			ISwInterfaces_Consumer & getIConsumerService();

			/**
			 * @brief    : Acces au service de properties
			 * @return   : ISwProperties & - Référence sur le service
			 */
			ISwProperties & getPropertiesService();

			/**
			 * @brief    : Acces au service de connexion
			 * @return   : ISwPins_Manager & - Référence sur le service
			 */
			ISwPins_Manager & getPinsService();

			/**
			 * @brief    :  Acces au service d'éxécution
			 * @return   : ISwExecutable_Service& Référence sur le service
			 */
			ISwExecutable_Service& getExecutableService();

			/**
			 * @brief    : Acces au service de configuration
			 * @return   : ISwServiceOwnerConfigurable & - Référence sur le service
			 */
			ISwServiceOwnerConfigurable & getOwnerConfigurableService();
			
			/**
			 * @brief    : Acces au service de owner (sauvegarde dans le stream Streamwork)
			 * @return   : SwOwner_Class & - Référence sur le service
			 */
			ISwServiceOwner& getOwnerService();

			/**
			 * @brief    : Retourne le compteur d'historique enregistrer par "l'interface ISwPersitent"
			 *			   /!\ Le Composant doit etre Owner (setOwnerServiceAvaibility)
			 * @return   : HistoryIndex à utiliser dans le finalizer
			 */
			quint64 getHistoryIndex();


			//---------------------------------------------------------------------
			// Interface ISwPersistentConfigurable
			//---------------------------------------------------------------------         

			/**
			* @brief	: Quand un service est ajouté
			*
			* @param	: ISwService * service - pointeur sur le service
			*/
			void OnRegisterService(ISwService * service);
			/**
			* @brief	: Quand un service est supprimé
			*
			* @param	: ISwService * service - Pointeur sur le service
			*/
			void OnUnregisterService(ISwService * service);

	
private:

            /**
             * @brief    : evenement sur changement d'activation
             */
            virtual void eventActivationChanged();

			//---------------------------------------------------
			//	   Interface ISwInterfaces_ConsumerObserver
			//---------------------------------------------------

			/**
			 * @brief    : Callback avant changement de la disponibilité de l'interface 
			 * @param	 : QString interface_name - le nom de l'interface
			 * @param	 : SwComponent_Class * provider_host - pointeur sur un composant_class
			 */
			void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
			
			/**
			 * @brief    : Callback apres changement de la disponibilité de l'interface
			 * @param	 : QString interface_name - le nom de l'interface
			 * @param	 : SwComponent_Class * provider_host - pointeur sur un composant_class
			 */
			void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            

			 /**
             * @brief    : Callback avant changement de la disponibilité de l'interface si le service est actif
             * @param	 : QString interface_name - Nom de l'interface
             * @param	 : SwComponent_Class * provider_host - Je sais pas
             * @note	 : Interne
             */
            void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
            /**
			 * @brief    : Callback apres changement de la disponibilité de l'interface si le service est actif
             * @param	 : QString interface_name - Nom de l'interface
             * @param	 : SwComponent_Class * provider_host - Je sais pas
             * @note	 : Interne
             */
            void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);

			/**
	         * @brief    : Callback sur reception d'une data
	         * @param	 : SwPin * src - Pointeur sur le pin
	         * @param	 : SwData_Class * data - pointeur sur les data
	         * @note	 : note a surcharger pour receptionner les data
	         */
			void OnReceiveData(SwPin * src,SwData_Class * data); 

			/**
             * @brief    : Active l'observabilité d'une propriété notifié par "eventPropertyChange"
             * @param	 : ISwProperty * property - Pointeur sur une propriété
             */
            //void enableListeningChangeForProperty(ISwProperty * property);			


 			//------------------------------------------------------------------
			// Interface ISwShortcut
			//------------------------------------------------------------------

			/**
			 * @brief    : Callback d'appel avec en parametre le nom associé a la commande
			 * @param	 : QString name - nom de la commande appelée
			 */
		    void processCommand(QString name) final;
			         
			//------------------------------------------------------------------
			// Template pour la gestion des interfaces
			//------------------------------------------------------------------

			enum CALLBACK_EVENTS
			{
				BEFORE_POINTER_ASSIGNEMENT,
				AFTER_POINTER_ASSIGNEMENT
			};

			/**
			@param interfaceName : QString  => nom de l'interface (utilisé pour le unconsume)
			@param interfaceHandle : T * *  => pointeur sur le pointeur d'interface
			@param callback : >  => methode à appeler lors des évennements de disponibilité d'interface (à utilisé de préférence avec une lambda expression)
			*/
			template<typename T> inline void registerInterfaceCallback(QString interfaceName, T ** interfaceHandle, std::function<void(CALLBACK_EVENTS)> callback)
			{
				getIConsumerService().RegisterConsumedInterface<T>(interfaceName, interfaceHandle);
				_mapIConsummedWithCallBack.insert(interfaceName, callback);
			}


			inline void registerInternalShortcut(QString shortcutCategory, QString shortcutName,  std::function<void()> shortcutCallback)
			{
				ISwServiceShortcuts* serviceShortcuts = dynamic_cast <ISwServiceShortcuts *>(SW_APP->QueryService(CG_SW_SERVICE_SHORTCUTS));
				if (serviceShortcuts){
					serviceShortcuts->registerCommand(shortcutCategory, shortcutName, this);

					_mapShortcutNameWithCategory.insert(shortcutName, shortcutCategory);
					_mapShortcutWithCallBack.insert(shortcutName, shortcutCallback);
				}
				else
					qCritical() << "Unable to register shortcut, because the service is not available -> Faire le TODO";

				//TODO : S'abonner à la notif de dispo du service && si service pas dispo -> On enregistre en temporaire
				// Quand service dispo on registerCommand sur les temporaire
			}

			/**
			 * Gestion des services
			 */

            /* service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;

            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;

            /* service de gestion des propriétés */
            SwProperties_Class * _properties_service;

            /* service de gestion des pins */
            SwPins_Manager_Class * _pins_service;

			/* Service de gestion des raccourcis*/
			ISwServiceShortcuts * _shortcuts_service;

			/* Service de gestion de l'execution*/
			SwExecutable_Class * _executable_service;

			/* Service de gestion de la conf user*/
			SwOwnerConfigurable_Class * _ownerConf_service;

			/* Service de gestion de la conf user*/
			SwOwner_Class * _owner_service;

            /* desactivation des services */
            bool _disable_service;

			/* Liste des interfaces produites */
			QList<QString> _listIProvided;

			/* Map des interfaces consommées */
			QMap<QString,void **> _mapIConsummed;

			/* hash des interfaces consommées vers les methode de disponibilité*/
			QHash<QString, std::function<void( CALLBACK_EVENTS )>> _mapIConsummedWithCallBack;

			QHash<QString, std::function<void(ISwService*)>> _mapServiceWithCallBack;
			bool _allreadyListenerOfService;

			QHash<QString, std::function<void()>> _mapShortcutWithCallBack;
			QHash<QString, QString> _mapShortcutNameWithCategory;
			

			/* Liste des pin enregistrée*/
			QList<SwPin*> _listPin;


			/**
			 *	Booléen d'activation de service
			 */

			bool _isExecutable;
			bool _isConsumer;
			bool _isProvider;
			bool _isProperty;
			bool _isPin;
			bool _isOwnerConf;
			bool _isOwner;
			bool _isInitialized;
			quint64				  _historyIndex;

			//Enable log of time
			bool _doCheckTimer;

        };
    }
}

#endif
