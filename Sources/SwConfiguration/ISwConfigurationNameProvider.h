/*!
\file ISwConfigurationNameProvider.h
\date 16/04/2009
\brief interface d acces a la liste en execution
\author  Big
\version 1.0
 */

#ifndef _ISwConfigurationNameProvider_H
#define _ISwConfigurationNameProvider_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include "ISwConfigurationNameListener.h"


namespace StreamWork 
{
    namespace SwConfiguration
    { 
        /*!
        \class ISwConfigurationNameProvider
        \brief interface d acces a la liste en execution
        */
        class ISwConfigurationNameProvider
            {

        public:
	        /*! \brief acces au nom configuration selectionnee */
	        virtual QString GetSelectedName()=0;  
	                  
            virtual void AddConfigurationNameListener (ISwConfigurationNameListener * listener) = 0;
            virtual void RemoveConfigurationNameListener (ISwConfigurationNameListener * listener) = 0;            
       };
    }
}

#endif
