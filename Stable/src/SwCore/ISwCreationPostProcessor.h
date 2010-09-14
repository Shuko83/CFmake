/*!
\file ISwCreationPostProcessor.h
\date 12/04/2006
\brief Interface permettant l'implementation de traitement suite a la cr&eacute; d'un module 
\author  Big
\version 1.0
 */

#ifndef _ISwCreationPostProcessor_H
#define _ISwCreationPostProcessor_H

#include "SwComponent_Class.h"

namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \class ISwCreationPostProcessor
        \brief Interface permettant l'implementation de traitement suite a la cr&eacute; d'un module 
        Mise en oeuvre:
        Soit un objet Foo supportant cette interface, il faut enregistrer Foo aupr&eacute;s de la banque de plugin
        \code
        SW_APP->ComponentsBank().AddCreationPostProcessor(Foo);
        \endcode
        A chaque creation de composant la methode AfterCreatePostProcess de Foo est appel&eacute; avec le composant
        cr&eacute;&eacute; en param&egrave;tre.
        Avant la destruction de Foo, il faut le desenregistrer
        \code
        SW_APP->ComponentsBank().RemoveCreationPostProcessor(Foo);
        \endcode
        */
        class ISwCreationPostProcessor  {
        public:
	        /*! \brief methode appel&eacute;e apr&eacute;s la creation d'un composant
            \param[in] h_comp handle sur le composant qui vient d'etre cr&eacute;&eacute;*/
	        virtual void AfterCreatePostProcess(SwComponent_Class *h_comp)=0;
        };
    }
}

#endif
