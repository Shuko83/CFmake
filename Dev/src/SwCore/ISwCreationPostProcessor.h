/*!
\file ISwCreationPostProcessor.h
\date 12/04/2006
\brief Interface de tout traitement a effectuer sur les modules apr�s leur creations
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
        \brief Interface de tout traitement a effectuer sur les modules apr�s leur creations
        Mise en oeuvre:
        Soit un objet a supportant cette interface, il faut enregistrer a aupr�s de la banque de plugin
        \code
        SW_APP->ComponentsBank().AddCreationPostProcessor(a);
        \endcode
        A chaque creation de composant la methode AfterCreatePostProcess de a est appel� avec le composant
        cr�� en parametre.
        Avant la destruction de a, il faut le desenregistrer
        \code
        SW_APP->ComponentsBank().RemoveCreationPostProcessor(a);
        \endcode
        */
        class ISwCreationPostProcessor  {
        public:
	        /*! \brief methode appel�e apr�s la creation d'un composant
            \param[in] h_comp handle sur le composant qui vient d'etre cr��*/
	        virtual void AfterCreatePostProcess(SwComponent_Class *h_comp)=0;
        };
    }
}

#endif
