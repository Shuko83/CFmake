/*!
\file ISwHost.h
\date 12/04/2006
\brief interface permettant d'acceder a son composant hote
\author  Big
\version 1.0
 */

#ifndef _ISwHost_H
#define _ISwHost_H
/*
  * INCLUDES LOCAUX
 */
#include "SwComponent_Class.h"

namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \interface ISwHost
        \brief interface permettant d'acceder a son composant hote
        @ingroup SwCoreGrp
        @swinterface

        Toute entité implémentant cette interface permet de connaître le composant hôte auquel est rattaché l’interface
        */
        class ISwHost  {
        public:
	        /*! \brief acces a son composant hote */
	        virtual SwComponent_Class * GetHostComponent()=0;            
        };
    }
}

#endif
