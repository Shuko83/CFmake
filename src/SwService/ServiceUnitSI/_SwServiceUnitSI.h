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

#include "SwSiEnums.h"
#include "SwException.h"
#include "SwMacros.h"
#include "ISwServiceUnitSI.h"
#include "SwEnum.h"

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
								 public ISwServiceUnitSI
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

			/**
			* @brief	: Registration with the Service for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			*/
			virtual void registerTypeSI(SwSiUnitFamily unitFamily, ISwUnitSI * obj) ;

			/**
			* @brief	: Termination with the Service for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			*/
			virtual void unregisterTypeSI(SwSiUnitFamily unitFamily, ISwUnitSI * obj) ;

			/**
			* @brief	: Notify all listener precision modification for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			*/
			virtual void changePrecisionUnitSI(SwSiUnitFamily unitFamily, int precision) ;
			
			/**
			* @brief	: Notify all listener unit modification for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			 */
			virtual void changeUnitSI(SwSiUnitFamily unitFamily, SwSiUnit unit) ;


			//////////////////////////////////////////////////////////////////////////
			/////////////////////// CONVERSION SIMPLE VALUE //////////////////////////
			////////////// USED FOR ALTITUDE, ANGLE, DISTANCE, VITESSE////////////////

			/**
			* @brief	: Convert all unit in a given family unit in a conversion map 
			*			  from base value (cf. sw_unit.h)
			* @param	: SwSiUnitFamily - the family unit
			* @param	: QHash<SwSiUnit, QString> - map with unit and converted value
			* @return	: bool - conversion status
			*/
			virtual bool convertDefaultUnit(SwSiUnitFamily typeSI, QHash<SwSiUnit, QString> & mapConversion) ;

			/**
			* @brief	: Convert the val from the unitFrom to the unitTo
			* @param	: double valFrom- the value to convert
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: SwSiUnit unitTo - the conversion unit asked
			* @param	: bool error - conversion status
			* @return	: double- the value converted
			*/
			virtual double convertUnit(double valFrom, SwSiUnit unitFrom, SwSiUnit unitTo, bool * error) ;

			/**
			* @brief	: Convert the val from the unitFrom to all other unit in a conversion map
			* @param	: double valFrom- the value to convert
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: QHash<SwSiUnit, QString> - map with unit and converted value
			* @return	: bool error - conversion status
			*/
			virtual bool convertUnits(QString valFrom, SwSiUnit unitFrom, QHash<SwSiUnit, QString> & mapConversion) ;


			//////////////////////////////////////////////////////////////////////////
			/////////////////////// CONVERSION COMPLEX VALUE /////////////////////////
			///////////////////////// USED FOR COORDINATES ///////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//	Remarque: - pour des unités de type "coordonnées"
			//			  - firstValFrom correspond à latitude pour DD,DM ou DMS / 
			//				correspond à la valeur entière pour MGRS, GEOREF ou UTM ZONE
			//			  - firstValFrom correspond à longitude pour DD,DM ou DMS / sinon est vide
			//
			//	QPair<QString, QString> : - Premier QString latitude pour DD,DM ou DMS / correspond à la valeur entière 
			//								pour MGRS, GEOREF ou UTM ZONE
			//							  - Second QString longitude pour DD,DM ou DMS / sinon est vide

			/**
			* @brief	: Convert all unit in a given family unit in a conversion map 
			*			  from base value (cf. sw_unit.h)
			* @param	: SwSiUnitFamily - the family unit
			* @param	: QMap<SwSiUnit,QPair<QString, QString>> - map with unit, and converted value (latitude/longitude)
			* @return	: bool - conversion status
			*/
			virtual bool convertDefaultComplexUnit(SwSiUnitFamily typeSI, QMap<SwSiUnit,QPair<QString, QString>> & mapConversion);

			/**
			* @brief	: Set in firstValTo and secondValueTo conversion result from reset unit (Degree Decimal) to unitTo
			* @param	: SwSiUnit unitTo - the conversion unit asked
			* @param	: QString & firstValTo - the value converted (depend if unit need two values)
			* @param	: QString & secondValTo - the value converted (depend if unit need two values)
			* @return	: bool - conversion status
			*/
			virtual bool resetComplexUnit(SwSiUnit unitTo,QString & firstValTo, QString & secondValTo);
			/**
			* @brief	: Convert the firstValFrom and secondValFrom from the unitFrom to the unitTo in a conversion map
			* @param	: QString firstValFrom- the value to convert (depend if unit need two values)
			* @param	: QString secondValFrom - the value to convert (depend if unit need two values)
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: SwSiUnit unitTo - the conversion unit asked
			* @param	: QMap<SwSiUnit,QPair<QString, QString>> - map with unit, and converted value (latitude/longitude)
			* @return	: bool - conversion status
			*/
			virtual bool convertComplexUnit(QString firstValFrom, QString secondValFrom, SwSiUnit unitFrom, SwSiUnit unitTo, 
											QMap<SwSiUnit,QPair<QString, QString>> & mapConversion)  throw(SwException);

			/**
			* @brief	: Convert the firstValFrom and secondValFrom from the unitFrom to all other unit in a conversion map
			* @param	: QString firstValFrom- the value to convert (depend if unit need two values)
			* @param	: QString secondValFrom - the value to convert (depend if unit need two values)
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: QMap<SwSiUnit,QPair<QString, QString>> - map with unit, and converted value (latitude/longitude)
			* @return	: bool error - conversion status
			*/
			virtual bool convertComplexUnits(QString firstValFrom, QString secondValFrom, SwSiUnit unitFrom, 
											 QMap<SwSiUnit,QPair<QString, QString>> & mapConversion) ;


			//////////////////////////////////////////////////////////////////////////
			///////////////////////////// INFORMATIONS ///////////////////////////////
			//////////// USED FOR ALL FAMILY UNIT (SIMPLE AND COMPLEX) ///////////////

			/**
			* @brief	: Return the current precision used for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: int - current precision used in the service (precision is a number means number of decimal)
			*/
			virtual int getCurrentPrecision(SwSiUnitFamily typeSI) ;

			/**
			* @brief	: Return the current unit used for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: SwSiUnit - current unit used in the service 
			*/
			virtual SwSiUnit getCurrentUnit(SwSiUnitFamily typeSI) ;

			/**
			* @brief	: Return the default unit used for a given family unit in Starlinx
			* @param	: SwSiUnitFamily - the family unit
			* @return	: SwSiUnit - current unit used by Starlinx 
			*/
			virtual SwSiUnit getDefaultUnit(SwSiUnitFamily typeSI) ;
			
			/**
			* @brief	: Return the maximum value for an unit
			* @param	: SwSiUnit - the unit
			* @return	: double - maximum value 
			*/
			virtual double getMaximumValue(SwSiUnit unit) ;

			/**
			* @brief	: Return the minimum value for an unit
			* @param	: SwSiUnit - the unit
			* @return	: double - minimum value 
			*/
			virtual double getMinimumValue(SwSiUnit unit) ;

			/**
			* @brief	: Return the string name family value for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: QString - string name family
			*/
			virtual QString getNameFamilyUnit(SwSiUnitFamily familyUnitSI) ;
			
			/**
			* @brief	: Return the given family unit for a string family unit
			* @param	: QString - string name family
			* @return	: SwSiUnitFamily - the family unit
			*/
			virtual SwSiUnitFamily getFamilyUnitName(QString familyUnitSI);

			/**
			* @brief	: Return the string name unit value for a given unit
			* @param	: SwSiUnit - the unit
			* @return	: QString - string name unit
			*/
			virtual QString getNameUnit(SwSiUnit unitSI) ;

			/**
			* @brief	: Return the given unit for a string name unit
			* @param	: QString - the string name unit
			* @return	: SwSiUnit - the unit
			*/
			virtual SwSiUnit getUnitName(QString unitName) ;
			
			/**
			* @brief	: Return the given unit for a string name unit suffix
			* @param	: QString - the string name unit
			* @return	: SwSiUnit - the unit
			*/
			virtual SwSiUnit getUnitNameSuffix(QString unitSuffixName);

			/**
			* @brief	: Return the string name suffix for a given unit
			* @param	: SwSiUnit - the unit
			* @return	: QString - the suffix name unit
			*/
			virtual QString getSuffix(SwSiUnit unit) ;
				
			/**
			* @brief	: Return a unit list for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: QList<SwSiUnit> - unit list
			*/
			virtual QList<SwSiUnit> getUnitListFamily(SwSiUnitFamily familyUnitSI);

			/**
			* @brief	: Return if the unit is inthe family unit SI
			* @param	: SwSiUnitFamily - the family unit
			* @return	: SwSiUnit - the unit
			*/
			bool isUnitFromFamily(SwSiUnitFamily familyUnitSI, SwSiUnit unit);

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
			QMultiMap<SwSiUnitFamily,ISwUnitSI*> _mapListenersSIType;

			//current unit for a family
			QHash<SwSiUnitFamily,SwSiUnit> _default; 

			//current precision for a family
			QHash<SwSiUnitFamily,int> _defaultPrecision; 

			//current precision for a family
			QHash<SwSiUnitFamily,SwSiUnit> _defaultStarlinxUnit; 


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
