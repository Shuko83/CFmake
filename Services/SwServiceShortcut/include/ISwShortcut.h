/*!
\file ISwShortcut.h
\date 12/04/2006
\brief Interface des boutons des devices
\author  AAY
\version 1.0
 */

#ifndef _ISWSHORTCUT_H
#define _ISWSHORTCUT_H
/*
  * INCLUDES LOCAUX
 */
#include <QString>


namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \interface ISwShortcut
        \brief Interface des boutons des devices
        @ingroup SwGuiGrp
        @swinterface
        */
        class ISwShortcut {
        public:

		   /**
		    * @brief    : Callback d'appel avec en parametre le nom associé a la commande
		    * @param	 : QString name - nom de la commande appelée
		    */
		   virtual void processCommand(QString name) = 0;
        };
    }
}

#endif
