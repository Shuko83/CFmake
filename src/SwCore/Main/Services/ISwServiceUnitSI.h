///////////////////////////////////////////////////////////
//  ISwServiceUnitSI.h
//  Implementation of the Class ISwServiceUnitSI
//  Created on:      31-déc.-2013 11:25:24
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////

#ifndef ISwServiceUnitSI_H
#define ISwServiceUnitSI_H

#include "ISwService.h"
#include "ISwUnitSI.h"

#include <QHash>

//Nom du service
#define CG_SW_SERVICE_UNIT_SI "ServiceUnitSI"

/**
 * ! \interface ISwServiceUnitSI \brief service de gestion des unités SI
 * @ingroup SwGuiGrp
 * @swinterface
 * cf. example sw::Gui::Common TestServiceSIComponent
 */

namespace StreamWork 
{    
	namespace SwCore 
	{
		class ISwServiceUnitSI : public ISwService
		{

		public:

			/**
			* @brief	: Registration with the Service for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			*/
			virtual void registerTypeSI(SwSiUnitFamily unitFamily, ISwUnitSI * obj) =0;

			/**
			* @brief	: Termination with the Service for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			*/
			virtual void unregisterTypeSI(SwSiUnitFamily unitFamily, ISwUnitSI * obj) =0;

			/**
			* @brief	: Notify all listener precision modification for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			*/
			virtual void changePrecisionUnitSI(SwSiUnitFamily unitFamily, int precision) =0;
			
			/**
			* @brief	: Notify all listener unit modification for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @param	: ISwUnitSI - the listener
			* @return	: void
			 */
			virtual void changeUnitSI(SwSiUnitFamily unitFamily, SwSiUnit unit) =0;


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
			virtual bool convertDefaultUnit(SwSiUnitFamily typeSI, QHash<SwSiUnit, QString> & mapConversion) =0;

			/**
			* @brief	: Convert the val from the unitFrom to the unitTo
			* @param	: double valFrom- the value to convert
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: SwSiUnit unitTo - the conversion unit asked
			* @param	: bool error - conversion status
			* @return	: double- the value converted
			*/
			virtual double convertUnit(double valFrom, SwSiUnit unitFrom, SwSiUnit unitTo, bool * error) =0;

			/**
			* @brief	: Convert the val from the unitFrom to all other unit in a conversion map
			* @param	: double valFrom- the value to convert
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: QHash<SwSiUnit, QString> - map with unit and converted value
			* @return	: bool error - conversion status
			*/
			virtual bool convertUnits(QString valFrom, SwSiUnit unitFrom, QHash<SwSiUnit, QString> & mapConversion) =0;


			//////////////////////////////////////////////////////////////////////////
			/////////////////////// CONVERSION COMPLEX VALUE /////////////////////////
			///////////////////////// USED FOR COORDINATES ///////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//	Remarque: - pour des unités de type "coordonnées"
			//			  - firstValFrom correspond à latitude pour DD,DM ou DMS / 
			//				correspond à la valeur entière pour MGRS, GEOREF ou UTM ZONE
			//			  - secondValFrom correspond à longitude pour DD,DM ou DMS / sinon est vide
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
			virtual bool convertDefaultComplexUnit(SwSiUnitFamily typeSI, QMap<SwSiUnit,QPair<QString, QString>> & mapConversion) =0;

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
											QMap<SwSiUnit,QPair<QString, QString>> & mapConversion) =0;

			/**
			* @brief	: Convert the firstValFrom and secondValFrom from the unitFrom to all other unit in a conversion map
			* @param	: QString firstValFrom- the value to convert (depend if unit need two values)
			* @param	: QString secondValFrom - the value to convert (depend if unit need two values)
			* @param	: SwSiUnit unitFrom - the previously unit
			* @param	: QMap<SwSiUnit,QPair<QString, QString>> - map with unit, and converted value (latitude/longitude)
			* @return	: bool error - conversion status
			*/
			virtual bool convertComplexUnits(QString firstValFrom, QString secondValFrom, SwSiUnit unitFrom, 
											 QMap<SwSiUnit,QPair<QString, QString>> & mapConversion) =0;

