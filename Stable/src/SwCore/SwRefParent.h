/*!
 \file SwRefParent.h
 \brief Implementation of the Class SwRefParent
 \version 1.0
 \date 23-ao�t-2006 10:35:13
 \author F.Bighelli
*/

#ifndef _SwRefParent_H
#define _SwRefParent_H

#include "SwCoreConstantes.h"
#include "ISwRef.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		 \class SwRefParent
		 \brief Class compteur de r�f�rence sur un parent reference compté


		*/
		class BUILD_SWCORE SwRefParent : public virtual ISwRef
		{
		private:
			/*! \brief Compteur de r�ferences */
			ISwRef * _parent;
		public:
			/*! \brief Constructeur */
			SwRefParent(ISwRef * parent);
			/*! \brief Ajout d'une r�f�rence */
			virtual void _addRef();
			/*! \brief Retrait d'une r�f�rence */
			virtual void _release();
			/*! \brief Permet de connaitre le nombre de r�ferences*/
			virtual unsigned long _getReferencesNb() const;
			/*! \brief get du parent */
			virtual ISwRef * getSwRefParent();			
			/*! \brief set du parent */
			virtual void setSwRefParent(ISwRef * parent);
		protected:
			/*! \brief Destructeur */
			virtual ~SwRefParent();
		};
	}

}
#endif
