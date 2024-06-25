/*!
 \file _SwEditorStreamView.h
 \brief Implementation d'une vue d'une scene de stream
 \version 1.0
 \date 03/01/07
 \author F.Bighelli
*/

#ifndef __SwEditorStreamView_H
#define __SwEditorStreamView_H

/*
  * INCLUDES GLOBAUX
  */
#include <QGraphicsView>
#include <QGraphicsScene>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwEditorStreamsManager.h"
#include "_SwEditorQGraphicsView.h"

using namespace StreamWork::SwCore;

namespace StreamWork {
    namespace SwEditor {
        /*!
            \class _SwEditorStreamView
            \brief Implementation d'une vue d'une scene de stream
        */
        class _SwEditorStreamView : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public SwCore::ISwObserver
        {
        protected:
            /* \brief actions object */
            _SwEditorQGraphicsView * _view;
            /* \brief default scene */
            QGraphicsScene * _default_scene;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwInterfaces_Consumer_Class * _consumer_service;
            /* \brief service de gestion des propriétés */
            SwProperties_Class * _properties_service;
            /* \brief Handle sur le manager de streams */
            ISwEditorStreamsManager * _manager;
        public:
            /*! \brief Constructeur */
            _SwEditorStreamView();
            /*! \brief Destructeur */
            virtual ~_SwEditorStreamView();
            
            /*! \brief Initialisation des ressources
                \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw( SwException );
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_ConsumerObserver
            //---------------------------------------------------------------------
            /*! \brief Avant changement de la disponibilité de l'interface */
            virtual void BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
            /*! \brief Apres changement de la disponibilité de l'interface */
            virtual void AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
            //---------------------------------------------------------------------
            // Interface ISwObserver
            //---------------------------------------------------------------------
            /*! \brief methode appelée par l'observable*/
            void Update( StreamWork::SwCore::ISwObservable * sender = nullptr );
		};
    }
}

#endif
