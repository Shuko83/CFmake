#ifndef _SWERROR_H
#define _SWERROR_H

//QT includes

//Other External includes

//Local includes
#include "Error.h"
// External include



namespace StreamWork
{
    namespace SwCore
    {

		/**
		* @class SwError
		* @brief Déclarre une erreurde catégory SwAlert
		*/
		class SwError : public alert::Error
		{
			DECLARE_CATEGORY("SwAlert")
		public :
			SwError(QString title, QString description, alert::EnumErrorLevel::ErrorLevel level);
			virtual ~SwError();
		};
	}
}

#endif