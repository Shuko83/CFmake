/**
@file Component.h
@brief Implementation avance d'un composant par defaut
@author Big
 */

#ifndef _STREAMWORK_SWCORE_COMPONENT_H
#define _STREAMWORK_SWCORE_COMPONENT_H

#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwProperties.h>
#include <ISwInterfaces_ConsumerObserver.h>
#include <ISwPin_Listener.h>
#include <ISwPins_Manager.h>

namespace StreamWork {

    namespace SwCore {
    
        class SwInterfaces_Provider_Class;
        class SwInterfaces_Consumer_Class;
        class SwProperties_Class;
        class SwPins_Manager_Class;
        /**
        @class Component
        @brief Implementation avance d'un composant par defaut
        */
        class Component: public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwPin_Listener {
            Q_OBJECT
        public:
            /** @brief Constructor */
            Component();
            /** @brief Destructor */
            virtual ~Component();

            /*! \brief Initialisation des ressources
            \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw(SwException);


             /*! \brief Callback sur les changements de propriétés */
            void OnPropertyChange(ISwProperty * property);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_ConsumerObserver
            //---------------------------------------------------------------------
	        /*! \brief Callback avant changement de la disponibilité de l'interface */
            void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
	        /*! \brief Callback apres changement de la disponibilité de l'interface */
	        void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
            //----------------------------------------------------
            // Interface ISwPin_Listener
            //----------------------------------------------------
            /*! \brief Callback sur reception d'une data
                \note a surcharger pour receptionner les data*/
	        void OnReceiveData(SwPin * src,SwData_Class * data);            

        protected:
            /*! \brief Initialisation du composant
                \note A Surcharger*/
            virtual void initializeComponent() throw(SwException);
            /*! \brief terminaison du composante
                \note A Surcharger*/
            virtual void terminateComponent();
            /*! \brief evenement de changement de propriete
                \note A Surcharger*/
            virtual void eventPropertyChange(ISwProperty * property);
            /*! \brief evenement avant changement de la disponibilité de l'interface
                \note A Surcharger*/
            virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
            /*! \brief evenement apres changement de la disponibilité de l'interface
                \note A Surcharger*/
            virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
            /*! \brief evenement sur reception d'une data
                \note A Surcharger*/
            virtual void eventReceiveData(SwPin * src,SwData_Class * data);

            /*! \brief Acces au service fournisseur d'interface*/
            ISwInterfaces_Provider & getIProviderService();
            /*! \brief Acces au service consommateur d'interface*/
            ISwInterfaces_Consumer & getIConsumerService();
            /*! \brief Acces au service de properties*/
            ISwProperties & getPropertiesService();
            /*! \brief Acces au service de connexion */
            ISwPins_Manager & getPinsService();
        private:
            /* service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;
            /* service de gestion des propriétés */
            SwProperties_Class * _properties_service;
            /* service de gestion des pins */
            SwPins_Manager_Class * _pins_service;
            /* desactivation des services */
            bool _disable_service;
        
        };
    
    }

}

#endif
