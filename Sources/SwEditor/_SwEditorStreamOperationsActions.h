/*!
 \file __SwEditorStreamOperationsActions.h
 \brief Implementation of the Class __SwEditorStreamOperationsActions une tabbar des streams editer
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef ___SwEditorStreamOperationsActions_H
#define ___SwEditorStreamOperationsActions_H

/*
  * INCLUDES GLOBAUX
  */
#include <QToolBox>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwEditorStreamOperations.h"
#include "_SwStreamOperationsActions.h"

using namespace StreamWork::SwCore;

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class __SwEditorStreamOperationsActions 
	        \brief Implementation of the Class __SwEditorStreamOperationsActions une tabbar des streams editer
        */
        class _SwEditorStreamOperationsActions : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
        {
        protected:
            /* \brief actions object */
            _SwStreamOperationsActions * _actions_object;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;
            /* \brief service de gestion des propriétés */
            SwProperties_Class * _properties_service;
            /* \brief Handle sur le navigateur de streams */
            ISwEditorStreamOperations * _selection;
        public:
            /*! \brief Constructeur */
            _SwEditorStreamOperationsActions();
            /*! \brief Destructeur */
            virtual ~_SwEditorStreamOperationsActions();

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
