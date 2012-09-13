#include "SwRefPtrTools.h"

using namespace StreamWork::SwCore;

SwRefPtrTools * SwRefPtrTools::getInstance()
{
	if(_instance == 0){
		_instance = new SwRefPtrTools();
	}
	return _instance;
}


SwRefPtrTools::SwRefPtrTools(void)
{
}

SwRefPtrTools::~SwRefPtrTools(void)
{
}

void SwRefPtrTools::debug( std::string msg )
{
	std::cout << msg.c_str() << std::endl;
}

SwRefPtrTools * SwRefPtrTools::_instance = 0;
