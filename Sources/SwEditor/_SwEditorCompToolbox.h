/*!
 \file _SwEditorCompToolbox.h
 \brief Implementation of the Class _SwEditorCompToolbox generant un boite a outils contenant la liste des composants
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwEditorCompToolbox_H
#define __SwEditorCompToolbox_H

/*
  * INCLUDES GLOBAUX
  */
#include <QSplitter>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"

using namespace StreamWork::SwCore;

namespace StreamWork {
    namespace SwEditor {
        /*!
            \class _SwEditorCompToolbox
            \brief Implementation of the Class _SwEditorCompToolbox generant un boite a outils contenant la liste des composants
        */
        class _SwEditorCompToolbox : public SwComponent_Class
        {
        protected:
            /* \brief TreeView */
            QSplitter * _toolbox;
            /* \brief service de fourniture d'interface */
            SwInterfaces_Provider_Class * _provider_service;
            /* \brief service de gestion des propriétés */
            SwProperties_Class * _properties_service;
            /* \brief(Re)Construction du widget */
            void BuildWidget();
        public:
            /*! \brief Constructeur */
            _SwEditorCompToolbox();
            /*! \brief Destructeur */
            virtual ~_SwEditorCompToolbox();
            
            /*! \brief Initialisation des ressources
                \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw( SwException );
        };
    }
}

#endif
