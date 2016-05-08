///////////////////////////////////////////////////////////
//  _SwServiceUnitSI.cpp
//  Implementation of the Class _SwServiceUnitSI
//  Created on:      31-déc.-2013 11:17:05
//  Original author: eulogia.pellegrino
///////////////////////////////////////////////////////////

#include "_SwServiceUnitSI.h"
#include <QsException>
#include <GeoCoords.hpp>

using namespace StreamWork::SwCore;
using namespace GeographicLib;

//-------------------------------------------------------------------------
_SwServiceUnitSI::_SwServiceUnitSI()
{
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

	QList<SiEnums::SiUnit> altitudeList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilyAltitude);
	foreach(SiEnums::SiUnit alt, altitudeList)
		altitudeUnits.AddKey((int)alt,SiEnums::units[(int)alt].suffix);

	QList<SiEnums::SiUnit> angleList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilyAngle);
	foreach(SiEnums::SiUnit angle, angleList)
		angleUnits.AddKey((int)angle, QString::fromLatin1(SiEnums::units[(int)angle].suffix));

	QList<SiEnums::SiUnit> distanceList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilyDistance);
	foreach(SiEnums::SiUnit distance, distanceList)
		distanceUnits.AddKey((int)distance,SiEnums::units[(int)distance].suffix);

	QList<SiEnums::SiUnit> speedList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilySpeed);
	foreach(SiEnums::SiUnit speed, speedList)
		speedUnits.AddKey((int)speed,SiEnums::units[(int)speed].suffix);

	QList<SiEnums::SiUnit> durationList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilyDuration);
	foreach(SiEnums::SiUnit durationL, durationList)
		durationUnits.AddKey((int)durationL,SiEnums::units[(int)durationL].suffix);

	QList<SiEnums::SiUnit> frequencyList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilyFrequency);
	foreach(SiEnums::SiUnit frequency, frequencyList)
		frequencyUnits.AddKey((int)frequency,SiEnums::units[(int)frequency].suffix);

	QList<SiEnums::SiUnit> coordinateList = ServiceUnitSI::getUnitListFamily(SiEnums::SiUnitFamilyCoordinates);
	foreach(SiEnums::SiUnit coordinate, coordinateList)
		coordinateUnits.AddKey((int)coordinate,SiEnums::units[(int)coordinate].suffix);
}

//-------------------------------------------------------------------------
_SwServiceUnitSI::~_SwServiceUnitSI()
{

}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Altitude()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilyAltitude);
	altitudeUnits.FromInt(tmpval);
	return altitudeUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Altitude(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilyAltitude, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Angle()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilyAngle);
	angleUnits.FromInt(tmpval);
	return angleUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Angle(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilyAngle, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Speed()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilySpeed);
	speedUnits.FromInt(tmpval);
	return speedUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Speed(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilySpeed, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Distance()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilyDistance);
	distanceUnits.FromInt(tmpval);
	return distanceUnits;
}
//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Distance(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilyDistance, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Duration()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilyDuration);
	durationUnits.FromInt(tmpval);
	return durationUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Duration(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilyDuration, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Frequency()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilyFrequency);
	frequencyUnits.FromInt(tmpval);
	return frequencyUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Frequency(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilyFrequency, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentUnitSI_Coordinates()
{
	uint tmpval = ServiceUnitSI::getCurrentUnit(SiEnums::SiUnitFamilyCoordinates);
	coordinateUnits.FromInt(tmpval);
	return coordinateUnits;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentUnitSI_Coordinates(SwEnum unitSI)
{
	SwEnum tempUnit = unitSI;
	ServiceUnitSI::changeUnitSI(SiEnums::SiUnitFamilyCoordinates, (SiEnums::SiUnit)tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Altitude()
{
	uint tmpval = ServiceUnitSI::getCurrentPrecision(SiEnums::SiUnitFamilyAltitude);
	altitudePrecision.FromInt(tmpval);
	return altitudePrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Altitude(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	ServiceUnitSI::changePrecisionUnitSI(SiEnums::SiUnitFamilyAltitude, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Angle()
{
	uint tmpval = ServiceUnitSI::getCurrentPrecision(SiEnums::SiUnitFamilyAngle);
	anglePrecision.FromInt(tmpval);
	return anglePrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Angle(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	ServiceUnitSI::changePrecisionUnitSI(SiEnums::SiUnitFamilyAngle, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Speed()
{
	uint tmpval = ServiceUnitSI::getCurrentPrecision(SiEnums::SiUnitFamilySpeed);
	speedPrecision.FromInt(tmpval);
	return speedPrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Speed(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	ServiceUnitSI::changePrecisionUnitSI(SiEnums::SiUnitFamilySpeed, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Distance()
{
	uint tmpval = ServiceUnitSI::getCurrentPrecision(SiEnums::SiUnitFamilyDistance);
	distancePrecision.FromInt(tmpval);
	return distancePrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Distance(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	ServiceUnitSI::changePrecisionUnitSI(SiEnums::SiUnitFamilyDistance, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Duration()
{
	uint tmpval = ServiceUnitSI::getCurrentPrecision(SiEnums::SiUnitFamilyDuration);
	durationPrecision.FromInt(tmpval);
	return durationPrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Duration(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	ServiceUnitSI::changePrecisionUnitSI(SiEnums::SiUnitFamilyDuration, tempUnit.ToInt());
}

//-------------------------------------------------------------------------
SwEnum _SwServiceUnitSI::getCurrentPrecision_Frequency()
{
	uint tmpval = ServiceUnitSI::getCurrentPrecision(SiEnums::SiUnitFamilyFrequency);
	frequencyPrecision.FromInt(tmpval);
	return frequencyPrecision;
}

//-------------------------------------------------------------------------
void _SwServiceUnitSI::setCurrentPrecision_Frequency(SwEnum precisionSI)
{
	SwEnum tempUnit = precisionSI;
	ServiceUnitSI::changePrecisionUnitSI(SiEnums::SiUnitFamilyFrequency, tempUnit.ToInt());
}