
/**
 * Include Globaux
 */
#include "_SwServiceCodeTimer.h"

using namespace StreamWork::SwCore;

//-----------------------------------------------------------------------
StreamWork::SwCore::_SwServiceCodeTimer::_SwServiceCodeTimer()
{

}

//-----------------------------------------------------------------------
StreamWork::SwCore::_SwServiceCodeTimer::~_SwServiceCodeTimer()
{

}

//-----------------------------------------------------------------------
double StreamWork::SwCore::_SwServiceCodeTimer::getTime( void * caller )
{
	if(_timers.contains(caller))
	{
		return _timers.value(caller)->getDiffMs().count();
	}

	return 0.;
}

//-----------------------------------------------------------------------
void StreamWork::SwCore::_SwServiceCodeTimer::start( void * caller )
{
	_SwPerfChrono * tmp = new _SwPerfChrono();
	_timers.insert(caller,tmp);
	tmp->start();
}
