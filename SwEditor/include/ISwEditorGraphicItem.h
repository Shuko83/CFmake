/*!
\file ISwEditorGraphicItem.h
\date 02/01/2007
\brief service de dessin d'un composant dans l'editeur de stream
\author  Big
\version 1.0
 */

#ifndef _ISwEditorGraphicItem_H
#define _ISwEditorGraphicItem_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QGraphicsItem>
/*
  * INCLUDES LOCAUX
 */
#include "Main/Services/Management/ISwService.h"
#include "Component/Interfaces/ISwHost.h"
#include "Component/Interfaces/ISwPersistent.h"
#include "ISwEditorInterfacesGraphicItemManager.h"
#include "ISwEditorPinGraphicItemManager.h"


//Nom du service
#define CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM "EditorGraphicItem"
#define CG_SW_EDITOR_GRAPHIC_ITEM_OFFSET_TYPE 100000


namespace StreamWork 
{
    namespace SwEditor
    {
        /*!
        \class ISwEditorGraphicItem
        \brief service fournisseur d'interfaces
        */
        class ISwEditorGraphicItem : public SwCore::ISwService, public SwCore::ISwHost, public SwCore::ISwPersistent {
        public: 
            /*! \brief Fournit l'item graphique qt correspondant
            \return Renvoie un pointeur sur l'item graphique qt correspondant */
            virtual QGraphicsItem * GetRelatedItem() = 0;
            /*! \brief resize de l'item */
            virtual void ResizeItem(QRectF rect,bool propagate_to_parent=false)=0;
            /*! \brief autorise ou non la mise a jour de la selection par itemChange */
            virtual void EnableSelectionUpdateByItemChange(bool enable)=0;
            /*! \brief Acces a l'interface manager */
            virtual ISwEditorInterfacesGraphicItemManager * GetInterfacesManager()=0;
            /*! \brief Acces au pin manager */
            virtual ISwEditorPinGraphicItemManager * GetPinsManager()=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
            
        };
    }
}

#endif
