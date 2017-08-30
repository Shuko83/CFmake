/**
@file _SwServiceExtensionsImpl.cpp
@brief Extensions service implementation
@author Big
 */

#include "_SwServiceExtensionsImpl.h"
#include "SwExtensionImpl.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwServiceExtensionsImpl::_SwServiceExtensionsImpl() {
	//TO DO creation
}
/** @brief Destructor */
_SwServiceExtensionsImpl::~_SwServiceExtensionsImpl() {
  //TO DO destruction
}
/*! \brief Permet d'enregistrer une extension avec le type en nom (utliser la methode precedente*/
void _SwServiceExtensionsImpl::registerExtensionWithType(QString extension_type_name,QString extension_name,QString component_name) {
    _extensions[extension_type_name].push_back(new SwExtensionImpl(extension_type_name,extension_name,component_name));
}
/*! \brief Permet d'avoir une liste d'extension pourt un type donne (utliser la methode precedente)*/
QList<ISwExtension *> _SwServiceExtensionsImpl::getExtensionWithType(QString extension_type_name) {
    return _extensions[extension_type_name];
}
