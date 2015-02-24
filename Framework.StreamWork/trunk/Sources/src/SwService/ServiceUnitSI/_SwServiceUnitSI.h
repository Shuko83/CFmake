///////////////////////////////////////////////////////////
//  _SwServiceUnitSI.h
//  Implementation of the Class _SwServiceUnitSI
//  Created on:      31-déc.-2013 11:17:04
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////

#ifndef STREAMWORK_SWCORE_SWSERVICEUNITSI_H
#define STREAMWORK_SWCORE_SWSERVICEUNITSI_H

#include <QString>
#include <QObject>

#include "SwException.h"
#include "SwMacros.h"
#include "ISwServiceUnitSI.h"
#include "SwEnum.h"

#include "ServiceUnitSI.h"

/**
 * @class _SWServiceUnitSI 
 * @brief service de mapping des unités systeme
 *  internationale
 * @ingroup SwGuiCommon
 */
namespace StreamWork 
{    
	namespace SwCore 
	{
		class _SwServiceUnitSI : public QObject,
			public virtual ISwServiceUnitSI,
			public virtual ServiceUnitSI
		{
			Q_OBJECT

			//current unit family
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Altitude
				READ getCurrentUnitSI_Altitude
				WRITE setCurrentUnitSI_Altitude);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Angle
				READ getCurrentUnitSI_Angle
				WRITE setCurrentUnitSI_Angle);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Speed
				READ getCurrentUnitSI_Speed
				WRITE setCurrentUnitSI_Speed);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Distance
				READ getCurrentUnitSI_Distance
				WRITE setCurrentUnitSI_Distance);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Duration
				READ getCurrentUnitSI_Duration
				WRITE setCurrentUnitSI_Duration);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Frequency
				READ getCurrentUnitSI_Frequency
				WRITE setCurrentUnitSI_Frequency);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentUnitSI_Coordinates
				READ getCurrentUnitSI_Coordinates
				WRITE setCurrentUnitSI_Coordinates);

			//current unit precision
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentPrecision_Altitude
				READ getCurrentPrecision_Altitude
				WRITE setCurrentPrecision_Altitude);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentPrecision_Angle
				READ getCurrentPrecision_Angle
				WRITE setCurrentPrecision_Angle);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentPrecision_Speed
				READ getCurrentPrecision_Speed
				WRITE setCurrentPrecision_Speed);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentPrecision_Distance
				READ getCurrentPrecision_Distance
				WRITE setCurrentPrecision_Distance);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentPrecision_Duration
				READ getCurrentPrecision_Duration
				WRITE setCurrentPrecision_Duration);
			Q_PROPERTY(StreamWork::SwCore::SwEnum currentPrecision_Frequency
				READ getCurrentPrecision_Frequency
				WRITE setCurrentPrecision_Frequency);

		public:

			_SwServiceUnitSI();
			virtual ~_SwServiceUnitSI();

			//Q_PROPERTY
			/**
			* @brief	: get and set current unit for each family
			*/
			StreamWork::SwCore::SwEnum getCurrentUnitSI_Altitude();
			void setCurrentUnitSI_Altitude(StreamWork::SwCore::SwEnum unitSI);

			StreamWork::SwCore::SwEnum getCurrentUnitSI_Angle();
			void setCurrentUnitSI_Angle(StreamWork::SwCore::SwEnum unitSI);

			StreamWork::SwCore::SwEnum getCurrentUnitSI_Speed();
			void setCurrentUnitSI_Speed(StreamWork::SwCore::SwEnum unitSI);

			StreamWork::SwCore::SwEnum getCurrentUnitSI_Distance();
			void setCurrentUnitSI_Distance(StreamWork::SwCore::SwEnum unitSI);

			StreamWork::SwCore::SwEnum getCurrentUnitSI_Duration();
			void setCurrentUnitSI_Duration(StreamWork::SwCore::SwEnum unitSI);

			StreamWork::SwCore::SwEnum getCurrentUnitSI_Frequency();
			void setCurrentUnitSI_Frequency(StreamWork::SwCore::SwEnum unitSI);

			StreamWork::SwCore::SwEnum getCurrentUnitSI_Coordinates();
			void setCurrentUnitSI_Coordinates(StreamWork::SwCore::SwEnum unitSI);

			/**
			* @brief	: get and set current precision for each family
			*/
			StreamWork::SwCore::SwEnum getCurrentPrecision_Altitude();
			void setCurrentPrecision_Altitude(StreamWork::SwCore::SwEnum precisionSI);

			StreamWork::SwCore::SwEnum getCurrentPrecision_Angle();
			void setCurrentPrecision_Angle(StreamWork::SwCore::SwEnum precisionSI);

			StreamWork::SwCore::SwEnum getCurrentPrecision_Speed();
			void setCurrentPrecision_Speed(StreamWork::SwCore::SwEnum precisionSI);

			StreamWork::SwCore::SwEnum getCurrentPrecision_Distance();
			void setCurrentPrecision_Distance(StreamWork::SwCore::SwEnum precisionSI);

			StreamWork::SwCore::SwEnum getCurrentPrecision_Duration();
			void setCurrentPrecision_Duration(StreamWork::SwCore::SwEnum precisionSI);

			StreamWork::SwCore::SwEnum getCurrentPrecision_Frequency();
			void setCurrentPrecision_Frequency(StreamWork::SwCore::SwEnum precisionSI);

		private:
			StreamWork::SwCore::SwEnum altitudeUnits;
			StreamWork::SwCore::SwEnum angleUnits;
			StreamWork::SwCore::SwEnum distanceUnits;
			StreamWork::SwCore::SwEnum speedUnits;
			StreamWork::SwCore::SwEnum durationUnits;
			StreamWork::SwCore::SwEnum frequencyUnits;
			StreamWork::SwCore::SwEnum coordinateUnits;

			StreamWork::SwCore::SwEnum altitudePrecision;
			StreamWork::SwCore::SwEnum anglePrecision;
			StreamWork::SwCore::SwEnum distancePrecision;
			StreamWork::SwCore::SwEnum speedPrecision;
			StreamWork::SwCore::SwEnum durationPrecision;
			StreamWork::SwCore::SwEnum frequencyPrecision;
		};
	}
}
#endif //STREAMWORK_SWCORE_SWSERVICEUNITSI_H
