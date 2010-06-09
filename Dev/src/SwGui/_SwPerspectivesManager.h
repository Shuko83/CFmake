/**
@file _SwPerspectivesManager.h
@brief Perspectives Manager
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI__SWPERSPECTIVESMANAGER_H
#define _STREAMWORK_SWGUI__SWPERSPECTIVESMANAGER_H

#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwWidget.h"
#include "ISwPerspective.h"
#include "_SwPerspectiveButton.h"

using namespace StreamWork::SwCore;

namespace StreamWork {

    namespace SwGui {
    
        /**
        @class _SwPerspectivesManager
        @brief Perspectives Manager
        */
        class _SwPerspectivesManager: public SwComponent_Class,
                                      public ISwInterfaces_ConsumerObserver,
                                      public ISwWidget {
            Q_OBJECT
        protected:
            /* widget central */
            QWidget * _mainWidget;
            /* widget choix tab */
            QWidget * _tabWidget;
            /* widget affichage perspective */
            QWidget * _perspectiveWidget;
            /* layout widget choix tab */
            QVBoxLayout * _vlayout;
            /* widget buttons perspective */
            QList<_SwPerspectiveButton *> _buttons;
            /* service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;
            /* service de gestion des propriétés */
            SwProperties_Class * _properties_service;
            // --- perspectives ---
            /* nombre de perspectives */
            uint _perspectives_nb;
            /* propriété nombre de perspectives */
            ISwProperty * _perspectives_nb_property;
            /* map des perspectives widgets*/
            QMap<QString,ISwPerspective *> _perspectives;
            /* handle temporaire d'interface perspectives*/
            ISwPerspective * _tmp_handle_perspective;
            /* handle PerspectiveCourante*/
            ISwPerspective * _currentPerspective;
			/* @brief propriété style sheet des boutons */
			ISwProperty * _buttons_stylesheet_property;
			/* @brief stylesheet utilise pour les boutons */
			QString	_buttons_stylesheet;
        public:
	        /** @brief Constructor */
	        _SwPerspectivesManager();
            /*! \brief Destructeur */
            virtual ~_SwPerspectivesManager();

            /*! \brief Initialisation des ressources
            \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw(SwException);
             /*! \brief Callback sur les changements de propriétés*/
            void OnPropertyChange(ISwProperty * property);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_ConsumerObserver
            //---------------------------------------------------------------------
	        /*! \brief Avant changement de la disponibilité de l'interface */
	        virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	        /*! \brief Apres changement de la disponibilité de l'interface */
	        virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
            //---------------------------------------------------------------------
            // Interface ISwWidget
            //---------------------------------------------------------------------
            /*! \brief Renvoie le widget
            \return le widget */
	        virtual QWidget & GetWidget();
        
        private:
            /** @brief sur changement de l'activation d'une perspective*/
            virtual void attachPerspective(ISwPerspective * perspective,int index);
            /** @brief sur changement de l'activation d'une perspective*/
            virtual void detachPerspective(ISwPerspective * perspective,int index);
            /** @brief ajout bouton*/
            virtual void addButtons(int number);
            /** @brief suppression bouton*/
            virtual void removeButtons(int number);
        public:
            /** @brief updateButtonsPosition*/
            virtual void updatePositionButton();
        
        public slots:
            /** @brief sur changement de checked*/
            void onPerspectiveToggle(ISwPerspective * perspective,bool checked);
        };

    }
    
}
#endif
