/*
  ID				: Keyword for the unit. Can contain only A-Z a-z chars.
  name				: English name of the unit. string type
  suffix			: Suffix of the unit
  base				: ID of base unit the unit belongs to
  conv				:		Conversion type: 
							Base :  the unit itself is a base
							Linear: Unit = factor*Base+offset; Base := Unit/factor
							Recipr: Unit = offset+factor/Base; Base := factor/Unit
							Param:  Unit = a*UserParam*Base
							RecipParam Unit = a*Base/UserParam
  constant			: Conversion constant
  valueBase			: default value for conversion 
  valueSecondBase	: second default value for conversion (use for longitude value)
  minValue			: minimum value system for an unit
  maxValue			: maximum value system for an unit

  Remarque : space are present in text name and suffix because the service find the first text name as required. If it's the same name, the unit will be not available for an specified unit
  */
#pragma warning(disable:4002) 
/*			ID,							name,						suffix,			base,					conv,			constant,			valueBase,	valueSecondBase,	minValue,		maxValue*/
SW_SI_UNIT(	Meter,						"Meter",					"m",			Meter,					Base,			1.0,				609.60,				0,				0,			 800000		)
SW_SI_UNIT(	KiloMeter,					"Kilometer",				"km",			Meter,					Linear,			0.001,					0,				0,				0,			  8000 		)
SW_SI_UNIT(	Feet,						"Feet",						"ft",			Meter,					Linear,			1.0/0.3048,				0,				0,				0,			26246719.16	)
SW_SI_UNIT(	Yard,						"Yard",						"yd",			Meter,					Linear,			1.0/0.9144,				0,				0,				0,		    8748906.38	)
SW_SI_UNIT(	Mile,						"Mile (int)",				"mi",			Meter,					Linear,			1.0/1609.344,			0,				0,				0,			  4970.96	)
SW_SI_UNIT(	NauticalMile,				"Nautical mile",			"NM",			Meter,					Linear,			1.0/1852.0,				0,				0,				0,			  4319.65	)
SW_SI_UNIT(	DataMile,					"Data mile",				"DM ",			Meter,					Linear,			1.0/1828.8,				0,				0,				0,			  4374,45	)
SW_SI_UNIT(	AltMeter,					"Meter (alt)",				"m ",			AltMeter,				Base,			1.0,				 1252.10,			0,			 -9753.6,		 304799.99	)
SW_SI_UNIT(	AltKilometer,				"Kilometer (alt)",			"km ",			AltKilometer,			Linear,			0.001,					0,				0,			  -9.75,		  304.79	)
SW_SI_UNIT(	AltFeet,					"Feet (alt)",				"ft ",			AltMeter,				Linear,			1.0/0.3048,				0,				0,			 -32000,		 999999.97	)
SW_SI_UNIT(	MeterPerSecond,				"Meter / second",			"m/s",			MeterPerSecond,			Base,			1.0,				  55.70,			0,			 -10000,		  10000	    )
SW_SI_UNIT(	KiloMeterPerHour,			"Kilometer / hour",			"km/h",			MeterPerSecond,			Linear,			3.6,					0,				0,			 -36000,		  36000		)
SW_SI_UNIT(	Knot,						"Knot",						"kt",			MeterPerSecond,			Linear,			3600.0/1852.0,			0,				0,			-19438.44,		 19438.44	)
SW_SI_UNIT(	MilePerHour,				"Mile / hour",				"mi/h",			MeterPerSecond,			Linear,			3600.0/1609.344,		0,				0,			-22369.36,		 22369.36	)
SW_SI_UNIT(	FeetPerSecond,				"Feet / second",			"ft/s",			MeterPerSecond,			Linear,			3.2808399,				0,				0,			-32808.39,		 32808.39	)
SW_SI_UNIT(	DataMilePerHour,			"Data mile / second",		"DM/h",			MeterPerSecond,			Linear,			3600.0/1828.8,			0,				0,			-19685.03,		 19685.03	)
SW_SI_UNIT(	Degree,						"Degree",					"°",			Degree,					Base,			1.0,				  90.00,			0,			  -360,			   360		)
SW_SI_UNIT(	DegreeMinute,				"Minute",					"'",			Degree,					Linear,			60.0,					0,				0,			 -21600,		  21600		)
SW_SI_UNIT(	DegreeSecond,				"Second",					"''",			Degree,					Linear,			3600.0,					0,				0,			-1296000,		 1296000	)
SW_SI_UNIT(	Radian,						"Radian",					"rad",			Degree,					Linear,			0.0174532925199433,		0,				0,			 -6.28,			  6.28      )
SW_SI_UNIT(	TimeSecond,					"Second ",					"s ",			TimeSecond,				Base,			1.0,					45,				0,				0,			  10000     )
SW_SI_UNIT(	Hertz,						"Hertz",					"Hz ",			Hertz,					Base,			1.0,					225,			0,				0,			  30000		)
SW_SI_UNIT(	CoordDegreeMinuteSecond,	"DMS",						"DMS",			CoordDegreeDecimal,		Coordinates,	0,						0,				0,				0,				0		)
SW_SI_UNIT(	CoordDegreeMinute,			"DM",						"DM",			CoordDegreeDecimal,		Coordinates,	0,						0,				0,				0,				0		)
SW_SI_UNIT(	CoordDegreeDecimal,			"DD",						"DD",			CoordDegreeDecimal,		Base,			0,					  33.12,		  43.24,			0,				0		)
SW_SI_UNIT(	CoordMGRS,					"MGRS",						"MGRS",			CoordDegreeDecimal,		Coordinates,	0,						0,				0,				0,				0		)
SW_SI_UNIT(	CoordGEOREF,				"GeoRef",					"GeoRef",		CoordDegreeDecimal,		Coordinates,	0,						0,				0,				0,				0		)
SW_SI_UNIT(	CoordUTM,					"UTM",						"UTM",			CoordDegreeDecimal,		Coordinates,	0,						0,				0,				0,				0		)
#pragma warning(default:4002) 

