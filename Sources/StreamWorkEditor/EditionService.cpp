/**
@file EditionService.cpp
@brief Si present, alors en mode edition
@author Big
 */

#include "EditionService.h"
 

/** @brief Constructor */
EditionService::EditionService() {
	_saveInProgress=false;
}
/** @brief Destructor */
EditionService::~EditionService() {
  //TO DO destruction
}
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void EditionService::Liberate() {

}
/*! \brief verifie la licence*/
bool EditionService::editorSavingInProgress(){
    return _saveInProgress;
}
/*! \brief verifie la licence*/
void EditionService::setEditorSavingInProgress(bool val){
    _saveInProgress=true;
}
