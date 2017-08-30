/*!
\file _ISwEditorInterfaceGraphicItemListener.h
\date 12/04/2006
\brief observateur des items graphics representant les interfaces
\author  Big
\version 1.0
 */

#ifndef __ISwEditorInterfaceGraphicItemListener_H
#define __ISwEditorInterfaceGraphicItemListener_H
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
        \class _ISwEditorInterfaceGraphicItemListener
        \brief  observateur des items graphics representant les interfaces
        */
        class _ISwEditorInterfaceGraphicItemListener  {
        public:
	        /*! \brief Sur  changement de l'item representant l'interface*/
	        virtual void OnInterfaceItemChange()=0;            
        };
    }
}

#endif
