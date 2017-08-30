/*!
\file _ISwEditorPinGraphicItemListener.h
\date 12/04/2006
\brief observateur des items graphics representant les pins
\author  Big
\version 1.0
 */

#ifndef __ISwEditorPinGraphicItemListener_H
#define __ISwEditorPinGraphicItemListener_H
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
        class _SwEditorPinGraphicItem;
        /*!
        \class _ISwEditorPinGraphicItemListener
        \brief  observateur des items graphics representant les pins
        */
        class _ISwEditorPinGraphicItemListener  {
        public:
	        /*! \brief Sur  changement de l'item representant le pin*/
	        virtual void OnPinItemChange(_SwEditorPinGraphicItem * source)=0;            
        };
    }
}

#endif
