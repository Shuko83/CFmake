/*!
\file ISwObserver.h
\date 12/04/2006
\brief interface de tout observer (DP Observer)
\author  Big
\version 1.0
 */

#ifndef _ISwObserver_H
#define _ISwObserver_H

namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \class ISwObserver
        \brief interface de tout observer (DP Observer)
        */
        class ISwObserver  {
        public:
	        /*! \brief methode appelée par l'observable*/
	        virtual void Update()=0;
        };
    }
}

#endif
