/*!
\file SwDefaultComplexeTypeAdapters.h
\date 12/04/2006
\brief complexe types adaptateur par default
\author  Big
\version 1.0
 */

#ifndef _SwDefaultComplexeTypeAdapters_H
#define _SwDefaultComplexeTypeAdapters_H

#include "SwCoreConstantes.h"
#include "ISwComplexeTypeAdapters.h"
#include "ISwSimpleTypeAdapter.h"
#include "ISwProperty.h"

namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \class SwDefaultComplexeTypeAdapters
        \brief complexe types adaptateur par default
        @ingroup SwCoreGrp

        exemple a donné
        */
		class BUILD_SWCORE SwDefaultComplexeTypeAdapters : public ISwComplexeTypeAdapters{
		protected:
		    /*! \brief propriété hôte */
			ISwProperty * _complexe_property;
		    /*! \brief propriété hôte */
		    QList<ISwSimpleTypeAdapter *> _liste;
		public:
			/*! \brief Constructeur */
			SwDefaultComplexeTypeAdapters();
			/*! \brief Destructeur */
			virtual ~SwDefaultComplexeTypeAdapters();
			//-----------------------------------------------------------------------
			// Interface ISwComplexeTypeAdapters
			//-----------------------------------------------------------------------
			/*! \brief Initialisation */
			void Initialize(ISwProperty * complexe_property);
			/*! \brief Acces aux adapteurs simples (la liste est vide si le complexe adapter n'est pas attaché a une propriété*/
			QList<ISwSimpleTypeAdapter *> * GetAdapters();
			/*! \brief Liberation de la fabrique*/
			void Liberate();
			//-----------------------------------------------------------------------
			// Creation de la liste
			//-----------------------------------------------------------------------
			/*! \brief Creation de la liste des simple type adapter  A SURCHARGER*/
			virtual void CreateSimpleTypeAdaptersList(ISwProperty * complexe_property);
		};
	}
}
#endif
