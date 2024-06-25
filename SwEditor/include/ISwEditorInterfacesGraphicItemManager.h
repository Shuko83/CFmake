/*!
\file ISwEditorInterfacesGraphicItemManager.h
\date 02/01/2007
\brief service de gestion des items graphiques représentant les interfaces
\author  Big
\version 1.0
 */

#ifndef _ISwEditorInterfacesGraphicItemManager_H
#define _ISwEditorInterfacesGraphicItemManager_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QGraphicsItem>
/*
  * INCLUDES LOCAUX
 */

namespace StreamWork 
{
    namespace SwEditor
    {
        class _SwEditorInterfaceGraphicItem;
        /*!
        \class ISwEditorInterfacesGraphicItemManager
        \brief service de gestion des items graphiques représentant les interfaces
        */
        class ISwEditorInterfacesGraphicItemManager  {
        public:
            /*! \brief Renvoie l'item graphique relatif a une interface fournit */
            virtual _SwEditorInterfaceGraphicItem * GetItemForProvidedInterface(QString name)=0;
            
        };
    }
}

#endif
