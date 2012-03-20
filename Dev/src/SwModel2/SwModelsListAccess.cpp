/**
@file SwModelsListAccess.cpp
@brief Simgleton d'access a la liste des models
@author F.Bighelli
 */

#include "SwModelsListAccess.h"
#include "_SwModelsList.h" 

using namespace StreamWork::SwModel;

SwModelsListAccess * _instance=0;
_SwModelsList * _internalInstance=0;

/** @brief Constructor */
SwModelsListAccess::SwModelsListAccess() {
	_internalInstance=new _SwModelsList(); 
}
/** @brief Destructor */
SwModelsListAccess::~SwModelsListAccess() {
    delete _internalInstance; 
}
/** @brief instance access */
SwModelsListAccess * SwModelsListAccess::getInstance() {
    if (_instance==0) {
        _instance=new SwModelsListAccess();
    }
    return _instance;
}

/** @brief internal access (backup compatibility) */
_SwModelsList * SwModelsListAccess::getInternal() {
    return _internalInstance;
}

/** @brief internal access (backup compatibility) */
QString SwModelsListAccess::getModelsDirectory() {
    return _internalInstance->getModelPaths();
}
/** @brief add model */
void SwModelsListAccess::addModel(QString hostComponent,QString modelFileName) {

}
