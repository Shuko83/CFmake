/*!
\file SwDefaultComplexeTypeAdapters.cpp
\date 12/04/2006
\brief complexe types adaptateur pour Test3DPos 
\author  Big
\version 1.0
 */

 
#include "Types/UserType/SwDefaultComplexeTypeAdapters.h"
 
using namespace StreamWork::SwCore;
 
/*! \brief Constructeur */
SwDefaultComplexeTypeAdapters::SwDefaultComplexeTypeAdapters() {
    _complexe_property=NULL;
}
/*! \brief Destructeur */
SwDefaultComplexeTypeAdapters::~SwDefaultComplexeTypeAdapters() {
	for (int i=0;i<_liste.count();i++) {
		_liste[i]->Liberate();
	}
	for (int i = 0; i < _liste.count(); i++) {
		delete _liste[i];
	}
}
//-----------------------------------------------------------------------
// Interface ISwComplexeTypeAdapters
//-----------------------------------------------------------------------
/*! \brief Initialisation */
void SwDefaultComplexeTypeAdapters::Initialize(ISwProperty * complexe_property) {
    _complexe_property=complexe_property;
	CreateSimpleTypeAdaptersList(_complexe_property);
}
/*! \brief Acces aux adapteurs simples (la liste est vide si le complexe adapter n'est pas attaché a une propriété*/
QList<ISwSimpleTypeAdapter *> * SwDefaultComplexeTypeAdapters::GetAdapters() {
    if (_complexe_property==NULL)
        return NULL;
    return &_liste;
}
/*! \brief Liberation de la fabrique*/
void SwDefaultComplexeTypeAdapters::Liberate() {

}               
//-----------------------------------------------------------------------
// Creation de la liste
//-----------------------------------------------------------------------
/*! \brief Creation de la liste des simple type adapter A SURCHARGER*/
void SwDefaultComplexeTypeAdapters::CreateSimpleTypeAdaptersList(ISwProperty * complexe_property) {

}

