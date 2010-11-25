/*!
\file ISwObservable.h
\date 12/04/2006
\brief interface de tout Observable (DP Observer)
\author  Big
\version 1.0
 */

#ifndef _ISwObservable_H
#define _ISwObservable_H
/*
  * INCLUDES LOCAUX
  */
#include "ISwObserver.h"

namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \interface ISwObservable
        \brief interface de tout Observable (DP Observer)
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwObservable {
        public:
	        /*! \brief Permet d'attacher un observer*/
	        virtual void Attach(ISwObserver * observer)=0;
	        /*! \brief Permet de detacher un observer*/
	        virtual void Detach(ISwObserver * observer)=0;
	        /*! \brief Permet notifier aux observers un changement */
	        virtual void Notify()=0;
        };

    }
}

#endif
