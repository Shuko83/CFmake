/*!
\file ISwFinalizerManager.h
\date 18/04/2006
\brief interface de d'enregistrement des finalisations (pour appel apres le chargement complet)
\author  Big
\version 1.0
 */

#ifndef _ISwFinalizerManager_H
#define _ISwFinalizerManager_H
/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include "Component/Interfaces/ISwFinalizer.h"

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \interface ISwFinalizerManager
        \brief interface de d'enregistrement des finalisations (pour appel apres le chargement complet)
        @ingroup SwCoreGrp
        @swinterface
        voir StreamWork::SwCore::ISwFinalizer pour plus d'explication.
        */

        class ISwFinalizerManager  {
        public:
            /*! \brief enregistre une finalisation pour un index d'historique donnée*/
	        virtual void RegisterFinalization(quint64 historic_index,ISwFinalizer * finalizer)=0;
        };
    }
}

#endif
