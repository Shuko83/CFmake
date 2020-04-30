/*!
 \file _SwGuiCompToPropertiesModel.h
 \brief Implementation of the Class _SwGuiCompToPropertiesModel generant un model QT a partir des propriétés d'un composant fournit
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompToPropertiesModel_H
#define __SwGuiCompToPropertiesModel_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAbstractItemModel>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <ISwComponentProvider.h>
#include "_SwPropertiesModelImpl.h"
//#include "ISwQAbstractItemViewSlots.h"
//#include "_SwQAbstractItemViewSlots_Catcher.h"

using namespace StreamWork::SwCore;

namespace StreamWork
{
	namespace SwGui
	{
        /*!
	        \class _SwGuiCompToPropertiesModel 
	        \brief Implementation of the Class _SwGuiCompToPropertiesModel generant un model QT a partir des propriétés d'un composant fournit
        */
        class _SwGuiCompToPropertiesModel : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
        {
        protected:
            /* service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;
            /* service de gestion des propriétés */
            SwProperties_Class * _properties_service;
	        /*Fournisseur de composant*/
	        ISwComponentProvider * _handle;
	        /*Fournisseur de composant*/
	        SwComponent_ClassPtr _provided_component;
	        /*Modele */
            _SwPropertiesModelImpl * _model;
        public:
            /*! \brief Constructeur */
            _SwGuiCompToPropertiesModel();
            /*! \brief Destructeur */
            virtual ~_SwGuiCompToPropertiesModel();

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
            //---------------------------------------------------------------------
            // Slot de changement de composant fournit
            //---------------------------------------------------------------------
            void OnProvidedComponentChange(ISwComponentProvider * provider);
        };
    }
}
#endif 
