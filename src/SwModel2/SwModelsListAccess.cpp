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


//-----------------------------------------------------------------------
SwModelsListAccess::SwModelsListAccess()
{
	_internalInstance=new _SwModelsList(); 
}

//-----------------------------------------------------------------------
SwModelsListAccess::~SwModelsListAccess()
{
    delete _internalInstance; 
}

//-----------------------------------------------------------------------
SwModelsListAccess * SwModelsListAccess::getInstance()
{
    if (_instance==0) {
        _instance=new SwModelsListAccess();
    }
    return _instance;
}

//-----------------------------------------------------------------------
_SwModelsList * SwModelsListAccess::getInternal()
{
    return _internalInstance;
}

//-----------------------------------------------------------------------
QString SwModelsListAccess::getModelsDirectory()
{
    return _internalInstance->getModelPaths();
}

//-----------------------------------------------------------------------
void SwModelsListAccess::addModel(QString hostComponent, QString modelName)
{
    _internalInstance->addModel(hostComponent,modelName);
}
//-----------------------------------------------------------------------
bool SwModelsListAccess::checkModelName(QString modelName)
{
	return _internalInstance->checkModelName(modelName);
}
