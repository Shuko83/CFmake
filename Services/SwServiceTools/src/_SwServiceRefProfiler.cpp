#include "_SwServiceRefProfiler.h"

using namespace StreamWork::SwCore;

_SwServiceRefProfiler::_SwServiceRefProfiler(void)
{
}

_SwServiceRefProfiler::~_SwServiceRefProfiler(void)
{
}

void _SwServiceRefProfiler::startWatch()
{
	SwRefPtrTools::getInstance()->startWatch();
}

void _SwServiceRefProfiler::cleanWatch()
{
	SwRefPtrTools::getInstance()->cleanWatch();
}

void _SwServiceRefProfiler::stopWatch()
{
	SwRefPtrTools::getInstance()->stopWatch();
}

void _SwServiceRefProfiler::printResults()
{
	SwRefPtrTools::getInstance()->printResults();
}
