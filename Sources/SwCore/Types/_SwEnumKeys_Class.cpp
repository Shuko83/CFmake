/*!
 \file _SwEnumKeys_Class.hcpp
 \brief Classe pour la gestion des enumeres
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#include "Types/_SwEnumKeys_Class.h"

using namespace StreamWork::SwCore;

/*! \brief Constructor */
_SwEnumKeys_Class::_SwEnumKeys_Class():SwRef() {
	_enum_values.clear();
}

/*! \brief Destructor */
_SwEnumKeys_Class::~_SwEnumKeys_Class()
{
	_enum_values.clear();
}
