/*!
	\file {{COMPONENT_NAME}}.h
	\brief {{DESCRIPTION}}
	\version 1.0
	\author {{AUTHOR}}
	\date {{DATE}}
*/

#ifndef _{{NAMESPACE_UNDERSCORE_UPPER}}_{{COMPONENT_NAME}}_H
#define _{{NAMESPACE_UNDERSCORE_UPPER}}_{{COMPONENT_NAME}}_H

/*
* INCLUDES GLOBAUX
*/

/*
* INCLUDES LOCAUX
*/
#include "BaseModelConsumerAssisted.h"
{{#SECTION_ISWPIN_OUTPUT}}#include <tdlcommon\interfaces\IBaseMessage.h>{{/SECTION_ISWPIN_OUTPUT}}
{{#SECTION_INCLUDE}}#include "{{INCLUDE_FILE}}"
{{/SECTION_INCLUDE}}

namespace {{NAMESPACE_1}}
{
	namespace {{NAMESPACE_2}}
	{
		namespace {{NAMESPACE_3}}
		{	

			/**
			*	@class {{COMPONENT_NAME}} 
			*	@brief {{COMPONENT_DESC}}
			*/               
			class {{COMPONENT_NAME}} : virtual public BaseModelConsumerAssisted
			{
				Q_OBJECT
				{{#FOR_COMPONENT_PROPERTIES}}{{CONTENT}}{{/FOR_COMPONENT_PROPERTIES}}

			public:

				/*! \brief Constructeur */
				{{COMPONENT_NAME}}();

				/*! \brief Destructeur */
				virtual ~{{COMPONENT_NAME}}();

				/**
				* @brief : Initialisation du composant
				*/
				virtual void initializeBaseModelConsumer() throw(SwException);

				{{#SECTION_ISWSHORTCUT}}
				//------------------------------------------------------------------
				// Interface ISwShortcut
				//------------------------------------------------------------------

				 /**
				 * @brief    : Callback d'appel avec en parametre le nom associé a la commande
				 * @param	 : QString name - nom de la commande appelée
				 */
				virtual void processCommand(QString name);

				{{/SECTION_ISWSHORTCUT}}{{#SECTION_ISWPERSISTENT}}
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

				{{/SECTION_ISWPERSISTENT}}{{#SECTION_ISCONSUMED}}
				//----------------------------------------------------
				// Interface de consomation d'interfaces
				//----------------------------------------------------

				/**
				* @brief    : Callback appelée lors de la disponibilité de l'interface
				* @param	 : QString interfaceName - Nom de l'interface
				*/
				virtual void interfaceIsAvailable(QString interface_name);

				/**
				* @brief    : Callback appelée lors de l'indisponibilité de l'interface
				* @param	 : QString interfaceName - Nom de l'interface
				*/
				virtual void interfaceIsUnavailable(QString interface_name);

				{{/SECTION_ISCONSUMED}}{{#SECTION_ISWEXECUTABLE_SERVICE}}
				//----------------------------------------------------
				// Interface ISwExecutable_Service
				//----------------------------------------------------

				/**
				* @brief     Initialisation du composant executable
				* @param	 double start_time - le temps de début
				* @param	 ISwExecution_Service * executor - Pointeur sur le service d'exécution
				*/
				virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);   

				/**
				* @brief     Démarage (Premier pas d'execution)
				* @param	 double current_time - Temps de début
				*/
				virtual void Start(double current_time) throw (SwException);            

				/**
				* @brief     Boucle d'éxecution
				* @param	 double current_time - Temps d'éxecution
				* @param	 bool is_first_call - Si c'est le premier appel
				*/
				virtual void Execute(double current_time,bool is_first_call) throw (SwException);  

				/**
				* @brief    Methode appelé au stop
				* @param	double current_time - 
				*/
				virtual void Stop(double current_time);                  
				
				{{/SECTION_ISWEXECUTABLE_SERVICE}}{{#SECTION_ISWPIN_LISTENER}}
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

				{{/SECTION_ISWPIN_LISTENER}}{{#SECTION_ISWPIN_OUTPUT}}
				/**
				 * @brief    : Emission de message par default
				 * @param	 : tdlcommon::interfaces::IBaseMessage * baseMessage - Message de base
				 * @param	 : double time - time
				 */
				void sendMessage(tdlcommon::interfaces::IBaseMessage * baseMessage, double time);

				{{/SECTION_ISWPIN_OUTPUT}}{{#SECTION_ISWPERSISTENTCONFIGURABLE}}
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

				{{/SECTION_ISWPERSISTENTCONFIGURABLE}}//----------------------------------------------------
				// Accesseur 
				//----------------------------------------------------
				{{#SECTION_ACCESSOR}}{{CONTENT}}
				{{/SECTION_ACCESSOR}}
				
			protected:
			
				{{#SECTION_HAS_PIN}}//---------------------------------------------------
				//				Declaration des pins
				{{#SECTION_ISWPIN_LISTENER}}SwPin * _pinIn;{{/SECTION_ISWPIN_LISTENER}}
				{{#SECTION_ISWPIN_OUTPUT}}SwPin * _pinOut;{{/SECTION_ISWPIN_OUTPUT}}{{/SECTION_HAS_PIN}}
				
				{{#SECTION_HAS_INTERFACES}}//---------------------------------------------------
				//				Declaration des interfaces
				{{#SECTION_INTERFACECONSUMED}}{{INTERFACE_TYPE}}*	{{CONSUMED_VAR_NAME}};
				{{/SECTION_INTERFACECONSUMED}}
				{{#SECTION_INTERFACEPRODUCTED}}{{INTERFACE_TYPE}}*	{{PRODUCTED_VAR_NAME}};
				{{/SECTION_INTERFACEPRODUCTED}}{{/SECTION_HAS_INTERFACES}}
				{{#SECTION_HAS_PROPERTIES}}//---------------------------------------------------
				//				Declaration des proprietes
				{{#FOR_COMPONENT_PROPERTIES}}{{DECLARE}}
				{{/FOR_COMPONENT_PROPERTIES}}{{/SECTION_HAS_PROPERTIES}}

				

			};
		}
	}
}
#endif 
