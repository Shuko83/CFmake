/*!
\file ISwComplexeTypeAdaptersFactoriesBank.h
\date 12/04/2006
\brief banque des fabriques de complexe types adaptateur
\author  Big
\version 1.0
 */

#ifndef _ISwComplexeTypeAdaptersFactoriesBank_H
#define _ISwComplexeTypeAdaptersFactoriesBank_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwComplexeTypeAdaptersFactory;
		/*!
        \interface ISwComplexeTypeAdaptersFactoriesBank
        \brief  banque des fabriques de complexe types adaptateur
        @ingroup SwCoreGrp
        @swinterface
       */
        class ISwComplexeTypeAdaptersFactoriesBank  {
        public:
	        /*! \brief Creation d'un adapteur */
	        virtual void AddCTFactory(ISwComplexeTypeAdaptersFactory * factory)=0;
	        /*! \brief Acces a une factory pour un user type (cd QVariant userType)*/
	        virtual ISwComplexeTypeAdaptersFactory * GetFactoryForUserType(int user_type)=0;        
        };
    }
}

#endif
