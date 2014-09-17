///////////////////////////////////////////////////////////
//  _SwServiceUnitSI.cpp
//  Implementation of the Class _SwServiceUnitSI
//  Created on:      31-déc.-2013 11:17:05
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////

#include "_SwServiceUnitSI.h"
#include "ExceptionManager.h"
#include <GeographicLib/GeoCoords.hpp>

using namespace StreamWork::SwCore;
using namespace GeographicLib;

//-------------------------------------------------------------------------
_SwServiceUnitSI::_SwServiceUnitSI()
{
	_mapListenersSIType.clear();
	int family;
	for (family = 0; family < SwSiUnitFamilyNum; family++)
	{
		_default[(SwSiUnitFamily)family] = unitFamilies[family].base;
		_defaultPrecision[(SwSiUnitFamily)family] = unitFamilies[family].precision;
	}

	//Valeur d'init du service de configuration
	_default[SwSiUnitFamilyAltitude]			= SwSiUnitAltFeet;
	_default[SwSiUnitFamilySpeed]				= SwSiUnitMeterPerSecond;
	_default[SwSiUnitFamilyDistance]			= SwSiUnitNauticalMile;
	_default[SwSiUnitFamilyFrequency]			= SwSiUnitHertz;
	_default[SwSiUnitFamilyAngle]				= SwSiUnitDegree;
	_default[SwSiUnitFamilyCoordinates]			= SwSiUnitCoordDegreeMinuteSecond;
	_default[SwSiUnitFamilyDuration]			= SwSiUnitTimeSecond;

	_defaultPrecision[SwSiUnitFamilyAltitude]	 = 0;
	_defaultPrecision[SwSiUnitFamilySpeed]		 = 0;
	_defaultPrecision[SwSiUnitFamilyDistance]	 = 0;
	_defaultPrecision[SwSiUnitFamilyFrequency]	 = 2;
	_defaultPrecision[SwSiUnitFamilyAngle]		 = 1;
	_defaultPrecision[SwSiUnitFamilyDuration]	 = 0;

	//Add family unit enum and precision  => PROPERTIES 
	QStringList listPrecision = QStringList() << "1" << "0.1" << "0.01";
	for(int nbPrec = 0; nbPrec <listPrecision.size(); nbPrec++)
	{
		altitudePrecision.AddKey(nbPrec, listPrecision.at(nbPrec));
		anglePrecision.AddKey(nbPrec, listPrecision.at(nbPrec));
		distancePrecision.AddKey(nbPrec, listPrecision.at(nbPrec));
		speedPrecision.AddKey(nbPrec, listPrecision.at(nbPrec));
		durationPrecision.AddKey(nbPrec, listPrecision.at(nbPrec));
		frequencyPrecision.AddKey(nbPrec, listPrecision.at(nbPrec));
	}

	QList<SwSiUnit> altitudeList = getUnitListFamily(SwSiUnitFamilyAltitude);
	foreach(SwSiUnit alt, altitudeList)
		altitudeUnits.AddKey((int)alt,units[(int)alt].suffix);

	QList<SwSiUnit> angleList = getUnitListFamily(SwSiUnitFamilyAngle);
	foreach(SwSiUnit angle, angleList)
		angleUnits.AddKey((int)angle,units[(int)angle].suffix);

	QList<SwSiUnit> distanceList = getUnitListFamily(SwSiUnitFamilyDistance);
	foreach(SwSiUnit distance, distanceList)
		distanceUnits.AddKey((int)distance,units[(int)distance].suffix);

	QList<SwSiUnit> speedList = getUnitListFamily(SwSiUnitFamilySpeed);
	foreach(SwSiUnit speed, speedList)
		speedUnits.AddKey((int)speed,units[(int)speed].suffix);

	QList<SwSiUnit> durationList = getUnitListFamily(SwSiUnitFamilyDuration);
	foreach(SwSiUnit durationL, durationList)
		durationUnits.AddKey((int)durationL,units[(int)durationL].suffix);

	QList<SwSiUnit> frequencyList = getUnitListFamily(SwSiUnitFamilyFrequency);
	foreach(SwSiUnit frequency, frequencyList)
		frequencyUnits.AddKey((int)frequency,units[(int)frequency].suffix);

	QList<SwSiUnit> coordinateList = getUnitListFamily(SwSiUnitFamilyCoordinates);
	foreach(SwSiUnit coordinate, coordinateList)
		coordinateUnits.AddKey((int)coordinate,units[(int)coordinate].suffix);
}

