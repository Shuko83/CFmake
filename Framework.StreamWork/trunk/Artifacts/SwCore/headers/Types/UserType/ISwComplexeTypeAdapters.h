/*!
\file ISwComplexeTypeAdapters.h
\date 12/04/2006
\brief complexe types adaptateur
\author  Big
\version 1.0
 */

#ifndef _ISwComplexeTypeAdapters_H
#define _ISwComplexeTypeAdapters_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwSimpleTypeAdapter;
        class ISwProperty;
		/*!
        \interface ISwComplexeTypeAdapters
        \brief complexe types adaptateur
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwComplexeTypeAdapters  {
        public:
	        /*! \brief Initialisation */
	        virtual void Initialize(ISwProperty * complexe_property)=0;
	        /*! \brief Acces aux adapteurs simples (la liste est vide si le complexe adapter n'est pas attaché a une propriété*/
	        virtual QList<ISwSimpleTypeAdapter *> * GetAdapters()=0;
 	        /*! \brief Liberation de l'adaptateur*/
	        virtual void Liberate()=0;               
        };
    }
}

#endif
