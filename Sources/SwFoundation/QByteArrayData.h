/**
* @file QByteArrayData.h
*/

#ifndef _STREAMWORK_SWFOUNDATION_QBYTEARRAYDATA_H
#define _STREAMWORK_SWFOUNDATION_QBYTEARRAYDATA_H

#ifdef SWFOUNDATION_LIB
# define BUILD_SWFOUNDATION Q_DECL_EXPORT
#else
# define BUILD_SWFOUNDATION Q_DECL_IMPORT
#endif

#include "Component/Pin/SwData_Class.h"
#include "Main/SwMacros.h"

#include <QByteArray>
#include <QString>

#define CG_SW_QBYTEARRAYDATA_TYPE "QByteArrayData"

namespace StreamWork 
{
	namespace SwFoundation 
	{

		SW_DECLARE_UUID_LOCAL(CG_SW_QBYTEARRAYDATA_UUID) 

		/**
		* @class QByteArrayMessage
		*/
		class BUILD_SWFOUNDATION QByteArrayData : virtual public SwCore::SwData_Class
		{
		private:
			QByteArray _internalData;

		public:
			QByteArrayData ();
			virtual ~QByteArrayData();		

			void setData (const QByteArray &data);
			QByteArray & getData();

			//---------------------------------------------------------------------
            // Identification
            //---------------------------------------------------------------------
            /*! \brief Identification de la donnée par chaine de caractere */
            virtual QString GetType();
            /*! \brief Identification de la donnée par valeur */
            virtual SwCore::SwUUID GetTypeId();

		protected:
			//---------------------------------------------------------------------
			// Serialisation interne
			//---------------------------------------------------------------------
			/*! \brief Lecture */
			virtual void InternalRead(QDataStream * data) throw(SwCore::SwException);
			/*! \brief Ecriture */
			virtual void InternalWrite(QDataStream * data);
			//---------------------------------------------------------------------
			// Creation
			//---------------------------------------------------------------------
			/*! \brief Creation */
			virtual SwData_Class * CreateInstance();
		};
	}
}
#endif

