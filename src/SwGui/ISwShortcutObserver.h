/*!
\file ISwShortcutObserver.h
\date 12/04/2006
\brief interface d'observabilité d'un device
\author  AAY
\version 1.0
 */

#ifndef _ISWSHORTCUTOBSERVER_H
#define _ISWSHORTCUTOBSERVER_H
/*
  * INCLUDES LOCAUX
 */


namespace StreamWork
{
    namespace SwGui
    {
		class ISwShortcut;

        /*!
        \interface ISwShortcutObserver
        \brief interface d'observabilité d'un shortcut
        @ingroup SwGuiGrp
        @swinterface
        */
        class ISwShortcutObserver {
        public:

           /**
            * @brief    : Méthode appelée lors de l'enregistrement d'un shortcut
            * @param	 : QString  commandName - le nom de la commande
            * @param	 : ISwShortcut * shortcut - Pointeur sur le shortcut
            */
           virtual void add(QString  commandName, ISwShortcut *shortcut) = 0 ; 

		    /**
            * @brief    : Méthode appelée lors du desenregistrement d'un shortcut
            * @param	 : QString  commandName - le nom de la commande
            * @param	 : ISwShortcut * shortcut - Pointeur sur le shortcut
            */
           virtual void remove(QString  commandName, ISwShortcut *shortcut) = 0 ; 
        };
    }
}

#endif
