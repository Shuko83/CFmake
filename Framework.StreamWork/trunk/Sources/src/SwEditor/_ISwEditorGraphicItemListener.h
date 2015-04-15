/*!
\file _ISwEditorGraphicItemListener.h
\date 12/04/2006
\brief observateur des items graphics representant les modules
\author  Big
\version 1.0
 */

#ifndef __ISwEditorGraphicItemListener_H
#define __ISwEditorGraphicItemListener_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */

namespace StreamWork 
{
    namespace SwEditor
    { 
        /*!
        \class _ISwEditorGraphicItemListener
        \brief  observateur des items graphics representant les modules
        */
        class _ISwEditorGraphicItemListener  {
        public:
	        /*! \brief Sur  changement de l'item representant le composant*/
	        virtual void OnItemChanged()=0;            
        };
    }
}

#endif
