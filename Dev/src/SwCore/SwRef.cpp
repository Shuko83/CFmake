/*!
 \file SwRef.cpp
 \brief Implementation of the Class SwRef
 \date 23-août-2006 10:35:13
 \author F.Bighelli
*/

#include "SwRef.h"
#include "SwRefPtrTools.h"
//#include "SwException.h"
//#include "SwMacros.h"
using namespace StreamWork::SwCore;


/*! \brief Constructeur */
SwRef::SwRef()
	:_referencesCounter(0)  {
	//A la construction, il y a 0 instance piisque non reference
}


/*! \brief Destructeur */
SwRef::~SwRef(){
}


/*! \brief Ajout d'une référence */
void SwRef::_addRef(){
	//Incrementation du compteur de référence
	_referencesCounter++;
}


/*! \brief Retrait d'une référence */
void SwRef::_release(){
   //Decrementation du compteur de reference
   _referencesCounter--;        
   //S'il n'existe plus aucune reference, l'instance se detruit
   if (_referencesCounter <= 0) {           //Destruction de l'instance
	   OnDestroy();
       delete this;
   }
}
/*! 
\brief Permet de connaitre le nombre de references
\return le nombre de references
*/
unsigned long SwRef::_getReferencesNb() const{
	return _referencesCounter;
}

LibIndeSig::iSignal0 & SwRef::getOnDestroySignal()
{
	return OnDestroy;
}
