/*!
 \file SwRef.h
 \brief Implementation of the Class SwRef
 \version 1.0
 \date 23-ao�t-2006 10:35:13
 \author F.Bighelli
*/

#ifndef _SwRef_H
#define _SwRef_H

#include "SwCoreConstantes.h"
#include "LibIndeSig.h"
#include "ISwRef.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		 \class SwRef
		 \brief Class compteur de r�f�rence
        @ingroup SwCoreGrp

        Cette classe impl�mente le principe de gestion de vie.
        Toute instance a d�une classe d�rivant de cette classe peut �tre partag� par diff�rentes ressources.
        Chaque ressource gardant une r�f�rence sur a doit effectuer lors du referencement :
        \code
        a.AddRef();
        \endcode
        Et lors du d�r�f�rencement
        \code
        a.Release();
        \endcode
        Les composants et les donn�es d�rivent de cette classe.

		*/
		class BUILD_SWCORE SwRef : public virtual ISwRef
		{
		private:
			/*! \brief Compteur de r�ferences */
			unsigned long _referencesCounter;
		public:
			/*! \brief Constructeur */
			SwRef();
			/*! \brief Ajout d'une r�f�rence */
			virtual void _addRef();
			/*! \brief Retrait d'une r�f�rence */
			virtual void _release();
			/*! \brief Permet de connaitre le nombre de r�ferences*/
			virtual unsigned long _getReferencesNb() const;
		protected:
			/*! \brief Destructeur */
			virtual ~SwRef();
		public:
			/*! \brief Permet de signaler une destruction de l'objet*/
			LibIndeSig::iSignal0 OnDestroy;
		};
	}

}
#endif
