/*!
\file ISwEditorGraphicScene.h
\date 02/01/2007
\brief service conteneur d'une scene graphique representant le stream
\author  Big
\version 1.0
 */

#ifndef _ISwEditorGraphicScene_H
#define _ISwEditorGraphicScene_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QGraphicsScene>
/*
  * INCLUDES LOCAUX
 */
#include <ISwService.h>
#include <ISwHost.h>
#include <ISwPersistent.h>

//Nom du service
#define CG_SW_SERVICE_EDITOR_GRAPHIC_SCENE "EditorGraphicScene"


namespace StreamWork 
{
    namespace SwEditor
    {
        /*!
        \class ISwEditorGraphicScene
        \brief service conteneur d'une scene graphique representant le stream
        */
        class ISwEditorGraphicScene : public SwCore::ISwService, public SwCore::ISwHost, public SwCore::ISwPersistent {
        public: 
            /*! \brief Fournit la scene graphique qt correspondant
            \return Renvoie un pointeur sur la scene graphique qt correspondant */
            virtual QGraphicsScene * GetRelatedScene() = 0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_EDITOR_GRAPHIC_SCENE); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}    
            
        };
    }
}

#endif
