/*!
 \file DisabledSwRef.cpp
 \brief Implementation du ISwRef dont la gestion du cycle de vie est laissé au developpeur (pointeur classique)
 \author QBN
*/

#include "SwRef/DisabledSwRef.h"
using namespace StreamWork::SwCore;

/*! \brief Constructeur */
DisabledSwRef::DisabledSwRef()
{

}

/*! \brief Destructeur */
DisabledSwRef::~DisabledSwRef()
{

}

/*! \brief Ajout d'une référence */
void DisabledSwRef::_addRef()
{

}

/*! \brief Retrait d'une référence */
void DisabledSwRef::_release()
{

}

/*! 
\brief Permet de connaitre le nombre de references
\return le nombre de references
*/
unsigned long DisabledSwRef::_getReferencesNb() const
{
	return 0;
}

LibIndeSig::iSignal0 & DisabledSwRef::getOnDestroySignal()
{
	return OnDestroy;
}
