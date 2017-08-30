/*!
\file SwNamed_Class.h
\date 12/04/2006
\brief Classe dont l'instance est nomm�e
\author  F.Bighelli
\version 1.0
 */

#ifndef _SwNamed_Class_H
#define _SwNamed_Class_H
/*
  * INCLUDES GLOBAUX
  */
#include <QString>
/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"
#include "SwException.h"
#include "LibIndeSig.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		\class SwNamed_Class
		\brief Classe dont l'instance est nomm�e
        @ingroup SwCoreGrp
		*/
		class BUILD_SWCORE SwNamed_Class {
		protected:
			/*! \brief Nom de l'instance */
			QString _instance_name;
			/*! \brief Description de l'instance */
			QString _instance_description;
		public:
			/*! \brief Constructeur*/
			SwNamed_Class();
			/*! \brief Destructeur*/
			virtual ~SwNamed_Class();
			/*! \brief Permet de recuperer une description de l'instance nomm�*/
			virtual QString GetDescription() const;
			/*! \brief Permet de recuperer le nom de l'instance nomm�*/
			virtual QString GetName() const;
			/*! \brief Permet de definir le nom de l'instance nomm�*/
			virtual void SetName(const QString & new_name) throw(SwException);
			/*! \brief Permet de definir la description de l'instance nomm�*/
			virtual void SetDescription(const QString & new_description);
		public:
			/*! \brief Permet de signaler un changement de nom de l'instance*/
			LibIndeSig::iSignal0 OnChangeName;
			/*! \brief Permet de signaler un changement de la description de l'instance*/
			LibIndeSig::iSignal0 OnChangeDescription;
		public:
			/*! \brief Permet de tester la validite du nom */
			static bool CheckNameValidity(const QString & name);
		};
	}
}

#endif