			/**
			* @brief	: Set in firstValTo and secondValueTo conversion result from reset unit (Degree Decimal) to unitTo
			* @param	: SwSiUnit unitTo - the conversion unit asked
			* @param	: QString & firstValTo - the value converted (depend if unit need two values)
			* @param	: QString & secondValTo - the value converted (depend if unit need two values)
			* @return	: bool - conversion status
			*/
			virtual bool resetComplexUnit(SwSiUnit unitTo,QString & firstValTo, QString & secondValTo) =0;


			//////////////////////////////////////////////////////////////////////////
			///////////////////////////// INFORMATIONS ///////////////////////////////
			//////////// USED FOR ALL FAMILY UNIT (SIMPLE AND COMPLEX) ///////////////

			/**
			* @brief	: Return the current precision used for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: int - current precision used in the service (precision is a number means number of decimal)
			*/
			virtual int getCurrentPrecision(SwSiUnitFamily typeSI) = 0;

			/**
			* @brief	: Return the current unit used for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: SwSiUnit - current unit used in the service 
			*/
			virtual SwSiUnit getCurrentUnit(SwSiUnitFamily typeSI) = 0;

			/**
			* @brief	: Return the default unit used for a given family unit in Starlinx
			* @param	: SwSiUnitFamily - the family unit
			* @return	: SwSiUnit - current unit used by Starlinx 
			*/
			virtual SwSiUnit getDefaultUnit(SwSiUnitFamily typeSI) = 0;
			
			/**
			* @brief	: Return the maximum value for an unit
			* @param	: SwSiUnit - the unit
			* @return	: double - maximum value 
			*/
			virtual double getMaximumValue(SwSiUnit unit) = 0;

			/**
			* @brief	: Return the minimum value for an unit
			* @param	: SwSiUnit - the unit
			* @return	: double - minimum value 
			*/
			virtual double getMinimumValue(SwSiUnit unit) = 0;

			/**
			* @brief	: Return the string name family value for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: QString - string name family
			*/
			virtual QString getNameFamilyUnit(SwSiUnitFamily familyUnitSI) = 0;

			/**
			* @brief	: Return the given family unit for a string family unit
			* @param	: QString - string name family
			* @return	: SwSiUnitFamily - the family unit
			*/
			virtual SwSiUnitFamily getFamilyUnitName(QString familyUnitSI) = 0;

			/**
			* @brief	: Return the string name unit value for a given unit
			* @param	: SwSiUnit - the unit
			* @return	: QString - string name unit
			*/
			virtual QString getNameUnit(SwSiUnit unitSI) = 0;

			/**
			* @brief	: Return the given unit for a string name unit
			* @param	: QString - the string name unit
			* @return	: SwSiUnit - the unit
			*/
			virtual SwSiUnit getUnitName(QString unitName) = 0;

			/**
			* @brief	: Return the given unit for a string name unit suffix
			* @param	: QString - the string name unit
			* @return	: SwSiUnit - the unit
			*/
			virtual SwSiUnit getUnitNameSuffix(QString unitSuffixName) = 0;

			/**
			* @brief	: Return the string name suffix for a given unit
			* @param	: SwSiUnit - the unit
			* @return	: QString - the suffix name unit
			*/
			virtual QString getSuffix(SwSiUnit unit) = 0;
				
			/**
			* @brief	: Return a unit list for a given family unit
			* @param	: SwSiUnitFamily - the family unit
			* @return	: QList<SwSiUnit> - unit list
			*/
			virtual QList<SwSiUnit> getUnitListFamily(SwSiUnitFamily familyUnitSI) = 0;


			//------------------------------------------------------------------
			// Interface ISwService
			//------------------------------------------------------------------
			virtual void Liberate(){};
			virtual QString GetServiceName(){return QString(CG_SW_SERVICE_UNIT_SI);}
			virtual QString GetServiceRealName(){return QString(typeid(this).name());}

		};
	}
}
#endif //ISwServiceUnitSI_H
