///////////////////////////////////////////////////////////
//  SwSiEnums.h
//  Implementation of the Class SwSiEnums
//  Created on:      31-déc.-2013 14:30:16
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//				Only used for simple conversion
// Complex conversion are converted with the library GeographicLib
////////////////////////////////////////////////////////////////////

#ifndef STREAMWORK_SWCORE_SWSIENUMS_H
#define STREAMWORK_SWCORE_SWSIENUMS_H

namespace StreamWork 
{    
	namespace SwCore 
	{		
		//CONVERT TYPE
		typedef enum
		{
			/** the unit itself is a base unit f.i.: 
			*	meter 
			*/
			swConvBase				= 0, 
			/** form of converting equation: 
			*	Unit = factor*Base+offset; 
			*	Base = (Unit-offset) / factor 
			*/
			swConvLinear			= 1, 
			/** form of converting equation: 
			*	Unit = factor / Base; 
			*	Base = factor / Unit 
			*/		
			swConvRecipr			= 2,
			/** special coordinate converting equation: 
			*	cf. geographiclib
			*/  
			swConvCoordinates		= 3
		} SwConvType;

		//UNIT DATA
		//enum 's SwSiUnit creation with all unit present in sw_units.h
		typedef enum
		{
			#ifdef  SW_SI_UNIT
			#undef  SW_SI_UNIT
			#endif
			#define SW_SI_UNIT(key,name,suffix,base,conv,factor,valueBase,valueSecondBase,minValue,maxValue) SwSiUnit##key,
			#include "sw_units.h"
			SwSiUnitNum,
			SwSiUnitNull
		} SwSiUnit;

		//Struct's unit creation with all unit present in sw_units.h
		typedef struct
		{
			SwSiUnit		key;					/**< Primary key */
			const char		*keyname;				/**< String contains primary key name */
			const char		*name;					/**< Name (localization string) */
			const char		*suffix;				/**< String contains the suffix of the unit */
			SwSiUnit		baseunit;				/**< Base of unit group that the unit belongs to */
			SwConvType		conversion;				/**< Base unit conversion */
			double			factor;					/**< Constant for conversion */
			double			valueBase;				/**< Default value for first conversion*/
			double			valueSecondBase;		/**< Second value for first conversion*/
			double			minValue;				/**< Minimum system value conversion*/
			double			maxValue;				/**< Maximum system value conversion*/
		} sUnit;

		/** unit list*/
		extern const sUnit units[SwSiUnitNum];

		/**
		 * @brief	: Find the unit with a name (cf. name in sw_units.h)
		 * @param	: char* keyname - the name searched
		 * @return	: SwSiUnit - the unit (if not found swSiUnitNull is returned)
		 */
		extern SwSiUnit unitID(char* keyname);

		/**
		 * @brief	: Convert the inValue from the unitFrom to the unitTo
		 * @param	: double - the value converted
		 * @param	: SwSiUnit inUnit - the previously unit
		 * @param	: SwSiUnit outUnit - the conversion unit asked
		 * @return	: bool error - conversion status
		 */
		extern double unitConvert(double inValue, SwSiUnit inUnit, SwSiUnit outUnit, bool *error);


		//FAMILY DATA
		//enum 's SwSiUnitFamily creation with all family present in sw_unitsFamily.h
		#ifdef  SW_SI_UNIT_FAMILY
		#undef  SW_SI_UNIT_FAMILY
		#endif
		#define SW_SI_UNIT_FAMILY(key,name,base,precision,baseStarlinx) SwSiUnitFamily##key,
		typedef enum
		{
			#include "sw_unitFamily.h"
			SwSiUnitFamilyNum,
			SwSiUnitFamilyNull
		} SwSiUnitFamily;

		//Struct's family unit creation with all family unit present in sw_units.h
		typedef struct
		{
			SwSiUnitFamily		 key;					/**< Primary key				*/
			const char*			name;					/**< Name (localization string) */
			SwSiUnit			base;					/**< Base unit of the family	*/
			int					precision;				/**< value's precision			*/
			SwSiUnit			baseStarlinx;			/**< Base unit of the family	*/
		} sUnitFamily;

		/** family unit list*/
		extern const sUnitFamily unitFamilies[SwSiUnitFamilyNum];

		/**
		 * @brief	: Find the family associated with an unit
		 * @param	: SwSiUnit - the unit searched
		 * @return	: SwSiUnitFamily - the family founded (if not swSiUnitFamilyNull is returned)
		 */
		extern SwSiUnitFamily unitSiFamilyID(SwSiUnit unit);

		/**
		 * @brief	: Return an unit list with all unit include for a given family
		 * @param	: SwSiUnitFamily - the family unit searched
		 * @return	: QList<SwSiUnit> - unit list
		 */
		extern QList<SwSiUnit> unitSiListFamily( SwSiUnitFamily unitFamily);

		/**
		 * @brief	: Return the base unit for a given family unit
		 * @param	: SwSiUnitFamily - the family unit searched
		 * @return	: SwSiUnit - base unit
		 */
		SwSiUnit findBaseOfUnit( SwSiUnitFamily unitFamily);


	}
}
#endif // STREAMWORK_SWCORE_SWSIENUMS_H
