/*!
\file ISwSimpleTypeAdapter.h
\date 12/04/2006
\brief simple type adaptateur
\author  Big
\version 1.0
 */

#ifndef _ISwSimpleTypeAdapter_H
#define _ISwSimpleTypeAdapter_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>
#include <QVariant>

namespace StreamWork 
{
    namespace SwCore
    {
		class ISwProperty;
        /*!
        \interface ISwSimpleTypeAdapter
        \brief simple type adaptateur
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwSimpleTypeAdapter  {
        public:
	        /*! \brief Initialisation */
	        virtual void Initialize(ISwProperty * simple_property)=0;
	        /*! \brief Initialisation */
	        virtual QString Label()=0;
	        /*! \brief Lecture */
	        virtual QVariant Get()=0;
	        /*! \brief Ecriture */
	        virtual void Set(QVariant)=0;
 	        /*! \brief Liberation de l'adaptateur*/
	        virtual void Liberate()=0;               
        };
    }
}

#endif
