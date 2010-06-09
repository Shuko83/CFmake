/*!
\file _SwComplexeTypeAdaptersFactoriesBankImpl.h
\date 12/04/2006
\brief implementation banque des fabriques de complexe types adaptateur
\author  Big
\version 1.0
 */

#ifndef __SwComplexeTypeAdaptersFactoriesBankImpl_H
#define __SwComplexeTypeAdaptersFactoriesBankImpl_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QMap>
#include "ISwComplexeTypeAdaptersFactoriesBank.h"


namespace StreamWork
{
    namespace SwCore
    {
		/*!
        \class _SwComplexeTypeAdaptersFactoriesBankImpl
        \brief  implementation banque des fabriques de complexe types adaptateur
        */
        class _SwComplexeTypeAdaptersFactoriesBankImpl : public ISwComplexeTypeAdaptersFactoriesBank{
	        /*! \brief Liste des factory */
			QMap<int,ISwComplexeTypeAdaptersFactory *> _ctfactories;
		public:
			/*! \brief Constructeur */
			_SwComplexeTypeAdaptersFactoriesBankImpl();
			/*! \brief Destructeur */
			virtual ~_SwComplexeTypeAdaptersFactoriesBankImpl();
	        /*! \brief Creation d'un adapteur */
	        void AddCTFactory(ISwComplexeTypeAdaptersFactory * factory);
	        /*! \brief Acces a une factory pour un user type (cd QVariant userType)*/
	        ISwComplexeTypeAdaptersFactory * GetFactoryForUserType(int user_type);
        };
    }
}

#endif
