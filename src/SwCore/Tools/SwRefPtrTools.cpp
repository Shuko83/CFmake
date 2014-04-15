#include "SwRefPtrTools.h"
#include <QDebug>
#include <iostream>

using namespace StreamWork::SwCore;


//======================   SwRefGenericContainer   ==========================================
/*void SwRefGenericContainer::setNull()
{
	SwRefPtrTools::getInstance()->_swRefList.remove(this);
	//delete this;
}

SwRefGenericContainer::SwRefGenericContainer( ISwRef * elt ) : _elt(elt)
{
	_eltName = QString(typeid(*_elt).name());
	connectDestroyToNull();
}

QString SwRefGenericContainer::getResult() const
{
	return QString(typeid(*_elt).name());
}
*/

//======================   SwRefPtrTools   ==========================================
SwRefPtrTools * SwRefPtrTools::getInstance()
{
	if(_instance == 0){
		_instance = new SwRefPtrTools();
	}
	return _instance;
}


SwRefPtrTools::SwRefPtrTools(void)
{
	_isWatching = false;
}

SwRefPtrTools::~SwRefPtrTools(void)
{
}

void SwRefPtrTools::debug( std::string msg )
{
	std::cout << msg.c_str() << std::endl;
}

SwRefPtrTools * SwRefPtrTools::_instance = 0;




void SwRefPtrTools::startWatch()
{
	_isWatching = true;
}

void SwRefPtrTools::stopWatch()
{
	_isWatching = false;
	printResults();
}

void SwRefPtrTools::cleanWatch()
{
	/*QMutableSetIterator<SwRefGenericContainer * > i(_swRefList);
	while (i.hasNext()){
		SwRefGenericContainer * eltContainer = i.next();
		eltContainer->disconnectDestroyToNull();
		i.remove();
		delete eltContainer;
	}*/
}

void SwRefPtrTools::printResults()
{
	/*std::cout << "============================  Memory Results  ==============================" << std::endl ;
		QSetIterator<SwRefGenericContainer * > i(_swRefList);
		int j = 0;
		while (i.hasNext()){
			try{
			SwRefGenericContainer * eltContainer = i.next();
			std::cout << j++ ;
			
				QString msg = eltContainer->getResult();
				if(msg.isNull()  || msg.isEmpty())
					std::cout << "pointer, error while retriving rtti data, qstring null" << std::endl ;	
				else
					std::cout << msg.toStdString().c_str() << std::endl ;
			}catch(...){
				std::cout << "pointer, error while retriving rtti data" << std::endl ;	
			}
		}
		std::cout << "============================  Summary  ==============================" << std::endl;
		std::cout << "	" << "Number of remaining objects : " << _swRefList.size() << " / " << count << std::endl;
		std::cout << "============================  End Memory Results  =============================="  << std::endl;*/
}

/*void SwRefPtrTools::registerSwRef( ISwRef * elt )
{
		if(_isWatching && elt != 0){
			SwRefGenericContainer * eltContainer = new SwRefGenericContainer(elt);
			count++;
			_swRefList.insert(eltContainer);
		}
}*/


bool SwRefPtrTools::isWatching()
{
	return _isWatching;
}
