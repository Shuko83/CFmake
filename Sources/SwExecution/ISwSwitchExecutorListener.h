/*!
\file ISwSwitchExecutorListener.h
\date 16/04/2009
\brief interface de notification acces a la liste en execution
\author  Big
\version 1.0
 */

#ifndef _ISwSwitchExecutorListener_H
#define _ISwSwitchExecutorListener_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>




namespace StreamWork 
{
    namespace SwExecution
    { 
        /*!
        \interface ISwSwitchExecutorListener
        \brief interface d acces a la liste en execution
        @ingroup SwExeGrp
        @swinterface
        */
        class ISwSwitchExecutorListener
            {

        public:
	        /*! \brief on modify */    
            virtual void OnListModified() = 0;
               
       };
    }
}

#endif
