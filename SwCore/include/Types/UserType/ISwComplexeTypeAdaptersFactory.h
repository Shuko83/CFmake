/*!
\file ISwComplexeTypeAdaptersFactory.h
\date 12/04/2006
\brief fabrique de complexe types adaptateur
\author  Big
\version 1.0
 */

#ifndef _ISwComplexeTypeAdaptersFactory_H
#define _ISwComplexeTypeAdaptersFactory_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwComplexeTypeAdapters;
		/*!
        \interface ISwComplexeTypeAdaptersFactory
        \brief fabrique de complexe types adaptateur
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwComplexeTypeAdaptersFactory  {
        public:
	        /*! \brief Creation d'un adapteur */
	        virtual ISwComplexeTypeAdapters * CreateAdapters()=0;
	        /*! \brief Acces au user type (cd QVariant userType)*/
	        virtual int GetUserType()=0;
	        /*! \brief Liberation de la fabrique*/
	        virtual void Liberate()=0;            
        };
    }
}

#endif
