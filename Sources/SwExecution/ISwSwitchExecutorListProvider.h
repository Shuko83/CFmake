/*!
\file ISwSwitchExecutorListProvider.h
\date 16/04/2009
\brief interface d acces a la liste en execution
\author  Big
\version 1.0
 */

#ifndef _ISwSwitchExecutorListProvider_H
#define _ISwSwitchExecutorListProvider_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include "ISwSwitchExecutorListener.h"



namespace StreamWork 
{
    namespace SwExecution
    { 
        /*!
        \interface ISwSwitchExecutorListProvider
        \brief interface d acces a la liste en execution
         @ingroup SwExeGrp
        @swinterface
       */
        class ISwSwitchExecutorListProvider
            {

        public:
	        /*! \brief acces au nom de la liste en execution */
	        virtual QString GetListName()=0;  
	                  
            virtual void AddSwitchExectuorListener (ISwSwitchExecutorListener * listener) = 0;
            virtual void RemoveSwitchExectuorListener (ISwSwitchExecutorListener * listener) = 0;            
       };
    }
}

#endif
