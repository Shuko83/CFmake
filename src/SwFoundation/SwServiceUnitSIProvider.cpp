/*!
\file SwServiceUnitSIProvider.cpp
\brief Implementation of the Class SwServiceUnitSIProvider permettant de provider des properties du service unit SI (sxCore)
\version 1.0
\date 02/04/2014 2:00
\author EPO
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "SwServiceUnitSIProvider.h"

using namespace StreamWork::SwFoundation;

//-----------------------------------------------------------------------
SwServiceUnitSIProvider::SwServiceUnitSIProvider(): SwAssistedComponent()
{
	_serviceUnitSI=dynamic_cast<_SwServiceUnitSI *>(SW_APP->QueryService(CG_SW_SERVICE_UNIT_SI));
}

//-----------------------------------------------------------------------
SwServiceUnitSIProvider::~SwServiceUnitSIProvider()
{
	_serviceUnitSI = 0;
}

//-----------------------------------------------------------------------
void SwServiceUnitSIProvider::initializeComponent() throw(SwException) 
{
	//retrieved the service of unit SI
	if(_serviceUnitSI)
	{
		createPropertiesForQObject(_serviceUnitSI);
	}
}