//-------------------------------------------------------------------------
_SwServiceUnitSI::~_SwServiceUnitSI()
{
	QMap<SwSiUnitFamily, ISwUnitSI *>::iterator iter = _mapListenersSIType.begin();
	while (iter != _mapListenersSIType.end()) 
		iter = _mapListenersSIType.erase(iter);
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::registerTypeSI(SwSiUnitFamily unitFamily, ISwUnitSI * obj)
{
	if(obj && (unitFamily != SwSiUnitFamilyNull))
		_mapListenersSIType.insert(unitFamily, obj);

}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::unregisterTypeSI(SwSiUnitFamily unitFamily, ISwUnitSI * obj)
{
	if (obj && (unitFamily != SwSiUnitFamilyNull))
		_mapListenersSIType.remove(unitFamily, obj);
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::changeUnitSI(SwSiUnitFamily unitFamily, SwSiUnit unit)
{
	EXCEPTION_TRY();
	{
		if (isUnitFromFamily(unitFamily, unit))
		{
			_default[unitFamily] = unit;
			QMap<SwSiUnitFamily, ISwUnitSI *>::const_iterator i = _mapListenersSIType.find(unitFamily);
			while (i != _mapListenersSIType.end() && i.key() == unitFamily) 
			{
				//notifie les wigdet de type unitFamily
				if (i.value())
					i.value()->processUnitChanged(unit);
				++i;
			}
		}
		else
		{
			//ERROR
		}
	}
	EXCEPTION_CATCH();
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::changePrecisionUnitSI(SwSiUnitFamily unitFamily, int precision)
{
	EXCEPTION_TRY();
	{
		_defaultPrecision[unitFamily] = precision;
		QMap<SwSiUnitFamily, ISwUnitSI *>::const_iterator i = _mapListenersSIType.find(unitFamily);
		while (i != _mapListenersSIType.end() && i.key() == unitFamily) 
		{
			//notifie les wigdet de type SI : "type"
			i.value()->processPrecisionChanged(precision);
			++i;
		}
	}
	EXCEPTION_CATCH();
}

//-------------------------------------------------------------------------
double _SwServiceUnitSI::convertUnit(double valFrom, SwSiUnit unitFrom, SwSiUnit unitTo, bool * error)
{
	return unitConvert(valFrom, unitFrom, unitTo, error);
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::convertUnits(QString valFrom, SwSiUnit unitFrom, QHash<SwSiUnit, QString> & mapConversion)
{
	QHash<SwSiUnit, QString>::iterator i = mapConversion.begin();
	while (i != mapConversion.end()) 
	{
		bool error;
		if (i.key() != unitFrom)
		{
			double result = unitConvert(valFrom.toDouble(), unitFrom, i.key(), &error);
			if (!error)
				i.value() = QString::number(result,'g',20);
			else
				return false;
		}
		++i;
	}
	return true;
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::convertDefaultUnit(SwSiUnitFamily typeSI, QHash<SwSiUnit, QString> & mapConversion)
{
	//use only simple unit 
	if (typeSI == SwSiUnitFamilyCoordinates)
		return false;

	//do this at start to initialize map 

	//LookUp for all unit of given family unit
	QList<SwSiUnit> listUnit = unitSiListFamily(typeSI);

	//LookUp for base unit of given family unit
	SwSiUnit unitSIBase = findBaseOfUnit(typeSI);

	int index = listUnit.indexOf(unitSIBase);
	if (index != -1)
	{
		//find base value to convert this to other unit
		double valueBase = units[unitSIBase].valueBase;
		if (valueBase != 0)
		{
			for (int nbUnit = 0; nbUnit < listUnit.size(); ++nbUnit)
			{
				bool error;
				double result = convertUnit(valueBase, unitSIBase, listUnit.at(nbUnit), &error);
				if (!error)
					mapConversion.insert(listUnit.at(nbUnit), QString::number(result,'g',20));
				else
					return false;
			}
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::convertComplexUnit(QString firstValFrom, QString secondValFrom,SwSiUnit unitFrom, SwSiUnit unitTo,
										  QMap<SwSiUnit,QPair<QString, QString>> & mapConversion) throw(SwException)
{
	QString result;
	bool valueDouble = false;

	//conversion to base value
	if (units[unitFrom].key != SwSiUnitNull)
	{
		//Choix des valeurs à garder

		QString coordLatLong = firstValFrom;
		switch(unitFrom)
		{
		case SwSiUnitCoordDegreeDecimal:
		case SwSiUnitCoordDegreeMinute:
		case SwSiUnitCoordDegreeMinuteSecond:
			if (!secondValFrom.isEmpty())
				coordLatLong+= " " + secondValFrom; //chaine de caractère contenant "lat long"
			else
				return false;
			break;
		case SwSiUnitCoordUTM:
		case SwSiUnitCoordMGRS:
		case SwSiUnitCoordGEOREF:
			break;
		default:return false;
		}

		GeoCoords coord;
		try
		{
			coord.Reset(coordLatLong.toStdString());
		}
		catch(GeographicErr & err)
		{
			QString msg=QString(err.what());
			LAUNCH_SWEXCEPTION("Convertor",msg)     
		}

		switch(unitTo)
		{
		case SwSiUnitCoordDegreeDecimal:
			result = QString::fromLatin1(coord.DDRepresentation(5).c_str());
			valueDouble = true;
			break;
		case SwSiUnitCoordDegreeMinute:
			result = QString::fromLatin1(coord.DMRepresentation(4).c_str());
			valueDouble = true;
			break;
		case SwSiUnitCoordDegreeMinuteSecond:
			result = QString::fromLatin1(coord.DMSRepresentation(1).c_str());
			valueDouble = true;
			break;
		case SwSiUnitCoordUTM:
			result = QString::fromLatin1(coord.UTMUPSRepresentation().c_str());
			break;
		case SwSiUnitCoordMGRS:
			result = QString::fromLatin1(coord.MGRSRepresentation().c_str());
			break;
		case SwSiUnitCoordGEOREF:
			result = QString::fromLatin1(coord.GEOREFRepresentation(5).c_str());
			break;
		default:return false;
		}
	}

	QStringList resultCut;

	if (valueDouble)
		resultCut = result.split(" ");
	else
	{
		resultCut.append(result);
		resultCut.append("");
	}

	if (resultCut.size() == 2) //lat et long values
	{
		QPair<QString, QString> p(resultCut.at(0), resultCut.at(1));
		mapConversion.insert(unitTo,p);
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::convertComplexUnits(QString firstValFrom, QString secondValFrom,SwSiUnit unitFrom,
										   QMap<SwSiUnit,QPair<QString, QString>> & mapConversion)
{
	QMap<SwSiUnit,QPair<QString, QString>>::iterator iMap = mapConversion.begin();
	while (iMap != mapConversion.end()) 
	{
		if (iMap.key() != unitFrom)
		{
			QMap<SwSiUnit,QPair<QString, QString>> mapConversionTemp;
			if (convertComplexUnit(firstValFrom, secondValFrom, unitFrom, iMap.key(), mapConversionTemp))
			{
				QMap<SwSiUnit,QPair<QString, QString>>::const_iterator i = mapConversionTemp.constBegin();
				if (i != mapConversionTemp.constEnd()) 
				{
					iMap.value().first = i.value().first;
					iMap.value().second = i.value().second;
				}
				else
					return false;
			}
			else
				return false;
		}
		++iMap;
	}
	return true; 
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::convertDefaultComplexUnit(SwSiUnitFamily typeSI, QMap<SwSiUnit,QPair<QString, QString>> & mapConversion)
{
	//do this at start to initialize map 

	//LookUp for all unit of given family unit
	QList<SwSiUnit> listUnit = unitSiListFamily(typeSI);

	//LookUp for base unit of given family unit
	SwSiUnit unitSIBase = findBaseOfUnit(typeSI);

	int index = listUnit.indexOf(unitSIBase);
	if (index != -1)
	{
		//find base value to convert this to other unit
		double firstValueBase = units[unitSIBase].valueBase;
		double secondValueBase = units[unitSIBase].valueSecondBase;

		if (firstValueBase != 0)
		{
			for (int nbUnit = 0; nbUnit < listUnit.size(); ++nbUnit)
			{
				QMap<SwSiUnit,QPair<QString, QString>> mapConversionUnit;
				if (convertComplexUnit(QString::number(firstValueBase), QString::number(secondValueBase), unitSIBase, listUnit.at(nbUnit), mapConversionUnit))
				{
					QMap<SwSiUnit,QPair<QString, QString>>::const_iterator i = mapConversionUnit.constBegin();
					if (i != mapConversionUnit.constEnd()) 
						mapConversion.insert(i.key(), i.value());
					else
						return false;
				}
			}
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::resetComplexUnit(SwSiUnit unitTo,QString & firstValTo, QString & secondValTo) throw(SwException)
{
	//reset complex value consists to convert the default values unit complex ( lat = 0° and lon = 0°)
	//to unitTo

	QMap<SwSiUnit,QPair<QString, QString>> mapResetUnit;
	if (convertComplexUnit("0","0", SwSiUnitCoordDegreeDecimal, unitTo, mapResetUnit))
	{
		QMap<SwSiUnit,QPair<QString, QString>>::const_iterator i = mapResetUnit.constBegin();
		if (i != mapResetUnit.constEnd()) 
		{
			firstValTo = i.value().first;
			secondValTo = i.value().second;

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

//-------------------------------------------------------------------------
int _SwServiceUnitSI::getCurrentPrecision(SwSiUnitFamily typeSI)
{
	QHash<SwSiUnitFamily, int>::iterator iter = _defaultPrecision.find(typeSI);
	if (iter != _defaultPrecision.end() && iter.key() == typeSI) 
	{
		return iter.value();
	}
	else
		return -1; //ERROR
}

//-------------------------------------------------------------------------
SwSiUnit _SwServiceUnitSI::getCurrentUnit(SwSiUnitFamily typeSI)
{
	return _default[typeSI];
}

//-------------------------------------------------------------------------
SwSiUnit _SwServiceUnitSI::getDefaultUnit(SwSiUnitFamily typeSI)
{
	return unitFamilies[typeSI].baseStarlinx;
}

//-------------------------------------------------------------------------
QString _SwServiceUnitSI::getSuffix(SwSiUnit unit)
{
	return units[unit].suffix;
}

//-------------------------------------------------------------------------
double _SwServiceUnitSI::getMinimumValue(SwSiUnit unit)
{
	return units[unit].minValue;
}

//-------------------------------------------------------------------------
double _SwServiceUnitSI::getMaximumValue(SwSiUnit unit)
{ 
	return units[unit].maxValue;
}

//-------------------------------------------------------------------------
QString _SwServiceUnitSI::getNameFamilyUnit(SwSiUnitFamily familyUnitSI )
{
	return unitFamilies[familyUnitSI].name;
}

//-------------------------------------------------------------------------
SwSiUnitFamily _SwServiceUnitSI::getFamilyUnitName(QString familyUnitSI)
{
	for (int nbFamilyUnit = 0; nbFamilyUnit < SwSiUnitFamilyNum; nbFamilyUnit++)
	{
		if (unitFamilies[(SwSiUnitFamily)nbFamilyUnit].name == familyUnitSI)
			return (SwSiUnitFamily)nbFamilyUnit; 
	}
	return SwSiUnitFamilyNull;
}

//-------------------------------------------------------------------------
QString _SwServiceUnitSI::getNameUnit(SwSiUnit unitSI )
{
	return units[unitSI].name;
}

//-------------------------------------------------------------------------
SwSiUnit _SwServiceUnitSI::getUnitName(QString unitName)
{
	for (int nbUnit = 0; nbUnit < SwSiUnitNum; nbUnit++)
	{
		if (units[(SwSiUnit)nbUnit].name == unitName)
			return (SwSiUnit)nbUnit; 
	}
	return SwSiUnitNull; 
}

//-------------------------------------------------------------------------
SwSiUnit _SwServiceUnitSI::getUnitNameSuffix(QString unitSuffixName)
{
	for (int nbUnit = 0; nbUnit < SwSiUnitNum; nbUnit++)
	{
		if (units[(SwSiUnit)nbUnit].suffix == unitSuffixName)
			return (SwSiUnit)nbUnit; 
	}
	return SwSiUnitNull;
}

//-------------------------------------------------------------------------
QList<SwSiUnit> _SwServiceUnitSI::getUnitListFamily(SwSiUnitFamily familyUnitSI)
{
	return unitSiListFamily(familyUnitSI);
}

//-------------------------------------------------------------------------
bool _SwServiceUnitSI::isUnitFromFamily(SwSiUnitFamily familyUnitSI, SwSiUnit unit)
{
	QList<SwSiUnit> listUnit = unitSiListFamily(familyUnitSI);
	if (listUnit.indexOf(unit) != -1)
		return true;
	return false;
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Altitude()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilyAltitude);
	altitudeUnits.FromInt(tmpval);
	return altitudeUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Altitude(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilyAltitude, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Angle()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilyAngle);
	angleUnits.FromInt(tmpval);
	return angleUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Angle(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilyAngle, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Speed()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilySpeed);
	speedUnits.FromInt(tmpval);
	return speedUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Speed(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilySpeed, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Distance()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilyDistance);
	distanceUnits.FromInt(tmpval);
	return distanceUnits;
}
//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Distance(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilyDistance, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Duration()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilyDuration);
	durationUnits.FromInt(tmpval);
	return durationUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Duration(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilyDuration, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Frequency()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilyFrequency);
	frequencyUnits.FromInt(tmpval);
	return frequencyUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Frequency(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilyFrequency, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Coordinates()
{
	uint tmpval = getCurrentUnit(SwSiUnitFamilyCoordinates);
	coordinateUnits.FromInt(tmpval);
	return coordinateUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Coordinates(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	changeUnitSI(SwSiUnitFamilyCoordinates, (StreamWork::SwCore::SwSiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Altitude()
{
	uint tmpval = getCurrentPrecision(SwSiUnitFamilyAltitude);
	altitudePrecision.FromInt(tmpval);
	return altitudePrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Altitude(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	changePrecisionUnitSI(SwSiUnitFamilyAltitude, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Angle()
{
	uint tmpval = getCurrentPrecision(SwSiUnitFamilyAngle);
	anglePrecision.FromInt(tmpval);
	return anglePrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Angle(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	changePrecisionUnitSI(SwSiUnitFamilyAngle, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Speed()
{
	uint tmpval = getCurrentPrecision(SwSiUnitFamilySpeed);
	speedPrecision.FromInt(tmpval);
	return speedPrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Speed(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	changePrecisionUnitSI(SwSiUnitFamilySpeed, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Distance()
{
	uint tmpval = getCurrentPrecision(SwSiUnitFamilyDistance);
	distancePrecision.FromInt(tmpval);
	return distancePrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Distance(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	changePrecisionUnitSI(SwSiUnitFamilyDistance, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Duration()
{
	uint tmpval = getCurrentPrecision(SwSiUnitFamilyDuration);
	durationPrecision.FromInt(tmpval);
	return durationPrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Duration(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	changePrecisionUnitSI(SwSiUnitFamilyDuration, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Frequency()
{
	uint tmpval = getCurrentPrecision(SwSiUnitFamilyFrequency);
	frequencyPrecision.FromInt(tmpval);
	return frequencyPrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Frequency(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	changePrecisionUnitSI(SwSiUnitFamilyFrequency, tempUnit.ToInt());
}