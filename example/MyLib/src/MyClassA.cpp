#include "MyClassA.h"

#include "MyCombinedStaticLibClass.h"

MyClassA::MyClassA()
	: _myint('A')
{
	//MyClassB b;
	MyCombinedStaticLibClass c;
}
