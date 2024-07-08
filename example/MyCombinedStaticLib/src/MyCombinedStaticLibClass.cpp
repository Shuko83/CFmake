#include "MyCombinedStaticLibClass.h"

#include "MyStaticLibClass.h"

MyCombinedStaticLibClass::MyCombinedStaticLibClass()
	: _myChar('C')
{
	MyStaticLibClass s;
}
