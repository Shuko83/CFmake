/*!
 \file _SwEditorStreamNavigatorActions.h
 \brief Implementation of the Class _SwEditorStreamNavigatorActions une tabbar des streams editer
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwEditorStreamNavigatorActions_H
#define __SwEditorStreamNavigatorActions_H

/*
  * INCLUDES GLOBAUX
  */
#include <QToolBox>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <ISwAction.h>
#include "ISwEditorStreamNavigation.h"
#include "_SwStreamNavigationActions.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorStreamNavigatorActions 
	        \brief Implementation of the Class _SwEditorStreamNavigatorActions une tabbar des streams editer
        */
        class _SwEditorStreamNavigatorActions : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
        {
        protected:
            /* \brief actions object */
            _SwStreamNavigationActions * _actions_object;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;
            /* \brief service de gestion des propriétés */
            SwProperties_Class * _properties_service;
            /* \brief Handle sur le navigateur de streams */
            ISwEditorStreamNavigation * _navigator;
        public:
            /*! \brief Constructeur */
            _SwEditorStreamNavigatorActions();
            /*! \brief Destructeur */
            virtual ~_SwEditorStreamNavigatorActions();

            /*! \brief Initialisation des ressources
                \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw(SwException);  	
             //---------------------------------------------------------------------
            // Interface ISwInterfaces_ConsumerObserver
            //---------------------------------------------------------------------
	        /*! \brief Avant changement de la disponibilité de l'interface */
	        virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	        /*! \brief Apres changement de la disponibilité de l'interface */
	        virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            

        };
    }
}
#endif 
