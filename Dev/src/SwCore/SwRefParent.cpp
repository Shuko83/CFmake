/*!
 \file SwRefParent.cpp
 \brief Implementation of the Class SwRefParent
 \date 23-août-2006 10:35:13
 \author F.Bighelli
*/

#include "SwRefParent.h"
#include "SwRefPtrTools.h"
//#include "SwException.h"

using namespace StreamWork::SwCore;


/*! \brief Constructeur */
SwRefParent::SwRefParent(ISwRef * parent)
	:_parent(parent)  {
}


/*! \brief Destructeur */
SwRefParent::~SwRefParent(){
}


/*! \brief Ajout d'une référence */
void SwRefParent::_addRef(){
	_parent->_addRef();
}


/*! \brief Retrait d'une référence */
void SwRefParent::_release(){
	_parent->_release();
}
/*! 
\brief Permet de connaitre le nombre de references
\return le nombre de references
*/
unsigned long SwRefParent::_getReferencesNb() const{
	return _parent->_getReferencesNb();
}

/*! \brief get du parent */
ISwRef * SwRefParent::getSwRefParent(){
    return _parent;
}			
/*! \brief set du parent */
void SwRefParent::setSwRefParent(ISwRef * parent) {
    _parent=parent;
}

LibIndeSig::iSignal0 & SwRefParent::getOnDestroySignal() {
	return OnDestroy;
}