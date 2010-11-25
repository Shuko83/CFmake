/**
@file Component.h
@brief Implementation avance d'un composant par defaut
@author Big
 */

#ifndef _STREAMWORK_SWEXECUTION_EXECUTABLECOMPONENT_H
#define _STREAMWORK_SWEXECUTION_EXECUTABLECOMPONENT_H

#include <SwComponent_Class.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwProperties.h>
#include <ISwInterfaces_ConsumerObserver.h>
#include <ISwPin_Listener.h>
#include <ISwPins_Manager.h>
#include "SwExecutionConstantes.h"
#include "ISwExecutable_Service.h"

namespace StreamWork {

    namespace SwCore {

        class SwInterfaces_Provider_Class;
        class SwInterfaces_Consumer_Class;
        class SwProperties_Class;
        class SwPins_Manager_Class;
        class SwPin;

    }
}

namespace StreamWork {

    namespace SwExecution {
    
        /**
        @class ExecutableComponent
        @brief Implementation avance d'un composant executable par defaut
        @ingroup SwExeGrp
        */
        class SWEXECUTION_EXPORT ExecutableComponent: 
            public StreamWork::SwCore::SwComponent_Class, 
            public StreamWork::SwCore::ISwInterfaces_ConsumerObserver, 
            public StreamWork::SwCore::ISwPin_Listener, 
            virtual public ISwExecutable_Service {

            Q_OBJECT
        public:
            /** @brief Constructor */
            ExecutableComponent();
            /** @brief Destructor */
            virtual ~ExecutableComponent();

            /*! \brief Initialisation des ressources
            \note tous les services du composants doivent  tre d clar s dans cette methodes*/
            virtual void InitializeResources() throw(StreamWork::SwCore::SwException);

            /*! \brief surcharge du setter pour signaler le changement d'activation */
            virtual void setActive(bool active);


             /*! \brief Callback sur les changements de propri t s */
            void OnPropertyChange( StreamWork::SwCore::ISwProperty * property);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_ConsumerObserver
            //---------------------------------------------------------------------
	        /*! \brief Callback avant changement de la disponibilit  de l'interface */
            void BeforeInterfaceAvailabilityChange(QString interface_name,StreamWork::SwCore::SwComponent_Class * provider_host);
	        /*! \brief Callback apres changement de la disponibilit  de l'interface */
	        void AfterInterfaceAvailabilityChange(QString interface_name,StreamWork::SwCore::SwComponent_Class * provider_host);            
            //----------------------------------------------------
            // Interface ISwPin_Listener
            //----------------------------------------------------
            /*! \brief Callback sur reception d'une data
                \note a surcharger pour receptionner les data*/
	        void OnReceiveData(StreamWork::SwCore::SwPin * src,StreamWork::SwCore::SwData_Class * data);            
            //----------------------------------------------------
            // Interface ISwExecutable_Service
            //----------------------------------------------------
	        /*! \brief Initialisation */
	        virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (StreamWork::SwCore::SwException);            
	        /*! \brief Demarrage */
	        virtual void Start(double current_time) throw (StreamWork::SwCore::SwException);            
	        /*! \brief Execution */
	        virtual void Execute(double current_time,bool is_first_call) throw (StreamWork::SwCore::SwException);            
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
            StreamWork::SwCore::SwComponent_Class * GetHostComponent();            

        protected:
            /*! \brief Initialisation du composant
                \note A Surcharger*/
            virtual void initializeComponent() throw(StreamWork::SwCore::SwException);
            /*! \brief evenement de changement de propriete
                \note A Surcharger*/
            virtual void eventPropertyChange(StreamWork::SwCore::ISwProperty * property);
            /*! \brief evenement avant changement de la disponibilit  de l'interface
                \note A Surcharger*/
            virtual void eventBeforeInterfaceAvailability(QString interface_name,StreamWork::SwCore::SwComponent_Class * provider_host);
            /*! \brief evenement apres changement de la disponibilit  de l'interface
                \note A Surcharger*/
            virtual void eventAfterInterfaceAvailability(QString interface_name,StreamWork::SwCore::SwComponent_Class * provider_host);
            /*! \brief evenement sur reception d'une data
                \note A Surcharger*/
            virtual void eventReceiveData(StreamWork::SwCore::SwPin * src,StreamWork::SwCore::SwData_Class * data);
            /*! \brief evenement sur changement d'activation
                \note A Surcharger*/
            virtual void eventActivationChanged();

            /*! \brief Acces au service fournisseur d'interface*/
            StreamWork::SwCore::ISwInterfaces_Provider & getIProviderService();
            /*! \brief Acces au service consommateur d'interface*/
            StreamWork::SwCore::ISwInterfaces_Consumer & getIConsumerService();
            /*! \brief Acces au service de properties*/
            StreamWork::SwCore::ISwProperties & getPropertiesService();
            /*! \brief Acces au service de connexion */
            StreamWork::SwCore::ISwPins_Manager & getPinsService();

            /*! \brief enable listening change for property */
            void enableListeningChangeForProperty(StreamWork::SwCore::ISwProperty * property);
            /*! \brief enable listening for pin */
            void enableListeningForPin(StreamWork::SwCore::SwPin * pin);
        private:
            /* service de fourniture d'interface */
            StreamWork::SwCore::SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            StreamWork::SwCore::SwInterfaces_Consumer_Class * _consumer_service;
            /* service de gestion des propri t s */
            StreamWork::SwCore::SwProperties_Class * _properties_service;
            /* service de gestion des pins */
            StreamWork::SwCore::SwPins_Manager_Class * _pins_service;
            /* desactivation des services */
            bool _disable_service;
        
        };
    
    }

}

#endif
