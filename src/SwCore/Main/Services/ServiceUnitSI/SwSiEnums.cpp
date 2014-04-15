///////////////////////////////////////////////////////////
//  SwSiEnums.cpp
//  Implementation of the Class SwSiEnums
//  Created on:      31-déc.-2013 14:30:17
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////

#include "SwSiEnums.h"
#include <exception>
#include <string.h>

using namespace StreamWork::SwCore;

/*********************************/
/*** Array of unit definitions ***/
/*********************************/


/******** UNIT ***********/
const StreamWork::SwCore::sUnit StreamWork::SwCore::units[SwSiUnitNum] = 
{
#define QT_TR_NOOP(name) name
#ifdef  SW_SI_UNIT
#undef  SW_SI_UNIT
#endif
#define SW_SI_UNIT(key,name,suffix,base,conv,factor,valueBase,valueSecondBase,minValue,maxValue) { SwSiUnit##key, #key, name,suffix, SwSiUnit##base, swConv##conv, (factor), valueBase, valueSecondBase,minValue , maxValue},
#include "sw_units.h"
};

/******** FAMILY **********/
const StreamWork::SwCore::sUnitFamily StreamWork::SwCore::unitFamilies[SwSiUnitFamilyNum] = 
{
#ifdef  SW_SI_UNIT_FAMILY
#undef  SW_SI_UNIT_FAMILY
#endif
#define SW_SI_UNIT_FAMILY(key,name,base,precision,baseStarlinx) { SwSiUnitFamily##key, name, SwSiUnit##base, precision, SwSiUnit##base},
#include "sw_unitFamily.h"
};

//-----------------------------------------------------------------------
SwSiUnitFamily unitSiFamilyID( SwSiUnit unit )
{
	int family;

	for (family = 0; family < SwSiUnitFamilyNum; family++)
	{
		if (unitFamilies[family].base == units[unit].baseunit)
		{
			return((SwSiUnitFamily)family);
		}
	}
	return(SwSiUnitFamilyNull);
}

//-----------------------------------------------------------------------
 QList<SwSiUnit> StreamWork::SwCore::unitSiListFamily( SwSiUnitFamily unitFamily )
{
	QList<SwSiUnit> listUnit;
	int unit;

	for (unit = 0; unit < SwSiUnitNum; unit++)
	{
		if (units[unit].baseunit == unitFamilies[unitFamily].base)
			listUnit.append(units[unit].key);
	}
	return listUnit;
}

//-----------------------------------------------------------------------
SwSiUnit unitID(char* keyname)
{
	SwSiUnit result = SwSiUnitNull;
	int unit;

	for (unit = 0; unit < SwSiUnitNum; unit++)
	{
		if (strcmp(units[unit].keyname, keyname) == 0)
		{
			result = (SwSiUnit)unit;
		}
	} 
	return(result);
}

//-----------------------------------------------------------------------
SwSiUnit StreamWork::SwCore::findBaseOfUnit(SwSiUnitFamily unitFamily)
{
	SwSiUnit result = SwSiUnitNull;
	int unit;

	for (unit = 0; unit < SwSiUnitNum; unit++)
	{
		if ((units[unit].baseunit == unitFamilies[unitFamily].base) && units[unit].conversion == swConvBase)
			return units[unit].key;
	}
	return(result);
}

//-----------------------------------------------------------------------
double StreamWork::SwCore::unitConvert(double inValue, SwSiUnit inUnit, SwSiUnit outUnit, bool *error)
{
	double outValue;
	double baseValue;

	/* error handling */ 
	if (error != NULL)
	{
		*error = false;
		//Si base name différente, pas de conversion possibleè
		if (units[inUnit].baseunit != units[outUnit].baseunit)
		{
			*error = true;
			throw std::exception(QString("Unable to convert "+QString(units[inUnit].keyname)+" to "+QString(units[outUnit].keyname)).toStdString().c_str());
			return(0);
		}
	}

	/* Convert from original unit to base unit */
	switch (units[inUnit].conversion)
	{
	case swConvBase:       { baseValue = inValue;                                                  break; }  
	case swConvLinear:     { baseValue = inValue / units[inUnit].factor; break; }  
	case swConvRecipr:     { baseValue = units[inUnit].factor / inValue;                           break; } 
	default: break;
	} 

	/* Convert from base unit to target unit */
	switch (units[outUnit].conversion)
	{
	case swConvBase:       { outValue = baseValue;                                                 break; }  
	case swConvLinear:     { outValue = baseValue * units[outUnit].factor; break; }  
	case swConvRecipr:     { outValue = units[outUnit].factor / baseValue;                         break; } 
	default: break;
	} 

	return outValue;
}