/*
  ID				:	Keyword for the tab. Can contain only A-Z a-z chars.
  tabname			:	English name unit type written on the option tab. string type
  platename			:	English name unit type written in the window. string type
  base_unit			:	Default unit loaded at start 
  base_starlinx		:   Default value's conversion for Starlinx

*/

/*							ID			,		tabname			,		base_unit			,	precision		,	base_starlinx		*/
SW_SI_UNIT_FAMILY(		Altitude		,		"Altitude"		,		AltMeter			,		 0			,	AltMeter			)
SW_SI_UNIT_FAMILY(		Angle			,		"Angle"			,		Degree				,		 1			,	Degree				)
SW_SI_UNIT_FAMILY(		Distance		,		"Distance"		,		Meter				,		 2			,	Meter				)
SW_SI_UNIT_FAMILY(		Speed			,		"Speed"			,		MeterPerSecond		,		 1			,	MeterPerSecond		)
SW_SI_UNIT_FAMILY(		Duration		,		"Duration"		,		TimeSecond			,		 2			,	TimeSecond			)
SW_SI_UNIT_FAMILY(		Frequency		,		"Frequency"		,		Hertz				,		 2			,	Hertz				)
SW_SI_UNIT_FAMILY(		Coordinates		,		"Coordinates"	,		CoordDegreeDecimal	,		-1			,	CoordDegreeDecimal	)
