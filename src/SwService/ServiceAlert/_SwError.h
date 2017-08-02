#ifndef _SWERROR_H
#define _SWERROR_H

//Local includes
#include "Error.h"
#include "AlertManager.h"

#pragma warning(push)
#pragma warning( disable : 4250 ) // c'est normal que le compilateur choisisse via la dominance la methode à utiliser ici en raison de l'heritage virtuel en diamant.


namespace StreamWork
{
    namespace SwCore
    {

		/**
		* @class SwError
		* @brief Déclare une erreur de catégory SwAlert
		*/
		class SwError : public alert::Error
		{
			DECLARE_CATEGORY("SwAlert")
		public :
            SwError(QString title, QString description, alert::EnumErrorLevel::ErrorLevel level, alert::AlertManager * alertManager);
			virtual ~SwError();
		};
	}
}

#pragma warning(pop)

#endif