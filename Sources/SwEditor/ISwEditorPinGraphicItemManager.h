/*!
\file ISwEditorPinGraphicItemManager.h
\date 02/01/2007
\brief service de gestion des items graphiques représentant les pins
\author  Big
\version 1.0
 */

#ifndef _ISwEditorPinGraphicItemManager_H
#define _ISwEditorPinGraphicItemManager_H
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
        class _SwEditorPinGraphicItem;
        /*!
        \class ISwEditorInterfacesGraphicItemManager
        \brief service de gestion des items graphiques représentant les pins
        */
        class ISwEditorPinGraphicItemManager  {
        public:
            /*! \brief Renvoie l'item graphique relatif a un pin */
            virtual _SwEditorPinGraphicItem * GetItemForPin(QString name)=0;

        };
    }
}

#endif
