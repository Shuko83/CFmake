/*!
\file _ISwConfigurationNameListener.h
\date 16/04/2009
\brief interface de notification du nom de la configuration courante
\author  Big
\version 1.0
 */

#ifndef _ISwConfigurationNameListener_H
#define _ISwConfigurationNameListener_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>




namespace StreamWork 
{
    namespace SwConfiguration
    { 
        /*!
        \class ISwConfiguraionNameListener
        \brief interface d acces au changmement de nom de la configuration 
        */
        class ISwConfigurationNameListener
            {

        public:
	        /*! \brief on modify */    
            virtual void OnNameModified() = 0;
               
       };
    }
}

#endif
